/*
 *  squarenet.h
 *  dcsqd
 *
 *  Created by Sam Steele on 2/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifdef WIN32
#pragma pack(1)
#define __PACKED__
#else
#define __PACKED__ __attribute__((__packed__));
#endif

enum snChannel { CHAN_SERVER, CHAN_CHAT, CHAN_GAME };
enum snPacketType { 
	SERVER_VERSION,
	SERVER_AUTH,
	SERVER_ERROR,
	SERVER_MSG,
	CHAT_INFO,
	CHAT_JOIN,
	CHAT_MSG,
	CHAT_PART,
	GAME_CHALLENGE,
	GAME_START,
	GAME_ADD,
	GAME_PLAYERMOVE,
	GAME_WIN,
	GAME_LOSE
};

struct snPacket {
	snChannel chan;
	snPacketType type;
	char data[120];
} __PACKED__;

struct snServerVersion {
	char protoVersion;
} __PACKED__;

struct snAuth {
	char user[16];
	char pass[16];
} __PACKED__;

struct snServerMsg {
	char msg[120];
} __PACKED__;

struct snChatMsg {
	char user[16];
	char msg[100];
} __PACKED__;

struct snChatJoin {
	char user[16];
} __PACKED__;

struct snChatPart {
	char user[16];
} __PACKED__;

struct snGameChallenge {
	int gameid;
	char user[16];
	int squares;
	int score;
	int time;
	int win_mode;
	int lose_mode;
	int accept;
} __PACKED__;

struct snGameStart {
	int gameid;
	char host[20];
	int r, g, b;
} __PACKED__;

struct snGamePlayerMove {
	int x,y;
} __PACKED__;

struct snGameAdd {
	float x,y;
	float xv,yv;
	float size;
	int type;
	int tm;
	int id;
} __PACKED__;

enum infoType_t { chatInfoUser, chatInfoTopic };

struct snChatInfo {
	infoType_t infoType;
	char data[100];
} __PACKED__;