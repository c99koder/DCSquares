//
//  DCSquaresMobileAppDelegate.m
//  DCSquaresMobile
//
//  Created by Sam Steele on 6/11/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "DCSquaresMobileAppDelegate.h"
#import "NSString+MD5.h"

@implementation DCSquaresMobileAppDelegate

@synthesize window;
int invalid_code(char *text) {
  for(int x=0;x<strlen(text);x++) {
    if(text[x]=='?' || text[x]=='@') return 1;
  }
  return 0;
}

void encode(int score, int size, char *text) {
  int val,x;
	
  for(x=0;x<size;x++) {
		val=(int)(score/pow(22,size-x));
    text[x]=(val+'C');
		score-=val*(int)pow(22,size-x);
  }
	
  text[x]=(score)+'A';
	text[x+1] = '\0';
}

void dcsencrypt(int seed, char *text, unsigned char *out) {
	int new_seed=0;
	int x=0,y=0;
	
	out[x++]=seed+'A';
	for(y=0;y<strlen((char *)text);y++) {
		out[x]=text[y] - 'A';
		new_seed=out[x];
		out[x]+=seed;
		out[x]%=26;
		seed=new_seed;
		out[x]+='A';
		if(x==4 || x == 10) {
			out[++x] = '-';
		}
		x++;
	}
	out[x]='\0';
}

