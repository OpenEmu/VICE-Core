/*
 * mousedrv.c - Mouse handling for SDL.
 *
 * Written by
 *  Hannu Nuotio <hannu.nuotio@tut.fi>
 *
 * Based on code by
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Oliver Schaertel <orschaer@forwiss.uni-erlangen.de>
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
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "vice.h"

#include "vice_sdl.h"

#include "mouse.h"
#include "mousedrv.h"
#include "ui.h"
#include "vsyncapi.h"

int mouse_x, mouse_y;
int mouse_accelx = 2, mouse_accely = 2;
static unsigned long mouse_timestamp = 0;

void mousedrv_mouse_changed(void)
{
    ui_check_mouse_cursor();
}

int mousedrv_resources_init(void)
{
    return 0;
}

/* ------------------------------------------------------------------------- */

int mousedrv_cmdline_options_init(void)
{
    return 0;
}

/* ------------------------------------------------------------------------- */

void mousedrv_init(void)
{
}

/* ------------------------------------------------------------------------- */

void mouse_button(int bnumber, int state)
{
    switch (bnumber) {
    case SDL_BUTTON_LEFT:
        mouse_button_left(state);
        break;
    case SDL_BUTTON_MIDDLE:
        mouse_button_middle(state);
        break;
    case SDL_BUTTON_RIGHT:
        mouse_button_right(state);
        break;
    case SDL_BUTTON_WHEELUP:
        mouse_button_up(state);
        break;
    case SDL_BUTTON_WHEELDOWN:
        mouse_button_down(state);
        break;
    default:
        break;
    }
}

int mousedrv_get_x(void)
{
    return mouse_x >> 1;
}

int mousedrv_get_y(void)
{
    return mouse_y >> 1;
}

void mouse_move(int x, int y)
{
    mouse_x += x * mouse_accelx;
    mouse_y -= y * mouse_accely;
    mouse_timestamp = vsyncarch_gettime();
}

unsigned long mousedrv_get_timestamp(void)
{
    return mouse_timestamp;
}
