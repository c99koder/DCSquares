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
int lobby_connect(char *host, char *username, char *password);