/*
 Copyright (c) 2016, OpenEmu Team
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the OpenEmu Team nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY OpenEmu Team ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL OpenEmu Team BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "videoarch.h"
#include "palette.h"
#include "log.h"

extern uint32_t* videoBuffer;
extern int vid_width, vid_height;


/** video_arch */

void video_arch_canvas_init(struct video_canvas_s *canvas)
{
    canvas->video_draw_buffer_callback = NULL;
}

video_canvas_t*
video_canvas_create(video_canvas_t* canvas,
                    unsigned int *width, unsigned int *height,
                    int mapped)
{
    canvas->videoconfig->rendermode = VIDEO_RENDER_RGB_1X1;
    canvas->videoconfig->filter = VIDEO_FILTER_NONE;
    
    int w = canvas->draw_buffer->visible_width;
    int h = canvas->draw_buffer->visible_height;
    
    *width = w;
    *height = h;
    
    canvas->width = w;
    canvas->height = h;
    canvas->depth = 32;
    canvas->pitch = w * 4;
    
    video_canvas_set_palette(canvas, canvas->palette);
    
    return canvas;
}



// VICE wants to change the size of the canvas -> adapt View
void video_canvas_resize(video_canvas_t * canvas, char resize_canvas)
{
    vid_width  = canvas->draw_buffer->canvas_width;
    vid_height = canvas->draw_buffer->canvas_height;
}

void video_canvas_refresh(video_canvas_t *canvas,
                          unsigned int xs, unsigned int ys,
                          unsigned int xi, unsigned int yi,
                          unsigned int w, unsigned int h)
{
    video_canvas_render(canvas, (uint8_t*)videoBuffer, w, h, xs, ys, xi, yi, canvas->pitch, canvas->depth);
}

int makecol_depth(int depth, int r, int g, int b) {
    int rshift = 16;
    int rbits = 0;
    int gshift = 8;
    int gbits = 0;
    int bshift = 0;
    int bbits = 0;
    
    return (r >> rbits) << rshift
        | (g >> gbits) << gshift
        | (b >> bbits) << bshift
        | 0xff000000;
}

int video_canvas_set_palette(video_canvas_t *c, palette_t *p)
{
    if (!p) {
        return 0;
    }
    
    c->palette = p;
    palette_entry_t* pp = &p->entries[0];
    for (int i=0; i<p->num_entries; i++, pp++) {
        int col = makecol_depth(c->depth, pp->red, pp->green, pp->blue);
        video_render_setphysicalcolor(c->videoconfig, i, col, c->depth);
    }
    
    for (int i=0; i<256; i++) {
        video_render_setrawrgb(i,
                               makecol_depth(c->depth, i, 0, 0),
                               makecol_depth(c->depth, 0, i, 0),
                               makecol_depth(c->depth, 0, 0, i));
    }
    
    return 0;
}
