#include <3ds.h>
#include <time.h>
#include <string.h>
#include <cmath>
#include <GL/gl.h>
#include <gfx_device.h>

#include "squares.h"
#include "rand.h"
#include "font.h"
#include "hud.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TICKS_PER_SEC (268123480.0f)

GLuint LoadPNG(const u8 *png_data, const u32 size) {
    int width, height, comp;
    unsigned char *data = stbi_load_from_memory(png_data, size, &width, &height, &comp, 4); // 4 forces RGBA components / 4 bytes-per-pixel
    if (data) {
        GLuint texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
        stbi_image_free(data);
        return texId;
    }
    
    return 0;
}

extern const u8 bigsquare_png[];
extern const u32 bigsquare_png_size;
extern const u8 enemy_png[];
extern const u32 enemy_png_size;
extern const u8 evil_png[];
extern const u32 evil_png_size;
extern const u8 game_bg_png[];
extern const u32 game_bg_png_size;
extern const u8 invincible_png[];
extern const u32 invincible_png_size;
extern const u8 minus1000_png[];
extern const u32 minus1000_png_size;
extern const u8 orb_shadow_png[];
extern const u32 orb_shadow_png_size;
extern const u8 player_png[];
extern const u32 player_png_size;
extern const u8 plus1000_png[];
extern const u32 plus1000_png_size;
extern const u8 score_png[];
extern const u32 score_png_size;
extern const u8 slowdown_png[];
extern const u32 slowdown_png_size;
extern const u8 smallsquare_png[];
extern const u32 smallsquare_png_size;
extern const u8 speedup_png[];
extern const u32 speedup_png_size;
extern const u8 logo_png[];
extern const u32 logo_png_size;
extern const u8 stat_bg_png[];
extern const u32 stat_bg_png_size;

int game_tex;
int player_tex;
int score_tex;
int enemy_tex;
int invinc_tex;
int slow_tex;
int mini_tex;
int plus_tex;
int evil_tex;
int speed_tex;
int minus_tex;
int big_tex;
int shadow_tex;
int logo_tex;
int stat_tex;

extern int combo[MAX_PLAYERS];
extern int score[MAX_PLAYERS];
extern int squares[MAX_PLAYERS];
extern int scoreval;
extern float tickval;
extern float power;
extern float speedval;
extern int totalsquares;
extern int powerup_mode;
squarelist *player[MAX_PLAYERS];
int selected_menu_item = -1;
int exitflag=0;
int paused=0;
float game_gt;
extern int bg_tex;
extern int effect_type;
extern float effect_timer;
extern int maxcombo[MAX_PLAYERS];

void LoadGLTextures() {
    game_tex = LoadPNG(game_bg_png, game_bg_png_size);
    player_tex = LoadPNG(player_png, player_png_size);
    score_tex = LoadPNG(score_png, score_png_size);
    enemy_tex = LoadPNG(enemy_png, enemy_png_size);
    invinc_tex = LoadPNG(invincible_png, invincible_png_size);
    slow_tex = LoadPNG(slowdown_png, slowdown_png_size);
    mini_tex = LoadPNG(smallsquare_png, smallsquare_png_size);
    plus_tex = LoadPNG(plus1000_png, plus1000_png_size);
    evil_tex = LoadPNG(evil_png, evil_png_size);
    speed_tex = LoadPNG(speedup_png, speedup_png_size);
    minus_tex = LoadPNG(minus1000_png, minus1000_png_size);
    big_tex = LoadPNG(bigsquare_png, bigsquare_png_size);
    shadow_tex = LoadPNG(orb_shadow_png, orb_shadow_png_size);
    logo_tex = LoadPNG(logo_png, logo_png_size);
    stat_tex = LoadPNG(stat_bg_png, stat_bg_png_size);
}

void draw_menu_item(int position, char const *title) {
    glDisable(GL_TEXTURE_2D);
    glLoadIdentity();
    glTranslatef(20, (position * 40) + ((position+1) * 20) - 10, 0);
    glColor4f(0.0,0.0,0.0,1.0);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0.9);
    glVertex3f(280,0,0.9);
    glVertex3f(280,40,0.9);
    glVertex3f(0,40,0.9);
    glEnd();
    if(selected_menu_item == position)
        glColor4f(0.2,0.2,0.8,1.0);
    else
        glColor4f(0.0,0.0,0.6,1.0);
    glBegin(GL_QUADS);
    glVertex3f(4,4,0.9);
    glVertex3f(276,4,0.9);
    glVertex3f(276,36,0.9);
    glVertex3f(4,36,0.9);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0,1.0,1.0,1.0);
    draw_string(title,140 - ((strlen(title) * 16.0f) / 2.0f),12,1,16);
    glLoadIdentity();
}

