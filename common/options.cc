/*
 *  options.cc
 *  DCSquares
 *
 *  Created by Sam Steele on 10/20/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */
#ifdef WIN32
#include <windows.h>
#include <shlobj.h>
#endif
#ifdef MACOS
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
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
#include <dirent.h>
#include <sys/stat.h>
#endif
#ifdef DREAMCAST
#include <kos.h>
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#if defined(DREAMCAST) || defined(SDL)
#include "sys.h"
#endif
#include "texture.h"
#include "options.h"
#include "text.h"
#include "theme.h"

#ifdef DREAMCAST
#include "vmuify.h"
#endif

gameoptions_t gameoptions;
void center(int x, int y,char *text, int point, float fade);
void render_bg(int tex, float fade);
void status(char *text);

extern int menu_tex;
extern themeinfo_t themeinfo;

void scan_directory(char *path, char dir[][256], int *count) {
#ifdef WIN32
    BOOL            fFinished;
    HANDLE          hList;
    TCHAR           szDir[MAX_PATH+1];
    WIN32_FIND_DATA FileData;
		int x=0;

    // Get the proper directory path
    sprintf(szDir, "%s\\*", path);

    // Get the first file
    hList = FindFirstFile(szDir, &FileData);
    if (hList == INVALID_HANDLE_VALUE)
    { 
        printf("No files found\n\n");
    }
    else
    {
        // Traverse through the directory structure
        fFinished = FALSE;
        while (!fFinished)
        {
            // Check the object is a directory or not
            if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if ((strcmp(FileData.cFileName, ".") != 0) &&
(strcmp(FileData.cFileName, "..") != 0))
                {
									strcpy(dir[x++],FileData.cFileName);
                }
            }

            if (!FindNextFile(hList, &FileData))
            {
                if (GetLastError() == ERROR_NO_MORE_FILES)
                {
                    fFinished = TRUE;
                }
            }
        }
    }

    FindClose(hList);
*count=x;
#endif
#ifdef DREAMCAST
  dirent_t *de;
  uint32 d;
	int x=0;
  d = fs_open(path,O_RDONLY|O_DIR); 
  
  while ( (de=fs_readdir(d)) ) {
    if(de->name[0]!='.') strcpy(dir[x++],de->name);
  }
  fs_close(d);
	*count=x;
