/*
 * menu_plus4hw.c - PLUS4 HW menu for SDL UI.
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

#include "types.h"

#include "machine.h"
#include "menu_common.h"
#include "menu_joystick.h"
#include "menu_ram.h"
#include "menu_rom.h"
#include "petmodel.h"

#ifdef HAVE_RS232
#include "menu_rs232.h"
#endif

#include "menu_sid.h"
#include "pets.h"
#include "uimenu.h"

/* PET VIDEO SETTINGS */

/* PET MEMORY SETTINGS */

UI_MENU_DEFINE_RADIO(RamSize)
UI_MENU_DEFINE_RADIO(IOSize)
UI_MENU_DEFINE_TOGGLE(SuperPET)
UI_MENU_DEFINE_TOGGLE(Ram9)
UI_MENU_DEFINE_TOGGLE(RamA)

static const ui_menu_entry_t pet_memory_menu[] = {
    SDL_MENU_ITEM_TITLE("Memory size"),
    { "4kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_RamSize_callback,
      (ui_callback_data_t)4 },
    { "8kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_RamSize_callback,
      (ui_callback_data_t)8 },
    { "16kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_RamSize_callback,
      (ui_callback_data_t)16 },
    { "32kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_RamSize_callback,
      (ui_callback_data_t)32 },
    { "96kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_RamSize_callback,
      (ui_callback_data_t)96 },
    { "128kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_RamSize_callback,
      (ui_callback_data_t)128 },
    SDL_MENU_ITEM_SEPARATOR,
    SDL_MENU_ITEM_TITLE("I/O size"),
    { "256 bytes",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_IOSize_callback,
      (ui_callback_data_t)0x100 },
    { "2kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_IOSize_callback,
      (ui_callback_data_t)0x800 },
    SDL_MENU_ITEM_SEPARATOR,
    SDL_MENU_ITEM_TITLE("8296 memory blocks"),
    { "$9xxx as RAM",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_Ram9_callback,
      NULL },
    { "$Axxx as RAM",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_RamA_callback,
      NULL },
    SDL_MENU_ITEM_SEPARATOR,
    SDL_MENU_ITEM_TITLE("SUPERPET I/O"),
    { "Enable SUPERPET I/O (disables 8x96)",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_SuperPET_callback,
      NULL },
    SDL_MENU_LIST_END
};

/* PETREU */

UI_MENU_DEFINE_TOGGLE(PETREU)
UI_MENU_DEFINE_RADIO(PETREUsize)
UI_MENU_DEFINE_FILE_STRING(PETREUfilename)

static const ui_menu_entry_t petreu_menu[] = {
    { "Enable PET REU",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_PETREU_callback,
      NULL },
    SDL_MENU_ITEM_SEPARATOR,
    SDL_MENU_ITEM_TITLE("Memory size"),
    { "128kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_PETREUsize_callback,
      (ui_callback_data_t)128 },
    { "512kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_PETREUsize_callback,
      (ui_callback_data_t)512 },
    { "1024kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_PETREUsize_callback,
      (ui_callback_data_t)1024 },
    { "2048kB",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_PETREUsize_callback,
      (ui_callback_data_t)2048 },
    SDL_MENU_ITEM_SEPARATOR,
    SDL_MENU_ITEM_TITLE("RAM image"),
    { "PET REU image file",
      MENU_ENTRY_DIALOG,
      file_string_PETREUfilename_callback,
      (ui_callback_data_t)"Select PET REU image" },
    SDL_MENU_LIST_END
};

/* PETDWW */

UI_MENU_DEFINE_TOGGLE(PETDWW)
UI_MENU_DEFINE_FILE_STRING(PETDWWfilename)

static const ui_menu_entry_t petdww_menu[] = {
    { "Enable PET DWW",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_PETDWW_callback,
      NULL },
    SDL_MENU_ITEM_SEPARATOR,
    SDL_MENU_ITEM_TITLE("RAM image"),
    { "PET DWW image file",
      MENU_ENTRY_DIALOG,
      file_string_PETDWWfilename_callback,
      (ui_callback_data_t)"Select PET DWW image" },
    SDL_MENU_LIST_END
};

/* PET MODEL SELECTION */

static UI_MENU_CALLBACK(custom_PETModel_callback)
{
    int model, selected;

    selected = vice_ptr_to_int(param);

    if (activated) {
        petmodel_set(selected);
    } else {
        model = petmodel_get();

        if (selected == model) {
            return sdl_menu_text_tick;
        }
    }

    return NULL;
}

static const ui_menu_entry_t pet_model_menu[] = {
    { "2001", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_2001 },
    { "3008", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_3008 },
    { "3016", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_3016 },
    { "3032", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_3032 },
    { "3032B", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_3032B },
    { "4016", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_4016 },
    { "4032", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_4032 },
    { "4032B", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_4032B },
    { "8032", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_8032 },
    { "8096", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_8096 },
    { "8296", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_8296 },
    { "Super PET", MENU_ENTRY_RESOURCE_RADIO, custom_PETModel_callback, (ui_callback_data_t)PETMODEL_SUPERPET },
    SDL_MENU_LIST_END
};

UI_MENU_DEFINE_RADIO(KeymapIndex)

static const ui_menu_entry_t pet_keyboard_menu[] = {
    { "Graphics",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_KeymapIndex_callback,
      (ui_callback_data_t)2 },
    { "Business (UK)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_KeymapIndex_callback,
      (ui_callback_data_t)0 },
    SDL_MENU_LIST_END
};

UI_MENU_DEFINE_TOGGLE(Crtc)
UI_MENU_DEFINE_TOGGLE(UserportDAC)

const ui_menu_entry_t pet_hardware_menu[] = {
    { "Select PET model",
      MENU_ENTRY_SUBMENU,
      submenu_callback,
      (ui_callback_data_t)pet_model_menu },
    { "Keyboard",
      MENU_ENTRY_SUBMENU,
      submenu_radio_callback,
      (ui_callback_data_t)pet_keyboard_menu },
    SDL_MENU_ITEM_SEPARATOR,
    { "Joystick settings",
      MENU_ENTRY_SUBMENU,
      submenu_callback,
      (ui_callback_data_t)joystick_userport_only_menu },
    { "SID cart settings",
      MENU_ENTRY_SUBMENU,
      submenu_callback,
      (ui_callback_data_t)sid_pet_menu },
    { "RAM pattern settings",
      MENU_ENTRY_SUBMENU,
      submenu_callback,
      (ui_callback_data_t)ram_menu },
    { "ROM settings",
      MENU_ENTRY_SUBMENU,
      submenu_callback,
      (ui_callback_data_t)pet_rom_menu },
    { "PET REU settings",
      MENU_ENTRY_SUBMENU,
      submenu_callback,
      (ui_callback_data_t)petreu_menu },
    { "PET DWW settings",
      MENU_ENTRY_SUBMENU,
      submenu_callback,
      (ui_callback_data_t)petdww_menu },
    { "PET Userport DAC enable",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_UserportDAC_callback,
      NULL },
    { "Memory and I/O settings",
      MENU_ENTRY_SUBMENU,
      submenu_callback,
      (ui_callback_data_t)pet_memory_menu },
    { "CRTC chip enable",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_Crtc_callback,
      NULL },
#ifdef HAVE_RS232
    { "RS232 settings",
      MENU_ENTRY_SUBMENU,
      submenu_callback,
      (ui_callback_data_t)rs232_nouser_menu },
#endif
    SDL_MENU_LIST_END
};
