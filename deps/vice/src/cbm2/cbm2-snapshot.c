/*
 * cbm2-snapshot.c - CBM-6x0/7x0 snapshot handling.
 *
 * Written by
 *  Andre Fachat <fachat@physik.tu-chemnitz.de>
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

#include "cbm2-snapshot.h"
#include "cbm2.h"
#include "cbm2acia.h"
#include "cbm2memsnapshot.h"
#include "cia.h"
#include "crtc.h"
#include "drive-snapshot.h"
#include "drive.h"
#include "ioutil.h"
#include "joystick.h"
#include "keyboard.h"
#include "log.h"
#include "machine.h"
#include "maincpu.h"
#include "sid-snapshot.h"
#include "sound.h"
#include "snapshot.h"
#include "tapeport.h"
#include "userport.h"
#include "tpi.h"
#include "types.h"
#include "vice-event.h"


#define SNAP_MAJOR          0
#define SNAP_MINOR          0

int cbm2_snapshot_write_to_stream(snapshot_stream_t *stream, int save_roms, int save_disks,
                                  int event_mode)
{
    snapshot_t *s;

    s = snapshot_create_from_stream(stream, SNAP_MAJOR, SNAP_MINOR, machine_get_name());

    if (s == NULL) {
        return -1;
    }

    sound_snapshot_prepare();

    if (maincpu_snapshot_write_module(s) < 0
        || cbm2_snapshot_write_module(s, save_roms) < 0
        || crtc_snapshot_write_module(s) < 0
        || ciacore_snapshot_write_module(machine_context.cia1, s) < 0
        || tpicore_snapshot_write_module(machine_context.tpi1, s) < 0
        || tpicore_snapshot_write_module(machine_context.tpi2, s) < 0
        || acia1_snapshot_write_module(s) < 0
        || sid_snapshot_write_module(s) < 0
        || drive_snapshot_write_module(s, save_disks, save_roms) < 0
        || event_snapshot_write_module(s, event_mode) < 0
        || tapeport_snapshot_write_module(s, save_disks) < 0
        || keyboard_snapshot_write_module(s) < 0
        || userport_snapshot_write_module(s) < 0) {
        snapshot_free(s);
        return -1;
    }

    snapshot_free(s);
    return 0;
}

int cbm2_snapshot_write(const char *name, int save_roms, int save_disks, int event_mode)
{
    snapshot_stream_t *stream;
    int res;

    stream = snapshot_file_write_fopen(name);
    res = cbm2_snapshot_write_to_stream(stream, save_roms, save_disks, event_mode);
    if (res) {
        snapshot_fclose_erase(stream);
    } else if (snapshot_fclose(stream) == EOF) {
        snapshot_set_error(SNAPSHOT_WRITE_CLOSE_EOF_ERROR);
        res = -1;
    }
    return res;
}

int cbm2_snapshot_read_from_stream(snapshot_stream_t *stream, int event_mode)
{
    snapshot_t *s;
    uint8_t minor, major;

    s = snapshot_open_from_stream(stream, &major, &minor, machine_get_name());

    if (s == NULL) {
        return -1;
    }

    if (!snapshot_version_is_equal(major, minor, SNAP_MAJOR, SNAP_MINOR)) {
        log_error(LOG_DEFAULT, "Snapshot version (%d.%d) not valid: expecting %d.%d.", major, minor, SNAP_MAJOR, SNAP_MINOR);
        snapshot_set_error(SNAPSHOT_MODULE_INCOMPATIBLE);
        goto fail;
    }

    if (maincpu_snapshot_read_module(s) < 0
        || crtc_snapshot_read_module(s) < 0
        || cbm2_snapshot_read_module(s) < 0
        || ciacore_snapshot_read_module(machine_context.cia1, s) < 0
        || tpicore_snapshot_read_module(machine_context.tpi1, s) < 0
        || tpicore_snapshot_read_module(machine_context.tpi2, s) < 0
        || acia1_snapshot_read_module(s) < 0
        || sid_snapshot_read_module(s) < 0
        || drive_snapshot_read_module(s) < 0
        || event_snapshot_read_module(s, event_mode) < 0
        || tapeport_snapshot_read_module(s) < 0
        || keyboard_snapshot_read_module(s) < 0
        || userport_snapshot_read_module(s) < 0) {
        goto fail;
    }

    snapshot_free(s);
    sound_snapshot_finish();

    return 0;

fail:
    if (s != NULL) {
        snapshot_free(s);
    }

    machine_trigger_reset(MACHINE_RESET_MODE_SOFT);

    return -1;
}

int cbm2_snapshot_read(const char *name, int event_mode)
{
    snapshot_stream_t *stream;
    int res;

    stream = snapshot_file_read_fopen(name);
    res = cbm2_snapshot_read_from_stream(stream, event_mode);
    snapshot_fclose(stream);
    return res;
}
