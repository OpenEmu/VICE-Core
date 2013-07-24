/*
 * console.c - Console access interface.
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "console.h"
#include "lib.h"
#include "log.h"
#include "ui.h"
#include "uimon.h"

static console_t *console_log_local = NULL;

#if defined(HAVE_READLINE)
#include <readline/readline.h>
#include <readline/history.h>
#else
static FILE *mon_input, *mon_output;
#endif

int console_init(void)
{
#if defined(HAVE_READLINE) && defined(HAVE_RLNAME)
    rl_readline_name = "VICE";
#endif

    return 0;
}

console_t *uimon_window_open(void)
{
    if (!isatty(fileno(stdin))) {
        log_error(LOG_DEFAULT, "console_open: stdin is not a tty.");
        console_log_local = NULL;
    }
    else if (!isatty(fileno(stdout))) {
        log_error(LOG_DEFAULT, "console_open: stdout is not a tty.");
        console_log_local = NULL;
    }
    else {
        console_log_local = lib_malloc(sizeof(console_t));

#if !defined(HAVE_READLINE)
        mon_input = stdin;
        mon_output = stdout;
#endif

        console_log_local->console_xres = 80;
        console_log_local->console_yres = 25;
        console_log_local->console_can_stay_open = 1;
        console_log_local->console_cannot_output = 0;
    }
    ui_focus_monitor();
    return console_log_local;
}

void uimon_window_close(void)
{
    lib_free(console_log_local);
    console_log_local = NULL;

    uimon_window_suspend();
}

void uimon_window_suspend( void )
{
    ui_restore_focus();
#ifdef HAVE_MOUSE
    ui_check_mouse_cursor();
#endif
}

console_t *uimon_window_resume(void)
{
    if (console_log_local) {
        ui_focus_monitor();
        return console_log_local;
    }
    log_error(LOG_DEFAULT, "uimon_window_resume: log was not opened.");
    return uimon_window_open();
}

int uimon_out(const char *buffer)
{
    fprintf(stdout, "%s", buffer);
    return 0;
}

#ifndef HAVE_READLINE
static char *readline(const char *prompt)
{
    char *p = lib_malloc(1024);

    console_out(NULL, "%s", prompt);

    fflush(mon_output);
    if (fgets(p, 1024, mon_input) == NULL) {
        /* FIXME: handle error */
    }

    /* Remove trailing newlines.  */
    {
        int len;

        for (len = strlen(p); len > 0 && (p[len - 1] == '\r' || p[len - 1] == '\n'); len--) {
            p[len - 1] = '\0';
        }
    }

    return p;
}
#endif

char *uimon_get_in(char **ppchCommandLine, const char *prompt)
{
    char *p, *ret_sting;

    p = readline(prompt);
    if (p && *p) {
        add_history(p);
    }
    ret_sting = lib_stralloc(p);
    free(p);

    return ret_sting;
}

int console_close_all(void)
{
    return 0;
}

void uimon_notify_change( void )
{
}

void uimon_set_interface(struct monitor_interface_s **monitor_interface_init, int count)
{
}

