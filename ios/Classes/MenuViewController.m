#import "MenuViewController.h"

@implementation MenuViewController
@synthesize speed;

- (void)viewDidLoad {
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
	self.speed = 2;
	_spawnTimer = 0;

	_updatethd = [[NSThread alloc] initWithTarget:self selector:@selector(_updatethd) object:nil];
	[_updatethd start];
	
	[UIAccelerometer sharedAccelerometer].delegate = self;
}

- (void)viewWillDisappear:(BOOL)animated {
	[_updatethd cancel];
	[_updatethd release];
	_updatethd = nil;
	[[_squaresView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
}

-(void)update:(NSNumber *)seconds {
	_spawnTimer += [seconds doubleValue];
	
	if(_spawnTimer > (1.0 / 1.25)) {
		[_squaresView addSquare:((6 + (rand() % 4)) * 2 * 2) controller:self];
		_spawnTimer = 0;
	}
	[[_squaresView subviews] makeObjectsPerformSelector:@selector(update:) withObject:seconds];
}
- (Square *)player {
	return nil;
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
