//
//  SquaresView.h
//  UIKitSquares
//
//  Created by Sam Steele on 3/8/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Square.h"

@interface SquaresView : UIView {
}
-(id)initWithFrame:(CGRect)frame;
-(void)addSquare:(float)size controller:(id)controller;
@end
