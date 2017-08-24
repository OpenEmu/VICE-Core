/*
 * ui.h
 *
 * Written by
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
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

#ifndef VICE_UI_H_
#define VICE_UI_H_

#include "vice.h"

/* Number of drives we support in the UI.  */
#define NUM_DRIVES 4

/* ------------------------------------------------------------------------- */
/* Prototypes */

struct video_canvas_s;
struct palette_s;

void ui_display_speed(float percent, float framerate, int warp_flag);
void ui_display_paused(int flag);
void ui_dispatch_events(void);
extern void ui_exit(void);
extern void ui_show_text(const char *title, const char *text, int width, int height);

extern void ui_autorepeat_on(void);
extern void ui_autorepeat_off(void);
extern void ui_pause_emulation(int flag);
extern int ui_emulation_is_paused(void);
extern void ui_check_mouse_cursor(void);
extern void ui_restore_mouse(void);

extern void archdep_ui_init(int argc, char *argv[]);
extern void ui_set_application_icon(const char *icon_data[]);
extern void ui_set_selected_file(int num);

extern void ui_common_init(void);
extern void ui_common_shutdown(void);

#endif
