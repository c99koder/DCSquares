//
//  DCSquaresMobileAppDelegate.h
//  DCSquaresMobile
//
//  Created by Sam Steele on 6/11/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MenuViewController.h"
#import "GameViewController.h"
#import "GameOverViewController.h"
#import "ScoresViewController.h"
#import "Account.h"

@interface DCSquaresMobileAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet MenuViewController *_titleViewController;
	IBOutlet MenuViewController *_howToPlay1ViewController;
	IBOutlet MenuViewController *_howToPlay2ViewController;
	IBOutlet GameViewController *_gameViewController;
	IBOutlet GameOverViewController *_gameOverViewController;
	IBOutlet ScoresViewController *_scoresViewController;
	IBOutlet Account *_accountViewController;
}
@property (nonatomic, retain) UIWindow *window;
- (IBAction)newGame:(id)sender;
- (IBAction)gameOver:(id)sender;
- (IBAction)howToPlay:(id)sender;
- (IBAction)howToPlay2:(id)sender;
- (IBAction)scores:(id)sender;
- (IBAction)returnToTitle:(id)sender;
- (IBAction)setupAccount:(id)sender;
- (IBAction)registerAccount:(id)sender;
- (IBAction)loginAccount:(id)sender;
- (NSString *)formatNumber:(int)number;
- (NSString *)formatTime:(int)seconds;
@end

