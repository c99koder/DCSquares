/* UpdateChecker */

#import <Cocoa/Cocoa.h>

@interface UpdateChecker : NSObject
{
    IBOutlet id updateTxt;
    IBOutlet id updateWin;
    IBOutlet id webBtn;
}
- (IBAction)checkUpdate:(id)sender;
- (IBAction)CloseBtnClicked:(id)sender;
- (IBAction)init:(id)sender;
- (IBAction)WebBtnClicked:(id)sender;
@end
