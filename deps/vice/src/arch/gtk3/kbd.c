/** \file   kbd.c
 * \brief   Native GTK3 UI keyboard stuff
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
#include <gtk/gtk.h>
#include "debug_gtk3.h"
#include "lib.h"
#include "log.h"
#include "ui.h"
#include "kbddebugwidget.h"
#include "keyboard.h"
#include "kbd.h"
#include "uimenu.h"

/*
 * Forward declarations
 */
static gboolean kbd_hotkey_handle(GdkEvent *report);


/** \brief  Initial size of the hotkeys array
 */
#define HOTKEYS_SIZE_INIT   64


/** \brief  List of custom hotkeys
 */
static kbd_gtk3_hotkey_t *hotkeys_list = NULL;


/** \brief  Size of the hotkeys array
 *
 * This will be HOTKEYS_SIZE_INIT element after initializing and will grow
 * by doubling its size when the array is full.
 */
static int hotkeys_size = 0;


/** \brief  Number of registered hotkeys
 */
static int hotkeys_count = 0;




/** \brief  Initialize keyboard handling
 */
void kbd_arch_init(void)
{
    /* do NOT call kbd_hotkey_init(), keyboard.c calls this function *after*
     * the UI init stuff is called, allocating the hotkeys array again and thus
     * causing a memory leak
     */
}


/** \brief  Shutdown keyboard handling (NOP)
 */
void kbd_arch_shutdown(void)
{
    /* Also don't call kbd_hotkey_shutdown() here */
}


/** \brief  Get keynum from \a keyname
 *
 * \param[in]   keyname key name as a string
 *
 * \return  keynum or -1 on error
 *
 * \note    You don't see "signed $type-not-plain-char" too often :)
 */
signed long kbd_arch_keyname_to_keynum(char *keyname)
{
    guint sym = gdk_keyval_from_name(keyname);
    /* printf("kbd_arch_keyname_to_keynum %s=%u\n", keyname, sym); */

    if (sym == GDK_KEY_VoidSymbol) {
        return -1;
    }

    return (signed long)sym;
}


/** \brief  Get keyname from keynum
 *
 * \param[in]   keynum  key number
 *
 * \return  key value for \a keynum
 */
const char *kbd_arch_keynum_to_keyname(signed long keynum)
{
    return gdk_keyval_name((guint)keynum);
}


/** \brief  Initialize numpad joystick key mapping
 *
 * \param[out]  joykeys pointer to joykey array
 */
void kbd_initialize_numpad_joykeys(int *joykeys)
{
    joykeys[0] = GDK_KEY_KP_0;
    joykeys[1] = GDK_KEY_KP_1;
    joykeys[2] = GDK_KEY_KP_2;
    joykeys[3] = GDK_KEY_KP_3;
    joykeys[4] = GDK_KEY_KP_4;
    joykeys[5] = GDK_KEY_KP_6;
    joykeys[6] = GDK_KEY_KP_7;
    joykeys[7] = GDK_KEY_KP_8;
    joykeys[8] = GDK_KEY_KP_9;
}


/** \brief  Get modifiers keys for keyboard event
 *
 * \param[in]   report  GDK keypress event
 *
 * \return  bitmasks of modifiers combined into a single int
 */
static int kbd_get_modifier(GdkEvent *report)
{
    int ret = 0;
    if (report->key.state & GDK_SHIFT_MASK) {
        ret |= KBD_MOD_LSHIFT;
    }
    if (report->key.state & GDK_SHIFT_MASK) {
        ret |= KBD_MOD_RSHIFT;
    }
    if (report->key.state & GDK_MOD1_MASK) {
        ret |= KBD_MOD_LALT;
    }
    if (report->key.state & GDK_MOD5_MASK) {
        ret |= KBD_MOD_RALT;
    }
    if (report->key.state & GDK_CONTROL_MASK) {
        ret |= KBD_MOD_LCTRL;
    }
    return ret;
}


/** \brief  Gtk keyboard event handler
 *
 * \param[in]   widget  widget triggering the event
 * \param[in]   report  event object
 * \param[in]   gp      extra data (unused)
 *
 * \return  TRUE if event handled (won't get passed to the emulated machine)
 */
