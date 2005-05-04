/*
 *  score.cc
 *  DCSquares
 *
 *  Created by Sam Steele on 10/21/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */
#ifdef WIN32
#ifndef DIRECTX
#include <windows.h>
#include <shlobj.h>
#else
#include "stdafx.h"
#include "DCSquares-MFC.h"
#endif
#endif
#ifdef MACOS
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

void MessageBox(char *title, char *message);
void status(char *text);
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef SDL
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#ifdef MACOS
#include <SDL_mixer/SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif
#endif
#ifdef UNIX
#include <unistd.h>
#endif
#ifdef DREAMCAST
#include <kos.h>

#include "libmenu.h"
score_table_t * tab=NULL;
#endif
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "text.h"
#include "score.h"
#include "http.h"

struct score_list_node *score_list_head=NULL;

void save_scores() {
	struct score_list_node *current=score_list_head;
	
#ifdef DREAMCAST
	for(int x=0; x<10; x++) {
		if(current!=NULL) {
			strcpy(tab->entries[x].name,current->name);
			tab->entries[x].score = current->score;
			tab->entries[x].difficulty = current->combo;
      tab->entries[x].level_reached = 0;
      tab->entries[x].time_lasted = current->time;
			current=current->next;
		} else {
			strcpy(tab->entries[x].name,"---");
			tab->entries[x].score = 0;
			tab->entries[x].difficulty = 0;
      tab->entries[x].level_reached = 0;
      tab->entries[x].time_lasted = 0;
		}
	}
	
	if(goat_save_score_table(0,tab)<0) perror("save");
#else
	FILE *f;
	char buf[200];

#ifdef UNIX
	sprintf(buf,"%s/.dcsquares-scores",getenv("HOME"));
#endif
#ifdef WIN32
  SHGetSpecialFolderPathA(NULL,buf,CSIDL_APPDATA,0);
  strcat(buf,"\\squares-scores.ini");
  printf("%s\n",buf);
#endif
	f=fopen(buf,"wb");
	if(f) {
		for(int x=0; x<10; x++) {
			if(current!=NULL) {
				fputs(current->name,f);
				sprintf(buf,"\n%i\n",current->score);
				fputs(buf,f);
				sprintf(buf,"%i\n",current->combo);
				fputs(buf,f);
				sprintf(buf,"%i\n",current->time);
				fputs(buf,f);
				sprintf(buf,"%i\n",current->level);
				fputs(buf,f);
				current=current->next;
			} else {
				fputs("---\n0\n0\n0\n0\n",f);
			}
		}
		fclose(f);
	}
#endif
}

void score_list_init() {
	score_list_head=NULL;
#ifndef DREAMCAST
	FILE *f;
	char buf[200];
	char name[8];
	uint32 score,time,combo,level;

#ifdef UNIX
	sprintf(buf,"%s/.dcsquares-scores",getenv("HOME"));
#endif
#ifdef WIN32
  SHGetSpecialFolderPathA(NULL,buf,CSIDL_APPDATA,0);
  strcat(buf,"\\squares-scores.ini");
  printf("%s\n",buf);
#endif
  f=fopen(buf,"rb");
	if(f) {
		for(int x=0; x< 10; x++) {
			fgets(buf,200,f);
			strncpy(name,buf,8);
			if(name[strlen(name)-1]=='\n') {
				name[strlen(name)-1]='\0';
			}
			fgets(buf,200,f);
			score=atoi(buf);
			fgets(buf,200,f);
			combo=atoi(buf);
			fgets(buf,200,f);
			time=atoi(buf);
			fgets(buf,200,f);
			level=atoi(buf);
			if(score>0) score_list_insert(name,score,combo,time,level);
		}
		fclose(f);
	}
#endif
#ifdef DREAMCAST
	tab = goat_load_score_table(0);

	if (!tab) {
		tab = (score_table_t *)malloc(SCORE_TABLE_SIZE_CNT(10));
		tab->settings[0] = 0;
		tab->settings[1] = 0;
		tab->play_cnt = 1;
		tab->bitfield = SCORE_NAME | SCORE_SCORE | SCORE_TIME;
		tab->score_cnt = 10;
	} else {
		for(int x=0; x<tab->score_cnt; x++) {
			if(tab->entries[x].score>0) {
				score_list_insert(tab->entries[x].name,tab->entries[x].score,tab->entries[x].difficulty,tab->entries[x].time_lasted,tab->entries[x].level_reached);
			}
		}
	}
#endif
	/*score_list_insert("Sam",1000,56,1234,0);
	score_list_insert("Brian",1020,58,12344,0);
	score_list_insert("Gil",1060,52,13444,0);
	score_list_insert("Dan",10400,45,124424,0);
	score_list_insert("Jay",108000,76,4364,0);*/
}

int score_list_size() {
	int cnt=0;
	struct score_list_node *current=score_list_head;
	
	while(current!=NULL) {
		cnt++;
		current=current->next;
	}

	return cnt;
}

