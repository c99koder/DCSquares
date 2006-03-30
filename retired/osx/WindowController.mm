#import "WindowController.h"

@implementation WindowController
-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}
- (IBAction)onHowToPlay:(id)sender
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://dcsquares.c99.org/howtoplay.php"]];
}
- (IBAction)onWebsite:(id)sender
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://dcsquares.c99.org/"]];
}
@end