static gboolean kbd_event_handler(GtkWidget *w, GdkEvent *report, gpointer gp)
{
    gint key;

    key = report->key.keyval;
    switch (report->type) {
        case GDK_KEY_PRESS:
            /* fprintf(stderr, "GDK_KEY_PRESS: %u %04x.\n",
                       report->key.keyval,  report->key.state); */
#ifdef WIN32_COMPILE
/* HACK: The Alt-Gr Key seems to work differently on windows and linux.
         On Linux one Keypress "ISO_Level3_Shift" will be produced, and
         the modifier mask for combined keys will be GDK_MOD5_MASK.
         On Windows two Keypresses will be produced, first "Control_L"
         then "Alt_R", and the modifier mask for combined keys will be
         GDK_MOD2_MASK.
         The following is a hack to compensate for that and make it
         always work like on linux.
*/
            if ((report->key.keyval == GDK_KEY_Alt_R) && (report->key.state & GDK_MOD2_MASK)) {
                /* Alt-R with modifier MOD2 */
                key = report->key.keyval = GDK_KEY_ISO_Level3_Shift;
                report->key.state &= ~GDK_MOD2_MASK;
                /* release control in the emulated keymap */
                keyboard_key_released(GDK_KEY_Control_L, KBD_MOD_LCTRL);
            } else if (report->key.state & GDK_MOD2_MASK) {
                report->key.state &= ~GDK_MOD2_MASK;
                report->key.state |= GDK_MOD5_MASK;
            }
            /* fprintf(stderr, "               %u %04x.\n",
                       report->key.keyval,  report->key.state); */
#endif
            kdb_debug_widget_update(report);

            if (gtk_window_activate_key(GTK_WINDOW(w), (GdkEventKey *)report)) {
                return TRUE;
            }
            /* For some reason, the Alt-D of going fullscreen doesn't
             * return true when CAPS LOCK isn't on, but only it does
             * this. */
            if (key == GDK_KEY_d && report->key.state & GDK_MOD1_MASK) {
                return TRUE;
            }

            /* check the custom hotkeys */
            if (kbd_hotkey_handle(report)) {
                return TRUE;
            }

#if 0
            if ((key == GDK_KEY_p || key == GDK_KEY_P)
                    && (report->key.state & GDK_MOD1_MASK)) {
                debug_gtk3("Got Alt+P");
                ui_toggle_pause();
                return TRUE;
            }
#endif

            keyboard_key_pressed((signed long)key, kbd_get_modifier(report));
            return TRUE;
        case GDK_KEY_RELEASE:
            /* fprintf(stderr, "GDK_KEY_RELEASE: %u %04x.\n",
                       report->key.keyval,  report->key.state); */
#ifdef WIN32_COMPILE
            /* HACK: remap control,alt+r to alt-gr, see above */
            if (report->key.keyval == GDK_KEY_Alt_R) {
                key = report->key.keyval = GDK_KEY_ISO_Level3_Shift;
            }
            /* fprintf(stderr, "                 %u %04x.\n",
                       report->key.keyval,  report->key.state); */
#endif
            if (key == GDK_KEY_Shift_L ||
                key == GDK_KEY_Shift_R ||
                key == GDK_KEY_ISO_Level3_Shift) {
                keyboard_key_clear();
            }
            keyboard_key_released(key, kbd_get_modifier(report));
            break;
        case GDK_ENTER_NOTIFY:
        case GDK_LEAVE_NOTIFY:
        case GDK_FOCUS_CHANGE:
            keyboard_key_clear();
            break;
        default:
            break;
    }                           /* switch */
    return FALSE;
}


/** \brief  Connect keyboard event handlers to the current window
 *
 * \param[in]   widget  GtkWindow instance
 * \param[in]   data    extra event data
 */
void kbd_connect_handlers(GtkWidget *widget, void *data)
{
    g_signal_connect(
            G_OBJECT(widget),
            "key-press-event",
            G_CALLBACK(kbd_event_handler), data);
    g_signal_connect(
            G_OBJECT(widget),
            "key-release-event",
            G_CALLBACK(kbd_event_handler), data);
    g_signal_connect(
            G_OBJECT(widget),
            "enter-notify-event",
            G_CALLBACK(kbd_event_handler), data);
    g_signal_connect(
            G_OBJECT(widget),
            "leave-notify-event",
            G_CALLBACK(kbd_event_handler), data);
}


