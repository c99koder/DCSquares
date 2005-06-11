/* ChatController */

#import <Cocoa/Cocoa.h>

@interface ChatController : NSObject
{
    IBOutlet NSTableView *tblNames;
    IBOutlet NSTextView *txtChat;
    IBOutlet NSTextField *txtInput;
    IBOutlet NSWindow *wndChat;
}
- (IBAction)chatSend:(id)sender;
- (IBAction)onChatConnect:(id)sender;
@end
