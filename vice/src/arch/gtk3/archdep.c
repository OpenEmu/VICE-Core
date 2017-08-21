/*
 * archdep.c - Miscellaneous system-specific stuff.
 *
 * Written by
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
 *  Bas Wassink <b.wassink@ziggo.nl>
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


/** \file   src/arch/gtk3/archdep.c
 * \brief   Wrappers for architecture/OS-specific code
 *
 * I've decided to use GLib's use of the XDG specification and the standard
 * way of using paths on Windows. So some files may not be where the older
 * ports expect them to be. For example, vicerc will be in $HOME/.config/vice
 * now, not $HOME/.vice.
 */

#include "vice.h"

#include <stdio.h>
#include <glib.h>
#include <glib/gstdio.h>

#include "log.h"
#include "lib.h"
#include "machine.h"
#include "util.h"


/** \brief  Prefix used for autostart disk images
 */
#define AUTOSTART_FILENAME_PREFIX   "autostart-"


/** \brief  Suffix used for autostart disk images
 */
#define AUTOSTART_FILENAME_SUFFIX   ".d64"


/** \brief  Reference to argv[0]
 *
 * FIXME: this is only used once I think, better pass this as an argument to
 *        the function using it
 */
static char *argv0 = NULL;

#ifdef UNIX_COMPILE
#include "archdep_unix.c"
#endif

#ifdef WIN32_COMPILE
#include "archdep_win32.c"
#endif


/** \brief  Initialize the UI, a stub for now
 *
 * Theoretically, it should not have to matter what system we run on, as long
 * as it has Gtk3.
 */
static void archdep_ui_init(int arg, char **argv)
{
    /* do nothing, just like in src/arch/x11/gnome/x11ui.c */
}


/** \brief  Get the program name
 *
 * This returns the final part of argv[0], as if basename where used.
 *
 * \return  program name, heap-allocated, free with lib_free()
 */
char *archdep_program_name(void)
{
    return lib_stralloc(g_path_get_basename(argv0));

}


/** \brief  Get the absolute path to the VICE dir
 *
 * \return  Path to VICE's directory
 */
const gchar *archdep_boot_path(void)
{
    const char *boot;
    char *prg_name = archdep_program_name();

    boot = g_path_get_dirname(g_find_program_in_path(prg_name));
    lib_free(prg_name);
    return boot;
}


/** \brief  Get the user's home directory
 *
 * \return  current user's home directory
 */
const char *archdep_home_path(void)
{
    return g_get_home_dir();
}



char *archdep_user_config_path(void)
{
    char *path;
    gchar *tmp = g_build_path(path_separator, g_get_user_config_dir(),
            VICEUSERDIR, NULL);
    path = lib_stralloc(tmp);
    g_free(tmp);
    return path;
}

/** \brief  Determine if \a path is an absolute path
 *
 * \param[in]   path    some path
 *
 * \return  bool
 */
int archdep_path_is_relative(const char *path)
{
    return !g_path_is_absolute(path);
}


/** \brief  Generate path to the default fliplist file
 *
 * On Unix, this will return "$HOME/.config/vice/fliplist-$machine.vfl", on
 * Windows this should return "%APPDATA%\\vice\\fliplist-$machine.vfl".
 *
 * \return  path to defaul fliplist file, must be freed with lib_free()
 */
char *archdep_default_fliplist_file_name(void)
{
    gchar *path;
    char *name;
    char *tmp;

    name = util_concat("fliplist-", machine_get_name(), ".vfl", NULL);
    path = g_build_path(path_separator, g_get_user_config_dir(), VICEUSERDIR,
            name, NULL);
    lib_free(name);
    /* transfer ownership of path to VICE */
    tmp = lib_stralloc(path);
    g_free(path);
    return tmp;
}


/** \brief  Create path(s) used by VICE for user-data
 *
 * \return  0 on success, -1 on failure
 */
static void archdep_create_user_config_dir(void)
{
    char *path = archdep_user_config_path();

    /* create config dir, fail silently if it exists
     * XXX: perhaps I should stat on failure to see if the directory already
     * existed, or there was another failure */
    (void)g_mkdir(path, 0644);
    lib_free(path);
}


/** \brief  Generate default autostart disk image path
 *
 * The path will be "$cfgdir/autostart-$emu.d64". this needs to be freed with
 * lib_free().
 *
 * \return  path to autostart disk image
 */
char *archdep_default_autostart_disk_image_file_name(void)
{
    char *cfg;
    gchar *path;
    char *name;
    char *tmp;

    cfg = archdep_user_config_path();
    name = util_concat(AUTOSTART_FILENAME_PREFIX, machine_get_name(),
            AUTOSTART_FILENAME_SUFFIX, NULL);
    path = g_build_path(path_separator, cfg, name, NULL);
    lib_free(name);
    lib_free(cfg);
    /* transfer ownership from non/glib to VICE */
    tmp = lib_stralloc(path);
    g_free(path);
    return tmp;
}


/** \brief  Generate path to vice.ini
 *
 * The value returned needs to be freed using lib_free()
 *
 * \return  absolute path to vice.ini
 */
char *archdep_default_resource_file_name(void)
{
    char *cfg;
    gchar *tmp;
    char *path;

    cfg = archdep_user_config_path();
    tmp = g_build_path(path_separator, cfg, "vice.ini", NULL);
    /* transfer ownership to VICE */
    path = lib_stralloc(tmp);
    g_free(tmp);
    return path;
}



/** \brief  Open the default log file
 *
 * XXX: For now, this returns stdout, until I figure out why MacOSX duplicates
 *      fd 0 (stdin)
 */
FILE *archdep_open_default_log_file(void)
{
    INCOMPLETE_IMPLEMENTATION();
    return stdout;
}

void archdep_signals_init(int do_core_dumps)
{
    /* NOP: Gtk3 should handle any signals, I think */
}



/** \brief  Arch-dependent init
 *
 * \param[in]   argc    pointer to argument count
 * \param[in]   argv    argument vector
 *
 * \return  0
 */
int archdep_init(int *argc, char **argv)
{
    char *prg_name;
    char *cfg_path;
    char *searchpath;
    char *vice_ini;

    argv0 = lib_stralloc(argv[0]);

    archdep_create_user_config_dir();

    /* sanity checks, to remove later: */
    prg_name = archdep_program_name();
    searchpath = archdep_default_sysfile_pathlist("C64");
    cfg_path = archdep_user_config_path();
    vice_ini = archdep_default_resource_file_name();

    printf("progran name    = \"%s\"\n", prg_name);
    printf("user home dir   = \"%s\"\n", archdep_home_path());
    printf("user config dir = \"%s\"\n", cfg_path);
    printf("prg boot path   = \"%s\"\n", archdep_boot_path());
    printf("VICE searchpath = \"%s\"\n", searchpath);
    printf("vice.ini path   = \"%s\"\n", vice_ini);

    lib_free(prg_name);
    lib_free(vice_ini);

    /* needed for early log control (parses for -silent/-verbose) */
    log_verbose_init(*argc, argv);

    /* initialize the UI */
    archdep_ui_init(*argc, argv);
    return 0;
}

