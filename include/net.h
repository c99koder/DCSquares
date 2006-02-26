/*
 *  net.h
 *  DCSquares
 *
 *  Created by Sam Steele on 10/28/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

void net_update();
void process_game_packet(snPacketType t, void *buf);
void process_packet(snPacket p);
void process_server_packet(snPacketType t, void *data);
void process_chat_packet(snPacketType t, void* data);
int lobby_connect(char *host, char *username, char *password);
void lobby_disconnect();
void lobby_send(snChannel c, snPacketType t, int len, void *data);
void game_send(snChannel c, snPacketType t, int len, void *data);
void os_chat_insert_text(int channel, char *text);
void os_chat_reload_users();