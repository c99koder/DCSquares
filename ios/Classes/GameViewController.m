//
//  GameViewController.m
//  DCSquaresMobile
//
//  Created by Sam Steele on 6/11/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "DCSquaresMobileAppDelegate.h"

@implementation GameViewController
@synthesize score, combo, squares, speed, size, time, player, powerType;
- (void)awakeFromNib {
	AudioServicesCreateSystemSoundID((CFURLRef)[NSURL fileURLWithPath: [[NSBundle mainBundle] pathForResource:@"collect" ofType:@"wav"]], &_scoreSound);
	AudioServicesCreateSystemSoundID((CFURLRef)[NSURL fileURLWithPath: [[NSBundle mainBundle] pathForResource:@"gameover" ofType:@"wav"]], &_gameOverSound);
	AudioServicesCreateSystemSoundID((CFURLRef)[NSURL fileURLWithPath: [[NSBundle mainBundle] pathForResource:@"powerup" ofType:@"wav"]], &_powerUpSound);
	AudioServicesCreateSystemSoundID((CFURLRef)[NSURL fileURLWithPath: [[NSBundle mainBundle] pathForResource:@"powerdown" ofType:@"wav"]], &_powerDownSound);
	[self.view setTransform: CGAffineTransformMakeRotation(M_PI/2.0f)];
	[self.view setCenter:CGPointMake(160,240)];
}

- (void)_updatethd {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSDate *lastDate = [NSDate date];
	NSDate *now = nil;
	while(![[NSThread currentThread] isCancelled]) {
		now = [NSDate date];
		[self performSelectorOnMainThread:@selector(update:) withObject:[NSNumber numberWithDouble:[now timeIntervalSinceDate:lastDate]] waitUntilDone:YES];
		lastDate = now;
		[NSThread sleepForTimeInterval:1.0f / 120.0f];
	}
	[pool release];
}

- (void)viewWillAppear:(BOOL)animated {
	self.score = self.combo = self.squares = 0;
	self.time = 0;
	self.speed = 1.4;
	self.size = 26;
	_scoreval = 100;
	
	_spawnTimer = 0;
	_introTimer = 3;
	_ticker = 0;
	_tickRate = 1.0;
	
	powerType = -1;
	_powerLevel = 0;
	_powerBar.alpha = 0;
	_powerLabel.alpha = 0;
	
	player = [[Square alloc] initWithFrame:CGRectMake([_squaresView frame].size.width/2 - 16,[_squaresView frame].size.height/2 - 16,self.size,self.size) type:PLAYER1 controller:self];
	[_squaresView addSubview: player];
	[UIAccelerometer sharedAccelerometer].updateInterval = 1.0f / 100.0f;
	[UIAccelerometer sharedAccelerometer].delegate = self;
	
	_powerLabel.text = @"Ready?";
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:0.5];
	_powerLabel.alpha = 1;
	[UIView commitAnimations];

	_updatethd = [[NSThread alloc] initWithTarget:self selector:@selector(_updatethd) object:nil];
	[_updatethd start];
}