/*
 * Hotkeys (keyboard shortcuts not connected to any GtkMenuItem) handling
 *
 * FIXME:   This approach is somewhat brittle, a better approach could be using
 *          GAction's in combination with GMenu vs GtkMenu. Something to
 *          consider for 3.5 (involves a lot of rewriting of Gtk code).
 */


/** \brief  Initialize the hotkeys
 *
 * This allocates an initial hotkeys array of HOTKEYS_SIZE_INIT elements
 */
void kbd_hotkey_init(void)
{
    debug_gtk3("initializing hotkeys list.");
    hotkeys_list = lib_malloc(HOTKEYS_SIZE_INIT * sizeof *hotkeys_list);
    hotkeys_size = HOTKEYS_SIZE_INIT;
    hotkeys_count = 0;
}



/** \brief  Clean up memory used by the hotkeys array
 */
void kbd_hotkey_shutdown(void)
{
    debug_gtk3("cleaning up memory used by the hotkeys.");
    lib_free(hotkeys_list);
}


/** \brief  Find hotkey index
 *
 * \param[in]   code    key code
 * \param[in]   mask    key mask
 *
 * \return  index in list, -1 when not found
 */
static int kbd_hotkey_get_index(guint code, guint mask)
{
    int i = 0;

    while (i < hotkeys_count) {
        if (hotkeys_list[i].code == code && hotkeys_list[i].mask) {
            return i;
        }
        i++;
    }
    return -1;
}


/** \brief  Look up the requested hotkey and trigger its callback when found
 *
 * \param[in]   report  GDK key press event instance
 *
 * \return  TRUE when the key was found and the callback triggered,
 *          FALSE otherwise
 */
static gboolean kbd_hotkey_handle(GdkEvent *report)
{
    int i = 0;
    gint code = report->key.keyval;

#if 0
    debug_gtk3("got code %d.", code);
#endif
    if (((GdkEventKey*)(report))->state & VICE_MOD_MASK) {

        while (i < hotkeys_count) {
#if 0
            debug_gtk3("checking index %d: hotkey %d, mask %d",
                    i, hotkeys_list[i].code, hotkeys_list[i].mask);
#endif
            if (hotkeys_list[i].code == code) {
#if 0
                debug_gtk3("Got non-modified key %d.", code);
#endif
                if (report->key.state & hotkeys_list[i].mask) {
#if 0
                        debug_gtk3("got modifers");
                        debug_gtk3("triggering callback of hotkey with index %d.", i);
#endif
                        hotkeys_list[i].callback();
                        return TRUE;
                    }
            }
            i++;
        }
    }
    return FALSE;
}


/** \brief  Add hotkey to the list
 *
 * \param[in]   code        GDK key code
 * \param[in]   mask        GDK key modifier bitmask
 * \param[in]   callback    function to call when hotkey is triggered
 *
 * \return  bool
 */
gboolean kbd_hotkey_add(guint code, guint mask, void (*callback)(void))
{
    if (callback == NULL) {
        log_error(LOG_ERR, "Error: NULL passed as callback.");
        return FALSE;
    }
    if (kbd_hotkey_get_index(code, mask) >= 0) {
        log_error(LOG_ERR, "Error: hotkey already registered.");
        return FALSE;
    }

    /* resize list? */
    if (hotkeys_count == hotkeys_size) {
        int new_size = hotkeys_size * 2;
        debug_gtk3("Resizing hotkeys list to %d items.", new_size);
        hotkeys_list = lib_realloc(
                hotkeys_list,
                (size_t)new_size * sizeof *hotkeys_list);
        hotkeys_size = new_size;
    }


    /* register hotkey */
    hotkeys_list[hotkeys_count].code = code;
    hotkeys_list[hotkeys_count].mask = mask;
    hotkeys_list[hotkeys_count].callback = callback;
    hotkeys_count++;
    return TRUE;
}


/** \brief  Add multiple hotkeys at once
 *
 * Adds multiple hotkeys from \a list. Terminate the list with NULL for the
 * callback value.
 *
 * \param[in]   list    list of hotkeys
 *
 * \return  TRUE on success, FALSE if the list was exhausted or a hotkey
 *          was already registered
 */
gboolean kbd_hotkey_add_list(kbd_gtk3_hotkey_t *list)
{
    int i = 0;

    while (list[i].callback != NULL) {
        if (!kbd_hotkey_add(list[i].code, list[i].mask, list[i].callback)) {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}
