//
//  stubs.c
//  VICE
//
//  Created by loic on 7/24/13.
//  Copyright (c) 2013 OpenEmu. All rights reserved.
//


#include "types.h"

/* loic: Like I know what I'm doing.. stubs */
/*
extern void maincpu_mainloop(void)
{
}
*/
extern void archdep_ui_init(int argc, char *argv[])
{
}

extern int c64scui_init(void)
{
}

extern int c64ui_init(void)
{
}

extern void c64ui_shutdown(void)
{
}

extern void c64scui_shutdown(void)
{
}

extern int c64dtvui_init(void)
{
}

extern void c64dtvui_shutdown(void)
{
}

struct disk_image_s;
struct drive_s;
typedef struct fd_drive_s fd_drive_t;

extern const int fdd_data_rates[4];
const int fdd_data_rates[4] = {500, 300, 250, 1000}; /* kbit/s */

extern fd_drive_t *fdd_init(int num, struct drive_s *drive)
{
}
extern void fdd_shutdown(fd_drive_t *drv)
{
}
extern void fdd_image_attach(fd_drive_t *drv, struct disk_image_s *image)
{
}
extern void fdd_image_detach(fd_drive_t *drv)
{
}
extern WORD fdd_read(fd_drive_t *drv)
{
}
extern int fdd_write(fd_drive_t *drv, WORD data)
{
}
extern void fdd_flush(fd_drive_t *drv)
{
}
extern void fdd_seek_pulse(fd_drive_t *drv, int dir)
{
}
extern void fdd_select_head(fd_drive_t *drv, int head)
{
}
extern void fdd_set_motor(fd_drive_t *drv, int motor)
{
}
extern void fdd_set_rate(fd_drive_t *drv, int rate)
{
}
extern int fdd_rotate(fd_drive_t *drv, int bytes)
{
}
extern inline int fdd_index(fd_drive_t *drv)
{
}
extern inline void fdd_index_count_reset(fd_drive_t *drv)
{
}
extern inline int fdd_index_count(fd_drive_t *drv)
{
}
extern inline int fdd_track0(fd_drive_t *drv)
{
}
extern inline int fdd_write_protect(fd_drive_t *drv)
{
}
extern inline int fdd_disk_change(fd_drive_t *drv)
{
}
extern inline WORD fdd_crc(WORD crc, BYTE b)
{
}
extern void fullscreen_capability(struct cap_fullscreen_s *cap_fullscreen)
{
}
extern void fullscreen_resume(void)
{
}
extern void gfxoutput_init_ffmpeg(void)
{
}

extern int mousedrv_resources_init(void)
{
}

extern int mousedrv_cmdline_options_init(void)
{
}

extern void mousedrv_init(void)
{
}


extern void mousedrv_mouse_changed(void)
{
}


extern int mousedrv_get_x(void)
{
}

extern int mousedrv_get_y(void)
{
}

extern unsigned long mousedrv_get_timestamp(void)
{
}


extern void mouse_button(int bnumber, int state)
{
}

extern void mouse_move(float dx, float dy)
{
}



/* Initialization  */
extern int ui_resources_init(void)
{
}
extern void ui_resources_shutdown(void)
{
}
extern int ui_cmdline_options_init(void)
{
}
extern int ui_init(int *argc, char **argv)
{
}
extern int ui_init_finish(void)
{
}
extern int ui_init_finalize(void)
{
}
extern void ui_shutdown(void)
{
}


extern void kbd_arch_init(void)
{
}

extern signed long kbd_arch_keyname_to_keynum(char *keyname)
{
}
extern const char *kbd_arch_keynum_to_keyname(signed long keynum)
{
}
extern void kbd_initialize_numpad_joykeys(int *joykeys)
{
}

struct console_s;
struct monitor_interface_s;

extern struct console_s *uimon_window_open(void)
{
}
extern void uimon_window_suspend(void)
{
}
extern struct console_s *uimon_window_resume(void)
{
}
extern void uimon_window_close(void)
{
}

