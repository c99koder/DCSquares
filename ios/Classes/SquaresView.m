//
//  SquaresView.m
//  UIKitSquares
//
//  Created by Sam Steele on 3/8/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "SquaresView.h"

@implementation SquaresView
-(id)initWithFrame:(CGRect)frame {
	if(self = [super initWithFrame:frame]) {
		self.contentMode = UIViewContentModeCenter;
	}
	return self;
}
-(void)addSquare:(float)size controller:(id)controller {
	Square *square;
	int max;
	
	if(rand()%6==0) max=4; else max=2;
	
	switch(rand() % 4) {
		case 0:
			square = [[Square alloc] initWithFrame:CGRectMake(-size,size+(rand()%(int)([self frame].size.height-(size * 2))),size,size) type:SCORE+rand()%max controller:controller];
			[square setSpeed:CGPointMake(1,0)];
			break;
		case 1:
			square = [[Square alloc] initWithFrame:CGRectMake([self frame].size.width + size,size+(rand()%(int)([self frame].size.height-(size * 2))),size,size) type:SCORE+rand()%max controller:controller];
			[square setSpeed:CGPointMake(-1,0)];
			break;
		case 2:
			square = [[Square alloc] initWithFrame:CGRectMake(size+(rand()%(int)([self frame].size.width-(size * 2))),-size,size,size) type:SCORE+rand()%max controller:controller];
			[square setSpeed:CGPointMake(0,1)];
			break;
		case 3:
			square = [[Square alloc] initWithFrame:CGRectMake(size+(rand()%(int)([self frame].size.width-(size * 2))),[self frame].size.height + size, size, size) type:SCORE+rand()%max controller:controller];
			[square setSpeed:CGPointMake(0,-1)];
			break;
	}
	[self addSubview:[square autorelease]];
}
- (void)dealloc {
	[super dealloc];
}
@end
