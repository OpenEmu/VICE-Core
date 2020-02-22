/*
 * plus4-snapshot.c -- Plus4 snapshot handling.
 *
 * Written by
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

#include "drive-snapshot.h"
#include "drive.h"
#include "ioutil.h"
#include "joyport.h"
#include "joystick.h"
#include "keyboard.h"
#include "log.h"
#include "machine.h"
#include "maincpu.h"
#include "plus4-snapshot.h"
#include "plus4memsnapshot.h"
#include "snapshot.h"
#include "sound.h"
#include "tapeport.h"
#include "ted.h"
#include "types.h"
#include "userport.h"
#include "vice-event.h"

/* #define DEBUGSNAPSHOT */

#ifdef DEBUGSNAPSHOT
#define DBG(x) printf x
#else
#define DBG(x)
#endif

#define SNAP_MAJOR 1
#define SNAP_MINOR 1

int plus4_snapshot_write_to_stream(snapshot_stream_t *stream, int save_roms, int save_disks,
                                   int event_mode)
{
    snapshot_t *s;

    s = snapshot_create_from_stream(stream, ((uint8_t)(SNAP_MAJOR)), ((uint8_t)(SNAP_MINOR)),
                        machine_name);
    if (s == NULL) {
        return -1;
    }

    sound_snapshot_prepare();

    /* Execute drive CPUs to get in sync with the main CPU.  */
    drive_cpu_execute_all(maincpu_clk);

    if (maincpu_snapshot_write_module(s) < 0
        || plus4_snapshot_write_module(s, save_roms) < 0
        || drive_snapshot_write_module(s, save_disks, save_roms) < 0
        || ted_snapshot_write_module(s) < 0
        || event_snapshot_write_module(s, event_mode) < 0
        || tapeport_snapshot_write_module(s, save_disks) < 0
        || keyboard_snapshot_write_module(s) < 0
        || joyport_snapshot_write_module(s, JOYPORT_1) < 0
        || joyport_snapshot_write_module(s, JOYPORT_2) < 0
        || userport_snapshot_write_module(s) < 0) {
        snapshot_free(s);
        DBG(("error writing snapshot modules.\n"));
        return -1;
    }
    DBG(("all snapshots written.\n"));

    snapshot_free(s);
    return 0;
}

int plus4_snapshot_write(const char *name, int save_roms, int save_disks, int event_mode)
{
    snapshot_stream_t *stream;
    int res;

    stream = snapshot_file_write_fopen(name);
    res = plus4_snapshot_write_to_stream(stream, save_roms, save_disks, event_mode);
    if (res) {
        snapshot_fclose_erase(stream);
    } else if (snapshot_fclose(stream) == EOF) {
        snapshot_set_error(SNAPSHOT_WRITE_CLOSE_EOF_ERROR);
        res = -1;
    }
    return res;
}

int plus4_snapshot_read_from_stream(snapshot_stream_t *stream, int event_mode)
{
    snapshot_t *s;
    uint8_t minor, major;

    s = snapshot_open_from_stream(stream, &major, &minor, machine_name);

    if (s == NULL) {
        return -1;
    }

    if (!snapshot_version_is_equal(major, minor, SNAP_MAJOR, SNAP_MINOR)) {
        log_error(LOG_DEFAULT, "Snapshot version (%d.%d) not valid: expecting %d.%d.", major, minor, SNAP_MAJOR, SNAP_MINOR);
        snapshot_set_error(SNAPSHOT_MODULE_INCOMPATIBLE);
        goto fail;
    }

    ted_snapshot_prepare();

    joyport_clear_devices();

    if (maincpu_snapshot_read_module(s) < 0
        || plus4_snapshot_read_module(s) < 0
        || drive_snapshot_read_module(s) < 0
        || ted_snapshot_read_module(s) < 0
        || event_snapshot_read_module(s, event_mode) < 0
        || tapeport_snapshot_read_module(s) < 0
        || keyboard_snapshot_read_module(s) < 0
        || joyport_snapshot_read_module(s, JOYPORT_1) < 0
        || joyport_snapshot_read_module(s, JOYPORT_2) < 0
        || userport_snapshot_read_module(s) < 0) {
        goto fail;
    }

    snapshot_free(s);

    sound_snapshot_finish();

    DBG(("all snapshots loaded.\n"));
    return 0;

fail:
    if (s != NULL) {
        snapshot_free(s);
    }

    machine_trigger_reset(MACHINE_RESET_MODE_SOFT);

    DBG(("error loading snapshot modules.\n"));
    return -1;
}

int plus4_snapshot_read(const char *name, int event_mode)
{
    snapshot_stream_t *stream;
    int res;

    stream = snapshot_file_read_fopen(name);
    res = plus4_snapshot_read_from_stream(stream, event_mode);
    snapshot_fclose(stream);
    return res;
}
