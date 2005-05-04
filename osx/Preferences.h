/* Preferences */

#import <Cocoa/Cocoa.h>

@interface Preferences : NSObject
{
    IBOutlet NSButton *autoSubmit;
    IBOutlet NSButton *bgm;
    IBOutlet NSTextField *password;
    IBOutlet NSWindow *prefsWin;
    IBOutlet NSButton *sfx;
    IBOutlet NSPopUpButton *theme;
    IBOutlet NSPopUpButton *updates;
    IBOutlet NSTextField *username;
}
- (IBAction)onPrefs:(id)sender;
- (IBAction)onThemeChange:(id)sender;
- (IBAction)onMusicChange:(id)sender;
- (char *)getTheme;
- (int)getSfx;
- (int)getBgm;
- (int)getAutoSubmit;
- (char *)getUsername;
- (char *)getPassword;
@end
