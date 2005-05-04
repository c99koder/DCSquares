/*
 *  net.cc
 *  DCSquares
 *
 *  Created by Sam Steele on 10/28/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */
#if 0
#include "net.h"

#ifdef DREAMCAST
#include <kos.h>
#include <lwip/lwip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include "squares.h"

#define PORT 6000
void debug(char *msg);

int gamesocket;
int sendsocket;
struct squarelist *netplayer;

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
	netplayer=create_square(40,40,10,PLAYER_NET);
	netplayer->r=0;
	netplayer->g=0;
	netplayer->b=0.8;
	netplayer->angle=0;
#ifdef DREAMCAST
  paused=1;
#endif
#ifdef MACOS
  net_sendpacket("2:.");
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
		val=strtok(buf,":");
		switch(atoi(val)) {
		  case 2: //Channel 2: Game Data
			  process_game_packet(strtok(NULL,"\0"));
				break;
		}	
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
	if(val[0]=='.') 
		paused=0;
	if(val[0]=='p') {
		netplayer->x=atoi(strtok(NULL,","));
		netplayer->y=atoi(strtok(NULL,","));
		netplayer->size=atoi(strtok(NULL,","));
		if(netplayer->size>12 || netplayer->size<0) debug(buf2);
	}
	if(val[0]=='c') {
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
	if(val[0]=='g') {
		c=get_square_by_id(atoi(strtok(NULL,",")));
		if(c!=NULL) {
			if(c->tm==0 || atoi(strtok(NULL,","))<c->tm) {
				c->deleted=1;
				net_sendpacket("2:s");
				give_points(PLAYER_NET);
			}
		}
	}
	if(val[0]=='s') {
		give_points(PLAYER1);
	}
}
#endif