extern int uimon_out(const char *buffer)
{
}
/*
extern char *uimon_in(const char *prompt)
{
}
*/
extern void uimon_notify_change(void)
{
}
extern void uimon_set_interface(struct monitor_interface_s ** interface, int num)
{
}
extern char *uimon_get_in(char ** in,const char * constant)
{
}

struct cmdline_option_ram_s;

extern void ui_cmdline_show_help(unsigned int num_options,
                                 struct cmdline_option_ram_s *options,
                                 void *userparam)
{
}

extern void uicolor_convert_color_table(unsigned int colnr, BYTE *data,
                                        long color_pixel, void *c)
{
}


extern int sound_init_mp3_device(void)
{
}



extern void ui_dispatch_events(void)
{
}

extern int vsid_ui_init(void)
{
}
extern void vsid_ui_close(void)
{
}

extern void vsid_ui_display_name(const char *name)
{
}
extern void vsid_ui_display_author(const char *author)
{
}
extern void vsid_ui_display_copyright(const char *copyright)
{
}
extern void vsid_ui_display_sync(int sync)
{
}
extern void vsid_ui_display_sid_model(int model)
{
}
extern void vsid_ui_display_tune_nr(int nr)
{
}
extern void vsid_ui_display_nr_of_tunes(int count)
{
}
extern void vsid_ui_set_default_tune(int nr)
{
}
extern void vsid_ui_display_time(unsigned int sec)
{
}
extern void vsid_ui_display_irqtype(const char *irq)
{
}

extern void vsid_ui_setdrv(char* driver_info_text)
{
}




extern int joy_arch_init(void)
{
}
extern void joystick_close(void)
{
}
extern void joystick(void)
{
}
extern int joystick_arch_init_resources(void)
{
}
extern int joystick_init_cmdline_options(void)
{
}


struct video_canvas_s;
struct palette_s;

extern int uicolor_alloc_color(unsigned int red, unsigned int green,
                               unsigned int blue, unsigned long *color_pixel,
                               BYTE *pixel_return)
{
}
extern void uicolor_free_color(unsigned int red, unsigned int green,
                               unsigned int blue, unsigned long color_pixel)
{
}
extern void uicolor_convert_color_table(unsigned int colnr, BYTE *data,
                                        long color_pixel, void *c);
extern int uicolor_set_palette(struct video_canvas_s *c,
                               const struct palette_s *palette)
{
}

/* Temporary! */
extern int uicolor_alloc_colors(struct video_canvas_s *c)
{
}


struct video_canvas_s;

typedef void (*void_hook_t)(void);

/* video filter type, resource "CHIPFilter" */
#define VIDEO_FILTER_NONE         0
#define VIDEO_FILTER_CRT          1
#define VIDEO_FILTER_SCALE2X      2

/* These constants are used to configure the video output.  */

/* no video output (dummy) */
#define VIDEO_RENDER_NULL       0
/* PAL or NTSC TV/Monitor (like VIC, VIC-II or TED) */
#define VIDEO_RENDER_PAL_1X1    1
#define VIDEO_RENDER_PAL_2X2    2
/* no filtering */
#define VIDEO_RENDER_RGB_1X1    3
#define VIDEO_RENDER_RGB_1X2    4 /* needed for y-stretch */
#define VIDEO_RENDER_RGB_2X2    5
#define VIDEO_RENDER_RGB_2X4    6 /* needed for y-stretch */
/* RGB(I) or monochrome Monitor (CRTC and VDC) */
#define VIDEO_RENDER_CRT_1X1    7 /* FIXME: to be written */
#define VIDEO_RENDER_CRT_1X2    8 /* needed for y-stretch */
#define VIDEO_RENDER_CRT_2X2    9
#define VIDEO_RENDER_CRT_2X4   10 /* needed for y-stretch */

struct video_canvas_s;
struct video_cbm_palette_s;
struct viewport_s;
struct geometry_s;
struct palette_s;

struct canvas_refresh_s
{
    BYTE *draw_buffer;
    int draw_buffer_line_size;
#ifdef __OS2__
    int bufh;
#endif
    int x;
    int y;
};
typedef struct canvas_refresh_s canvas_refresh_t;

