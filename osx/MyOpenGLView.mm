#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#ifdef OPENAL
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <stdlib.h>
#include <unistd.h>

#import "Preferences.h"
#import "UpdateChecker.h"
#import "MyOpenGLView.h"
#import "Scores.h"

#include "squares.h"
#include "game.h"
#include "theme.h"
#include "text.h"
#include "hud.h"
#include "texture.h"
#include "score.h"
#include "level.h"
#include "net.h"
#include "rand.h"

#ifdef OPENAL
extern ALuint buffers[6];
extern ALuint sources[6];
ALCdevice *dev = NULL;
ALCcontext *ctx = NULL;
#endif
float square_alpha=1.0;
float fade=0;
squarelist *player=NULL;
int paused=0;
extern int combo;
extern int maxcombo;
extern int score;
extern int highscore;
extern int squares;
extern int scoreval;
extern float tickval;
extern float power;
extern int effect_type;
extern float speedval;
extern int powerup_mode;
extern int gametime;
extern char highcode[20];
extern int bg_tex;
extern int title_tex;
extern int menu_tex;
int state=0;
NSWindow *gamewindow;
float gt=0;

void status(char *msg);

@implementation MyOpenGLView
/* This is the preferred way to determine events */
// if you want to get events sent to you, you must make yourself a first responder
- (BOOL)acceptsFirstResponder
{
    return YES;
}
- (BOOL)becomeFirstResponder
{
    return YES;
}
/*
	Override NSView's initWithFrame: to specify our pixel format:
*/	

- (id) initWithFrame: (NSRect) frame
{
	GLuint attribs[] = 
	{
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAWindow,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFADepthSize, 24,
		NSOpenGLPFAStencilSize, 8,
		NSOpenGLPFAAccumSize, 0,
		0
	};

	NSOpenGLPixelFormat* fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes: (NSOpenGLPixelFormatAttribute*) attribs]; 
	
	if (!fmt)
		NSLog(@"No OpenGL pixel format");

	return self = [super initWithFrame:frame pixelFormat: [fmt autorelease]];
}

- (void) awakeFromNib
{
	//printf("%s\n",getwd(NULL));
	//printf("%s\n",[prefs getUsername]);
	mousex=320;
	mousey=240;
	gamewindow=[self window];
	[[self window]setAcceptsMouseMovedEvents: YES];
	
	timer = [[NSTimer
		scheduledTimerWithTimeInterval: (1.0f / 120.0f)
					target: self
		selector: @selector( updateSquares )
					userInfo: nil
		repeats: YES] retain];
}

- (void) dealloc
{
	[timer invalidate];
	[timer release];
	[super dealloc];
}

- (void) updateSquares
{
	squarelist *c;
	int play=0;
	char tmp[100];

	if(fade>0) {
		fade -= [timer timeInterval];
		if(fade<=0) {
			if(state==0 || state==2) {
				if(state==0) {
					/*if([mnuChallengeMode state] == NSOnState) {
						current_level=level_list_head;
					}
					if([mnuFreePlay state] == NSOnState) {*/
						current_level=free_play;
					//}
					state=1;
				} else {
					if(check_win(gt) == 1) {
						state=1;
						if(current_level->next!=NULL) {
							current_level=current_level->next;
						} else {
							state=0;
						}
					} else {
						state=0;
					}
					if(state==0) {
						[NSCursor unhide];
					}
				}
				if(state!=0) {
					destroy_list();
	#ifdef OPENAL
					alSourceStop( sources[SND_TITLE] );
	#endif
					player=create_square(mousex,mousey,6,PLAYER1);
					square_alpha=1.0;
				}
			} else if(state==1) {
				if(combo > maxcombo) maxcombo = combo;
				if(check_win(gt) == 1) {
					state=2;
				} else {
					state=2;
					if([prefs getAutoSubmit] && strlen([prefs getUsername])>0 && strlen([prefs getPassword])>0) {
						status("Submitting score");
						[ScoresService submitScore:[NSString stringWithCString:[prefs getUsername]] 
						in_password:[NSString stringWithCString:[prefs getPassword]] 
															in_score:score in_combo:maxcombo in_time:gt in_platform:@"Mac" in_squares:squares];
						status(NULL);
						highcode[0]='\0';
					} else {
						int cnt=0;
						do {
							encrypt(genrand_int32()%26,(unsigned char *)build_code(score,squares,maxcombo,0),(unsigned char *)highcode);
							cnt++;
							if(cnt>20) {
								highcode[0]='\0';
								break;
							}
						} while(invalid_code(highcode));
					}
				}
				destroy_list();
				player=NULL;
#ifdef OPENAL
				alSourceStop(sources[SND_BGM]);
#endif
				[NSCursor unhide];
				square_alpha=0.4;
			}
			if(state != 2) {
				tickval=current_level->tickval; //1.0
				speedval=current_level->speedval; //1.2
				scoreval=current_level->scoreval; //100
				score=0;
				combo=0;
				squares=0;
				maxcombo=0;
				gt=0;
			}
			power=0;
			powerup_mode=-1;
			effect_type=-1;
		}
	}

	if(gt>1 && current_level->net==0) add_squares([timer timeInterval]);
	//if(current_level->net==1) lobby_update();
	update_squares([timer timeInterval]);
#ifdef OPENAL
	if([prefs getBgm]) {
		alGetSourcei(sources[(state==1)?1:0], AL_SOURCE_STATE, &play);
		if(play!=AL_PLAYING) {
			alSourcePlay( sources[(state==1)?1:0] );
		}
	}
#endif
	if(state==0) {
		//[updates autoUpdate];
	}
	if(state==1 && fade<=0) {
		c=check_collide(player);
		if(c!=NULL) {
			switch(c->type) {
				case SCORE:
			  case ENEMY:
					if(c->type==SCORE&&powerup_mode!=EVIL) {
#ifdef OPENAL
						if([prefs getSfx]) alSourcePlay( sources[SND_COLLECT] );
#endif
						break;
					}
					if(powerup_mode==INVINC) break;
#ifdef OPENAL
					if([prefs getSfx]) alSourcePlay( sources[SND_GAMEOVER] );
#endif
					fade=1.0;
					break;
				case 10:
				case 11:
				case 12:
				case 13:
#ifdef OPENAL
					if([prefs getSfx]) alSourcePlay( sources[SND_POWERUP] );
#endif
					break;
				case 14:
				case 15:
				case 16:
				case 17:
#ifdef OPENAL
					if([prefs getSfx]) alSourcePlay( sources[SND_POWERDOWN] );
#endif
					break;
				default:
#ifdef OPENAL
					if([prefs getSfx]) alSourcePlay( sources[SND_COLLECT] );
#endif
					break;
			}
		}
		switch(check_win(gt)) {
			case 1: //win
				fade=1.0;
				break;
			case -1: //lose
				fade=1.0;
				break;
		}
	}
	[self setNeedsDisplay:YES];
}

