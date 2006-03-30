/***************************************************************************
 *            squares.h
 *
 *  Sat Sep 25 00:50:43 2004
 *  Copyright  2004  chip
 *  chip@njit.lan
 ****************************************************************************/

#ifndef _SQUARES_H
#define _SQUARES_H

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_PLAYERS 4
	
#define PLAYER1 0
#define PLAYER2 1
#define PLAYER3 2
#define PLAYER4 3
#define PLAYER_NET 4
#define SCORE 5
#define ENEMY 6 
#define POWERUP 7
#define POWERDOWN 8
	
#define INVINC 0
#define SLOWMO 1
#define MINISQUARE 2
#define PLUS1000 3
#define EVIL 4
#define SPEED 5
#define MINUS1000 6
#define BIGSQUARE 7
	
struct squarelist {
	float x,y;
	float xv,yv;
	float size;
	float angle;
	float r,g,b;
	int type;
	int tm;
	int id;
	RefPtr<Texture> tex;
	RefPtr<Texture> shadow_tex;
	bool deleted;
	struct squarelist *next;
};

void render_squares(float square_alpha, bool game=false);
void update_squares(float s);
void set_fade_color(float f);
void destroy_list();
float limit(float val, float min, float max);
squarelist *check_collide(squarelist *player);
squarelist *create_square(int x, int y, int size, int type);
squarelist *get_square_by_id(int id);

#ifdef __cplusplus
}
#endif

#endif /* _SQUARES_H */
