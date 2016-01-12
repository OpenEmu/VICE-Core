#include "videoarch.h"
#include "palette.h"
#include "log.h"

// dummy
int video_arch_cmdline_options_init(void)
{
    return 0;
}

int video_arch_resources_init(void)
{
    return 0;
}

void video_arch_resources_shutdown(void)
{
}

int video_init_cmdline_options(void)
{
    return 0;
}


// Mac Video Log
log_t video_log = LOG_ERR;

int video_init(void)
{
    if (video_log == LOG_ERR)
        video_log = log_open("MacVideo");
    
    log_message(video_log, "initialized");
    
    return 0;
}

void video_shutdown(void)
{
    if (video_log != LOG_ERR)
        log_close(video_log);
}

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
    canvas->videoconfig->filter = VIDEO_FILTER_NONE;
    return canvas;
}

void video_canvas_destroy(video_canvas_t *canvas)
{
}

char video_canvas_can_resize(video_canvas_t *canvas)
{
    return 1;
}

// VICE wants to change the size of the canvas -> adapt View
void video_canvas_resize(video_canvas_t * canvas, char resize_canvas)
{
}

void video_canvas_refresh(video_canvas_t *canvas,
                          unsigned int xs, unsigned int ys,
                          unsigned int xi, unsigned int yi,
                          unsigned int w, unsigned int h)
{
    video_canvas_render(canvas, NULL, w, h, xs, ys, xi, yi, canvas->pitch, canvas->depth);
}

int video_canvas_set_palette(video_canvas_t *c, palette_t *p)
{
    return 0;
}
