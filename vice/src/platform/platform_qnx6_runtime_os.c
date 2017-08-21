/*
 * platform_qnx6_runtime_os.c - QNX 6.x runtime version discovery.
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

/* Tested and confirmed working on:
   - QNX 6.0
   - QNX 6.1
   - QNX 6.2
   - QNX 6.2.1
   - QNX 6.3
   - QNX 6.3.2
   - QNX 6.4
   - QNX 6.4.1
   - QNX 6.5
   - QNX 6.6
*/

#include "vice.h"

#ifdef __QNXNTO__

#include <sys/utsname.h>
#include <string.h>

#include "platform.h"

char platform_name[128];
static int got_os = 0;

char platform_cpu[20];
static int got_cpu = 0;

char *platform_get_qnx6_runtime_os(void)
{
    struct utsname name;

    if (!got_os) {
        uname(&name);
        sprintf(platform_name, "QNX %s", name.release);
        got_os = 1;
    }
    return platform_name;
}

char *platform_get_qnx6_runtime_cpu(void)
{
    struct utsname name;

    if (!got_cpu) {
        uname(&name);
        sprintf(platform_cpu, "%s", name.machine);
        got_cpu = 1;
    }
    return platform_cpu;
}
#endif
