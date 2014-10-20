#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "MenuViewController.h"

@interface ScoresCell : UITableViewCell {
	IBOutlet UILabel *rank;
	IBOutlet UILabel *name;
	IBOutlet UILabel *score;
	IBOutlet UILabel *combo;
	IBOutlet UILabel *time;
}
@property (nonatomic, retain) UILabel *rank;
@property (nonatomic, retain) UILabel *name;
@property (nonatomic, retain) UILabel *score;
@property (nonatomic, retain) UILabel *combo;
@property (nonatomic, retain) UILabel *time;
@end

@interface ScoresViewController : MenuViewController {
	IBOutlet UITableView *table;
	IBOutlet UIActivityIndicatorView *activityIndicator;
	NSArray *scores;
}
@end
