//
//  videoarch.h
//  C64C
//
//  Created by Stuart Carnie on 1/7/16.
//  Copyright © 2016 Stuart Carnie. All rights reserved.
//

#ifndef videoarch_h
#define videoarch_h

#include "video.h"

#ifdef __OBJC__
//#import <SpriteKit/SpriteKit.h>
#define ARC __unsafe_unretained
#else
typedef void SKMutableTexture;
#define ARC
#endif

/* custom canvas structure for MacVICE */
struct video_canvas_s
{
    unsigned int initialized;
    unsigned int created;
    unsigned int width, height, depth;
    struct video_render_config_s *videoconfig;
    struct draw_buffer_s *draw_buffer;
    struct viewport_s *viewport;
    struct geometry_s *geometry;
    struct palette_s *palette;
    struct video_resource_chip_s *video_resource_chip;
    struct video_draw_buffer_callback_s *video_draw_buffer_callback;

    // custom here
    int pitch;                  /* pitch width of scanline in byte */
    //ARC SKMutableTexture* tex;
};
typedef struct video_canvas_s video_canvas_t;

#endif /* videoarch_h */