#endif
#ifdef UNIX
  DIR *dp;
    struct dirent *dir_entry;
    struct stat stat_info;
	int x=0;
	
    if((dp = opendir(path)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    while((dir_entry = readdir(dp)) != NULL) {
        lstat(dir_entry->d_name,&stat_info);
        if(S_ISDIR(stat_info.st_mode)) {
            /* Directory, but ignore . and .. */
            if(strcmp(".",dir_entry->d_name) == 0 ||
                strcmp("..",dir_entry->d_name) == 0)
                continue;
            strcpy(dir[x++],dir_entry->d_name);
        }
    }
    closedir(dp);
		*count=x;
#endif
}

void write_options() {
FILE *fd;
char fn[256];
#ifdef UNIX
sprintf(fn,"%s/.dcsquares",getenv("HOME"));
#endif
#ifdef DREAMCAST
sprintf(fn,"/ram/squares.ini");
#endif
#ifdef WIN32
  SHGetSpecialFolderPathA(NULL,fn,CSIDL_APPDATA,0);
  strcat(fn,"\\squares.ini");
  printf("%s\n",fn);
#endif
unlink(fn);
fd=fopen(fn,"w");
if(!fd) return;
fprintf(fd,"theme=%s\n",gameoptions.theme);
fprintf(fd,"username=%s\n",gameoptions.username);
fprintf(fd,"password=%s\n",gameoptions.password);
fprintf(fd,"net=%i\n",gameoptions.net);
fprintf(fd,"music=%i\n",gameoptions.bgm);
fclose(fd);
#ifdef DREAMCAST
vmuify("/ram/squares.ini","/vmu/a1/squares2.ini","squares2.ini");
#endif
}

void load_options() {
    FILE *f;
	char buf[200];
	char *p=NULL;
	char *val;
	int oops=0;

	strcpy(gameoptions.theme,"default");
  gameoptions.bgm=1;
  gameoptions.username[0]='\0';
  gameoptions.password[0]='\0';
#ifdef DREAMCAST
	gameoptions.net=0;
#else
	gameoptions.net=1;
#endif

#ifdef DREAMCAST
	f=fopen("/vmu/a1/squares2.ini","rb");
	if(f) fseek(f,128+512,SEEK_SET);
#endif
#ifdef UNIX
	sprintf(buf,"%s/.dcsquares",getenv("HOME"));
  f=fopen(buf,"rb");
	if(!f) {
	  sprintf(buf,"%s/.dcscquares",getenv("HOME"));
    f=fopen(buf,"rb");
	  oops=1;
	}
#endif
#ifdef WIN32
  SHGetSpecialFolderPathA(NULL,buf,CSIDL_APPDATA,0);
  strcat(buf,"\\squares.ini");
  printf("%s\n",buf);
  f=fopen(buf,"r");
#endif
	if(!f) {
	  printf("File not found\n");
		return;
	}

	while(fgets(buf,200,f)!=NULL) {
	  p=NULL;
	  strtok(buf,"=");
	  val=strtok(NULL,"\n");
		//printf("%s=%s\n",buf,val);
		if(!strcmp(buf,"net")) {
			p=NULL;
			gameoptions.net=atoi(val);
		}
		if(!strcmp(buf,"music")) {
			p=NULL;
			gameoptions.bgm=atoi(val);
		}
	  if(!strcmp(buf,"theme")) {
			p=gameoptions.theme;
	  }
	  if(!strcmp(buf,"username")) {
			p=gameoptions.username;
	  }
	  if(!strcmp(buf,"password")) {
			p=gameoptions.password;
	  }
	  if(p!=NULL&&val!=NULL) {
	    strcpy(p,val);
	  }
	}
	printf("Closing");
	fclose(f);
#ifdef UNIX
	if(oops) {
	  sprintf(buf,"%s/.dcscquares",getenv("HOME"));
		unlink(buf);
		write_options();
	}
#endif
}

void transbox(int x, int y, int x1, int y1) {
  glLoadIdentity();
	glColor3f(0,0,0);
	glBegin(GL_QUADS);
	glVertex3f(x-4,y-4,0.5);
	glVertex3f(x1+4,y-4,0.5);
	glVertex3f(x1+4,y,0.5);
	glVertex3f(x-4,y,0.5);

	glVertex3f(x-4,y,0.5);
	glVertex3f(x,y,0.5);
	glVertex3f(x,y1,0.5);
	glVertex3f(x-4,y1,0.5);

	glVertex3f(x1,y,0.5);
	glVertex3f(x1+4,y,0.5);
	glVertex3f(x1+4,y1,0.5);
	glVertex3f(x1,y1,0.5);

	glVertex3f(x-4,y1,0.5);
	glVertex3f(x1+4,y1,0.5);
	glVertex3f(x1+4,y1+4,0.5);
	glVertex3f(x-4,y1+4,0.5);
  glEnd();
	glColor4f(0,0,0,0.6);
	glBegin(GL_QUADS);
	glVertex3f(x,y,0.5);
	glVertex3f(x1,y,0.5);
	glVertex3f(x1,y1,0.5);
	glVertex3f(x,y1,0.5);
	glEnd();
}

void line_input(char *buf) {
	char c;
	
	if(kbhit()) {
		c=getch();
		if((c<32 || c > 176) && c!=8) return;
		buf[strlen(buf)+1]='\0';
		buf[strlen(buf)]=c;

		if(c==8&&strlen(buf)>0) {
			buf[strlen(buf)-2]='\0';
		}
	}
}

#if 0

void SETIP(uint8 *dst, uint32 src) {
	dst[0]=(src >> 24) & 0xff;
	dst[1]=(src >> 16) & 0xff;
  dst[2]=(src >> 8) & 0xff;
  dst[3]=(src >> 0) & 0xff;
}

uint32 READIP(char *src) {
	uint32 dst;
	dst = (atoi(strtok(src,".")) << 24);
	dst |= (atoi(strtok(NULL,".")) << 16);
	dst |= (atoi(strtok(NULL,".")) << 8);
	dst |= (atoi(strtok(NULL,".")) << 0);
	return dst;
}

void network_setup() {
	set_font_size(16);
	int y[] = { 165, 190, 215, 240, 265, 305 };
	int sel=0;
  int loop=1;
	int flash=0;
	int rot=0;
	int x,mx,my,lmb=0,oldlmb=1;
  char buf[256];
	char ip[100]="0.0.0.0";
	char bc[100]="0.0.0.0";
	char nm[100]="0.0.0.0";
	char gw[100]="0.0.0.0";
	char dns[100]="0.0.0.0";
	uint8 ip8[4];
	
	SETIP(ip8,lwip_netcfg.ip);
	sprintf(ip,"%i.%i.%i.%i",ip8[0],ip8[1],ip8[2],ip8[3]);
	SETIP(ip8,lwip_netcfg.broadcast);
	sprintf(bc,"%i.%i.%i.%i",ip8[0],ip8[1],ip8[2],ip8[3]);
	SETIP(ip8,lwip_netcfg.netmask);
	sprintf(nm,"%i.%i.%i.%i",ip8[0],ip8[1],ip8[2],ip8[3]);
	SETIP(ip8,lwip_netcfg.gateway);
	sprintf(gw,"%i.%i.%i.%i",ip8[0],ip8[1],ip8[2],ip8[3]);
	SETIP(ip8,lwip_netcfg.dns[0]);
	sprintf(dns,"%i.%i.%i.%i",ip8[0],ip8[1],ip8[2],ip8[3]);
	
	set_font_size(20);
	while(loop==1) {
		if(sys_render_begin()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg(title_tex,1);
#ifdef DREAMCAST
			glKosFinishList();
#else
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			glLoadIdentity();
			transbox(0,0,640,55);
			transbox(145,148,640-145,308);
			transbox(40,380,600,422);
			center(320,46,"Network Setup",40,1);
			//draw_txt(150,165,"Broadband Adapter",1,1,1,1.0,20);
			draw_txt(170,171,"IP Address:",0.8,0.8,0.8,1.0,20);
			draw_txt(170+txt_width("IP Address: "),171,ip,0.9,0.9,0.9,1.0,20);
			draw_txt(170,196,"Broadcast:",0.8,0.8,0.8,1.0,20);
			draw_txt(170+txt_width("Broadcast: "),196,bc,0.9,0.9,0.9,1.0,20);
			draw_txt(170,221,"Netmask:",0.8,0.8,0.8,1.0,20);
			draw_txt(170+txt_width("Netmask: "),221,nm,0.9,0.9,0.9,1.0,20);
			draw_txt(170,246,"Gateway:",0.8,0.8,0.8,1.0,20);
			draw_txt(170+txt_width("Gateway: "),246,gw,0.9,0.9,0.9,1.0,20);
			draw_txt(170,271,"DNS:",0.8,0.8,0.8,1.0,20);
			draw_txt(170+txt_width("DNS: "),271,dns,0.9,0.9,0.9,1.0,20);
			draw_txt(170,310,"Return to Menu",0.8,0.8,0.8,1.0,20);

			center(320,402,"Use the mouse to select an option and click",20,1);
			center(320,422,"to toggle.  Use the keyboard to edit text.",20,1);
				
			glDisable(GL_TEXTURE_2D);
			glColor4f(1,1,1,1);
			glTranslatef(158,y[sel]-8,0.8);
			glRotatef(rot,0,0,1);
			glBegin(GL_QUADS);
			glVertex3f(-6,-6,0);
			glVertex3f(6,-6,0);
			glVertex3f(6,6,0);
			glVertex3f(-6,6,0);
			glEnd();
			if(flash && (sel>=0 && sel<=4)) {
				set_font_size(20);
				glLoadIdentity();
				if(sel==0) glTranslatef(170+txt_width("IP Address: ")+txt_width(ip),y[sel]-8,0.8);
				if(sel==1) glTranslatef(170+txt_width("Broadcast: ")+txt_width(bc),y[sel]-8,0.8);
				if(sel==2) glTranslatef(170+txt_width("Netmask: ")+txt_width(nm),y[sel]-8,0.8);
				if(sel==3) glTranslatef(170+txt_width("Gateway: ")+txt_width(gw),y[sel]-8,0.8);
				if(sel==4) glTranslatef(170+txt_width("DNS: ")+txt_width(dns),y[sel]-8,0.8);
				glBegin(GL_QUADS);
				glVertex3f(0,-8,0);
				glVertex3f(12,-8,0);
				glVertex3f(12,8,0);
				glVertex3f(0,8,0);
				glEnd();
			}
#ifndef MACOS
glLoadIdentity();
  glDisable(GL_TEXTURE_2D);
  glColor4f(0,0,0,0.8);
#ifdef WIN32
  glEnable(GL_BLEND);
#endif
  glBegin(GL_TRIANGLES);
  glVertex3f(mx-2,my-4,0.99);
  glVertex3f(mx+18,my+16,0.99);
  glVertex3f(mx-2,my+24,0.99);
  glEnd();
#ifdef WIN32
  glDisable(GL_BLEND);
#endif
  glColor3f(0,0.6,0.8);
  glBegin(GL_TRIANGLES);
  glVertex3f(mx,my,1.0);
  glVertex3f(mx+15,my+15,1.0);
  glVertex3f(mx,my+21,1.0);
  glEnd();
#endif

			sys_render_finish();			
		}
		
		read_mouse(&mx,&my,&lmb);
		if(mx<0) mx=0;
		if(mx>640) mx=640;
		if(my<0) my=0;
		if(my>480) my=480;
		for(x=0;x<=5;x++) {
		  if(my>y[x]-14 && my<y[x]+6) sel=x;
		}
		if(rot%2==0) {
			switch(poll_game_device(0)) {
				case MOVE_DOWN:
					sel++;
					if(sel>5) sel=5;
					break;
				case MOVE_UP:
					sel--;
					if(sel<0) sel=0;
					break;
				case FIRE_BTN:
					lmb=1;
					break;
			}
		}
		if(lmb&&oldlmb==0) {
		  if(sel==5) loop=0;
		}
		oldlmb=lmb;
		if(sel==0) line_input(ip);
		if(sel==1) line_input(bc);
		if(sel==2) line_input(nm);
		if(sel==3) line_input(gw);
		if(sel==4) line_input(dns);
		rot+=15;
		if(rot>=360) rot=0;
		if(rot%90==0) {flash++; flash%=2; }
		//delay(0.05);
	}
	//for(int x=0; x<2; x++) status("Saving network settings");
	lwip_netcfg.ip=READIP(ip);
	lwip_netcfg.broadcast=READIP(bc);
	lwip_netcfg.netmask=READIP(nm);
	lwip_netcfg.gateway=READIP(gw);
	lwip_netcfg.dns[0]=READIP(dns);
	netcfg_save(&lwip_netcfg);
	lwip_kos_shutdown();
	lwip_kos_init();
}
#endif

void select_options() {
	set_font_size(16);
	int boxx=themeinfo.game_x+100;
	int boxy=themeinfo.game_y+150;
	int boxx1=(themeinfo.game_x+themeinfo.game_w)-100;
#ifdef DREAMCAST
	int boxy1=(themeinfo.game_y+150)+80;
	int y[] = { boxy+40, boxy+60, boxy+100 };
	#define OPTIONS_MENU_END 2
#else
	int boxy1=(themeinfo.game_y+150)+180;
	int y[] = { boxy+40, boxy+60, boxy+100, boxy+120, boxy+140, boxy+180 };
	#define OPTIONS_MENU_END 5
#endif
	char oldtheme[100];
	int sel=0;
  int loop=1;
	int flash=0;
	int rot=0;
	int x,mx=0,my=0,lmb=0,oldlmb=1;
  char buf[256];
	char themes[100][256];
	int themecnt;
	int t=0;
	
	strcpy(oldtheme,gameoptions.theme);
	
	scan_directory("themes",themes,&themecnt);
	set_font_size(22);
	while(loop==1) {
		if(sys_render_begin()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg(menu_tex,1);
#ifdef DREAMCAST
			glKosFinishList();
#else
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			glLoadIdentity();
			transbox(boxx,boxy,boxx1,boxy1);
			center((themeinfo.game_x+themeinfo.game_w)/2,themeinfo.game_y+40,"Options",24,1);
//      sprintf(buf,"oldlmb: %i, lmb: %i",oldlmb,lmb);
//      draw_txt(10,10,buf,1,1,1,1,16);
			draw_txt(boxx+20,boxy+20,"General",1,1,1,1.0,20);
			draw_txt(boxx+40,boxy+40,"Theme:",0.8,0.8,0.8,1.0,18);
			draw_txt(boxx+40+txt_width("Theme: "),boxy+40,gameoptions.theme,0.9,0.9,0.9,1.0,18);
			draw_txt(boxx+40,boxy+60,"Music:",0.8,0.8,0.8,1.0,18);
			draw_txt(boxx+40+txt_width("Music: "),boxy+60,gameoptions.bgm?(char *)"On":(char *)"Off",0.9,0.9,0.9,1.0,18);
#ifndef DREAMCAST
			draw_txt(boxx+20,boxy+80,"DCSquares Online",1,1,1,1.0,20);
			draw_txt(boxx+40,boxy+100,"Username:",0.8,0.8,0.8,1.0,18);
			draw_txt(boxx+40+txt_width("Username: "),boxy+100,gameoptions.username,0.9,0.9,0.9,1.0,18);
			draw_txt(boxx+40,boxy+120,"Password:",0.8,0.8,0.8,1.0,18);
			draw_txt(boxx+40+txt_width("Password: "),boxy+120,gameoptions.password,0.9,0.9,0.9,1.0,18);
			draw_txt(boxx+40,boxy+140,"Internet Scores:",0.8,0.8,0.8,1.0,18);
			draw_txt(boxx+40+txt_width("Internet Scores: "),boxy+140,gameoptions.net?(char *)"On":(char *)"Off",0.9,0.9,0.9,1.0,18);
			draw_txt(boxx+40,boxy+180,"Return to Menu",0.8,0.8,0.8,1.0,18);
#else 
			draw_txt(boxx+40,boxy+100,"Return to Menu",0.8,0.8,0.8,1.0,18);
#endif
			center_shad(themeinfo.game_y+themeinfo.game_h-22,"Use the mouse to select an option and click",16,1);
			center_shad(themeinfo.game_y+themeinfo.game_h-2,"to toggle.  Use the keyboard to edit text.",16,1);
				
			glDisable(GL_TEXTURE_2D);
			glColor4f(1,1,1,1);
			glTranslatef(158,y[sel]-8,0.8);
			glRotatef(rot,0,0,1);
			glBegin(GL_QUADS);
			glVertex3f(-6,-6,0);
			glVertex3f(6,-6,0);
			glVertex3f(6,6,0);
			glVertex3f(-6,6,0);
			glEnd();
			if(flash && (sel==2 || sel==3)) {
				set_font_size(18);
				glLoadIdentity();
				if(sel==2) glTranslatef(boxx+40+txt_width("Username: ")+txt_width(gameoptions.username),y[sel]-8,0.8);
				if(sel==3) glTranslatef(boxx+40+txt_width("Password: ")+txt_width(gameoptions.password),y[sel]-8,0.8);
				glBegin(GL_QUADS);
				glVertex3f(0,-8,0);
				glVertex3f(12,-8,0);
				glVertex3f(12,8,0);
				glVertex3f(0,8,0);
				glEnd();
			}
#ifdef DREAMCAST
glLoadIdentity();
  glDisable(GL_TEXTURE_2D);
  glColor4f(0,0,0,0.8);
  glBegin(GL_TRIANGLES);
  glVertex3f(mx-2,my-4,0.99);
  glVertex3f(mx+18,my+16,0.99);
  glVertex3f(mx-2,my+24,0.99);
  glEnd();
  glColor3f(0,0.6,0.8);
  glBegin(GL_TRIANGLES);
  glVertex3f(mx,my,1.0);
  glVertex3f(mx+15,my+15,1.0);
  glVertex3f(mx,my+21,1.0);
  glEnd();
#endif

			sys_render_finish();			
		}
		
		read_mouse(&mx,&my,&lmb);
		if(mx<0) mx=0;
		if(mx>640) mx=640;
		if(my<0) my=0;
		if(my>480) my=480;
		for(x=0;x<=OPTIONS_MENU_END;x++) {
		  if(my>y[x]-14 && my<y[x]+6) sel=x;
		}
		if(rot%2==0) {
			switch(poll_game_device(0)) {
				case MOVE_DOWN:
					sel++;
					if(sel>OPTIONS_MENU_END) sel=OPTIONS_MENU_END;
					break;
				case MOVE_UP:
					sel--;
					if(sel<0) sel=0;
					break;
				case FIRE_BTN:
					lmb=1;
					break;
			}
		}
		if(lmb&&oldlmb==0) {
			if(sel==0) {
			  if(!strcmp(gameoptions.theme,themes[t])) t++;
				t%=themecnt;
			  strcpy(gameoptions.theme,themes[t]);
				t++;
			}
			if(sel==1) gameoptions.bgm=gameoptions.bgm?0:1;
			if(sel==4) gameoptions.net=gameoptions.net?0:1;
#ifdef DREAMCAST
			//if(sel==5) network_setup();
#endif
		  if(sel==OPTIONS_MENU_END) loop=0;
		}
		oldlmb=lmb;
		if(sel==2) {
			line_input(gameoptions.username);
		}
		if(sel==3) line_input(gameoptions.password);
		rot+=2;
		if(rot>=360) rot=0;
		if(rot%90==0) {flash++; flash%=2; }
		//delay(0.05);
	}
	write_options();
	if(strcmp(gameoptions.theme,oldtheme)) {
		unload_theme();
		load_theme(gameoptions.theme,1);
	}
}
