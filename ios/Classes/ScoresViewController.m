#import "DCSquaresMobileAppDelegate.h"

@implementation ScoresCell
@synthesize rank,name,score,combo,time;
-(id)initWithFrame:(CGRect)frame reuseIdentifier:(NSString *)identifier {
	if(self = [super initWithFrame:frame reuseIdentifier:identifier]) {
		rank = [[UILabel alloc] initWithFrame:CGRectMake(6,2,26,24)];
		rank.textColor = [UIColor colorWithRed:0 green:122.0f/256.0f blue:1 alpha:1];
		rank.backgroundColor = [UIColor clearColor];
		rank.textAlignment = UITextAlignmentRight;
		[self addSubview: rank];

		name = [[UILabel alloc] initWithFrame:CGRectMake(32,2,210,24)];
		name.textColor = [UIColor colorWithRed:0 green:122.0f/256.0f blue:1 alpha:1];
		name.backgroundColor = [UIColor clearColor];
		[self addSubview: name];
		
		score = [[UILabel alloc] initWithFrame:CGRectMake(260,2,70,24)];
		score.textColor = [UIColor colorWithRed:0 green:122.0f/256.0f blue:1 alpha:1];
		score.backgroundColor = [UIColor clearColor];
		score.textAlignment = UITextAlignmentRight;
		[self addSubview: score];
		
		combo = [[UILabel alloc] initWithFrame:CGRectMake(330,2,70,24)];
		combo.textColor = [UIColor colorWithRed:0 green:122.0f/256.0f blue:1 alpha:1];
		combo.backgroundColor = [UIColor clearColor];
		combo.textAlignment = UITextAlignmentCenter;
		[self addSubview: combo];
		
		time = [[UILabel alloc] initWithFrame:CGRectMake(400,2,50,24)];
		time.textColor = [UIColor colorWithRed:0 green:122.0f/256.0f blue:1 alpha:1];
		time.backgroundColor = [UIColor clearColor];
		time.textAlignment = UITextAlignmentCenter;
		[self addSubview: time];
	}
	return self;
}
-(void)dealloc {
	[super dealloc];
	[rank release];
	[name release];
	[score release];
	[combo release];
	[time release];
}
@end

@implementation ScoresViewController
-(void)viewDidLoad {
	[super viewDidLoad];
	UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0,0,461,26)];
	UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(32,2,210,24)];
	label.textColor = [UIColor whiteColor];
	label.backgroundColor = [UIColor clearColor];
	label.textAlignment = UITextAlignmentLeft;
	label.text = @"Name";
	[view addSubview: label];
	[label release];
	
	label = [[UILabel alloc] initWithFrame:CGRectMake(260,2,70,24)];
	label.textColor = [UIColor whiteColor];
	label.backgroundColor = [UIColor clearColor];
	label.textAlignment = UITextAlignmentCenter;
	label.text = @"Score";
	[view addSubview: label];
	[label release];
	
	label = [[UILabel alloc] initWithFrame:CGRectMake(330,2,70,24)];
	label.textColor = [UIColor whiteColor];
	label.backgroundColor = [UIColor clearColor];
	label.textAlignment = UITextAlignmentCenter;
	label.text = @"Combo";
	[view addSubview: label];
	[label release];
	
	label = [[UILabel alloc] initWithFrame:CGRectMake(400,2,50,24)];
	label.textColor = [UIColor whiteColor];
	label.backgroundColor = [UIColor clearColor];
	label.textAlignment = UITextAlignmentCenter;
	label.text = @"Time";
	[view addSubview: label];
	[label release];
	
	table.tableHeaderView = view;
	[view release];
}
-(void)_fetchScores {
	NSData *plistData;
	NSString *error;
	NSPropertyListFormat format;
	plistData = [NSData dataWithContentsOfURL:[NSURL URLWithString:@"http://dcsquares.c99.org/iphone/scores_plist.php"]];
	[scores release];
	scores = [[NSPropertyListSerialization propertyListFromData:plistData
																					 mutabilityOption:NSPropertyListImmutable
																										 format:&format
																					 errorDescription:&error] retain];
	[activityIndicator stopAnimating];
	[table reloadData];
	[table flashScrollIndicators];
}
-(void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[activityIndicator startAnimating];
	[NSTimer scheduledTimerWithTimeInterval:0.5
																	 target:self
																 selector:@selector(_fetchScores)
																 userInfo:nil
																	repeats:NO];
}
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [scores count];
}
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	return 26;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	ScoresCell *cell = (ScoresCell *)[table dequeueReusableCellWithIdentifier:@"scorecell"];
	if(!cell) cell = [[ScoresCell alloc] initWithFrame:CGRectZero reuseIdentifier:@"scorecell"];
	cell.rank.text = [NSString stringWithFormat:@"%i.", [indexPath row]+1];
	cell.name.text = [[scores objectAtIndex:[indexPath row]] objectForKey:@"name"];
	cell.score.text = [(DCSquaresMobileAppDelegate *)([UIApplication sharedApplication].delegate) formatNumber:[[[scores objectAtIndex:[indexPath row]] objectForKey:@"score"] intValue]];
	cell.combo.text = [[scores objectAtIndex:[indexPath row]] objectForKey:@"combo"];
	cell.time.text = [(DCSquaresMobileAppDelegate *)([UIApplication sharedApplication].delegate) formatTime:[[[scores objectAtIndex:[indexPath row]] objectForKey:@"time"] intValue]];
	return cell;
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
- (void)dealloc {
	[super dealloc];
	[scores release];
}
@end
