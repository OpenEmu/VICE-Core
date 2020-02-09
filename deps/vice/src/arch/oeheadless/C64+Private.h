#import "public/C64.h"
#import "videoarch.h"

@interface C64(Private)

- (void)createCanvas:(video_canvas_t *)canvas width:(unsigned int *)width height:(unsigned int *)height;

@end