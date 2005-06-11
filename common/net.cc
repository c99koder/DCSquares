/*
 *  net.cc
 *  DCSquares
 *
 *  Created by Sam Steele on 10/28/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */
#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#include <fcntl.h>
#include <sys/stat.h>
#define NET
#define uint32_t u_long
#endif
#ifdef UNIX
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define SOCKET int
#define NET
#endif
#ifdef DREAMCAST
#include <kos.h>
#include <lwip/lwip.h>
#include <lwip/sockets.h>
#define SOCKET int
#endif
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "squares.h"
#include "http.h"
#include "net.h"

#define PORT 6000
void debug(char *msg);

int lobbysocket;
int gamesocket;
int sendsocket;
struct squarelist *netplayer;

struct userlist_node *userlist=NULL;

struct userlist_node *get_userlist() {
	return userlist;
}

void add_user(char *username) {
	struct userlist_node *current = new userlist_node;
	
	strcpy(current->username,username);
	current->next=userlist;
	userlist=current;
}

int userlist_size() {
	struct userlist_node *current=userlist;
	int count=0;
	
	while(current!=NULL) {
		count++;
		current=current->next;
	}
	
	return count;
}

void lobby_send(char *packet) {
	int numbytes;
	char buf[256];

  memset(buf,0,128);
	strcpy(buf,packet);
	send(lobbysocket,buf,128,0);
}

int lobby_connect(char *host, char *username, char *password) {
  struct sockaddr_in sinRemote;
  char msg[300];
	int r;
	
  printf("Connecting to %s...",host);
	
  lobbysocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	
  sinRemote.sin_family = AF_INET;
  sinRemote.sin_addr.s_addr = resolve((char *)host);
	if(sinRemote.sin_addr.s_addr==0) return -1;
  sinRemote.sin_port = htons(PORT);
  
  connect(lobbysocket, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in));
	
	r=recv(lobbysocket,msg,128,0);
	
	printf("incoming: %s\n",msg);
	
	sprintf(msg,"0:1:%s:%s",username,password);
	lobby_send(msg);
}	

void lobby_update() {
	int r;
	char buf[256];
	timeval tv;
	fd_set readfds;
		
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	FD_ZERO(&readfds);
	FD_SET(lobbysocket, &readfds);

	select(lobbysocket+1, &readfds, NULL, NULL, &tv);
	
	if (FD_ISSET(lobbysocket, &readfds)) {
		r=recv(lobbysocket,buf,128,0);
		printf("%s\n",buf);
		process_packet(buf);
	}
}

void lobby_disconnect() {
#ifdef WIN32
	closesocket(lobbysocket);
#else
	close(lobbysocket);
#endif
}

void process_packet(char *msg) {
	int chan=atoi(strtok(msg,":"));
	int msgid=atoi(strtok(NULL,":"));
	char *data=strtok(NULL,"\0");
	
	switch(chan) {
		case 0: //Channel 0: Server Data
			process_server_packet(msgid,data);
			break;
		case 1: //Channel 1: Chat Data
			process_chat_packet(msgid,data);
			break;
		case 2: //Channel 2: Game Data
		//process_game_packet(strtok(NULL,"\0"));
		break;
	}	
}		

void process_server_packet(int msgid, char *data) {
	char *val;
	char buf[256];
	
	switch(msgid) {
		case 1: //Server text message
			sprintf(buf,"-- %s",data);
			os_chat_insert_text(buf);
			break;
	}
}

void process_chat_packet(int msgid, char *data) {
	char *val;
	char buf[256];
	
	switch(msgid) {
		case 0: //Room info
			break;
		case 1: //User chat message
			val=strtok(data,":");
			sprintf(buf,"<%s> %s",val,strtok(NULL,"\0"));
			os_chat_insert_text(buf);
			break;
		case 2: //User has joined channel
			add_user(data);
			sprintf(buf,"*** %s has joined the room",data);
			os_chat_insert_text(buf);
			os_chat_reload_users();
			break;
	}
}