- (void) prepareOpenGL
{
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
	texture_init();
	text_init("Helvetica-Bold.txf",20);
	levels_init();
#ifdef OPENAL
	dev = alcOpenDevice((ALubyte *)getenv("OPENAL_DEVICE"));  // getenv()==NULL is okay.
  if (dev != NULL) {
		ctx = alcCreateContext(dev, 0);
    if (ctx != NULL) {
			alcMakeContextCurrent(ctx);
      alcProcessContext(ctx);
		} // if
	} // if
#endif
	load_theme([prefs getTheme],1);
/*mALBuffer = LoadSoundFromFile( "../file.ogg" );*/
	state=0;
	square_alpha=0.4;
		tickval=1.0f; //1.0
	speedval=1.2f; //1.2
	scoreval=100; //100
	power=0;
	powerup_mode=-1;
	effect_type=-1;
}

- (void) reshape
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,640,480,0,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
	[[self window] center];
	
	[self addTrackingRect:[self bounds] owner:self userData:nil assumeInside:true];
}

- (void) mouseMoved: (NSEvent *) theEvent
{
	mousex=(int)[theEvent locationInWindow].x;
	mousey=480-(int)[theEvent locationInWindow].y;
		
	if((mousex >= 0) && (mousex < 640) && (mousey >= 0) && (mousey < 480)) {
		if(player!=NULL && fade<=0) {
			player->x=mousex;
			player->y=mousey;
		}
	}
}

- (void) mouseEntered: (NSEvent *) theEvent
{
	if(state==1) {
		[NSCursor hide];
	}
}

- (void) mouseExited: (NSEvent *) theEvent
{
	[NSCursor unhide];
}

- (void) mouseDown: (NSEvent *) theEvent
{
	if(state==0 || state==2) {
		fade=0.5;
		if(current_level==NULL) {
			current_level=free_play;
			//current_level=level_list_head;
		}
		[NSCursor hide];
	}
}

- (IBAction)onChallengeMode:(id)sender
{
		[mnuChallengeMode setState:NSOnState];
		[mnuFreePlay setState:NSOffState];
}	

- (IBAction)onFreePlay:(id)sender
{
		[mnuChallengeMode setState:NSOffState];
		[mnuFreePlay setState:NSOnState];
}	

/*
	Override the view's drawRect: to draw our GL content.
*/	 

- (void) drawRect: (NSRect) rect
{
		if(state!=2) gt+=[timer timeInterval];
    glClearColor( 0, 0, 0, 0 ) ;
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ) ;
		if(state==0) render_bg(title_tex,1);
		else if(state==1) render_bg(bg_tex,1); 
		else render_bg(menu_tex,1);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
    render_squares((gt<1?(gt*square_alpha):fade<=0?square_alpha:fade*square_alpha));
		if(state==0) render_title(gt);
		if(state==1) render_score(gt);
		if(state==2) render_win(gt,[timer timeInterval]);
		if(gt<0.5) {
#ifndef DREAMCAST
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			glDisable(GL_TEXTURE_2D);
			glColor4f(0.8,0.8,0.8,1-(gt/0.5));
			glBegin(GL_QUADS);
			glVertex3f(0,0,0.9);
			glVertex3f(640,0,0.9);
			glVertex3f(640,480,0.9);
			glVertex3f(0,480,0.9);
			glEnd();
		}
		if(fade>0) {
#ifndef DREAMCAST
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			glDisable(GL_TEXTURE_2D);
			glColor4f(0.8,0.8,0.8,1-(fade/0.5));
			glBegin(GL_QUADS);
			glVertex3f(0,0,0.9);
			glVertex3f(640,0,0.9);
			glVertex3f(640,480,0.9);
			glVertex3f(0,480,0.9);
			glEnd();
			
			if(state==1) {
				player->size+=2;
			}
		}
		[[self openGLContext] flushBuffer];
}

@end
NSAlert *alert;
void status(char *text) {
	if(text==NULL) {
		[NSApp endSheet:[alert window]];
	} else {
		alert = [[[NSAlert alloc] init] autorelease];
		[alert setMessageText:[NSString stringWithCString:text]];
		[alert setAlertStyle:NSInformationalAlertStyle];

		[alert beginSheetModalForWindow:gamewindow modalDelegate:nil didEndSelector:nil contextInfo:nil];
	}
}

void MessageBox(char *title, char *msg) {
	NSRunAlertPanel([NSString stringWithCString:title],[NSString stringWithCString:msg],@"OK",NULL,NULL);
}
