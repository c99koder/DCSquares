#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#ifdef MACOS
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <SDL_mixer/SDL_mixer.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef WIN32
//#include "glxext.h"
#else
#include <GL/glext.h>
#endif
#include <SDL/SDL_mixer.h>
#endif
#include "config.h"

extern "C" {
#include "sys.h"
#include "input.h"
}

bool keys[SDLK_LAST];

#define SCREEN_BPP     24

/* Set up some booleans */
#define TRUE  1
#define FALSE 0
bool isActive=1;

/* This is our SDL surface */
SDL_Surface *surface;

bool sys_render_begin() {
  SDL_Event event;
                                                                                
  if(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_ACTIVEEVENT:
        isActive= event.active.gain;
        break;
      case SDL_KEYDOWN:
				printf("key: %c (%i)\n",event.key.keysym.sym,event.key.keysym.sym);
        keys[event.key.keysym.sym]=1;
        if(event.key.keysym.sym==8 || (event.key.keysym.sym>32 && event.key.keysym.sym<=176)) push_key(event.key.keysym.sym);
        break;
      case SDL_KEYUP:
        keys[event.key.keysym.sym]=0;
        break;
      case SDL_QUIT:
        exit(0);
        break;
      default:
        break;
    }
  }
  return 1;//isActive;
}

void sys_render_finish() {
  SDL_GL_SwapBuffers();
}

/* function to release/destroy our resources and restoring the old desktop */
void Quit( int returnCode )
{
    /* clean up the window */
    SDL_Quit( );

    /* and exit appropriately */
    exit( returnCode );
}

/* general OpenGL initialization function */
int initGL( GLvoid )
{
        glEnable(GL_TEXTURE_2D);
        // Enable Texture Mapping
        glShadeModel(GL_SMOOTH);
        // Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                           // Black Background
        glClearDepth(1.0f);
                // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);
        // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);
        // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_TEXTURE_2D);
//        glDisable(GL_KOS_AUTO_UV);
        glDisable(GL_CULL_FACE);


    return( TRUE );
}

void c99_sys_init(char *title) 
{
    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags;
    /* main loop variable */
    int done = FALSE;
    /* used to collect events */
    SDL_Event event;
    /* this holds some info about our display */
    const SDL_VideoInfo *videoInfo;
    /* whether or not the window is active */
    int isActive = TRUE;
	unsigned long delayend;
    /* initialize SDL */
    if ( SDL_Init( SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,1024)<0) {
	delayend=SDL_GetTicks()+2500;
	while(SDL_GetTicks() < delayend);
	if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,1024)<0) {
		printf("Audio initialization failed: %s", Mix_GetError());
		//Quit(1);
	}
}
    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo( );

    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    //videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
//	    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
//	    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 2 );
    /* get a SDL surface */
    surface = SDL_SetVideoMode( VIDEO_X, VIDEO_Y, SCREEN_BPP,
				videoFlags );

    /* Verify there is a surface */
    if ( !surface )
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    Quit( 1 );
	}


    /* initialize OpenGL */
    initGL( );

    SDL_WM_SetCaption(title,title);
	
    SDL_ShowCursor(SDL_DISABLE);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
				glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,640,480,0,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
// glEnable(GL_MULTISAMPLE_ARB);

}
