#import "public/C128.h"
#import "videoarch.h"

@interface C128(Private)

- (void)createCanvas:(video_canvas_t *)canvas width:(unsigned int *)width height:(unsigned int *)height;
- (void)canvasDidResize:(video_canvas_t *)canvas;

- (void)initSoundSpeed:(int *)speed fragSize:(int *)fragSize fragNumber:(int *)fragNumber channels:(int *)channels;

@end