struct draw_buffer_s {
    /* The memory buffer where the screen of the emulated machine is drawn. Palettized, 1 byte per pixel */
    BYTE *draw_buffer;
    /* Width of draw_buffer in pixels */
    unsigned int draw_buffer_width;
    /* Height of draw_buffer in pixels. Typically same as geometry->screen_size.height */
    unsigned int draw_buffer_height;
    unsigned int draw_buffer_pitch;
    /* Width of emulator screen (physical screen on the machine where the emulator runs) in pixels */
    unsigned int canvas_physical_width;
    /* Height of emulator screen (physical screen on the machine where the emulator runs) in pixels */
    unsigned int canvas_physical_height;
    /* Maximum theoretical width of draw_buffer that would fit in the emulator screen.
     Typically, it is the same as canvas_physical_width if no horizontal stretch is used (videoconfig->doublesizex == 0) and smaller if it is used.
     TODO do we really need it? */
    unsigned int canvas_width;
    /* Maximum theoretical height of draw_buffer that would fit in the emulator screen.
     Typically, it is the same as canvas_physical_width if no vertical stretch is used (videoconfig->doublesizey == 0) and smaller if it is used.
     TODO do we really need it? */
    unsigned int canvas_height;
    /* Width of the visible subset of draw_buffer, in pixels. Typically same as geometry->screen_size.width */
    unsigned int visible_width;
    /* Height of the visible subset of draw_buffer, in pixels */
    unsigned int visible_height;
};
typedef struct draw_buffer_s draw_buffer_t;

struct cap_render_s {
    unsigned int sizex;
    unsigned int sizey;
    unsigned int rmode;
};
typedef struct cap_render_s cap_render_t;

#define FULLSCREEN_MAXDEV 4

struct cap_fullscreen_s {
    unsigned int device_num;
    const char *device_name[FULLSCREEN_MAXDEV];
    int (*enable)(struct video_canvas_s *canvas, int enable);
    int (*statusbar)(struct video_canvas_s *canvas, int enable);
    int (*double_size)(struct video_canvas_s *canvas, int double_size);
    int (*double_scan)(struct video_canvas_s *canvas, int double_scan);
    int (*device)(struct video_canvas_s *canvas, const char *device);
    int (*mode[FULLSCREEN_MAXDEV])(struct video_canvas_s *canvas, int mode);
};
typedef struct cap_fullscreen_s cap_fullscreen_t;


struct video_chip_cap_s {
    unsigned int dsize_allowed;
    unsigned int dsize_default;
    unsigned int dsize_limit_width;
    unsigned int dsize_limit_height;
    unsigned int dscan_allowed;
    unsigned int hwscale_allowed;
    unsigned int scale2x_allowed;
    unsigned int internal_palette_allowed;
    unsigned int palemulation_allowed;
    unsigned int double_buffering_allowed;
    const char *external_palette_name;
    cap_render_t single_mode;
    cap_render_t double_mode;
    cap_fullscreen_t fullscreen;
};
typedef struct video_chip_cap_s video_chip_cap_t;

#define VIDEO_MAX_OUTPUT_WIDTH  2048

struct video_render_color_tables_s {
    int updated;                /* tables here are up to date */
    DWORD physical_colors[256];
    SDWORD ytableh[256];        /* y for current pixel */
    SDWORD ytablel[256];        /* y for neighbouring pixels */
    SDWORD cbtable[256];        /* b component */
    SDWORD cbtable_odd[256];    /* b component + phase shift */
    SDWORD crtable[256];        /* r component */
    SDWORD crtable_odd[256];    /* r component + phase shift */
    SDWORD cutable[256];        /* u component */
    SDWORD cutable_odd[256];    /* u component + phase shift */
    SDWORD cvtable[256];        /* v component */
    SDWORD cvtable_odd[256];    /* v component + phase shift */
    
    /* YUV table for hardware rendering: (Y << 16) | (U << 8) | V */
    int yuv_updated;            /* yuv table updated for packed mode */
    DWORD yuv_table[512];
    SDWORD line_yuv_0[VIDEO_MAX_OUTPUT_WIDTH * 3];
    SWORD prevrgbline[VIDEO_MAX_OUTPUT_WIDTH * 3];
    BYTE rgbscratchbuffer[VIDEO_MAX_OUTPUT_WIDTH * 4];
};
typedef struct video_render_color_tables_s video_render_color_tables_t;

