//
//  Square.m
//  UIKitSquares
//
//  Created by Sam Steele on 3/8/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "DCSquaresMobileAppDelegate.h"
#import "Square.h"

@implementation Square
-(Square *)initWithFrame:(CGRect)frame type:(int)type controller:(id)controller {
	switch(type) {
		case PLAYER1:
			self = [super initWithImage:[UIImage imageNamed:@"player.png"]];
			break;
		case SCORE:
			self = [super initWithImage:[UIImage imageNamed:@"score.png"]];
			break;
		case ENEMY:
			self = [super initWithImage:[UIImage imageNamed:@"enemy.png"]];
			break;
		case POWERUP:
			type = 10+rand()%4;
			frame.size.height = 40;
			frame.size.width = 40;
			switch(type-10) {
				case INVINC:
					self = [super initWithImage:[UIImage imageNamed:@"invincible.png"]];
					break;
				case SLOWMO:
					self = [super initWithImage:[UIImage imageNamed:@"slowdown.png"]];
					break;
				case MINISQUARE:
					self = [super initWithImage:[UIImage imageNamed:@"smallsquare.png"]];
					break;
				case PLUS1000:
					self = [super initWithImage:[UIImage imageNamed:@"plus1000.png"]];
					break;
			}
			break;
		case POWERDOWN:
			type = 14+rand()%4;
			frame.size.height = 40;
			frame.size.width = 40;
			switch(type-10) {
				case EVIL:
					self = [super initWithImage:[UIImage imageNamed:@"evil.png"]];
					break;
				case SPEED:
					self = [super initWithImage:[UIImage imageNamed:@"speedup.png"]];
					break;
				case MINUS1000:
					self = [super initWithImage:[UIImage imageNamed:@"minus1000.png"]];
					break;
				case BIGSQUARE:
					self = [super initWithImage:[UIImage imageNamed:@"bigsquare.png"]];
					break;
			}
	}
	if(self) {
		self.frame = frame;
		_type = type;
		_position = frame.origin;
		_rotation = 0;
		_size = frame.size.width;
		_controller = controller;
		_evil = NO;
	}
	return self;
}
-(int)type {
	return _type;
}
-(void)setSpeed:(CGPoint)speed {
	_speed = speed;
}
-(void)update:(NSNumber *)seconds {
	float speedMod = (_type == PLAYER1)?1.25:1.0;
	
	if([_controller respondsToSelector:@selector(powerType)]) {
		if(((GameViewController *)_controller).powerType == SLOWMO && _type != PLAYER1)
			speedMod = 0.35;

		if(((GameViewController *)_controller).powerType == SPEED && _type != PLAYER1)
			speedMod = 1.75;
		
		if(_type == SCORE) {
			if(((GameViewController *)_controller).powerType == EVIL) {
				if(!_evil) {
					[self setImage:[UIImage imageNamed:@"enemy.png"]];
					_evil = YES;
				}
			} else {
				if(_evil) {
					[self setImage:[UIImage imageNamed:@"score.png"]];
					_evil = NO;
				}
			}
		}
		
		if(_type == ENEMY) {
			if(((GameViewController *)_controller).powerType == INVINC) {
				self.alpha = 0.6;
			} else {
				self.alpha = 1.0;
			}
		}
	}
	
	_position.x+=(_speed.x * [(GameViewController *)_controller speed] * speedMod) * ([seconds doubleValue] * 60.0f);
	_position.y+=(_speed.y * [(GameViewController *)_controller speed] * speedMod) * ([seconds doubleValue] * 60.0f);
	
	if(_type == PLAYER1) {
		if(_position.x < 8) _position.x = 8;
		if((_position.x  + _size) > [[self superview] frame].size.width-8) _position.x = [[self superview] frame].size.width  - _size - 8;
		if(_position.y < 8) _position.y = 8;
		if((_position.y + _size) > [[self superview] frame].size.height-8) _position.y = [[self superview] frame].size.height - _size - 8;
	} else {
		if(_position.x < -_size - 10 || _position.x > [[self superview] frame].size.width + _size + 10 ||
			 _position.y < -_size - 10 || _position.y > [[self superview] frame].size.height + _size + 10) {
			if(((GameViewController *)_controller).player && _type == SCORE)
				((GameViewController *)_controller).combo = 0;
			[self removeFromSuperview];
		}
		if(((GameViewController *)_controller).player && CGRectIntersectsRect([self frame], [((GameViewController *)_controller).player frame])) {
			if([_controller respondsToSelector:@selector(collision:)])
				[(GameViewController *)_controller collision:self];
			[self removeFromSuperview];
		}
	}
	if(_type == PLAYER1) {
		[self setTransform: CGAffineTransformMakeRotation(0)];
		[self setFrame:CGRectMake(_position.x, _position.y, [(GameViewController *)_controller size], [(GameViewController *)_controller size])];
	} else {
		[self setFrame:CGRectMake(_position.x, _position.y, _size, _size)];
	}
	if(_type == PLAYER1) {
		_rotation += 0.08  * ([seconds doubleValue] * 60.0f);
		if(_rotation >= 360) _rotation = 0;
		[self setTransform: CGAffineTransformMakeRotation(_rotation)];
	} 
}
- (void)dealloc {
	[super dealloc];
}
@end
