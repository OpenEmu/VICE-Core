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
#include "machine-video.h"
#include "viewport.h"

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

extern uint32_t* OEvideobuffer;
extern int vid_width, vid_height;

int oe_active_canvas_num = 0;
static int oe_num_screens = 0;
static video_canvas_t *oe_canvaslist[MAX_CANVAS_NUM];
video_canvas_t *oe_active_canvas = NULL;

/** video_arch */

void video_arch_canvas_init(struct video_canvas_s *canvas)
{

    if (oe_num_screens == MAX_CANVAS_NUM) {
        return;
    }

    canvas->video_draw_buffer_callback = NULL;

    if (oe_active_canvas_num == oe_num_screens) {
        oe_active_canvas = canvas;
    }

    canvas->index = oe_num_screens;

    oe_canvaslist[oe_num_screens++] = canvas;
}

void video_canvas_switch(int index)
{
    video_canvas_t *canvas;
    viewport_t *viewport;
    video_canvas_t *currentCanvas;
    viewport_t *currentViewport;

    if (oe_active_canvas_num == index) {
        return;
    }

    if (index >= oe_num_screens) {
        return;
    }

    currentCanvas = oe_active_canvas;
    currentViewport = currentCanvas->viewport;
    currentViewport->update_canvas = 0;


    canvas = oe_canvaslist[index];

    viewport = canvas->viewport;
    viewport->update_canvas = 1;

    oe_active_canvas_num = index;
    oe_active_canvas = canvas;


    video_viewport_resize(canvas, 1);
}

void video_canvas_next(){
    int index = (oe_active_canvas_num + 1) % MAX_CANVAS_NUM;
    video_canvas_switch(index);
}


video_canvas_t*
video_canvas_create(video_canvas_t* canvas,
                    unsigned int *width, unsigned int *height,
                    int mapped)
{

    //canvas->videoconfig->rendermode = VIDEO_RENDER_RGB_1X1;
    canvas->videoconfig->filter = VIDEO_FILTER_NONE;

    // visible width/height seem to be the only sensible values here...
    struct draw_buffer_s *db = canvas->draw_buffer;

    int w = db->visible_width;
    int h = db->visible_height;

    w *= canvas->videoconfig->scalex;
    h *= canvas->videoconfig->scaley;

    // this actually sets the canvas_physical_width/height in canvas->draw_buffer (?!)
    *width = w;
    *height = h;

    canvas->width = w;
    canvas->height = h;
    canvas->depth = 32;
    canvas->pitch = w * 4;

    // init rendering
    video_canvas_set_palette(canvas,canvas->palette);

    return canvas;
}



// VICE wants to change the size of the canvas -> adapt View
void video_canvas_resize(video_canvas_t * canvas, char resize_canvas)
{
   // vid_width  = canvas->draw_buffer->canvas_width;
   // vid_height = canvas->draw_buffer->canvas_height;

    struct draw_buffer_s *db = canvas->draw_buffer;
    int width = db->visible_width;
    int height = db->visible_height;

    width *= canvas->videoconfig->scalex;
    height *= canvas->videoconfig->scaley;

    canvas->width = width;
    canvas->height = height;

    vid_width = width;
    vid_height = height;
}

void video_canvas_refresh(video_canvas_t *canvas,
                          unsigned int xs, unsigned int ys,
                          unsigned int xi, unsigned int yi,
                          unsigned int w, unsigned int h)
{
    xi *= canvas->videoconfig->scalex;
    w *= canvas->videoconfig->scalex;

    yi *= canvas->videoconfig->scaley;
    h *= canvas->videoconfig->scaley;

    w = MIN(w, canvas->width - xi);
    h = MIN(h, canvas->height - yi);

    video_canvas_render(canvas, (uint8_t*)OEvideobuffer, w, h, xs, ys, xi, yi, canvas->pitch, canvas->depth);
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

    int i;
    int rshift = 0;
    int rbits = 0;
    int gshift = 0;
    int gbits = 0;
    int bshift = 0;
    int bbits = 0;
    DWORD rmask = 0;
    DWORD gmask = 0;
    DWORD bmask = 0;

    if (p == NULL) {
        return 0;	/* no palette, nothing to do */
    }

    c->palette = p;

    // set 32bit palette
    for (i = 0; i < p->num_entries; i++)
    {
        DWORD col;

        rbits = 0; rshift = 16; rmask = 0xff;
        gbits = 0; gshift = 8; gmask = 0xff;
        bbits = 0; bshift = 0; bmask = 0xff;

        col = (p->entries[i].red   >> rbits) << rshift
        | (p->entries[i].green >> gbits) << gshift
        | (p->entries[i].blue  >> bbits) << bshift
        | 0xff000000; // alpha

        video_render_setphysicalcolor(c->videoconfig, i, col, c->depth);
    }

    // set colors for palemu
    for (i = 0; i < 256; i++)
    {
        video_render_setrawrgb(i,
                               ((i & (rmask << rbits)) >> rbits) << rshift,
                               ((i & (gmask << gbits)) >> gbits) << gshift,
                               ((i & (bmask << bbits)) >> bbits) << bshift);
    }
    video_render_setrawalpha(0xff000000);
    video_render_initraw(c->videoconfig);
    
    return 0;
//
//    if (!p) {
//        return 0;
//    }
//    
//    c->palette = p;
//    palette_entry_t* pp = &p->entries[0];
//    for (int i=0; i<p->num_entries; i++, pp++) {
//        int col = makecol_depth(c->depth, pp->red, pp->green, pp->blue);
//        video_render_setphysicalcolor(c->videoconfig, i, col, c->depth);
//    }
//    
//    for (int i=0; i<256; i++) {
//        video_render_setrawrgb(i,
//                               makecol_depth(c->depth, i, 0, 0),
//                               makecol_depth(c->depth, 0, i, 0),
//                               makecol_depth(c->depth, 0, 0, i));
//    }
//    
//    return 0;
}
