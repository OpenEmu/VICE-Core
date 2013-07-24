/*
 * ui-resources.c
 *
 * Written by
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Andr� Fachat <fachat@physik.tu-chemnitz.de>
 *  Andreas Boose <viceteam@t-online.de>
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

#include "vice.h"

#include <stdio.h>

#include "lib.h"
#include "machine.h"
#include "resources.h"
#include "log.h"
#include "ui.h"
#include "util.h"

struct ui_resources_s {
    char *html_browser_command;
    int use_private_colormap;
    int save_resources_on_exit;
    int confirm_on_exit;
    int depth;
    int window0_width;
    int window0_height;
    int window0_xpos;
    int window0_ypos;
    int window1_width;
    int window1_height;
    int window1_xpos;
    int window1_ypos;
#if defined (USE_XF86_EXTENSIONS) && \
    (defined(USE_XF86_VIDMODE_EXT) || defined (HAVE_XRANDR))
    int fs_enabled_pending;
#endif
};
typedef struct ui_resources_s ui_resources_t;

static ui_resources_t ui_resources;

static unsigned int ui_resources_initialized = 0;

/* Warning: This cannot actually be changed at runtime.  */
static int set_depth(int d, void *param)
{
    /* Minimal sanity check.  */
    if (d < 0 || d > 32) {
        return -1;
    }

    ui_resources.depth = d;
    return 0;
}

static int set_width0(int d, void *param)
{
    /* Minimal sanity check.  */
    if (d < 0) {
        return -1;
    }

    ui_resources.window0_width = d;
    return 0;
}

static int set_height0(int d, void *param)
{
    /* Minimal sanity check.  */
    if (d < 0) {
        return -1;
    }

    ui_resources.window0_height = d;
    return 0;
}

static int set_xpos0(int d, void *param)
{
    ui_resources.window0_xpos = d;
    return 0;
}

static int set_ypos0(int d, void *param)
{
    ui_resources.window0_ypos = d;
    return 0;
}

static int set_width1(int d, void *param)
{
    /* Minimal sanity check.  */
    if (d < 0) {
        return -1;
    }

    ui_resources.window1_width = d;
    return 0;
}

static int set_height1(int d, void *param)
{
    /* Minimal sanity check.  */
    if (d < 0) {
        return -1;
    }

    ui_resources.window1_height = d;
    return 0;
}

static int set_xpos1(int d, void *param)
{
    ui_resources.window1_xpos = d;
    return 0;
}

static int set_ypos1(int d, void *param)
{
    ui_resources.window1_ypos = d;
    return 0;
}

static int set_html_browser_command(const char *val, void *param)
{
    util_string_set(&ui_resources.html_browser_command, val);
    return 0;
}

static int set_use_private_colormap(int val, void *param)
{
    ui_resources.use_private_colormap = val;
    return 0;
}

static int set_save_resources_on_exit(int val, void *param)
{
    ui_resources.save_resources_on_exit = val;
    return 0;
}

static int set_confirm_on_exit(int val, void *param)
{
    ui_resources.confirm_on_exit = val;
    return 0;
}

static const resource_string_t resources_string[] = {
    { "HTMLBrowserCommand", 
#ifdef MACOSX_SUPPORT    
      "/usr/bin/open %s",
#else
      "firefox %s",
#endif
      RES_EVENT_NO, NULL,
      &ui_resources.html_browser_command,
      set_html_browser_command, NULL },
    { NULL }
};

#if defined (USE_XF86_EXTENSIONS) && (defined(USE_XF86_VIDMODE_EXT) || defined (HAVE_XRANDR))
static int fullscreen_set_fs(int val, void *param)
{
    ui_resources.fs_enabled_pending = val;
    return 0;
}
#endif

/*
    FIXME: WindowXXX should be per window (for x128)
*/
static const resource_int_t resources_int[] = {
    { "PrivateColormap", 0, RES_EVENT_NO, NULL,
      &ui_resources.use_private_colormap, set_use_private_colormap, NULL },
    { "SaveResourcesOnExit", 0, RES_EVENT_NO, NULL,
      &ui_resources.save_resources_on_exit, set_save_resources_on_exit, NULL },
    { "ConfirmOnExit", 1, RES_EVENT_NO, NULL,
      &ui_resources.confirm_on_exit, set_confirm_on_exit, NULL },
    { "DisplayDepth", 0, RES_EVENT_NO, NULL,
      &ui_resources.depth, set_depth, NULL },
    { "Window0Width", 0, RES_EVENT_NO, NULL,
      &ui_resources.window0_width, set_width0, NULL },
    { "Window0Height", 0, RES_EVENT_NO, NULL,
      &ui_resources.window0_height, set_height0, NULL },
    { "Window0Xpos", -1, RES_EVENT_NO, NULL,
      &ui_resources.window0_xpos, set_xpos0, NULL },
    { "Window0Ypos", -1, RES_EVENT_NO, NULL,
      &ui_resources.window0_ypos, set_ypos0, NULL },
#if defined (USE_XF86_EXTENSIONS) && (defined(USE_XF86_VIDMODE_EXT) || defined (HAVE_XRANDR))
    { "UseFullscreen", 0, RES_EVENT_NO, NULL,
      &ui_resources.fs_enabled_pending, fullscreen_set_fs, NULL },
#endif
    { NULL }
};

static const resource_int_t extra_resources_int[] = {
    { "Window1Width", 0, RES_EVENT_NO, NULL,
      &ui_resources.window1_width, set_width1, NULL },
    { "Window1Height", 0, RES_EVENT_NO, NULL,
      &ui_resources.window1_height, set_height1, NULL },
    { "Window1Xpos", -1, RES_EVENT_NO, NULL,
      &ui_resources.window1_xpos, set_xpos1, NULL },
    { "Window1Ypos", -1, RES_EVENT_NO, NULL,
      &ui_resources.window1_ypos, set_ypos1, NULL },
    { NULL }
};

int ui_resources_init(void)
{
    ui_resources_initialized = 1;

    if (resources_register_string(resources_string) < 0) {
        return -1;
    }

    if (machine_class == VICE_MACHINE_C128) {
        if (resources_register_int(extra_resources_int) < 0) {
            return -1;
        }
    }

    return resources_register_int(resources_int);
}

void ui_resources_shutdown(void)
{
    if (ui_resources_initialized) {
        lib_free(ui_resources.html_browser_command);
    }
}
