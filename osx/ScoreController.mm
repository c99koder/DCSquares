#import "ScoreController.h"
#import "Scores.h"

#include <stdio.h>

@implementation ScoreController

- (IBAction)getScores:(id)sender
{
		int count=10;
		int index;
    scores = (NSArray*)[ScoresService getScores:@"" in_endDate:@"" in_count:@"15"];
		//printf("Read %i records\n",[scores count]);
		for(index =0; index< [scores count]; index++){
			scoreInfo = (NSDictionary*)[scores objectAtIndex:index];
    	NSString *name = [scoreInfo objectForKey: @"name"];
		}
		[scoreWindow center];
		[scoreWindow makeKeyAndOrderFront:nil];
		[scoreList reloadData];
}

- (int)numberOfRowsInTableView:(NSTableView *)tableView
{
    if(scores == NULL){
        return 0;
    }
    return [scores count];
    
}

-(id)tableView:(NSTableView *)aTableView
objectValueForTableColumn:(NSTableColumn *)aTableColumn
           row:(int)rowIndex
{
    id theValue;
    
		NSDictionary* info = (NSDictionary*)[scores objectAtIndex: rowIndex];
    theValue = [info objectForKey:[aTableColumn identifier]];
    
		return theValue;
}
@end
