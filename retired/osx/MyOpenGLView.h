/* MyOpenGLView */

#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView
{
    IBOutlet NSMenuItem *mnuChallengeMode;
    IBOutlet NSMenuItem *mnuFreePlay;
    IBOutlet Preferences *prefs;
    IBOutlet UpdateChecker *updates;
		NSTimer *timer;
		int mousex,mousey;
}
- (IBAction)onChallengeMode:(id)sender;
- (IBAction)onFreePlay:(id)sender;
- (void) mouseDown: (NSEvent *) theEvent ;
- (void) mouseEntered: (NSEvent *) theEvent ;
- (void) mouseExited: (NSEvent *) theEvent ;
- (void) drawRect: (NSRect) bounds ;
- (void) dealloc;
- (void) reshape;
@end
