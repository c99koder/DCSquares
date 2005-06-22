#include <stdlib.h>
#include <stdio.h>
#include "squares.h"
#include "rand.h"
#include "level.h"

extern float speedval;
extern float tickval;

extern int square_count;

void add_squares(float st) {
	squarelist *c;
	int max=0;
	int size=0;
	static float oldgt=0;
	static float gt=0;
	
	gt += st;
	
	if(gt-oldgt<(tickval/4.0f)) {
		return;
	}

	oldgt=gt;

	if(genrand_int32()%8==0) max=4; else max=2;
	//max=4;
	size=6+(genrand_int32()%4);

	if(genrand_int32()%((int(10.0f*((tickval>0.6)?tickval:0.6))))==0 || square_count < 5) {
		if(genrand_int32()%2==0) {
			c=create_square(0,(genrand_int32()%(480-size))+size,size,SCORE+(genrand_int32()%max));
			if(c!=NULL) c->xv=(genrand_int32()%2)+speedval;
		} else {
			c=create_square(640,(genrand_int32()%(480-size))+size,size,SCORE+(genrand_int32()%max));
			if(c!=NULL) c->xv=-((genrand_int32()%2)+speedval);
		}
		//sprintf(tmp,"2:c,%i,%i,%i,%i,%i,%i,%i",c->x, c->y, c->size, c->type, c->id, c->xv, c->yv);
		//net_sendpacket(tmp);			
	} 
	if(genrand_int32()%((int(10.0f*((tickval>0.6)?tickval:0.6))))==0 || square_count < 5) {
		if(genrand_int32()%2==0) {
			c=create_square((genrand_int32()%(640-size))+size,0,size,SCORE+(genrand_int32()%max));
			if(c!=NULL) c->yv=(genrand_int32()%2)+speedval;
		} else {
			c=create_square((genrand_int32()%(640-size))+size,480,size,SCORE+(genrand_int32()%max));
			if(c!=NULL) c->yv=-((genrand_int32()%2)+speedval);
		}
		//sprintf(tmp,"2:c,%i,%i,%i,%i,%i,%i,%i",c->x, c->y, c->size, c->type, c->id, c->xv, c->yv);
		//net_sendpacket(tmp);			
	}
}