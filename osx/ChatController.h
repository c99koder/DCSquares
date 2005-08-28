/* ChatController */

#import <Cocoa/Cocoa.h>

@interface ChatController : NSObject
{
    IBOutlet Preferences *prefs;
    IBOutlet NSTableView *tblNames;
    IBOutlet NSTextView *txtChat;
    IBOutlet NSTextField *txtInput;
    IBOutlet NSWindow *wndChat;
		NSTimer *timer;
}
- (IBAction)chatSend:(id)sender;
- (IBAction)onStartGame:(id)sender;
- (IBAction)onChatConnect:(id)sender;
@end
