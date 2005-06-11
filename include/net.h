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
void process_game_packet(char *buf);
void process_packet(char *msg);
void process_server_packet(int msgid, char*data);
void process_chat_packet(int msgid, char*data);
int lobby_connect(char *host, char *username, char *password);
void lobby_update();
struct userlist_node {
	char username[50];
	struct userlist_node *next;
};

struct userlist_node *get_userlist();
void add_user(char *username);
int userlist_size();

void os_chat_insert_text(char *text);
void os_chat_reload_users();