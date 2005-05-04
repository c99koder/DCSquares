#ifdef OPENAL
#include <OpenAL/al.h>
#endif
#include <unistd.h>
#include <dirent.h> 
#include <sys/stat.h>

#import "Preferences.h"
#include "theme.h"

#ifdef OPENAL
extern ALuint sources[6];
#endif

@implementation Preferences
- init
{
	self=[super init];
	[[NSUserDefaults standardUserDefaults] registerDefaults:[NSDictionary dictionaryWithContentsOfFile:@"DCSquares.plist"]];
	return self;
}
-(void)awakeFromNib
{
  DIR *dp;
	struct dirent *dir_entry;
	struct stat stat_info;
	int x=0;

	if((dp = opendir("themes")) == NULL) {
			fprintf(stderr,"cannot open directory\n");
			return;  
	} 
	
	[theme removeAllItems];
	
	while((dir_entry = readdir(dp)) != NULL) {
			lstat(dir_entry->d_name,&stat_info);
			if(S_ISDIR(stat_info.st_mode)) {
					/* Directory, but ignore . and .. */
					if(strcmp(".",dir_entry->d_name) == 0 ||
							strcmp("..",dir_entry->d_name) == 0)
							continue;
					[theme addItemWithTitle:[NSString stringWithCString:dir_entry->d_name]];
					//strcpy(dir[x++],dir_entry->d_name);
			}
	} 
	[theme selectItemWithTitle:[[NSUserDefaults standardUserDefaults] objectForKey:@"theme"]];
	closedir(dp);
}

- (IBAction)onPrefs:(id)sender
{
	[prefsWin center];
	[prefsWin makeKeyAndOrderFront:nil];
}
- (IBAction)onThemeChange:(id)sender
{
	unload_theme();
	load_theme((char *)[[theme titleOfSelectedItem] cString],1);
}
- (IBAction)onMusicChange:(id)sender
{
#ifdef OPENAL
	alSourceStop( sources[SND_TITLE] );
#endif
}
- (char *)getTheme
{
	return (char *)[[theme titleOfSelectedItem] cString];
}
- (char *)getUsername
{
	return (char *)[[username stringValue] cString];
}
- (char *)getPassword
{
	return (char *)[[password stringValue] cString];
}
-(int)getBgm
{
	return [bgm state];
}
-(int)getSfx
{
	return [sfx state];
}
-(int)getAutoSubmit
{
	return [autoSubmit state];
}
-(int)getUpdateInterval
{
	return [updates indexOfSelectedItem];
}
@end