/* options for the color generator and crt emulation */
typedef struct video_resources_s
{
    /* parameters for color generation */
    int color_saturation;
    int color_contrast;
    int color_brightness;
    int color_gamma;
    int color_tint;
    /* additional parameters for CRT emulation */
    int pal_scanlineshade;      /* amount of scanline shade */
    int pal_blur;               /* luma blur */
    int pal_oddlines_phase;     /* oddlines UV phase offset */
    int pal_oddlines_offset;    /* oddlines UV multiplier */
    
    int audioleak;              /* flag: enable video->audio leak emulation */
} video_resources_t;

/* render config for a specific canvas and video chip */
struct video_render_config_s {
    char *chip_name;               /* chip name prefix, (use to build resource names) */
    video_resources_t video_resources; /* options for the color generator and crt emulation */
    video_chip_cap_t *cap;         /* Which renderers are allowed?  */
    int rendermode;                /* What renderer is active?  */
    int double_size_enabled;       /* Double size enabled?  */
    int doublesizex;               /* contains the actual magnification factor - 1 (> 0 if double size is enabled and screen is large enough in x direction) */
    int doublesizey;               /* contains the actual magnification factor - 1 (> 0 if double size is enabled and screen is large enough in y direction) */
    int doublescan;                /* Doublescan enabled?  */
    int hwscale;                   /* Hardware scaling enabled? */
    int scale2x;                   /* Scale2x enabled?  */
    int filter;                    /* VIDEO_FILTER_NONE, VIDEO_FILTER_CRT, VIDEO_FILTER_SCALE2X */
    int external_palette;          /* Use an external palette?  */
    char *external_palette_name;   /* Name of the external palette.  */
    int double_buffer;             /* Double buffering enabled? */
    int readable;                  /* reading of frame buffer is safe and fast */
    struct video_cbm_palette_s *cbm_palette; /* Internal palette.  */
    struct video_render_color_tables_s color_tables;
    int fullscreen_enabled;
    int fullscreen_statusbar_enabled;
    char *fullscreen_device;
    int fullscreen_device_num;
    int fullscreen_double_size_enabled;
    int fullscreen_double_scan_enabled;
    int fullscreen_mode[FULLSCREEN_MAXDEV];
};
typedef struct video_render_config_s video_render_config_t;

extern void video_render_initconfig(video_render_config_t *config)
{
}
extern void video_render_setphysicalcolor(video_render_config_t *config,
                                          int index, DWORD color, int depth)
{
}
extern void video_render_setrawrgb(unsigned int index, DWORD r, DWORD g,
                                   DWORD b)
{
}
extern void video_render_initraw(struct video_render_config_s *videoconfig)
{
}

/**************************************************************/

extern int video_init_cmdline_options(void)
{
}
extern int video_init(void)
{
}
extern void video_shutdown(void)
{
}

extern struct video_canvas_s *video_canvas_create(struct video_canvas_s *canvas,
                                                  unsigned int *width, unsigned int *height,
                                                  int mapped)
{
}
extern void video_arch_canvas_init(struct video_canvas_s *canvas)
{
}
extern void video_canvas_shutdown(struct video_canvas_s *canvas)
{
}
extern struct video_canvas_s *video_canvas_init(void)
{
}
extern void video_canvas_refresh(struct video_canvas_s *canvas,
                                 unsigned int xs, unsigned int ys,
                                 unsigned int xi, unsigned int yi,
                                 unsigned int w, unsigned int h)
{
}
extern int video_canvas_set_palette(struct video_canvas_s *canvas,
                                    struct palette_s *palette)
{
}
/* This will go away.  */
extern int video_canvas_palette_set(struct video_canvas_s *canvas,
                                    struct palette_s *palette)
{
}
extern void video_canvas_create_set(struct video_canvas_s *canvas)
{
}
extern void video_canvas_destroy(struct video_canvas_s *canvas)
{
}
extern void video_canvas_map(struct video_canvas_s *canvas)
{
}
extern void video_canvas_unmap(struct video_canvas_s *canvas)
{
}
extern void video_canvas_resize(struct video_canvas_s *canvas, char resize_canvas)
{
}
extern void video_canvas_render(struct video_canvas_s *canvas, BYTE *trg,
                                int width, int height, int xs, int ys,
                                int xt, int yt, int pitcht, int depth)
{
}
extern void video_canvas_refresh_all(struct video_canvas_s *canvas)
{
}
extern char video_canvas_can_resize(struct video_canvas_s *canvas)
{
}
extern void video_viewport_get(struct video_canvas_s *canvas,
                               struct viewport_s **viewport,
                               struct geometry_s **geometry)
{
}
extern void video_viewport_resize(struct video_canvas_s *canvas, char resize_canvas)
{
}
extern void video_viewport_title_set(struct video_canvas_s *canvas,
                                     const char *title)
{
}
extern void video_viewport_title_free(struct viewport_s *viewport)
{
}

