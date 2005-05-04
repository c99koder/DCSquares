/*
 *  options.h
 *  DCSquares
 *
 *  Created by Sam Steele on 10/20/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

struct gameoptions_t {
  char theme[100];
  char username[100];
  char password[100];
  bool bgm;
	bool net;
};

void select_options();
void write_options();
void load_options();