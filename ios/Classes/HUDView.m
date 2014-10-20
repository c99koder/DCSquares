//
//  HUDView.m
//  UIKitSquares
//
//  Created by Sam Steele on 3/9/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "HUDView.h"

@implementation HUDView
-(void)setValue:(NSString *)value {
	_value.text = value;
}
-(NSString *)value {
	return _value.text;
}
- (void)dealloc {
	[super dealloc];
}

@end
