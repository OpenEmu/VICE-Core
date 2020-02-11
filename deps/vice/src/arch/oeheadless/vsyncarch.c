/** \file   vsyncarch.c
 * \brief   End-of-frame handling for Headless UI
 *
 * \note    This is altered and trimmed down to fit into the GTK3-native
 *          world, but it's still heavily reliant on UNIX internals.
 *
 * \author  Dag Lem <resid@nimrod.no>
 */

/*
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

#include "kbdbuf.h"
#include "ui.h"
#include "vsyncapi.h"
#include "videoarch.h"

#include "joy.h"

#include <time.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

static int pause_pending = 0;

/* ------------------------------------------------------------------------- */
#define TICKSPERSECOND  1000000000L  /* Nanoseconds resolution. */
#define TICKSPERMSEC    1000000L
#define TICKSPERUSEC    1000L
#define TICKSPERNSEC    1L

/* FIXME: this function should be a constant */
/* Number of timer units per second. */
unsigned long vsyncarch_frequency(void)
{
    return TICKSPERSECOND;
}

/* Get time in timer units. */
unsigned long vsyncarch_gettime(void)
{
    static uint64_t factor = 0;
    uint64_t time = mach_absolute_time();
    if (!factor) {
        mach_timebase_info_data_t info;
        mach_timebase_info(&info);
        factor = info.numer / info.denom;
    }
    return time * factor;
}

void vsyncarch_init(void)
{
}

void vsyncarch_display_speed(double speed, double frame_rate, int warp_enabled)
{

}

/* Sleep a number of timer units. */
void vsyncarch_sleep(unsigned long delay)
{

}

void vsyncarch_presync(void)
{
    ui_update_lightpen();
    kbdbuf_flush();
    joystick();
}

void vsyncarch_postsync(void)
{
    /* this function is called once a frame, so this
       handles single frame advance */
    if (pause_pending) {
        ui_pause_enable();
        pause_pending = 0;
    }
}

void vsyncarch_advance_frame(void)
{
    ui_pause_disable();
    pause_pending = 1;
}
