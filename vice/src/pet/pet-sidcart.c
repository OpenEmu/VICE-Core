/*
 * pet-sidcart.c - PET specific SID cart emulation.
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

#include "vice.h"

#include "cartio.h"
#include "cmdline.h"
#include "resources.h"
#include "sid.h"
#include "sidcart.h"
#include "sid-cmdline-options.h"
#include "sid-resources.h"
#include "sound.h"

int sidcart_address;
int sidcart_clock;

/* ------------------------------------------------------------------------- */

static int sidcart_sound_machine_init(sound_t *psid, int speed, int cycles_per_sec)
{
    if (!sidcart_clock) {
        return sid_sound_machine_init_vbr(psid, speed, cycles_per_sec, 1015);
    } else {
        return sid_sound_machine_init(psid, speed, cycles_per_sec);
    }
}

static sound_chip_t sidcart_sound_chip = {
    sid_sound_machine_open,
    sidcart_sound_machine_init,
    sid_sound_machine_close,
    sid_sound_machine_calculate_samples,
    sid_sound_machine_store,
    sid_sound_machine_read,
    sid_sound_machine_reset,
    sid_sound_machine_cycle_based,
    sid_sound_machine_channels,
    0 /* chip enabled */
};

static uint16_t sidcart_sound_chip_offset = 0;

void sidcart_sound_chip_init(void)
{
    sidcart_sound_chip_offset = sound_chip_register(&sidcart_sound_chip);
}

/* ------------------------------------------------------------------------- */

static io_source_t sidcart_8f00_device = {
    "SIDCART",
    IO_DETACH_CART, /* dummy */
    NULL,           /* dummy */
    0x8f00, 0x8fff, 0x1f,
    1, /* read is always valid */
    sid_store,
    sid_read,
    NULL, /* no peek */
    sid_dump,
    0, /* dummy (not a cartridge) */
    IO_PRIO_NORMAL,
    0
};

static io_source_t sidcart_e900_device = {
    "SIDCART",
    IO_DETACH_CART, /* dummy */
    NULL,           /* dummy */
    0xe900, 0xe9ff, 0x1f,
    1, /* read is always valid */
    sid_store,
    sid_read,
    NULL, /* no peek */
    sid_dump,
    0, /* dummy (not a cartridge) */
    IO_PRIO_NORMAL,
    0
};

static io_source_list_t *sidcart_list_item = NULL;

int sidcart_enabled(void)
{
    return sidcart_sound_chip.chip_enabled;
}

static int set_sidcart_enabled(int value, void *param)
{
    int val = value ? 1 : 0;

    if (val == sidcart_sound_chip.chip_enabled) {
        return 0;
    }

    if (val) {
        if (sidcart_address == 0x8f00) {
            sidcart_list_item = io_source_register(&sidcart_8f00_device);
        } else {
            sidcart_list_item = io_source_register(&sidcart_e900_device);
        }
    } else {
        io_source_unregister(sidcart_list_item);
        sidcart_list_item = NULL;
    }

    sidcart_sound_chip.chip_enabled = val;
#ifdef HAVE_RESID
    sid_set_enable(val);
#endif
    sound_state_changed = 1;

    return 0;
}

static int set_sid_address(int val, void *param)
{
    switch (val) {
        case 0x8f00:
        case 0xe900:
            break;
        default:
            return -1;
    }

    if (sidcart_address == val) {
        return 0;
    }

    if (sidcart_sound_chip.chip_enabled) {
        io_source_unregister(sidcart_list_item);
        if (val == 0x8f00) {
            sidcart_list_item = io_source_register(&sidcart_8f00_device);
        } else {
            sidcart_list_item = io_source_register(&sidcart_e900_device);
        }
    }

    sidcart_address = val;

    return 0;
}

static int set_sid_clock(int val, void *param)
{
    switch (val) {
        case SIDCART_CLOCK_C64:
        case SIDCART_CLOCK_NATIVE:
            break;
        default:
            return -1;
    }

    if (val != sidcart_clock) {
        sidcart_clock = val;
        sid_state_changed = 1;
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

static const resource_int_t sidcart_resources_int[] = {
    { "SidCart", 0, RES_EVENT_SAME, NULL,
      &sidcart_sound_chip.chip_enabled, set_sidcart_enabled, NULL },
    { "SidAddress", 0xe900, RES_EVENT_SAME, NULL,
      &sidcart_address, set_sid_address, NULL },
    { "SidClock", SIDCART_CLOCK_NATIVE, RES_EVENT_SAME, NULL,
      &sidcart_clock, set_sid_clock, NULL },
    RESOURCE_INT_LIST_END
};

int sidcart_resources_init(void)
{
#ifdef HAVE_RESID
    sid_set_enable(0);
#endif
    if (sid_resources_init() < 0) {
        return -1;
    }
    return resources_register_int(sidcart_resources_int);
}

/* ------------------------------------------------------------------------- */

static const cmdline_option_t sidcart_cmdline_options[] =
{
    { "-sidcart", SET_RESOURCE, CMDLINE_ATTRIB_NONE,
      NULL, NULL, "SidCart", (resource_value_t)1,
      NULL, "Enable the SID cartridge" },
    { "+sidcart", SET_RESOURCE, CMDLINE_ATTRIB_NONE,
      NULL, NULL, "SidCart", (resource_value_t)0,
      NULL, "Disable the SID cartridge" },
    { "-sidcartaddress", SET_RESOURCE, CMDLINE_ATTRIB_NEED_ARGS,
      NULL, NULL, "SidAddress", NULL,
      "<address>", "SID cartridge address (0x8F00/0xE900)" },
    { "-sidcartclock", SET_RESOURCE, CMDLINE_ATTRIB_NEED_ARGS,
      NULL, NULL, "SidClock", NULL,
      "<clock>", "SID cartridge clock (0: C64 clock, 1: PET clock)" },
    CMDLINE_LIST_END
};

int sidcart_cmdline_options_init(void)
{
    if (sid_cmdline_options_init(SIDTYPE_SIDCART) < 0) {
        return -1;
    }
    return cmdline_register_options(sidcart_cmdline_options);
}
