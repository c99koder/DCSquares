//
//  Square.h
//  UIKitSquares
//
//  Created by Sam Steele on 3/8/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

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

@interface Square : UIImageView {
	CGPoint _speed;
	CGPoint _position;
	float _rotation;
	float _size;
	int _type;
	id _controller;
	BOOL _evil;
}
-(Square *)initWithFrame:(CGRect)frame type:(int)type controller:(id)controller;
-(int)type;
-(void)setSpeed:(CGPoint)speed;
-(void)update:(NSNumber *)seconds;
@end
