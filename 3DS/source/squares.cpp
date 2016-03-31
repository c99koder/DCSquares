/***************************************************************************
 *            squares.cc
 *
 *  Sat Sep 25 00:53:25 2004
 *  Copyright  2004  chip
 *  chip@njit.lan
 ****************************************************************************/

#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>

#include "squares.h"
#include "rand.h"
#include "hud.h"

squarelist *squarehead=NULL;
int powerup_mode=-1;

extern float power;
extern int score[MAX_PLAYERS];
extern int combo[MAX_PLAYERS];
extern int maxcombo[MAX_PLAYERS];
extern int game_tex;
extern int score_tex;
extern int enemy_tex;
extern int invinc_tex;
extern int slow_tex;
extern int mini_tex;
extern int plus_tex;
extern int evil_tex;
extern int speed_tex;
extern int minus_tex;
extern int big_tex;
extern int shadow_tex;

extern float speedval;
extern float tickval;

int square_count=0;

float limit(float val, float min, float max) {
    if(val<min) {
        return min;
    } else if(val > max) {
        return max;
    } else {
        return val;
    }
}

void render_bg(int tex, float fade) {
    GLfloat uv[4]={0,0,1,1};
    
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
    glColor3f(fade,fade,fade);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glTexCoord2f(0,uv[0]);
    glVertex3f(0,0,0.0);
    glTexCoord2f(1,uv[1]);
    glVertex3f(SCREEN_W,0,0.0);
    glTexCoord2f(1,uv[2]);
    glVertex3f(SCREEN_W,SCREEN_H,0.0);
    glTexCoord2f(0,uv[3]);
    glVertex3f(0,SCREEN_H,0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void render_poly(int size, int tex, float fade) {
    GLfloat uv[4]={0,0,1,1};
    
    glEnable(GL_TEXTURE_2D);
    glColor4f(1,1,1,fade);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glTexCoord2f(0,uv[0]);
    glVertex3f(-size,-size,0.0);
    glTexCoord2f(1,uv[1]);
    glVertex3f(size,-size,0.0);
    glTexCoord2f(1,uv[2]);
    glVertex3f(size,size,0.0);
    glTexCoord2f(0,uv[3]);
    glVertex3f(-size,size,0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


extern float effect_mode;
extern float effect_timer;
extern int effect_type;

void render_squares(float square_alpha, bool game) {
    squarelist *c=squarehead;
    float dx,dy,i;
    
    render_bg(game_tex,1);
    
    if(effect_type!=-1) {
        switch(effect_type) {
            case INVINC:
                glColor4f(0,1,0,0.4 * effect_timer);
                glBegin(GL_QUADS);
                glVertex3f(0,0,0.0021);
                glVertex3f(640,0,0.0021);
                glVertex3f(640,480,0.0021);
                glVertex3f(0,480,0.0021);
                glEnd();
                break;
            case SLOWMO:
                glColor4f(0,0,0,0.4 * effect_timer);
                glBegin(GL_QUADS);
                glVertex3f(0,0,0.0021);
                glVertex3f(640,0,0.0021);
                glVertex3f(640,480,0.0021);
                glVertex3f(0,480,0.0021);
                glEnd();
                break;
            case EVIL:
                glColor4f(1,0,0,0.4 * effect_timer);
                glBegin(GL_QUADS);
                glVertex3f(0,0,0.0021);
                glVertex3f(640,0,0.0021);
                glVertex3f(640,480,0.0021);
                glVertex3f(0,480,0.0021);
                glEnd();
                break;
            case SPEED:
                glColor4f(1,1,1,0.4 * effect_timer);
                glBegin(GL_QUADS);
                glVertex3f(0,0,0.0021);
                glVertex3f(640,0,0.0021);
                glVertex3f(640,480,0.0021);
                glVertex3f(0,480,0.0021);
                glEnd();
                break;
        }
    }
    while(c!=NULL) {
        if(c->deleted==1) { c=c->next; continue; }
        glLoadIdentity();
        for(i=(c->type>PLAYER_NET && (powerup_mode==SLOWMO || powerup_mode==SPEED))?4:0;i>=0;i--) {
            dx=0;
            dy=0;
            if(c->xv!=0) {
                dx=(i*10) * effect_timer;
                if(c->xv>0) dx*=-1;
            }
            if(c->yv!=0) {
                dy=(i*10) * effect_timer;
                if(c->yv>0) dy*=-1;
            }
            glLoadIdentity();
            glTranslatef(c->x+dx,c->y+dy,0);
            glRotatef(c->angle,0,0,1);
            
            if(c->tex!=-1) {
                if(c->shadow_tex!=-1) {
                    glLoadIdentity();
                    glTranslatef(c->x+2+dx,c->y+2+dy,0.011);
                    glRotatef(c->angle,0,0,1);
                    glColor4f(1.0f,1.0f,1.0f,square_alpha*((float)(8-i)/8.0f));
                    render_poly(c->size,c->shadow_tex,square_alpha*((float)(4-i)/4.0f));
                }
                glLoadIdentity();
                glTranslatef(c->x+dx,c->y+dy,0.012);
                glRotatef(c->angle,0,0,1);
                glColor4f(1.0f,1.0f,1.0f,square_alpha);
                render_poly(c->size,(c->tex==score_tex && powerup_mode==EVIL)?enemy_tex:c->tex,(powerup_mode==INVINC && c->tex==enemy_tex)?1.0f-(0.8f*effect_timer):(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
            }
        }
        c=c->next;
    }
    glLoadIdentity();
}

void update_squares(float s) {
    squarelist *c=squarehead;
    squarelist *p=NULL;
    squarelist *o=NULL;
    float speedmod=1;
    
    square_count=0;
    
    c=squarehead;
    if(powerup_mode==SLOWMO) speedmod=0.5;
    if(powerup_mode==SPEED) speedmod=2;
    
    while(c!=NULL) {
        if(c->deleted==1) { c=c->next; continue; }
        square_count++;
        if(c->xv!=0) c->x+=((float)c->xv*speedmod*s*100);
        if(c->yv!=0) c->y+=((float)c->yv*speedmod*s*100);
        if(c->type<=PLAYER_NET) {
            c->angle+=180*s;
            if(c->angle>360) c->angle-=360;
        }
        if(c->type<PLAYER_NET) {
            if(c->x<0) c->x=0;
            if(c->x>SCREEN_W) c->x=SCREEN_W;
            if(c->y<0) c->y=0;
            if(c->y>SCREEN_H) c->y=SCREEN_H;
        }
        if(c->type>PLAYER_NET) {
            if(c->x<0-c->size || c->x>SCREEN_W+c->size || c->y < 0-c->size || c->y > SCREEN_H+c->size) {
                c->deleted=1;
                if(c->type==SCORE) {
                    for(int p=0; p<MAX_PLAYERS; p++) {
                        if(combo[p] > maxcombo[p]) maxcombo[p]=combo[p];
                        combo[p]=0;
                    }
                }
            }
        }
        c=c->next;
    }
    
    c=squarehead;
    while(c!=NULL) {
        if(c->deleted==1) {
            if(p==NULL) {
                squarehead=squarehead->next;
                o=c;
                c=squarehead;
            } else {
                p->next=c->next;
                o=c;
                c=p;
            }
            delete o;
        }
        if(c!=NULL) {
            p=c;
            c=c->next;
        }
    }
}

squarelist *check_collide(squarelist *player) {
    squarelist *c=squarehead;
    float min_x1, min_y1, min_x2, min_y2;
    float max_x1, max_y1, max_x2, max_y2;
    
    min_x2 = player->x - player->size;
    min_y2 = player->y - player->size;
    max_x2 = player->x + player->size;
    max_y2 = player->y + player->size;
    
    while(c!=NULL) {
        if(c->deleted==1) { c=c->next; continue; }
        
        if(c!=player) {
            min_x1 = c->x - c->size;
            min_y1 = c->y - c->size;
            max_x1 = c->x + c->size;
            max_y1 = c->y + c->size;
            
            if(min_x1 <= max_x2 && max_x1 >= min_x2 && min_y1 <= max_y2 && max_y1 >= min_y2) {
                if(c!=NULL&&c->type>PLAYER_NET) {
                    c->deleted=1;
                    if(c->type==SCORE && powerup_mode!=EVIL) {
                        //c->tm=gt;
                        //sprintf(tmp,"2:g,%i,%i",c->id,c->tm);
                        //net_sendpacket(tmp);
                        give_points(player);
                    }
                    if(powerup_mode!=INVINC && (c->type==ENEMY || (powerup_mode==EVIL && c->type==SCORE))) {
                        //Game over sound effect
                    }
                    if(c->type>=10 && c->type < 14) {
                        //power up sound effect
                        powerup_mode=c->type-10;
                        power=5;
                        if(powerup_mode==PLUS1000) score[player->type]+=1000;
                        if(powerup_mode==MINISQUARE) player->size=6;
                        if(powerup_mode==SLOWMO) {
                        }
                    }
                    if(c->type>=14) {
                        //power down sound effect
                        powerup_mode=c->type-10;
                        if(powerup_mode==MINUS1000) score[player->type]-=1000;
                        if(powerup_mode==BIGSQUARE) player->size=12;
                        power=5;
                    }
                    return c;
                }
            }
        }
        c=c->next;
    }
    return NULL;
}

void destroy_list() {
    squarelist *c=squarehead;
    squarelist *o=NULL;
    
    while(c!=NULL) {
        o=c;
        c=c->next;
        delete o;
    }
    
    squarehead=NULL;
}

float fade_color=0.0;
unsigned int square_pool=0;

void set_fade_color(float f) { fade_color=f; }

squarelist *get_square_by_id(int id) {
    squarelist *c=squarehead;
    
    while(c!=NULL) {
        if(c->id==id) return c;
        c=c->next;
    }
    
    return NULL;
}

squarelist *create_square(int x, int y, int size, int type) {
    squarelist *c=new squarelist;
    
    c->x=x;
    c->y=y;
    c->xv=0;
    c->yv=0;
    if(type<POWERUP) {
        c->size=size;
    } else {
        c->size=size+2;
    }
    c->angle=0;
    c->type=type;
    c->tm=0;
    c->id=square_pool++;
    c->deleted=0;
    c->tex=-1;
    c->shadow_tex=-1;
    switch(type) {
        case SCORE:
            c->tex=score_tex;
            break;
        case ENEMY:
            c->tex=enemy_tex;
            break;
        case POWERDOWN:
            c->type=14+(genrand_int32()%4);
            switch(c->type-10) {
                case EVIL:
                    c->tex=evil_tex;
                    break;
                case SPEED:
                    c->tex=speed_tex;
                    break;
                case MINUS1000:
                    c->tex=minus_tex;
                    break;
                case BIGSQUARE:
                    c->tex=big_tex;
                    break;
            }
            c->shadow_tex=shadow_tex;
            c->size=10;
            break;
        case POWERUP:
            c->type=10+(genrand_int32()%4);
            switch(c->type-10) {
                case INVINC:
                    c->tex=invinc_tex;
                    break;
                case SLOWMO:
                    c->tex=slow_tex;
                    break;
                case PLUS1000:
                    c->tex=plus_tex;
                    break;
                case MINISQUARE:
                    c->tex=mini_tex;
                    break;
            }
            c->shadow_tex=shadow_tex;
            c->size=10;
        default:
            break;
    }
    c->next=NULL;
    
    if(squarehead==NULL) {
        squarehead=c; 
    } else {
        c->next=squarehead;
        squarehead=c;
    }
    
    return c;
}

void add_squares(float st) {
    squarelist *c;
    int max=0;
    int size=0;
    static float oldgt=0;
    static float gt=0;
    
    gt += st;
    
    if(gt-oldgt<(tickval/4.0f)/* || current_level->net*/) {
        return;
    }
    
    oldgt=gt;
    
    if(genrand_int32()%8==0) max=4; else max=2;
    size=8+(genrand_int32()%4);
    
    if(genrand_int32()%((int(10.0f*((tickval>0.6)?tickval:0.6))))==0 || square_count < 5) {
        if(genrand_int32()%2==0) {
            c=create_square(0,(genrand_int32()%(SCREEN_H-size))+size,size,SCORE+(genrand_int32()%max));
            if(c!=NULL) c->xv=(genrand_int32()%2)+speedval;
        } else {
            c=create_square(SCREEN_W,(genrand_int32()%(SCREEN_H-size))+size,size,SCORE+(genrand_int32()%max));
            if(c!=NULL) c->xv=-((genrand_int32()%2)+speedval);
        }
    }
    if(genrand_int32()%((int(10.0f*((tickval>0.6)?tickval:0.6))))==0 || square_count < 5) {
        if(genrand_int32()%2==0) {
            c=create_square((genrand_int32()%(SCREEN_W-size))+size,0,size,SCORE+(genrand_int32()%max));
            if(c!=NULL) c->yv=(genrand_int32()%2)+speedval;
        } else {
            c=create_square((genrand_int32()%(SCREEN_W-size))+size,SCREEN_H,size,SCORE+(genrand_int32()%max));
            if(c!=NULL) c->yv=-((genrand_int32()%2)+speedval);
        }
    }
}
