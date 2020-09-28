#import "public/C64dtv.h"
#import "videoarch.h"

@interface C64dtv(Private)

- (void)createCanvas:(video_canvas_t *)canvas width:(unsigned int *)width height:(unsigned int *)height;
- (void)canvasDidResize:(video_canvas_t *)canvas;

- (void)initSoundSpeed:(int *)speed fragSize:(int *)fragSize fragNumber:(int *)fragNumber channels:(int *)channels;

@end
