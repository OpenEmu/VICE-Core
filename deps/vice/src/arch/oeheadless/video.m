/**
 * \file video.c
 * \brief Headless UI video stuff
 *
 * \author Marco van den Heuvel <blackystardust68@yahoo.com>
 * \author Michael C. Martin <mcmartin@gmail.com>
 */

/* This file is part of VICE, the Versatile Commodore Emulator.
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

#include "vice.h"

#include <stdio.h>

#include "cmdline.h"
#include "machine.h"
#include "resources.h"
#include "videoarch.h"
#include "video.h"
#import "C64+Private.h"
#include "palette.h"


/** \brief  Command line options related to generic video output
 */
static const cmdline_option_t cmdline_options[] =
{
    CMDLINE_LIST_END
};


/** \brief  Integer/boolean resources related to video output
 */
static const resource_int_t resources_int[] =
{
    RESOURCE_INT_LIST_END
};

/** \brief  Arch-specific initialization for a video canvas
 *  \param[inout] canvas The canvas being initialized
 *  \sa video_canvas_create
 */
void video_arch_canvas_init(struct video_canvas_s *canvas)
{
}


/** \brief  Initialize command line options for generic video resouces
 *
 * \return  0 on success, < 0 on failure
 */
int video_arch_cmdline_options_init(void)
{
    return cmdline_register_options(cmdline_options);
}


/** \brief  Initialize video-related resources
 *
 * \return  0 on success, < on failure
 */
int video_arch_resources_init(void)
{
    return resources_register_int(resources_int);
}

/** \brief Clean up any memory held by arch-specific video resources. */
void video_arch_resources_shutdown(void)
{
}

/** \brief Query whether a canvas is resizable.
 *  \param canvas The canvas to query
 *  \return TRUE if the canvas can be resized.
 */
char video_canvas_can_resize(video_canvas_t *canvas)
{
    return 1;
}

/** \brief Create a new video_canvas_s.
 *  \param[inout] canvas A freshly allocated canvas object.
 *  \param[in]    width  Pointer to a width value. May be NULL if canvas
 *                       size is not yet known.
 *  \param[in]    height Pointer to a height value. May be NULL if canvas
 *                       size is not yet known.
 *  \param        mapped Unused.
 *  \return The completely initialized canvas. The window that holds
 *          it will be visible in the UI at time of return.
 */
video_canvas_t *video_canvas_create(video_canvas_t *canvas,
                                    unsigned int *width, unsigned int *height,
                                    int mapped)
{
    [C64.shared createCanvas:canvas width:width height:height];
    return canvas;
}

/** \brief Free a previously created video canvas and all its
 *         components.
 *  \param[in] canvas The canvas to destroy.
 */
void video_canvas_destroy(struct video_canvas_s *canvas)
{
}

/** \brief Update the display on a video canvas to reflect the machine
 *         state. 
 * \param canvas The canvas to update.
 * \param xs     A parameter to forward to video_canvas_render()
 * \param ys     A parameter to forward to video_canvas_render()
 * \param xi     X coordinate of the leftmost pixel to update
 * \param yi     Y coordinate of the topmost pixel to update
 * \param w      Width of the rectangle to update
 * \param h      Height of the rectangle to update
 */
void video_canvas_refresh(struct video_canvas_s *canvas,
                          unsigned int xs, unsigned int ys,
                          unsigned int xi, unsigned int yi,
                          unsigned int w, unsigned int h)
{
    if (canvas->screen.pixels == NULL) { return; }
    
    video_canvas_render(canvas, (uint8_t *)canvas->screen.pixels, w, h, xs, ys, xi, yi, canvas->screen.pitch, canvas->screen.bpp);
}

/** \brief Update canvas size to match the draw buffer size requested
 *         by the emulation core.
 * \param canvas The video canvas to update.
 * \param resize_canvas Ignored - the canvas will always resize.
 */

void video_canvas_resize(struct video_canvas_s *canvas, char resize_canvas)
{

}

/** \brief Assign a palette to the canvas.
 * \param canvas The canvas to update the palette
 * \param palette The new palette to assign
 * \return Zero on success, nonzero on failure
 */
int video_canvas_set_palette(struct video_canvas_s *canvas,
                             struct palette_s *palette)
{
    canvas->palette = palette;
    
    for (int i = 0; i < palette->num_entries; i++) {
        unsigned int col = palette->entries[i].red<<16 | palette->entries[i].green<<8 | palette->entries[i].blue;
        video_render_setphysicalcolor(canvas->videoconfig, i, col, 32);
    }
    
    for (int i = 0; i < 256; i++) {
        video_render_setrawrgb(i, i, i, i);
    }
    
    video_render_initraw(canvas->videoconfig);
    
    return 0;
}

/** \brief Perform any frontend-specific initialization.
 *  \return 0 on success, nonzero on failure
 */
int video_init(void)
{
    return 0;
}

/** \brief Perform any frontend-specific uninitialization. */
void video_shutdown(void)
{

}
