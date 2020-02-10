/** \file   kbd.c
 * \brief   Headless UI keyboard stuff
 *
 * \author  Marco van den Heuvel <blackystardust68@yahoo.com>
 * \author  Michael C. Martin <mcmartin@gmail.com>
 * \author  Oliver Schaertel
 * \author  pottendo <pottendo@gmx.net>
 * \author  Bas Wassink <b.wassink@ziggo.nl>
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
#include "lib.h"
#include "log.h"
#include "ui.h"
#include <Carbon/Carbon.h>

/* UNIX-specific; for kbd_arch_get_host_mapping */
#include <locale.h>
#include <string.h>


#include "keyboard.h"
#include "kbd.h"


int kbd_arch_get_host_mapping(void)
{
    return KBD_MAPPING_US;
}


/** \brief  Initialize keyboard handling
 */
void kbd_arch_init(void)
{
    /* do NOT call kbd_hotkey_init(), keyboard.c calls this function *after*
     * the UI init stuff is called, allocating the hotkeys array again and thus
     * causing a memory leak
     */
}

void kbd_arch_shutdown(void)
{
    /* Also don't call kbd_hotkey_shutdown() here */
}

static const char *keycode_to_keyname[] = {
        [kVK_ANSI_A] = "kVK_ANSI_A",
        [kVK_ANSI_S] = "kVK_ANSI_S",
        [kVK_ANSI_D] = "kVK_ANSI_D",
        [kVK_ANSI_F] = "kVK_ANSI_F",
        [kVK_ANSI_H] = "kVK_ANSI_H",
        [kVK_ANSI_G] = "kVK_ANSI_G",
        [kVK_ANSI_Z] = "kVK_ANSI_Z",
        [kVK_ANSI_X] = "kVK_ANSI_X",
        [kVK_ANSI_C] = "kVK_ANSI_C",
        [kVK_ANSI_V] = "kVK_ANSI_V",
        [kVK_ANSI_B] = "kVK_ANSI_B",
        [kVK_ANSI_Q] = "kVK_ANSI_Q",
        [kVK_ANSI_W] = "kVK_ANSI_W",
        [kVK_ANSI_E] = "kVK_ANSI_E",
        [kVK_ANSI_R] = "kVK_ANSI_R",
        [kVK_ANSI_Y] = "kVK_ANSI_Y",
        [kVK_ANSI_T] = "kVK_ANSI_T",
        [kVK_ANSI_1] = "kVK_ANSI_1",
        [kVK_ANSI_2] = "kVK_ANSI_2",
        [kVK_ANSI_3] = "kVK_ANSI_3",
        [kVK_ANSI_4] = "kVK_ANSI_4",
        [kVK_ANSI_6] = "kVK_ANSI_6",
        [kVK_ANSI_5] = "kVK_ANSI_5",
        [kVK_ANSI_Equal] = "kVK_ANSI_Equal",
        [kVK_ANSI_9] = "kVK_ANSI_9",
        [kVK_ANSI_7] = "kVK_ANSI_7",
        [kVK_ANSI_Minus] = "kVK_ANSI_Minus",
        [kVK_ANSI_8] = "kVK_ANSI_8",
        [kVK_ANSI_0] = "kVK_ANSI_0",
        [kVK_ANSI_RightBracket] = "kVK_ANSI_RightBracket",
        [kVK_ANSI_O] = "kVK_ANSI_O",
        [kVK_ANSI_U] = "kVK_ANSI_U",
        [kVK_ANSI_LeftBracket] = "kVK_ANSI_LeftBracket",
        [kVK_ANSI_I] = "kVK_ANSI_I",
        [kVK_ANSI_P] = "kVK_ANSI_P",
        [kVK_ANSI_L] = "kVK_ANSI_L",
        [kVK_ANSI_J] = "kVK_ANSI_J",
        [kVK_ANSI_Quote] = "kVK_ANSI_Quote",
        [kVK_ANSI_K] = "kVK_ANSI_K",
        [kVK_ANSI_Semicolon] = "kVK_ANSI_Semicolon",
        [kVK_ANSI_Backslash] = "kVK_ANSI_Backslash",
        [kVK_ANSI_Comma] = "kVK_ANSI_Comma",
        [kVK_ANSI_Slash] = "kVK_ANSI_Slash",
        [kVK_ANSI_N] = "kVK_ANSI_N",
        [kVK_ANSI_M] = "kVK_ANSI_M",
        [kVK_ANSI_Period] = "kVK_ANSI_Period",
        [kVK_ANSI_Grave] = "kVK_ANSI_Grave",
        [kVK_ANSI_KeypadDecimal] = "kVK_ANSI_KeypadDecimal",
        [kVK_ANSI_KeypadMultiply] = "kVK_ANSI_KeypadMultiply",
        [kVK_ANSI_KeypadPlus] = "kVK_ANSI_KeypadPlus",
        [kVK_ANSI_KeypadClear] = "kVK_ANSI_KeypadClear",
        [kVK_ANSI_KeypadDivide] = "kVK_ANSI_KeypadDivide",
        [kVK_ANSI_KeypadEnter] = "kVK_ANSI_KeypadEnter",
        [kVK_ANSI_KeypadMinus] = "kVK_ANSI_KeypadMinus",
        [kVK_ANSI_KeypadEquals] = "kVK_ANSI_KeypadEquals",
        [kVK_ANSI_Keypad0] = "kVK_ANSI_Keypad0",
        [kVK_ANSI_Keypad1] = "kVK_ANSI_Keypad1",
        [kVK_ANSI_Keypad2] = "kVK_ANSI_Keypad2",
        [kVK_ANSI_Keypad3] = "kVK_ANSI_Keypad3",
        [kVK_ANSI_Keypad4] = "kVK_ANSI_Keypad4",
        [kVK_ANSI_Keypad5] = "kVK_ANSI_Keypad5",
        [kVK_ANSI_Keypad6] = "kVK_ANSI_Keypad6",
        [kVK_ANSI_Keypad7] = "kVK_ANSI_Keypad7",
        [kVK_ANSI_Keypad8] = "kVK_ANSI_Keypad8",
        [kVK_ANSI_Keypad9] = "kVK_ANSI_Keypad9",
        
        [kVK_Return] = "kVK_Return",
        [kVK_Tab] = "kVK_Tab",
        [kVK_Space] = "kVK_Space",
        [kVK_Delete] = "kVK_Delete",
        [kVK_Escape] = "kVK_Escape",
        [kVK_Command] = "kVK_Command",
        [kVK_Shift] = "kVK_Shift",
        [kVK_CapsLock] = "kVK_CapsLock",
        [kVK_Option] = "kVK_Option",
        [kVK_Control] = "kVK_Control",
        [kVK_RightCommand] = "kVK_RightCommand",
        [kVK_RightShift] = "kVK_RightShift",
        [kVK_RightOption] = "kVK_RightOption",
        [kVK_RightControl] = "kVK_RightControl",
        [kVK_Function] = "kVK_Function",
        [kVK_F17] = "kVK_F17",
        [kVK_VolumeUp] = "kVK_VolumeUp",
        [kVK_VolumeDown] = "kVK_VolumeDown",
        [kVK_Mute] = "kVK_Mute",
        [kVK_F18] = "kVK_F18",
        [kVK_F19] = "kVK_F19",
        [kVK_F20] = "kVK_F20",
        [kVK_F5] = "kVK_F5",
        [kVK_F6] = "kVK_F6",
        [kVK_F7] = "kVK_F7",
        [kVK_F3] = "kVK_F3",
        [kVK_F8] = "kVK_F8",
        [kVK_F9] = "kVK_F9",
        [kVK_F11] = "kVK_F11",
        [kVK_F13] = "kVK_F13",
        [kVK_F16] = "kVK_F16",
        [kVK_F14] = "kVK_F14",
        [kVK_F10] = "kVK_F10",
        [kVK_F12] = "kVK_F12",
        [kVK_F15] = "kVK_F15",
        [kVK_Help] = "kVK_Help",
        [kVK_Home] = "kVK_Home",
        [kVK_PageUp] = "kVK_PageUp",
        [kVK_ForwardDelete] = "kVK_ForwardDelete",
        [kVK_F4] = "kVK_F4",
        [kVK_End] = "kVK_End",
        [kVK_F2] = "kVK_F2",
        [kVK_PageDown] = "kVK_PageDown",
        [kVK_F1] = "kVK_F1",
        [kVK_LeftArrow] = "kVK_LeftArrow",
        [kVK_RightArrow] = "kVK_RightArrow",
        [kVK_DownArrow] = "kVK_DownArrow",
        [kVK_UpArrow] = "kVK_UpArrow",
};

static size_t keycode_to_keynum_len = sizeof(keycode_to_keyname) / sizeof(keycode_to_keyname[0]);


signed long kbd_arch_keyname_to_keynum(char *keyname)
{
    for (int i = 0; i < keycode_to_keynum_len; i++) {
        if (keycode_to_keyname[i] == NULL) {
            continue;
        }
        
        if (strcasecmp(keyname, keycode_to_keyname[i]) == 0) {
            return i;
        }
    }
    return -1;
}

const char *kbd_arch_keynum_to_keyname(signed long keynum)
{
    static char *keyname = "kVK_A";
    
    char const *res = NULL;
    
    if (keynum < keycode_to_keynum_len) {
        res = keycode_to_keyname[keynum];
    }
    
    return res ?: keyname;
}

void kbd_initialize_numpad_joykeys(int *joykeys)
{
}

/** \brief  Initialize the hotkeys
 *
 * This allocates an initial hotkeys array of HOTKEYS_SIZE_INIT elements
 */
void kbd_hotkey_init(void)
{
}

/** \brief  Clean up memory used by the hotkeys array
 */
void kbd_hotkey_shutdown(void)
{
}
