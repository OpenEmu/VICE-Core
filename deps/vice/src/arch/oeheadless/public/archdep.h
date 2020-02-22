#import <Foundation/Foundation.h>

@protocol C64Delegate <NSObject>

- (void)updateAudioBuffer:(int16_t const *)buffer samples:(NSInteger)samples;
- (void)canvasWillResizeWidth:(NSUInteger)width height:(NSUInteger)height;

@end