char *build_code(int score, int squares, int combo, int level) {
	unsigned char tmp[10];
	static unsigned char tmp2[40];
	int x,check=0;
	
	encode(score,5,(char *)tmp);
	strcpy((char*)tmp2,(char *)tmp);
	encode(squares,1,(char *)tmp);
	strcat((char *)tmp2,(char *)tmp);
	encode(combo,1,(char *)tmp);
	strcat((char *)tmp2,(char *)tmp);
	encode(level,1,(char *)tmp);
	strcat((char *)tmp2,(char *)tmp);
	
	if([[UIDevice currentDevice].model isEqualToString:@"iPod"])
		strcat((char *)tmp2,"E");
	else
		strcat((char *)tmp2,"F");
	
	for(x=0;x<strlen((char *)tmp2);x++) {
		check += tmp2[x]-'A';
	}
	check %= 26;
	tmp2[x]=check + 'A';
	tmp2[x+1]='\0';
	
	return (char *)tmp2;
}
- (NSString *)formatNumber:(int)number {
	CFNumberFormatterRef numberFormatter = CFNumberFormatterCreate(NULL, CFLocaleCopyCurrent(), kCFNumberFormatterDecimalStyle);
	NSString *o = (NSString *)CFNumberFormatterCreateStringWithNumber(NULL, numberFormatter, (CFNumberRef)[NSNumber numberWithInt:number]);
	free(numberFormatter);
	NSString *output = [NSString stringWithString:o];
	[o release];
	return output;
}
- (NSString *)formatTime:(int)seconds {
	if(seconds <= 0)
		return @"0:00";
	int h = seconds / 3600;
	int m = (seconds%3600) / 60;
	int s = seconds%60;
	if(h)
		return [NSString stringWithFormat:@"%i:%02i:%02i", h, m, s];
	else
		return [NSString stringWithFormat:@"%i:%02i", m, s];
}
- (void)applicationDidFinishLaunching:(UIApplication *)application {
	[UIApplication sharedApplication].idleTimerDisabled = YES;
	[application setStatusBarHidden:YES];
	srand(time(0));
	[window addSubview:_titleViewController.view];
	[application setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:YES];
}
- (void)dealloc {
	[window release];
	[super dealloc];
}
- (IBAction)newGame:(id)sender {
	[[window subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
	[window addSubview:_gameViewController.view];
}
- (void)submitCode {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	unsigned char code[40] = "\0";
	if(_gameViewController.score > 1000) {
		dcsencrypt(rand()%26, build_code(_gameViewController.score, _gameViewController.squares, _gameViewController.combo, 0), code);
		
		NSString *result = [NSString stringWithContentsOfURL:[NSURL URLWithString:[NSString stringWithFormat:
																																							 @"http://dcsquares.c99.org/iphone/score_post_raw.php?s=%s&username=%@&passwordmd5=%@&time=%f",
																																							 code,
																																							 [[NSUserDefaults standardUserDefaults] objectForKey:@"c99org_user"],
																																							 [[[NSUserDefaults standardUserDefaults] objectForKey:@"c99org_password"] md5sum],
																																								_gameViewController.time]]];
	}
	[pool release];
}
- (IBAction)gameOver:(id)sender {
	unsigned char code[40] = "\0";
	if(_gameViewController.score > 1000)
		dcsencrypt(rand()%26, build_code(_gameViewController.score, _gameViewController.squares, _gameViewController.combo, 0), code);
	
	[[window subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
	[window addSubview:_gameOverViewController.view];
	_gameOverViewController.scoreLabel.text = [NSString stringWithFormat:@"%i", _gameViewController.score];
	_gameOverViewController.squaresLabel.text = [NSString stringWithFormat:@"%i", _gameViewController.squares];
	_gameOverViewController.comboLabel.text = [NSString stringWithFormat:@"%i", _gameViewController.combo];
	_gameOverViewController.timeLabel.text = [self formatTime:_gameViewController.time];
	_gameOverViewController.codeView.hidden = (code[0] == '\0') || [[[NSUserDefaults standardUserDefaults] objectForKey:@"autosubmit"] isEqualToString:@"YES"];
	_gameOverViewController.codeLabel.text = [NSString stringWithUTF8String:(char *)code];
	
	if(code[0] != '\0' && [[[NSUserDefaults standardUserDefaults] objectForKey:@"autosubmit"] isEqualToString:@"YES"]) {
		_gameOverViewController.setupAccountButton.hidden = YES;
		[NSThread detachNewThreadSelector:@selector(submitCode) toTarget:self withObject:nil];
	} else if(![[[NSUserDefaults standardUserDefaults] objectForKey:@"autosubmit"] isEqualToString:@"YES"]) {
		_gameOverViewController.setupAccountButton.hidden = NO;
	}
}
- (IBAction)setupAccount:(id)sender {
	[_gameOverViewController presentModalViewController:_accountViewController animated:YES];
}
- (IBAction)registerAccount:(id)sender {
	NSString *result = [NSString stringWithContentsOfURL:[NSURL URLWithString:[NSString stringWithFormat:
																												@"http://dcsquares.c99.org/iphone/new_user.php?username=%@&password=%@&UDID=%@",
																												_accountViewController.username.text,
																												_accountViewController.password.text,
																																						 [[UIDevice currentDevice] uniqueIdentifier]]]];
	NSLog(@"%@", result);
	if([result isEqualToString:@"OK"]) {
		[[NSUserDefaults standardUserDefaults] setObject:_accountViewController.username.text forKey:@"c99org_user"];
		[[NSUserDefaults standardUserDefaults] setObject:_accountViewController.password.text forKey:@"c99org_password"];
		[[NSUserDefaults standardUserDefaults] setObject:@"YES" forKey:@"autosubmit"];
		[[NSUserDefaults standardUserDefaults] synchronize];
		_gameOverViewController.setupAccountButton.hidden = YES;
		[_gameOverViewController dismissModalViewControllerAnimated:YES];
		UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Registration Successful" message:@"Your account has been created successfully!" delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil] autorelease];
		[alert show];
		[NSThread detachNewThreadSelector:@selector(submitCode) toTarget:self withObject:nil];
	} else if([result isEqualToString:@"FAIL"] || [result length] == 0) {
		UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Registration Failed" message:@"An error occured while creating your account.  Please try again shortly." delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil] autorelease];
		[alert show];
	} else {
		UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Registration Failed" message:result delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil] autorelease];
		[alert show];
	}
}
- (IBAction)loginAccount:(id)sender {
	NSString *result = [NSString stringWithContentsOfURL:[NSURL URLWithString:[NSString stringWithFormat:
																																						 @"http://dcsquares.c99.org/iphone/auth_test.php?username=%@&password=%@&UDID=%@",
																																						 _accountViewController.username.text,
																																						 _accountViewController.password.text,
																																						 [[UIDevice currentDevice] uniqueIdentifier]]]];
	NSLog(@"%@", result);
	if([result isEqualToString:@"OK"]) {
		[[NSUserDefaults standardUserDefaults] setObject:_accountViewController.username.text forKey:@"c99org_user"];
		[[NSUserDefaults standardUserDefaults] setObject:_accountViewController.password.text forKey:@"c99org_password"];
		[[NSUserDefaults standardUserDefaults] setObject:@"YES" forKey:@"autosubmit"];
		[[NSUserDefaults standardUserDefaults] synchronize];
		_gameOverViewController.setupAccountButton.hidden = YES;
		[_gameOverViewController dismissModalViewControllerAnimated:YES];
		UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Login Successful" message:@"You have successfully logged into DCSquares Online on this device." delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil] autorelease];
		[alert show];
		[NSThread detachNewThreadSelector:@selector(submitCode) toTarget:self withObject:nil];
	} else {
		UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Login Failed" message:@"Check your username and password and try again." delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil] autorelease];
		[alert show];
	}
}
- (IBAction)howToPlay:(id)sender {
	[[window subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
	[window addSubview:_howToPlay1ViewController.view];
}
- (IBAction)howToPlay2:(id)sender {
	[[window subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
	[window addSubview:_howToPlay2ViewController.view];
}
- (IBAction)returnToTitle:(id)sender {
	[[window subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
	[window addSubview:_titleViewController.view];
}
- (IBAction)scores:(id)sender {
	[[window subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
	[window addSubview:_scoresViewController.view];
}
@end
