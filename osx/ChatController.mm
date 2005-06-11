#import "ChatController.h"
#include "net.h"
@implementation ChatController

- (IBAction)chatSend:(id)sender
{
}

- (IBAction)onChatConnect:(id)sender
{
}

- (int)numberOfRowsInTableView:(NSTableView *)tableView
{
	if(scores == NULL){
		return 0;
	}
	return [scores count];
	
}

-(id)tableView:(NSTableView *)aTableView
objectValueForTableColumn:(NSTableColumn *)aTableColumn
           row:(int)rowIndex
{
	id theValue;
	
		NSDictionary* info = (NSDictionary*)[scores objectAtIndex: rowIndex];
    theValue = [info objectForKey:[aTableColumn identifier]];
    
		return theValue;
}

@end
