#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "MenuViewController.h"

@interface GameOverViewController : MenuViewController {
	IBOutlet UILabel *scoreLabel;
	IBOutlet UILabel *squaresLabel;
	IBOutlet UILabel *comboLabel;
	IBOutlet UILabel *timeLabel;
	IBOutlet UIView *codeView;
	IBOutlet UILabel *codeLabel;
	IBOutlet UIButton *setupAccountButton;
}
@property (nonatomic, retain) UILabel *scoreLabel;
@property (nonatomic, retain) UILabel *squaresLabel;
@property (nonatomic, retain) UILabel *comboLabel;
@property (nonatomic, retain) UILabel *timeLabel;
@property (nonatomic, retain) UIView *codeView;
@property (nonatomic, retain) UILabel *codeLabel;
@property (nonatomic, retain) UIButton *setupAccountButton;
@end