typedef struct video_draw_buffer_callback_s {
    int (*draw_buffer_alloc)(struct video_canvas_s *canvas, BYTE **draw_buffer,
                             unsigned int fb_width, unsigned int fb_height,
                             unsigned int *fb_pitch);
    void (*draw_buffer_free)(struct video_canvas_s *canvas, BYTE *draw_buffer);
    void (*draw_buffer_clear)(struct video_canvas_s *canvas, BYTE *draw_buffer,
                              BYTE value, unsigned int fb_width,
                              unsigned int fb_height, unsigned int fb_pitch);
} video_draw_buffer_callback_t;

struct raster_s;

extern int video_resources_init(void)
{
}
extern void video_resources_shutdown(void)
{
}
extern int video_resources_chip_init(const char *chipname,
                                     struct video_canvas_s **canvas,
                                     video_chip_cap_t *video_chip_cap)
{
}
extern void video_resources_chip_shutdown(struct video_canvas_s *canvas)
{
}
extern int video_cmdline_options_chip_init(const char *chipname,
                                           video_chip_cap_t *video_chip_cap)
{
}
extern int video_arch_resources_init(void)
{
}
extern void video_arch_resources_shutdown(void)
{
}

/* Video render interface */

/* VIC/VIC-II/TED related color/palette types */
typedef struct video_cbm_color_s {
    float luminance;        /* luminance                      */
    float angle;            /* angle on color wheel           */
    int direction;          /* +1 (pos), -1 (neg) or 0 (grey) */
    char *name;             /* name of this color             */
} video_cbm_color_t;

typedef struct video_cbm_palette_s {
    unsigned int num_entries;           /* number of colors in palette */
    video_cbm_color_t *entries;         /* array of colors             */
    float saturation; /* base saturation of all colors except the grey tones */
    float phase;      /* color phase (will be added to all color angles) */
} video_cbm_palette_t;

extern void video_color_palette_internal(struct video_canvas_s *canvas,
                                         struct video_cbm_palette_s *cbm_palette)
{
}
extern int video_color_update_palette(struct video_canvas_s *canvas)
{
}
extern void video_color_palette_free(struct palette_s *palette)
{
}

/* FIXME: implement a central function that inits the renderer(s) */
extern void video_render_1x2_init(void)
{
}
extern void video_render_2x2_init(void)
{
}
extern void video_render_pal_init(void)
{
}
extern void video_render_crt_init(void)
{
}


/* number of timer units per second - used to calc speed and fps */
extern signed long vsyncarch_frequency(void)
{
}

/* provide the actual time in timer units */
extern unsigned long vsyncarch_gettime(void)
{
}

/* call when vsync_init is called */
extern void vsyncarch_init(void)
{
}

/* display speed(%) and framerate(fps) */
extern void vsyncarch_display_speed(double speed, double fps, int warp_enabled)
{
}

/* sleep the given amount of timer units */
extern void vsyncarch_sleep(signed long delay)
{
}

#if defined (HAVE_OPENGL_SYNC) && !defined(USE_SDLUI)
/* synchronize with vertical blanks */
extern void vsyncarch_verticalblank(struct video_canvas_s *c, float rate,
                                    int frames)
{
}

