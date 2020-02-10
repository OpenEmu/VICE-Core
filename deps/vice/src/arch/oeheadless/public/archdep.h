#import <Foundation/Foundation.h>

@protocol C64Delegate <NSObject>

- (void)updateAudioBuffer:(int16_t const *)buffer samples:(NSInteger)samples;

@end