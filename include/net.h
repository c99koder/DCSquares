/*
 *  net.h
 *  DCSquares
 *
 *  Created by Sam Steele on 10/28/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

void net_update();
void netplay_init();
void net_sendpacket(char *packet);
void process_game_packet(snPacketType t, void *buf);
void process_packet(snPacket p);
void process_server_packet(snPacketType t, void *data);
void process_chat_packet(snPacketType t, void* data);
int lobby_connect(char *host, char *username, char *password);
void lobby_update();
void lobby_disconnect();
void lobby_send(snChannel c, snPacketType t, int len, void *data);
struct userlist_node {
	char username[50];
	struct userlist_node *next;
};

struct userlist_node *get_userlist();
void add_user(char *username);
int userlist_size();

void os_chat_insert_text(char *text);
void os_chat_reload_users();