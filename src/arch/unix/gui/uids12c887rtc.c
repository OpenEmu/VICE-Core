/*
 * uids12c887rtc.c
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

#include <stdio.h>

#include "uilib.h"
#include "uimenu.h"
#include "uids12c887rtc.h"

UI_MENU_DEFINE_TOGGLE(DS12C887RTC)
UI_MENU_DEFINE_RADIO(DS12C887RTCbase)

static ui_menu_entry_t ds12c887rtc_c64_base_submenu[] = {
    { "$D500", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0xd500, NULL },
    { "$D600", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0xd600, NULL },
    { "$D700", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0xd700, NULL },
    { "$DE00", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0xde00, NULL },
    { "$DF00", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0xdf00, NULL },
    { NULL }
};

static ui_menu_entry_t ds12c887rtc_c128_base_submenu[] = {
    { "$D700", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0xd700, NULL },
    { "$DE00", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0xde00, NULL },
    { "$DF00", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0xdf00, NULL },
    { NULL }
};

static ui_menu_entry_t ds12c887rtc_vic20_base_submenu[] = {
    { "$9800", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0x9800, NULL },
    { "$9C00", UI_MENU_TYPE_TICK, (ui_callback_t)radio_DS12C887RTCbase,
      (ui_callback_data_t)0x9c00, NULL },
    { NULL }
};

ui_menu_entry_t ds12c887rtc_c64_submenu[] = {
    { N_("Enable"), UI_MENU_TYPE_TICK,
      (ui_callback_t)toggle_DS12C887RTC, NULL, NULL },
    { N_("Base address"), UI_MENU_TYPE_NORMAL,
      NULL, NULL, ds12c887rtc_c64_base_submenu },
    { NULL }
};

ui_menu_entry_t ds12c887rtc_c128_submenu[] = {
    { N_("Enable"), UI_MENU_TYPE_TICK,
      (ui_callback_t)toggle_DS12C887RTC, NULL, NULL },
    { N_("Base address"), UI_MENU_TYPE_NORMAL,
      NULL, NULL, ds12c887rtc_c128_base_submenu },
    { NULL }
};

ui_menu_entry_t ds12c887rtc_vic20_submenu[] = {
    { N_("Enable"), UI_MENU_TYPE_TICK,
      (ui_callback_t)toggle_DS12C887RTC, NULL, NULL },
    { N_("Base address"), UI_MENU_TYPE_NORMAL,
      NULL, NULL, ds12c887rtc_vic20_base_submenu },
    { NULL }
};
