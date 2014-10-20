//
//  HUDView.h
//  UIKitSquares
//
//  Created by Sam Steele on 3/9/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface HUDView : UIImageView {
	IBOutlet UILabel *_title;
	IBOutlet UILabel *_value;
}
-(void)setValue:(NSString *)value;
-(NSString *)value;
@end
