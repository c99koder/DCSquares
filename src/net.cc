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
#ifdef TIKI
#include <Tiki/tiki.h>
#include <Tiki/gl.h>
#include <Tiki/texture.h>

using namespace Tiki;
using namespace Tiki::GL;
#endif
#include "squares.h"
#include "http.h"
#include "squarenet.h"
#include "level.h"
#include "net.h"

#define PORT 6000

void debug(char *msg);

SOCKET lobbysocket;
SOCKET gamesocket;
char remote_host[20];
struct squarelist *netplayer;
snAuth AUTH;

std::list<std::string> userlist;

void game_send(snChannel chan, snPacketType type, int len, void *data) {
	struct sockaddr_in their_addr; // connector's address information
	snPacket p;
  memset(&p,0,sizeof(p));
	
	p.chan=(snChannel)htonl(chan);
	p.type=(snPacketType)htonl(type);
	memcpy(p.data,data,len);
	
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(PORT);
	their_addr.sin_addr.s_addr = inet_addr(remote_host);

	if(sendto(gamesocket, (char *)&p, sizeof(p), 0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
		perror("sendto");
	}
}

void lobby_send(snChannel chan, snPacketType type, int len, void *data) {
	snPacket p;
  memset(&p,0,sizeof(p));

	p.chan=(snChannel)htonl(chan);
	p.type=(snPacketType)htonl(type);
	memcpy(p.data,data,len);

	send(lobbysocket,(char *)&p,sizeof(p),0);
}

int lobby_connect(char *host, char *username, char *password) {
  struct sockaddr_in sinRemote;
	struct sockaddr_in my_addr;    // my address information
	
  char msg[300];
	int r;
	
  printf("Connecting to %s...",host);
	
  lobbysocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	
  sinRemote.sin_family = AF_INET;
  sinRemote.sin_addr.s_addr = resolve((char *)host);
	if(sinRemote.sin_addr.s_addr==0) return -1;
  sinRemote.sin_port = htons(PORT);
  
  connect(lobbysocket, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in));
	
	strcpy(AUTH.user,username);
	strcpy(AUTH.pass,password);

	if ((gamesocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;         // host byte order
	my_addr.sin_port = htons(PORT);     // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct
	
	if (bind(gamesocket, (struct sockaddr *)&my_addr,sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}
	
}	

void net_update() {
	int r;
	snPacket p;
	timeval tv;
	fd_set readfds;
		
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	FD_ZERO(&readfds);
	FD_SET(lobbysocket, &readfds);

	select(lobbysocket+1, &readfds, NULL, NULL, &tv);
	
	if (FD_ISSET(lobbysocket, &readfds)) {
		r=recv(lobbysocket,(char *)&p,sizeof(snPacket),0);
		process_packet(p);
	}
	
	FD_ZERO(&readfds);
	FD_SET(gamesocket, &readfds);

	select(gamesocket+1, &readfds, NULL, NULL, &tv);
	
	if (FD_ISSET(gamesocket, &readfds)) {
		r=recv(gamesocket,(char *)&p,sizeof(snPacket),0);
		process_packet(p);
  }
}

void lobby_disconnect() {
#ifdef WIN32
	closesocket(lobbysocket);
#else
	close(lobbysocket);
#endif
}

void process_packet(snPacket p) {
	p.chan=(snChannel)ntohl(p.chan);
	p.type=(snPacketType)ntohl(p.type);
	switch(p.chan) {
		case CHAN_SERVER: //Channel 0: Server Data
			process_server_packet(p.type,p.data);
			break;
		case CHAN_CHAT: //Channel 1: Chat Data
			process_chat_packet(p.type,p.data);
			break;
		case CHAN_GAME: //Channel 2: Game Data
			process_game_packet(p.type,p.data);
			break;
	}	
}		

void process_server_packet(snPacketType t, void *data) {
	char buf[256];
	
	switch(t) {
		case SERVER_VERSION: //Server version message
			printf("Protocol: %i\n",((snServerVersion *)data)->protoVersion);
			lobby_send(CHAN_SERVER,SERVER_AUTH,sizeof(AUTH),&AUTH);
			break;
		case SERVER_MSG: //Server text message
			sprintf(buf,"-- %s",data);
			os_chat_insert_text(CHAN_SERVER,buf);
			break;
	}
}

void process_chat_packet(snPacketType t, void *data) {
	char buf[256];
	
	switch(t) {
		case CHAT_INFO: //Room info
			switch(((snChatInfo *)data)->infoType) {
				case chatInfoUser: //User
					userlist.push_back(((snChatInfo *)data)->data);
					os_chat_reload_users();
			
					snGameChallenge chal;
					chal.gameid=htonl(-1);
					strcpy(chal.user,((snChatInfo *)data)->data);
					chal.squares=htonl(10);
					chal.score=htonl(0);
					chal.time=htonl(30);//60*3);
					chal.win_mode=htonl(MODE_SQUARES);
					chal.lose_mode=htonl(MODE_TIME);
					
					lobby_send(CHAN_GAME,GAME_CHALLENGE,sizeof(chal),&chal);
					sprintf(buf,"Sending challenge to %s",chal.user);
					os_chat_insert_text(CHAN_GAME,buf);
					break;
			}
			break;
		case CHAT_MSG: //User chat message
			sprintf(buf,"<%s> %s",((snChatMsg *)data)->user,((snChatMsg *)data)->msg);
			os_chat_insert_text(CHAN_CHAT,buf);
			break;
		case CHAT_JOIN: //User has joined channel
			userlist.push_back(((snChatJoin *)data)->user);
			sprintf(buf,"*** %s has joined the room",((snChatJoin *)data)->user);
			os_chat_insert_text(CHAN_CHAT,buf);
			os_chat_reload_users();
			break;
		case CHAT_PART: //User has parted channel
			userlist.remove(((snChatJoin *)data)->user);
			sprintf(buf,"*** %s has left the room",((snChatJoin *)data)->user);
			os_chat_insert_text(CHAN_CHAT,buf);
			os_chat_reload_users();
			break;
	}
}

void play_game(level_node *level);

void process_game_packet(snPacketType t, void *data) {
	char *val;
	squarelist *c;
	char buf[256];

	switch(t) {
		case GAME_START:
			snChatMsg m;
			
			strcpy(remote_host,((snGameStart *)data)->host);
			
			play_game(multi_play);
			break;
			
		case GAME_CHALLENGE:
			if(ntohl(((snGameChallenge *)data)->accept)==1) {
				multi_play->squares=ntohl(((snGameChallenge *)data)->squares);
				multi_play->score=ntohl(((snGameChallenge *)data)->score);
				multi_play->time=ntohl(((snGameChallenge *)data)->time);
				multi_play->win_mode=ntohl(((snGameChallenge *)data)->win_mode);
				multi_play->lose_mode=ntohl(((snGameChallenge *)data)->lose_mode);
				multi_play->net=1;
				sprintf(buf,"Accepted challenge!");
				os_chat_insert_text(CHAN_GAME,buf);
				snGameStart gs;
				gs.gameid=((snGameChallenge *)data)->gameid;
				gs.r=htonl(rand()%255);
				gs.g=htonl(rand()%255);
				gs.b=htonl(rand()%255);
				lobby_send(CHAN_GAME,GAME_START,sizeof(snGameStart),&gs);
			} else if(ntohl(((snGameChallenge *)data)->accept)==0) {
				sprintf(buf,"Challenge recieved from %s",((snGameChallenge *)data)->user);
				os_chat_insert_text(CHAN_GAME,buf);
				((snGameChallenge *)data)->accept=htonl(1);
				lobby_send(CHAN_GAME,GAME_CHALLENGE,sizeof(snGameChallenge),data);
			} else { //-1 to reject
				os_chat_insert_text(CHAN_GAME,"Challenge rejected");
			}
			break;
		
		case GAME_PLAYERMOVE:
			if(netplayer!=NULL) {
				netplayer->x=ntohl(((snGamePlayerMove *)data)->x);
				netplayer->y=ntohl(((snGamePlayerMove *)data)->y);
			}
			break;
	}
#if 0	
	if(val[0]=='p' && netplayer!=NULL) { //Network player's current coordinates
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
#endif
}
