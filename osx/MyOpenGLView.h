/* MyOpenGLView */

#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView
{
	NSTimer *timer;
	int state;
	IBOutlet Preferences *prefs;
	IBOutlet UpdateChecker *updates;
	int mousex,mousey;
}

- (void) mouseDown: (NSEvent *) theEvent ;
- (void) mouseEntered: (NSEvent *) theEvent ;
- (void) mouseExited: (NSEvent *) theEvent ;
- (void) drawRect: (NSRect) bounds ;
- (void) dealloc;
- (void) reshape;
@end
