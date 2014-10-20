//
//  GameViewController.h
//  DCSquaresMobile
//
//  Created by Sam Steele on 6/11/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioServices.h>
#import "SquaresView.h"
#import "HUDView.h"

@interface GameViewController : UIViewController<UIAccelerometerDelegate> {
	IBOutlet SquaresView *_squaresView;
	IBOutlet HUDView *_scoreHUD;
	IBOutlet HUDView *_squaresHUD;
	IBOutlet HUDView *_timeHUD;
	IBOutlet HUDView *_comboHUD;
	IBOutlet UILabel *_powerLabel;
	IBOutlet UIView *_powerBar;
	int score, combo, squares, _scoreval, powerType;
	float speed, size, time, _powerLevel;
	float _spawnTimer, _introTimer;
	float _ticker, _tickRate;
	Square *player;
	NSThread *_updatethd;
	SystemSoundID _scoreSound, _gameOverSound, _powerUpSound, _powerDownSound;
}
@property(readwrite) int score, combo, squares, powerType;
@property(readwrite) float speed, size, time;
@property(readonly) Square *player;
-(void)collision:(Square *)square;
@end
