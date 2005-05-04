/* ScoreController */

#import <Cocoa/Cocoa.h>

@interface ScoreController : NSObject
{
    IBOutlet id scoreList;
		IBOutlet NSWindow *scoreWindow;
		
		NSArray *scores;
		NSDictionary *scoreInfo;
}
- (IBAction)getScores:(id)sender;
@end