- (void)viewWillDisappear:(BOOL)animated {
	[_updatethd cancel];
	[_updatethd release];
	_updatethd = nil;
	[[_squaresView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
	player = nil;
}
-(void)update:(NSNumber *)seconds {
	self.time += [seconds doubleValue];
	_spawnTimer += (powerType == SLOWMO)?([seconds doubleValue]/2.0f):[seconds doubleValue];
	_ticker += [seconds doubleValue];
	if(_powerLevel > 0) {
		_powerLevel -= [seconds doubleValue];
		if(_powerLevel < 0.5 && _powerLabel.alpha == 1) {
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 0;
			_powerBar.alpha = 0;
			[UIView commitAnimations];
		}
		if(_powerLevel < 0)
			_powerLevel = 0;
	}
	
	if(_powerLevel == 0)
		powerType = -1;
	
	_introTimer -= [seconds doubleValue];
	if(_introTimer > 0) {
		if(_introTimer < 2.0 && _introTimer > 1.5 && _powerLabel.alpha == 1) {
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 0;
			[UIView commitAnimations];
		}
		if(_introTimer < 1.5 && _introTimer > 1.0 && _powerLabel.alpha == 0) {
			_powerLabel.text = @"Collect the green squares!";
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 1;
			[UIView commitAnimations];
		}
		if(_introTimer < 0.5 && _powerLabel.alpha == 1) {
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 0;
			[UIView commitAnimations];
		}
	}
	
	CGRect frame = _powerBar.frame;
	frame.size.width = 260 * (_powerLevel / 8.0f);
	_powerBar.frame = frame;
	
	if(self.time > 5) {
		if(_spawnTimer > _tickRate) {
			[_squaresView addSquare:((6 + (rand() % 4)) * 2 * 2) controller:self];
			_spawnTimer = 0;
		}
	}
	

	if(self.time > 3) {
		[[_squaresView subviews] makeObjectsPerformSelector:@selector(update:) withObject:seconds];
		if(_ticker > _tickRate) {
			self.score = self.score + 1;
			_ticker = 0;
		}
	}
	[_scoreHUD setValue: [NSString stringWithFormat:@"%i", self.score]];
	if(self.combo > 4) {
		[_comboHUD setValue: [NSString stringWithFormat:@"%i", self.combo]];
	} else {
		[_comboHUD setValue: @""];
	}
[_timeHUD setValue: [(DCSquaresMobileAppDelegate *)([UIApplication sharedApplication].delegate) formatTime: (self.time - 2)]];
}
- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
	UIAccelerationValue x = acceleration.x * -1;
	UIAccelerationValue y = acceleration.y;
	int negx,negy;
	
	if(x>0.15) x=0.15;
	if(x<-0.15) x=-0.15;
	if(y>0.15) y=0.15;
	if(y<-0.15) y=-0.15;
	
	if(x<0) negx = -1; else negx = 1;
	if(y<0) negy = 1; else negy = -1;
	
	float deltaX = (1/cos(8*x) - 1) * negx;
	float deltaY = (1/cos(8*y) - 1) * negy;
	
	[player setSpeed: CGPointMake(deltaY,deltaX)];
}
-(void)collision:(Square *)square {
	int type = [square type];
	if(powerType == INVINC && type == ENEMY)
		type = SCORE;
	if(powerType == EVIL && type == SCORE)
		type = ENEMY;
	
	switch(type) {
		case SCORE:
			self.squares = self.squares + 1;
			if(self.squares % 5 == 0) {
				_scoreval += 20 + (int)(60.0f * (1.0f - _tickRate));
				if(_scoreval > 1000) _scoreval = 1000;
				_tickRate-=0.025;
				if(_tickRate<0.1) _tickRate=0.1;
				self.speed+=0.06;
				if(self.speed>6) self.speed = 6;
				self.size += 2;
				if(self.size > 48) self.size = 48;
			}
			self.combo += 1;
			self.score += _scoreval;
			if(self.combo > 4) self.score += 2*self.combo;
			AudioServicesPlaySystemSound(_scoreSound);
			break;
		case ENEMY:
			AudioServicesPlaySystemSound(_gameOverSound);
			self.time -= 2;
			[(DCSquaresMobileAppDelegate *)[UIApplication sharedApplication].delegate gameOver:self];
			break;
		case INVINC+10:
			powerType = INVINC;
			_powerLevel = 8;
			AudioServicesPlaySystemSound(_powerUpSound);
			_powerBar.backgroundColor = [UIColor greenColor];
			_powerLabel.text = @"Invincibility";
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 1;
			_powerBar.alpha = 1;
			[UIView commitAnimations];
			break;
		case SLOWMO+10:
			powerType = SLOWMO;
			_powerLevel = 8;
			AudioServicesPlaySystemSound(_powerUpSound);
			_powerBar.backgroundColor = [UIColor greenColor];
			_powerLabel.text = @"Slow Motion";
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 1;
			_powerBar.alpha = 1;
			[UIView commitAnimations];
			break;
		case MINISQUARE+10:
			powerType = -1;
			self.size = 26;
			AudioServicesPlaySystemSound(_powerUpSound);
			_powerBar.backgroundColor = [UIColor clearColor];
			_powerLabel.text = @"Mini Square";
			_powerLevel = 2;
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 1;
			[UIView commitAnimations];
			break;
		case PLUS1000+10:
			powerType = -1;
			self.score += 1000;
			AudioServicesPlaySystemSound(_powerUpSound);
			_powerBar.backgroundColor = [UIColor clearColor];
			_powerLabel.text = @"+1000 Points";
			_powerLevel = 2;
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 1;
			[UIView commitAnimations];
			break;
		case EVIL+10:
			powerType = EVIL;
			_powerLevel = 8;
			AudioServicesPlaySystemSound(_powerDownSound);
			_powerBar.backgroundColor = [UIColor redColor];
			_powerLabel.text = @"Evil Squares";
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 1;
			_powerBar.alpha = 1;
			[UIView commitAnimations];
			break;
		case SPEED+10:
			powerType = SPEED;
			_powerLevel = 8;
			AudioServicesPlaySystemSound(_powerDownSound);
			_powerBar.backgroundColor = [UIColor redColor];
			_powerLabel.text = @"Speed Up";
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 1;
			_powerBar.alpha = 1;
			[UIView commitAnimations];
			break;
		case MINUS1000+10:
			powerType = -1;
			self.score -= 1000;
			AudioServicesPlaySystemSound(_powerDownSound);
			_powerBar.backgroundColor = [UIColor clearColor];
			_powerLabel.text = @"-1000 Points";
			_powerLevel = 2;
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 1;
			[UIView commitAnimations];
			break;
		case BIGSQUARE+10:
			powerType = -1;
			self.size = 50;
			AudioServicesPlaySystemSound(_powerDownSound);
			_powerBar.backgroundColor = [UIColor clearColor];
			_powerLabel.text = @"Big Square";
			_powerLevel = 2;
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:0.5];
			_powerLabel.alpha = 1;
			[UIView commitAnimations];
			break;
	}
	[_squaresHUD setValue: [NSString stringWithFormat:@"%i", self.squares]];
	[_scoreHUD setValue: [NSString stringWithFormat:@"%i", self.score]];
	if(self.combo > 4) {
		[_comboHUD setValue: [NSString stringWithFormat:@"%i", self.combo]];
	} else {
		[_comboHUD setValue: @""];
	}
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return NO;
}


- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}


- (void)dealloc {
	[super dealloc];
	[_updatethd release];
}


@end
