/*-
 * WSDL stubs for:  http://dcsquares.c99.org/2.0/SOAP/scores.php?wsdl
 *   Generated by:  samsteele
 *           Date:  Sun Apr 17 20:21:41 2005
 *        Emitter:  Objective-C
 */

#ifndef __WSStub__
#define __WSStub__

#import <CoreServices/CoreServices.h>
#import <Foundation/Foundation.h>
#import "WSGeneratedObj.h"

/*-
 *   Web Service:  ScoresService
 * Documentation:
 */
/*-
 *   Method Name:  getHighScore
 * Documentation:  <no documentation>
 */

@interface getHighScore : WSGeneratedObj

// update the parameter list for the invocation.
- (void) setParameters:(SInt32) in_userID;

// result returns an id from the reply dictionary as specified by the WSDL.
- (id) resultValue;

@end; /* getHighScore */


/*-
 *   Method Name:  submitScore
 * Documentation:  <no documentation>
 */

@interface submitScore : WSGeneratedObj

// update the parameter list for the invocation.
- (void) setParameters:(NSString*) in_username in_password:(NSString*) in_password in_score:(SInt32) in_score in_combo:(SInt32) in_combo in_time:(double) in_time in_platform:(NSString*) in_platform in_squares:(SInt32) in_squares;

// result returns an id from the reply dictionary as specified by the WSDL.
- (id) resultValue;

@end; /* submitScore */


/*-
 *   Method Name:  getScores
 * Documentation:  <no documentation>
 */

@interface getScores : WSGeneratedObj

// update the parameter list for the invocation.
- (void) setParameters:(NSString*) in_startDate in_endDate:(NSString*) in_endDate in_count:(NSString*) in_count;

// result returns an id from the reply dictionary as specified by the WSDL.
- (id) resultValue;

@end; /* getScores */


/*-
 *   Method Name:  getRank
 * Documentation:  <no documentation>
 */

@interface getRank : WSGeneratedObj

// update the parameter list for the invocation.
- (void) setParameters:(SInt32) in_userID;

// result returns an id from the reply dictionary as specified by the WSDL.
- (id) resultValue;

@end; /* getRank */


/*-
 * Web Service:  ScoresService
 */
@interface ScoresService : NSObject 

+ (id) getRank:(SInt32) in_userID;
+ (id) getHighScore:(SInt32) in_userID;
+ (id) submitScore:(NSString*) in_username in_password:(NSString*) in_password in_score:(SInt32) in_score in_combo:(SInt32) in_combo in_time:(double) in_time in_platform:(NSString*) in_platform in_squares:(SInt32) in_squares;
+ (id) getScores:(NSString*) in_startDate in_endDate:(NSString*) in_endDate in_count:(NSString*) in_count;

@end;


#endif /* __WSStub__ */
/*-
 * End of WSDL document at http://dcsquares.c99.org/2.0/SOAP/scores.php?wsdl
 */