void play_game() {
    int frames=0;
    float st=0,gt=0,ot=0,ft=0,fps=0;
    squarelist *c;
    circlePosition cp;

    int x,loop=1;
    int oldx[MAX_PLAYERS],oldy[MAX_PLAYERS];
    
    tickval=1.0f; //1.0
    speedval=1.2f; //1.2
    scoreval=100; //100
    power=0;
    powerup_mode=-1;
    totalsquares=0;
    
    destroy_list();
    
    for(int p=0; p<MAX_PLAYERS; p++) {
        player[p]=create_square(((SCREEN_W/(MAX_PLAYERS+1))*(p+1))-4,(SCREEN_H/2)-4,6,p);
        player[p]->tex = player_tex;
        score[p]=0;
        combo[p]=0;
        squares[p]=0;
        maxcombo[p]=0;
        oldx[p]=-1;
        oldy[p]=-1;
    }
    
    st = svcGetSystemTick() / TICKS_PER_SEC;
    
    while(aptMainLoop() && loop == 1) {
        hidScanInput();
        hidCircleRead(&cp);

        if (hidKeysHeld() & KEY_DLEFT)
            player[0]->xv = -2.0f;
        else if (hidKeysHeld() & KEY_DRIGHT)
            player[0]->xv = 2.0f;
        else
            player[0]->xv = (cp.dx / 128.0f) * 2.0f;
        
        if (hidKeysHeld() & KEY_DUP)
            player[0]->yv = -2.0f;
        else if (hidKeysHeld() & KEY_DDOWN)
            player[0]->yv = 2.0f;
        else
            player[0]->yv = (cp.dy / 128.0f) * -2.0f;
        
        if(paused==0) {
            gt = svcGetSystemTick() / TICKS_PER_SEC - st;
            
            if((gt-ot)>(1.0f / 120.0f)) {
                if(gt>2.0f)
                    add_squares(gt - ot);
                update_squares(gt - ot);
                
                frames++;
                
                //Top screen
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glLoadIdentity();
                render_squares(gt<1?(gt):1.0,true);
                render_effects(gt);
                
                for(int p=0; p < MAX_PLAYERS; p++) {
                    c=check_collide(player[p]);
                    if(c!=NULL && ((c->type == ENEMY && powerup_mode!=INVINC) || (c->type == SCORE && powerup_mode==EVIL))) {
                        loop=0;
                    }
                }
                ft+=(gt-ot);
                if(ft>=1) {
                    fps=float(frames+fps)/2.0f;
                    ft=0;
                    frames=0;
                }
                
                ot = gt;
                gfxFlush(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL), 240, 400, GX_TRANSFER_FMT_RGB8);
                
                //Bottom screen
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glLoadIdentity();
                glDisable(GL_TEXTURE_2D);
                glColor4f(0.8,0.8,0.8,1.0);
                glBegin(GL_QUADS);
                glVertex3f(0,0,0.9);
                glVertex3f(320,0,0.9);
                glVertex3f(320,240,0.9);
                glVertex3f(0,240,0.9);
                glEnd();
                
                render_score(gt);
                
                gfxFlush(gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL), 240, 320, GX_TRANSFER_FMT_RGB8);
                gfxFlushBuffers();
                gfxSwapBuffersGpu();
                gspWaitForVBlank();
            }
        } else {
            //status("Paused");
            //net_update();
        }
    }
    for(int p=0; p<MAX_PLAYERS; p++) {
        if(combo[p]>maxcombo[p]) maxcombo[p]=combo[p];
    }
    game_gt=gt;
    
    st+=gt;
    gt=0;
    ot=0;
    x=player[0]->size;
    
    for(int p=0; p<MAX_PLAYERS; p++) {
        oldx[p]=player[p]->x;
        oldy[p]=player[p]->y;
    }
    
    while(gt <= 0.9f) {
        gt = svcGetSystemTick() / TICKS_PER_SEC - st;
        
        update_squares(gt-ot);
        for(int p=0; p<MAX_PLAYERS; p++) {
            player[p]->size=x+(200.0f * gt);
            player[p]->x=oldx[p];
            player[p]->y=oldy[p];
        }
        
        frames++;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render_squares(1.0f-((gt)/2.0f),true);
        
        ot=gt;
        gfxFlush(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL), 240, 400, GX_TRANSFER_FMT_RGB8);
        gfxFlushBuffers();
        gfxSwapBuffersGpu();
        gspWaitForVBlank();
    }
    
    destroy_list();
}

