/*
 * video.m - MacVICE video interface
 *
 * Written by
 *  Christian Vogelgsang <chris@vogelgsang.org>
 *  Michael Klein <michael.klein@puffin.lb.shuttle.de>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "videoarch.h"
#include "palette.h"
#include "resources.h"
#include "cmdline.h"
#include "translate.h"
#include "log.h"

#include "viewport.h"

// Mac Video Log
//log_t video_log = LOG_ERR;

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

extern uint32_t* OEvideobuffer;
extern int vid_width, vid_height;

int oe_active_canvas_num = 0;
static int oe_num_screens = 0;
static video_canvas_t *oe_canvaslist[MAX_CANVAS_NUM];
video_canvas_t *oe_active_canvas = NULL;

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

extern int vsync_frame_counter;

// ----- Palette Stuff -----

int video_canvas_set_palette(video_canvas_t *c, palette_t *p)
{
    int i;
    int rshift = 0;
    int rbits = 0;
    int gshift = 0;
    int gbits = 0;
    int bshift = 0;
    int bbits = 0;
    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    
    if (p == NULL) {
        return 0;    /* no palette, nothing to do */
    }
    
    c->palette = p;
    
    // set 32bit palette
    for (i = 0; i < p->num_entries; i++)
    {
        uint32_t col;
        
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
}

// ----- Color Stuff -----

int uicolor_alloc_color(unsigned int red, unsigned int green,
                        unsigned int blue, unsigned long *color_pixel,
                        uint8_t *pixel_return)
{
    return 0;
}

void uicolor_convert_color_table(unsigned int colnr, uint8_t *data,
                                 long color_pixel, void *c)
{
}

void uicolor_free_color(unsigned int red, unsigned int green,
                        unsigned int blue, unsigned long color_pixel)
{
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

