//
//  Account.h
//  DCSquaresMobile
//
//  Created by Sam Steele on 1/24/09.
//  Copyright 2009 Last.fm. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface Account : UIViewController {
	IBOutlet UITextField *username;
	IBOutlet UITextField *password;
}
@property (nonatomic, retain) UITextField *username;
@property (nonatomic, retain) UITextField *password;

@end
