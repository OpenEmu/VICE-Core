/*
 * kcs.c - Cartridge handling, KCS cart.
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

/* #define DEBUG_KCS */
#ifdef DEBUG_KCS
#define DBG(_x_)        log_debug _x_
#else
#define DBG(_x_)
#endif

#include "vice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CARTRIDGE_INCLUDE_SLOTMAIN_API
#include "c64cartsystem.h"
#undef CARTRIDGE_INCLUDE_SLOTMAIN_API
#include "c64export.h"
#include "cartio.h"
#include "cartridge.h"
#include "crt.h"
#include "kcs.h"
#include "log.h"
#include "maincpu.h"
#include "monitor.h"
#include "snapshot.h"
#include "types.h"
#include "util.h"

/*
    KCS Power Cartridge

    - 16kb ROM, 128 bytes RAM

    io1:
    - the second last page of the first 8k ROM bank is visible
    - when reading, bit 1 of the address selects mapping mode (EXROM)
    - when writing, bit 1 of the address selects mapping mode (EXROM)
      FIXME: its more complicated than that

    io2:
    - cartridge RAM (128 bytes)
    - writes go to cartridge RAM
    - when reading, if bit 7 of the address is set, freeze mode (nmi)
      is released. (FIXME: does not match schematic)

    - the cartridge starts in 16k game mode

    FIXME: the above is still not 100% correct
     - BLOADing a frozen program does not work/hangs
     - restarting from the freezer ("continue") often hangs

    the original software uses:

    bit $de00   -> ROMH off

    sta $de00
    sta $de02
    sta $de80   -> ROMH on (a000)

    bit $df80 at beginning of freezer NMI

    ... and also code is running in deXX area
*/

/*
 * ROM is selected if:                 OE = (!IO1 = 0) | !(!ROMH & !ROML)
 * RAM is selected if:                 CS = PHI2 & (!A4) & (!IO2)           ?
 */

/*      74LS275 (4 flip flops)
 *
 *      !reset <- !reset
 *      C1     <- 74LS00 pin8
 *
 *      Q (out)      D (in)
 *
 *      D6     Q0 <- D0  !GAME
 *      !GAME  Q1 <- D1  R/!W
 *      D7     Q2 <- D2  !EXROM
 *      !EXROM Q3 <- D3  74LS02 pin13
 *
 */

static int config;
#ifdef DEBUG_KCS
static int oldconfig;
#endif

static BYTE kcs_io1_read(WORD addr)
{
#ifdef DEBUG_KCS
    oldconfig = config;
#endif
    /* FIXME: the software reads from de00 - and from all kind of adresses when
     *        code in the IO1 ROM mirror is running. the following is not exactly
     *        what happens */
    /* !EXROM = A1 & 74LS90 pin8=0 */
    /* config = (addr & 2) ? CMODE_RAM : CMODE_8KGAME; */
    config = 0; /* exrom, game */
    config |= (addr & 2) ? 2 : 0; /* exrom */
    config |= 0; /* game */

#ifdef DEBUG_KCS
    if(oldconfig != config) {
        DBG(("KCS: io1 r de%02x cfg: %s -> %s", addr, cart_config_string(oldconfig), cart_config_string(config)));
    }
#endif
    cart_config_changed_slotmain((BYTE)config, (BYTE)config, CMODE_READ);
    return roml_banks[0x1e00 + (addr & 0xff)];
}

static BYTE kcs_io1_peek(WORD addr)
{
    return roml_banks[0x1e00 + (addr & 0xff)];
}

static void kcs_io1_store(WORD addr, BYTE value)
{
    int mode = CMODE_WRITE;
#ifdef DEBUG_KCS
    oldconfig = config;
#endif

#if 0
    /* FIXME: the software writes to de80 - what that does is unknown */
    if ((addr & 0x80) == 0x80) {
        mode |= CMODE_RELEASE_FREEZE;
    }
#endif
    /* FIXME: the other two adresses written to are de00 and de02 */
    if ((addr & 0xff) == 2) {
        config ^= 2; /* invert exrom */
    } else {
        config &= ~2; /* clear exrom */
        config |= (addr & 2) ? 2 : 0; /* exrom */;
    }
    config |= 1; /* game */

#ifdef DEBUG_KCS
    if(oldconfig != config) {
        DBG(("KCS: io1 w de%02x,%02x cfg: %s -> %s", addr, value, cart_config_string(oldconfig), cart_config_string(config)));
    }
#endif
    cart_config_changed_slotmain((BYTE)config, (BYTE)config, mode);
}

static BYTE kcs_io2_read(WORD addr)
{
#if 1
    /* the software reads from df80 at beginning of nmi handler */
    /* FIXME: nothing really is connected to addr bit7 according to the schematics */
    if (addr & 0x80) {
        cart_config_changed_slotmain((BYTE)config, (BYTE)config, CMODE_READ | CMODE_RELEASE_FREEZE);
    }
#endif
    /* FIXME: in the schematics A4 is connected to CS - this smells like a mistake */
    return export_ram0[addr & 0x7f];
}

static BYTE kcs_io2_peek(WORD addr)
{
    /* FIXME: in the schematics A4 is connected to CS - this smells like a mistake */
    return export_ram0[addr & 0x7f];
}

static void kcs_io2_store(WORD addr, BYTE value)
{
#if 0
    /* FIXME: nothing really is connected to addr bit7 according to the schematics */
    if (addr & 0x80) {
        cart_config_changed_slotmain((BYTE)config, (BYTE)config, CMODE_WRITE | CMODE_RELEASE_FREEZE);
    }
#endif
    /* FIXME: in the schematics A4 is connected to CS - this smells like a mistake */
    export_ram0[addr & 0x7f] = value;
}