void title_screen() {
    u64 st=0,gt=0,ot=0;
    
    tickval=1.0f; //1.0
    speedval=1.2f; //1.2
    scoreval=100; //100
    power=0;
    powerup_mode=-1;
    effect_type=-1;
    selected_menu_item=-1;
    touchPosition touch;
    
    st = svcGetSystemTick();
    destroy_list();
    init_genrand(time(NULL));
    
    while(aptMainLoop()) {
        hidScanInput();
        
        if (keysDown() & KEY_START)
            break;
        
        hidTouchRead(&touch);
        if(touch.px > 20 && touch.px < 300) {
            for(int i = 0; i < 4; i++) {
                int top = (i * 40) + ((i+1) * 20) - 10;
                if(touch.py > top && touch.py < top + 40) {
                    selected_menu_item = i;
                    break;
                }
                if(i == 3)
                    selected_menu_item = -1;
            }
        } else {
            switch(selected_menu_item) {
                case 0:
                    play_game();
                    break;
                case 1:
                    break;
                case 2:
                    break;
            }
            if(selected_menu_item == 3)
                break;
            else
                selected_menu_item = -1;
        }

        if(paused == 0) {
            gt = svcGetSystemTick() - st;
            if((gt - ot) / TICKS_PER_SEC >= (1.0f / 120.0f)) {
                add_squares((gt - ot) / TICKS_PER_SEC);
                update_squares((gt - ot) / TICKS_PER_SEC);
                ot = gt;
            }
        
            //Top screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();
            render_squares(0.6);
            render_title();
            gfxFlush(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL), 240, 400, GX_TRANSFER_FMT_RGB8);
            
            //Bottom screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();
            glDisable(GL_TEXTURE_2D);
            glColor4f(0.8,0.8,0.8,1.0);
            glBegin(GL_QUADS);
            glVertex3f(0,0,0.9);
            glVertex3f(320,0,0.9);
            glVertex3f(320,240,0.9);
            glVertex3f(0,240,0.9);
            glEnd();

            draw_menu_item(0, "New Game");
            draw_menu_item(1, "How To Play");
            draw_menu_item(2, "Options");
            draw_menu_item(3, "Quit");

            gfxFlush(gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL), 240, 320, GX_TRANSFER_FMT_RGB8);
            gfxFlushBuffers();
            gfxSwapBuffersGpu();
            gspWaitForVBlank();
        }
    }
    st+=gt;
    gt=0;
    ot=0;
    while(gt <= 0.49f) {
        gt = (clock() / CLOCKS_PER_SEC) - st;
        
        update_squares(gt-ot);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        render_squares(1.0f-((gt)/2.0f));
        glLoadIdentity();
        glColor4f(0.8,0.8,0.8,(gt/0.5));
        glBegin(GL_QUADS);
        glVertex3f(0,0,0.9);
        glVertex3f(640,0,0.9);
        glVertex3f(640,480,0.9);
        glVertex3f(0,480,0.9);
        glEnd();
        gfxFlush(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL), 240, 400, GX_TRANSFER_FMT_RGB8);
        gfxFlushBuffers();
        gfxSwapBuffersGpu();
        gspWaitForVBlank();
        
        ot=gt;
    }
    
    destroy_list();
    powerup_mode = -1;
    power = 0;
}

int main()
{
    gfxInitDefault();
    hidInit();
    romfsInit();
    void* device = gfxCreateDevice(240, 400, 0);
    gfxMakeCurrent(device);
    
    glViewport(0, 0, 240, 400);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 240, 400, 0, -1.0f, 1.0f);
    glTranslatef(240.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    LoadGLTextures();
    font_init();
    
    title_screen();
    
    gfxExit();
    hidExit();
    romfsExit();
    return 0;
}
