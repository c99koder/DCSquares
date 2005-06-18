#import "Preferences.h"
#import "ChatController.h"
#include "net.h"

NSTableView *osTblNames;
NSTextView *osTxtChat;

@implementation ChatController

- (IBAction)chatSend:(id)sender
{
	lobby_send(1,1,[[txtInput stringValue] cString]);
	[txtInput setStringValue:@""];
	[[txtInput window] makeFirstResponder:txtInput];
}

- (void)updateLobby
{
	lobby_update();
}

- (IBAction)onChatConnect:(id)sender
{	
	[wndChat makeKeyAndOrderFront:nil];
	osTblNames = tblNames;
	osTxtChat = txtChat;
	lobby_connect("192.168.11.101",[prefs getUsername],[prefs getPassword]);
	timer = [[NSTimer
		scheduledTimerWithTimeInterval: (1.0f / 10.0f)
														target: self
													selector: @selector( updateLobby )
													userInfo: nil
													 repeats: YES] retain];
}

- (int)numberOfRowsInTableView:(NSTableView *)tableView
{
	return userlist_size();
}

-(id)tableView:(NSTableView *)aTableView
objectValueForTableColumn:(NSTableColumn *)aTableColumn
           row:(int)rowIndex
{
	struct userlist_node *current=get_userlist();
	int count=0;
	
	while(current!=NULL) {
		if(count==rowIndex) {
			return [[NSString alloc] initWithCString:current->username];
		}
		current=current->next;
		count++;
	}
	return nil;
}

@end

void os_chat_insert_text(char *text) {
	[[osTxtChat textStorage] appendAttributedString:[[NSAttributedString alloc] initWithString:[[NSString alloc] initWithCString:text]]];
	[[osTxtChat textStorage] appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
	[osTxtChat scrollLineDown:nil];
}

void os_chat_reload_users() {
	[osTblNames reloadData];
}