/* keep vertical blank sync prepared */
extern void vsyncarch_prepare_vbl(void)
{
}
#endif

/* this is called before vsync_do_vsync does the synchroniation */
extern void vsyncarch_presync(void)
{
}

/* this is called after vsync_do_vsync did the synchroniation */
extern void vsyncarch_postsync(void)
{
}

/* set ui dispatcher function */
extern void_hook_t vsync_set_event_dispatcher(void_hook_t hook)
{
}

extern int vsyncarch_vbl_sync_enabled(void)
{
}

#if defined (HAVE_OPENGL_SYNC) && !defined(USE_SDLUI)
/* wait for next vertical retrace */
extern void vsyncarch_sync_with_raster(struct video_canvas_s *c)
{
}
#endif


typedef enum {
    UI_JAM_RESET, UI_JAM_HARD_RESET, UI_JAM_MONITOR, UI_JAM_NONE
} ui_jam_action_t;

typedef enum {
    UI_DRIVE_ENABLE_NONE = 0,
    UI_DRIVE_ENABLE_0 = 1 << 0,
    UI_DRIVE_ENABLE_1 = 1 << 1,
    UI_DRIVE_ENABLE_2 = 1 << 2,
    UI_DRIVE_ENABLE_3 = 1 << 3
} ui_drive_enable_t;


/* Print a message.  */
extern void ui_message(const char *format,...)
{
}

/* Print an error message.  */
extern void ui_error(const char *format,...)
{
}

/* Display a mesage without interrupting emulation */
extern void ui_display_statustext(const char *text, int fade_out)
{
}

/* Let the user browse for a filename; display format as a titel */
extern char* ui_get_file(const char *format,...)
{
}

/* Drive related UI.  */
extern void ui_enable_drive_status(ui_drive_enable_t state,
                                   int *drive_led_color)
{
}
extern void ui_display_drive_track(unsigned int drive_number,
                                   unsigned int drive_base,
                                   unsigned int half_track_number)
{
}
/* The pwm value will vary between 0 and 1000.  */
extern void ui_display_drive_led(int drive_number, unsigned int pwm1,
                                 unsigned int led_pwm2)
{
}
extern void ui_display_drive_current_image(unsigned int drive_number,
                                           const char *image)
{
}
extern int ui_extend_image_dialog(void)
{
}

/* Tape related UI */
extern void ui_set_tape_status(int tape_status)
{
}
extern void ui_display_tape_motor_status(int motor)
{
}
extern void ui_display_tape_control_status(int control)
{
}
extern void ui_display_tape_counter(int counter)
{
}
extern void ui_display_tape_current_image(const char *image)
{
}


/* Show a CPU JAM dialog.  */
extern ui_jam_action_t ui_jam_dialog(const char *format, ...)
{
}

/* Update all menu entries.  */
extern void ui_update_menus(void)
{
}

/* Recording UI */
extern void ui_display_playback(int playback_status, char *version)
{
}
extern void ui_display_recording(int recording_status)
{
}
extern void ui_display_event_time(unsigned int current, unsigned int total)
{
}

/* Joystick UI */
extern void ui_display_joyport(BYTE *joyport)
{
}

/* Volume UI */
void ui_display_volume(int vol)
{
}

/* Event related UI. */
extern void ui_dispatch_next_event(void)
{
}

extern int console_close_all(void)
{
}

typedef struct console_s {
    /* Console geometry.  */
    /* Be careful - geometry might change at run-time! */
    unsigned int console_xres;
    unsigned int console_yres;
    
    /* It is allowed to leave the console open after control is given back
     to the emulation.  */
    int console_can_stay_open;
    
    /* == 0 if the console can output anything;
     * != 0 if it cannot (for example, because it is closed).
     */
    int console_cannot_output;
    
    struct console_private_s *private;
    
} console_t;

extern int console_init(void)
{
}

extern int console_out(console_t *log, const char *format, ...)
{
}
extern int console_flush(console_t *log)
{
}
extern char *console_in(console_t *log, const char *prompt)
{
}

