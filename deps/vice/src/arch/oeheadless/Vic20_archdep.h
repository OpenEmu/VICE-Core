#import <Foundation/Foundation.h>

@protocol Vic20Delegate <NSObject>

- (void)updateAudioBuffer:(int16_t const *)buffer samples:(NSInteger)samples;
- (void)canvasWillResizeWidth:(NSUInteger)width height:(NSUInteger)height;

@end
