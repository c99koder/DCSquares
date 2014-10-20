#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "SquaresView.h"

@interface MenuViewController : UIViewController<UIAccelerometerDelegate> {
	IBOutlet SquaresView *_squaresView;
	float speed;
	float _spawnTimer;
	float _ticker, _tickRate;
	NSThread *_updatethd;
}
@property(readwrite) float speed;
@end
