#import "UpdateChecker.h"

#include <unistd.h>
#include "http.h"

UpdateChecker *updater;

void status(char *text);

@implementation UpdateChecker
- (IBAction)init:(id)sender
{
	updater=self;
}

- (IBAction)checkUpdate:(id)sender
{
	char ct[100];
	int len=0;
	FILE *f;

	status("Checking for updates");
	http_get_file("/tmp/DCS_LATEST","www.dcsquares.c99.org",80,"/updates/LATEST-OSX",ct,&len);
	f=fopen("/tmp/DCS_LATEST","r");
	if(!f) return;
	[[NSUserDefaults standardUserDefaults] setValue:[NSDate date] forKey:@"lastUpdate"];

	fgets(ct,100,f);
	if(strcmp("2.0.0\n",ct)) {
		http_get_file("/tmp/DCS_CHANGES","www.dcsquares.c99.org",80,"/updates/CHANGES-OSX",ct,&len);
		[updateTxt readRTFDFromFile:@"/tmp/DCS_CHANGES"];
		[updateWin center];
		[updateWin makeKeyAndOrderFront:updateWin];
	}
	fclose(f);
	unlink("/tmp/DCS_LATEST");
	unlink("/tmp/DCS_CHANGES");
	status(NULL);
	//exit(0);
}

- (IBAction)CloseBtnClicked:(id)sender
{
	[updateWin orderOut:updateWin];
}

- (IBAction)WebBtnClicked:(id)sender
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://dcsquares.c99.org/download.php"]];
	[updateWin orderOut:updateWin];
}
- (void)autoUpdate
{
	NSDate *last=[[NSUserDefaults standardUserDefaults] objectForKey:@"lastUpdate"];
	NSDate *next;
	NSString *interval=[[NSUserDefaults standardUserDefaults] objectForKey:@"autoUpdate"];
	
	if(last!=nil) {
		if([interval compare:@"Daily"]==NSOrderedSame) {
			next=[last addTimeInterval:(60*60*24)];
		} else if([interval compare:@"Weekly"]==NSOrderedSame) {
			next=[last addTimeInterval:(60*60*24*7)];
		} else if([interval compare:@"Monthly"]==NSOrderedSame) {
			next=[last addTimeInterval:(60*60*24*7*30)];
		} else {
			return;
		}
	}

	if((last==nil) || [next compare:[NSDate date]]==NSOrderedAscending || [next compare:[NSDate date]]==NSOrderedSame) {
		[self checkUpdate:self];
	}
}
@end
