/** \file   archdep_default_logger.c
 * \brief   Write to default frogger
 *
 * \author  Marco van den Heuvel <blackystardust68@yahoo.com>
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

#include "archdep.h"
#include "archdep_defs.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef ARCHDEP_OS_WINDOWS
# include <windows.h>
#endif

#include "ioutil.h"
#include "lib.h"
#include "util.h"

#include "archdep_default_logger.h"


#ifdef ARCHDEP_OS_WINDOWS

/** \brief  Write message to Windows debugger/logger
 *
 * param[in]    level_string    log level string
 * param[in]    txt             log message
 *
 * \note    Shamelessly copied from win32/archdep.c
 *
 * \return  0 on success, < 0 on failure
 */
int archdep_default_logger(const char *level_string, const char *txt)
{
    char *out = lib_msprintf("*** %s %s\n", level_string, txt);
    OutputDebugString(out);
    lib_free(out);
    return 0;
}

#elif defined(ARCHDEP_OS_UNIX)

/** \brief  Write log message to stdout
 *
 * param[in]    level_string    log level string
 * param[in]    txt             log message
 *
 * \note    Shamelessly copied from unix/archdep.c
 *
 * \return  0 on success, < 0 on failure
 */
int archdep_default_logger(const char *level_string, const char *txt)
{
    if (fputs(level_string, stdout) == EOF
            || fprintf(stdout, "%s", txt) < 0
            || fputc ('\n', stdout) == EOF) {
        return -1;
    }
    return 0;
}

#elif defined(ARCHDEP_OS_BEOS)

int archdep_default_logger(const char *level_string, const char *txt)
{
    /* this is just silly */
    if (fputs(level_string, stdout) == EOF
            || fprintf(stdout, txt) < 0
            || fputc ('\n', stdout) == EOF) {
        return -1;
    }
    return 0;
}


#else
    /* Unsupported OS's (AmigaOS,*/

int archdep_default_logger(const char *level_string, const char *text)
{
    return 0;
}

#endif
