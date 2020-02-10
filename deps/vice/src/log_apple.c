/*
 * log.c - Logging facility.
 *
 * Written by
 *  Ettore Perazzoli <ettore@comm2000.it>
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

#include <os/log.h>

#include <stdarg.h>
#include <stdio.h>

#include "lib.h"
#include "log.h"
#include "util.h"

#ifdef DBGLOGGING
#define DBG(x) printf x
#else
#define DBG(x)
#endif

#define VICE_SUBSYSTEM "org.openemu.VICE"

static os_log_t VICE_LOG;
static os_log_t* logs;
static log_t num_logs = 0;

static int log_enabled = 1; /* cv: this flag allows to temporarly disable all logging */
static int verbose = 0;

static int log_verbose_opt(const char *param, void *extra_param)
{
    verbose = vice_ptr_to_int(extra_param);
    return 0;
}

static int log_silent_opt(const char *param, void *extra_param)
{
    int silent = vice_ptr_to_int(extra_param);
    log_enabled = ! silent;
    return 0;
}

int log_set_verbose(int n)
{
    if (n) {
        return log_verbose_opt(NULL, (void*)1);
    }
    return log_verbose_opt(NULL, (void*)0);
}


int log_set_silent(int n)
{
    log_enabled = !n;
    return 0;
}


int log_verbose_init(int argc, char **argv)
{
    return 0;
}

#ifndef __X1541__

int log_resources_init(void)
{
    return 0;
}

void log_resources_shutdown(void)
{
}

int log_cmdline_options_init(void)
{
    return 0;
}
#endif

/* ------------------------------------------------------------------------- */

int log_init_with_fd(FILE *f)
{
    return 0;
}

int log_init(void)
{
    VICE_LOG  = os_log_create("org.openemu.VICE", "");
    return (VICE_LOG == NULL) ? -1 : 0;
}

log_t log_open(const char *id)
{
    log_t new_log = 0;
    log_t i;
    
    for (i = 0; i < num_logs; i++) {
        if (logs[i] == NULL) {
            new_log = i;
            break;
        }
    }
    if (i == num_logs) {
        new_log = num_logs++;
        logs = lib_realloc(logs, sizeof(os_log_t) * num_logs);
    }
    
    logs[new_log] = os_log_create(VICE_SUBSYSTEM, id);
    
    return new_log;
}

int log_close(log_t log)
{
    if (logs[(unsigned int)log] == NULL) {
        return -1;
    }
    
    logs[(unsigned int)log] = NULL;
    
    return 0;
}

void log_close_all(void)
{
    log_t i;
    
    for (i = 0; i < num_logs; i++) {
        log_close(i);
    }
    
    lib_free(logs);
    logs = NULL;
}

static int log_helper(log_t log, uint8_t log_type, const char *format,
                      va_list ap)
{
    if (!log_enabled) {
        return 0;
    }
    
    const signed int logi = (signed int)log;
    if ((logi != LOG_DEFAULT) && (logi != LOG_ERR)) {
        if ((logs == NULL) || (logi < 0)|| (logi >= num_logs) || (logs[logi] == NULL)) {
            return -1;
        }
    }
    
    os_log_t logger = NULL;
    if ((logi != LOG_DEFAULT) && (logi != LOG_ERR) && (logs[logi] != NULL)) {
        logger = logs[logi];
    } else {
        logger = VICE_LOG;
    }
    
    char *logtxt = lib_mvsprintf(format, ap);
    os_log_with_type(logger, log_type, "%{public}s", logtxt);
    lib_free(logtxt);
    
    return 0;
}

int log_message(log_t log, const char *format, ...)
{
    va_list ap;
    int rc;
    
    va_start(ap, format);
    rc = log_helper(log, OS_LOG_TYPE_DEFAULT, format, ap);
    va_end(ap);
    
    return rc;
}

int log_warning(log_t log, const char *format, ...)
{
    va_list ap;
    int rc;
    
    va_start(ap, format);
    rc = log_helper(log, OS_LOG_TYPE_INFO, format, ap);
    va_end(ap);
    
    return rc;
}

int log_error(log_t log, const char *format, ...)
{
    va_list ap;
    int rc;
    
    va_start(ap, format);
    rc = log_helper(log, OS_LOG_TYPE_ERROR, format, ap);
    va_end(ap);
    
    return rc;
}

int log_debug(const char *format, ...)
{
    va_list ap;
    int rc;
    
    va_start(ap, format);
    rc = log_helper(LOG_DEFAULT, OS_LOG_TYPE_DEBUG, format, ap);
    va_end(ap);
    
    return rc;
}

int log_verbose(const char *format, ...)
{
    va_list ap;
    int rc = 0;
    
    va_start(ap, format);
    if (verbose) {
        rc = log_helper(LOG_DEFAULT, OS_LOG_TYPE_DEBUG, format, ap);
    }
    va_end(ap);
    
    return rc;
}

void log_enable(int on)
{
    log_enabled = on;
}