static int kcs_io1_dump(void)
{
    mon_out("EXROM: %d GAME: %d (%s)\n", ((config >> 1) & 1), (config & 1) ^ 1, cart_config_string(config & 3));
    return 0;
}

/* ---------------------------------------------------------------------*/

static io_source_t kcs_io1_device = {
    CARTRIDGE_NAME_KCS_POWER,
    IO_DETACH_CART,
    NULL,
    0xde00, 0xdeff, 0xff,
    1, /* read is always valid */
    kcs_io1_store,
    kcs_io1_read,
    kcs_io1_peek,
    kcs_io1_dump,
    CARTRIDGE_KCS_POWER,
    0,
    0
};

static io_source_t kcs_io2_device = {
    CARTRIDGE_NAME_KCS_POWER,
    IO_DETACH_CART,
    NULL,
    0xdf00, 0xdfff, 0xff,
    1, /* read is always valid */
    kcs_io2_store,
    kcs_io2_read,
    kcs_io2_peek,
    NULL,
    CARTRIDGE_KCS_POWER,
    0,
    0
};

static io_source_list_t *kcs_io1_list_item = NULL;
static io_source_list_t *kcs_io2_list_item = NULL;

static const c64export_resource_t export_res_kcs = {
    CARTRIDGE_NAME_KCS_POWER, 1, 1, &kcs_io1_device, &kcs_io2_device, CARTRIDGE_KCS_POWER
};

/* ---------------------------------------------------------------------*/

void kcs_freeze(void)
{
    config = CMODE_ULTIMAX;
    cart_config_changed_slotmain(config, config, CMODE_READ /* | CMODE_RELEASE_FREEZE*/);
}

void kcs_config_init(void)
{
    config = CMODE_16KGAME;
    cart_config_changed_slotmain(config, config, CMODE_READ);
}

void kcs_config_setup(BYTE *rawcart)
{
    memcpy(roml_banks, rawcart, 0x2000);
    memcpy(romh_banks, &rawcart[0x2000], 0x2000);
    config = CMODE_16KGAME;
    cart_config_changed_slotmain(config, config, CMODE_READ);
}

/* ---------------------------------------------------------------------*/

static int kcs_common_attach(void)
{
    if (c64export_add(&export_res_kcs) < 0) {
        return -1;
    }

    kcs_io1_list_item = io_source_register(&kcs_io1_device);
    kcs_io2_list_item = io_source_register(&kcs_io2_device);
    return 0;
}

int kcs_bin_attach(const char *filename, BYTE *rawcart)
{
    if (util_file_load(filename, rawcart, 0x4000, UTIL_FILE_LOAD_SKIP_ADDRESS) < 0) {
        return -1;
    }
    return kcs_common_attach();
}

int kcs_crt_attach(FILE *fd, BYTE *rawcart)
{
    crt_chip_header_t chip;
    int i;

    for (i = 0; i <= 1; i++) {
        if (crt_read_chip_header(&chip, fd)) {
            return -1;
        }

        if ((chip.start != 0x8000 && chip.start != 0xa000) || chip.size != 0x2000) {
            return -1;
        }

        if (crt_read_chip(rawcart, chip.start - 0x8000, &chip, fd)) {
            return -1;
        }
    }

    return kcs_common_attach();
}

void kcs_detach(void)
{
    c64export_remove(&export_res_kcs);
    io_source_unregister(kcs_io1_list_item);
    io_source_unregister(kcs_io2_list_item);
    kcs_io1_list_item = NULL;
    kcs_io2_list_item = NULL;
}

/* ---------------------------------------------------------------------*/

#define CART_DUMP_VER_MAJOR   0
#define CART_DUMP_VER_MINOR   3
#define SNAP_MODULE_NAME  "CARTKCS"

int kcs_snapshot_write_module(snapshot_t *s)
{
    snapshot_module_t *m;

    m = snapshot_module_create(s, SNAP_MODULE_NAME,
                               CART_DUMP_VER_MAJOR, CART_DUMP_VER_MINOR);
    if (m == NULL) {
        return -1;
    }

    if (0
        || (SMW_B(m, (BYTE)config) < 0)
        || (SMW_BA(m, roml_banks, 0x2000) < 0)
        || (SMW_BA(m, romh_banks, 0x2000) < 0)
        || (SMW_BA(m, export_ram0, 128) < 0)) {
        snapshot_module_close(m);
        return -1;
    }

    snapshot_module_close(m);
    return 0;
}

int kcs_snapshot_read_module(snapshot_t *s)
{
    BYTE vmajor, vminor;
    snapshot_module_t *m;

    m = snapshot_module_open(s, SNAP_MODULE_NAME, &vmajor, &vminor);
    if (m == NULL) {
        return -1;
    }

    if ((vmajor != CART_DUMP_VER_MAJOR) || (vminor != CART_DUMP_VER_MINOR)) {
        snapshot_module_close(m);
        return -1;
    }

    if (0
        || (SMR_B_INT(m, &config) < 0)
        || (SMR_BA(m, roml_banks, 0x2000) < 0)
        || (SMR_BA(m, romh_banks, 0x2000) < 0)
        || (SMR_BA(m, export_ram0, 128) < 0)) {
        snapshot_module_close(m);
        return -1;
    }

    snapshot_module_close(m);

    return kcs_common_attach();
}