int score_list_rank(uint32 score) {
	struct score_list_node *current=score_list_head;
	int cnt=0;
	
	while(current!=NULL && current->score > score) {
		cnt++;
		current=current->next;
	}
	
	return cnt+1;
}

void score_list_insert(char *name, uint32 score,uint32 combo,uint32 time,uint32 level) {
	struct score_list_node *current=score_list_head;
	struct score_list_node *prev=NULL;
	struct score_list_node *scorenode=new score_list_node;
	
	strcpy(scorenode->name,name);
	scorenode->score=score;
	scorenode->combo=combo;
	scorenode->time=time;
	scorenode->level=level;
	
	while(current != NULL && current->score > score) {
		//printf("%i %i\n",current->score,score);
		prev=current;
		current=current->next;
	}
	
	scorenode->next=current;
	
	if(prev==NULL) {
		score_list_head=scorenode;
	}
	
	if(prev!=NULL) {
		prev->next=scorenode;
	}
}

char highcode[20]="";
int highscore=1000;

int invalid_code(char *text) {
  for(int x=0;x<strlen(text);x++) {
    if(text[x]=='?' || text[x]=='@') return 1;
  }
  return 0;
}

void encode(int score, char *text) {
  char tmp[30]="";
  char sig[4]="DCS";
  int val;
  //printf("%i\n",score);
  for(int x=0;x<5;x++) {
	text[(x*2)]=sig[x%3];
	val=(int)(score/pow(22,5-x));
    text[(x*2)+1]=(val+'C');
	//printf("%i/%i=%i\n",score,(int)pow(22,4-x),val);
	score-=val*(int)pow(22,5-x);
  }
  text[(5*2)]=sig[5%3];
  text[(5*2)+1]=(score)+'C';
  //printf("%i\n",score);
  text[12]='\0';
  //printf("%s\n",text);
#ifdef MACOS
  strcat(text,"B@");
#endif  
#ifdef WIN32
  strcat(text,"BA");
#endif  
#ifdef DREAMCAST
  strcat(text,"BB");
#endif  
#ifdef LINUX
	strcat(text,"BC");
#endif
  text[14]='\0';
  //printf("%s\n",text);
}

void encrypt(int seed, unsigned char *text, unsigned char *out) {
	int new_seed=0;
	int x;
	strcpy((char *)out,(char *)text);
	for(x=0;x<strlen((char *)out);x++) {
		//printf("%c\n",out[x]);
		out[x]-='?';
		new_seed=out[x];
		out[x]+=seed;
		out[x]%=27;
		seed=out[x];//new_seed;
		out[x]+='?';
	}
	out[x]='\0';
	//printf("%s\n",out);
}

void urlencode(char *c, char *buf) {
	char tmp[3];
	int x;
	
	buf[0]='\0';
	
	for(x=0;x<strlen(c);x++) {
	  switch(c[x]) {
		  case ' ':
			  strcpy(tmp,"%20");
				break;
			default:
			  tmp[0]=c[x];
				tmp[1]='\0';
		}
		strcat(buf,tmp);
	}
}

void submit_code(char *s, char *username, char *password) {
  char buf[512];
	char ct[20];
	int len;
	FILE *f;
	char u[256];
	char p[256];
	int x;
#ifdef DREAMCAST
	char cwd[200];
	strcpy(cwd,fs_getwd());
	printf("%s\n",cwd);
	fs_chdir("/ram");
#endif
	urlencode(username,u);
	urlencode(password,p);
	sprintf(buf,"Submitting code for %s",username);
#ifdef DIRECTX
	theApp.statusDlg.statusTxt.SetWindowText(buf);
	theApp.statusDlg.ShowWindow(SW_SHOW);
	theApp.statusDlg.UpdateWindow();
#else
	//status(buf);
#endif
	sprintf(buf,"/score_post_raw.php?username=%s&password=%s&s=%s",u,p,s);
	//http_get_file("post.tmp","dcsquares.c99.org",80,buf,ct,&len);
	
	f=fopen("post.tmp","rb");
	if(!f) {
		sprintf(buf,"Unable to connect to server");
	} else {
		fgets(buf,200,f);
		fclose(f);
		unlink("post.tmp");
	}
	if(!strcmp("OK",buf)) {
		buf[0]='\0';
	} else if(!strcmp("AUTH ERROR",buf)) {
	    sprintf(buf,"Incorrect username or password");
	}
#ifdef DIRECTX
	theApp.statusDlg.ShowWindow(SW_HIDE);
	if(buf[0]!='\0') {
		MessageBox(NULL,buf,"DCSquares",MB_OK|MB_ICONEXCLAMATION);
	}
#endif
#ifdef MACOS
	status(NULL);
	if(buf[0]!='\0') {
		MessageBox("Unable to submit score",buf);
	}

#endif
#ifdef DREAMCAST
    fs_chdir(cwd);
#endif	
}