#if 0
void net_sendpacket(char *packet) {
	struct sockaddr_in their_addr; // connector's address information
	int numbytes;
	char buf[256];
	unsigned short int len=htons(strlen(packet));

	their_addr.sin_family = AF_INET;     // host byte order
	their_addr.sin_port = htons(PORT); // short, network byte order
#ifdef MACOS
	their_addr.sin_addr.s_addr = inet_addr("192.168.11.6"/*6*/);
#endif
#ifdef DREAMCAST
	their_addr.sin_addr.s_addr = inet_addr("192.168.11.103");
#endif
  memset(buf,0,128);
	strcpy(buf,packet);
	sendto(gamesocket, buf, 128, 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
}
extern int paused;

void netplay_init() {
	struct sockaddr_in my_addr;    // my address information

	if ((gamesocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			perror("socket");
			exit(1);
	}
	if ((sendsocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			perror("socket");
			exit(1);
	}

	my_addr.sin_family = AF_INET;         // host byte order
	my_addr.sin_port = htons(PORT);     // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct

	if (bind(gamesocket, (struct sockaddr *)&my_addr,
																				sizeof(struct sockaddr)) == -1) {
			perror("bind");
			exit(1);
	}
	debug("Hello Debug");
	//Create the remote network player object
	netplayer=create_square(40,40,10,PLAYER_NET);
	netplayer->r=0;
	netplayer->g=0;
	netplayer->b=0.8;
	netplayer->angle=0;
#ifdef DREAMCAST
  paused=1; //pause the Dreamcast so it waits for the Mac to start the game
#endif
#ifdef MACOS
  net_sendpacket("2:."); //Send the message to the Dreamcast telling it we're ready to play!
#endif
}

void give_points(int who);

void net_update(void) {
	struct sockaddr_in their_addr; // connector's address information
	int addr_len, numbytes;
	char buf[256];
	char *val;
	squarelist *c;
	unsigned short int len;
	
	timeval tv;
	fd_set readfds;
		
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&readfds);
	FD_SET(gamesocket, &readfds);
  //printf("Select\n");
	// don't care about writefds and exceptfds:
	select(gamesocket+1, &readfds, NULL, NULL, &tv);

	if (FD_ISSET(gamesocket, &readfds)) {
	  //printf("Recieve\n");
		addr_len = sizeof(struct sockaddr);
		numbytes=recvfrom(gamesocket,buf,128,0,(struct sockaddr *)&their_addr, &addr_len);
		buf[numbytes] = '\0';
		process_packet(buf);
  }
} 

void process_game_packet(char *buf) {
	char *val;
	squarelist *c;
	int x,y,size,type,id,xv,yv;
	char buf2[256];
	//printf("Data: %s\n",buf);
  strcpy(buf2,buf);
	val=strtok(buf,",");
	if(val[0]=='.') //Game is ready to start
		paused=0;
	if(val[0]=='p') { //Network player's current coordinates
		netplayer->x=atoi(strtok(NULL,","));
		netplayer->y=atoi(strtok(NULL,","));
		netplayer->size=atoi(strtok(NULL,","));
		if(netplayer->size>12 || netplayer->size<0) debug(buf2);
	}
	if(val[0]=='c') { //Spawn a new square
		x=atoi(strtok(NULL,","));
		y=atoi(strtok(NULL,","));
		size=atoi(strtok(NULL,","));
		type=atoi(strtok(NULL,","));
		id=atoi(strtok(NULL,","));
		xv=atoi(strtok(NULL,","));
		yv=atoi(strtok(NULL,","));
		c=create_square(x,y,size,type);
		if(c->id!=id) c->id=id;
		c->xv=xv;
		c->yv=yv;
	}
	if(val[0]=='g') { //Judgement!  Decide whether a remote player is allowed to collect a square by checking if we got to it first
		c=get_square_by_id(atoi(strtok(NULL,",")));
		if(c!=NULL) {
			if(c->tm==0 || atoi(strtok(NULL,","))<c->tm) { //Remote player got the square first
				c->deleted=1;
				net_sendpacket("2:s"); //Send the acknowledgement
				give_points(PLAYER_NET); //Update the remote player's score on our screen
			}
		}
	}
	if(val[0]=='s') { //Square collection acknowledgement
		give_points(PLAYER1); //give ourselves some points
	}
}
#endif
