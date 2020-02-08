/** \file   uisnapshot.c
 * \brief   Snapshot dialogs and menu item handlers
 *
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

#include <gtk/gtk.h>
#include <stdlib.h>

#include "lib.h"
#include "util.h"
#include "archdep.h"
#include "basewidgets.h"
#include "widgethelpers.h"
#include "debug_gtk3.h"
#include "machine.h"
#include "resources.h"
#include "filechooserhelpers.h"
#include "openfiledialog.h"
#include "savefiledialog.h"
#include "selectdirectorydialog.h"
#include "basedialogs.h"
#include "interrupt.h"
#include "vsync.h"
#include "snapshot.h"
#include "vice-event.h"
#include "uistatusbar.h"
#include "ui.h"
#include "uiapi.h"

#include "uisnapshot.h"


/*****************************************************************************
 *                              Helper functions                             *
 ****************************************************************************/


/** \brief  Construct filename for quickload/quicksave snapshots
 *
 * \return  filename for the quickload/save file, heap-allocated by VICE, so
 *          free after use with lib_free()
 */
static char *quicksnap_filename(void)
{
    char *fname;
    const char *mname;
    char *cfg;

    mname = machine_class == VICE_MACHINE_C64SC ? "c64sc" : machine_name;
    cfg = archdep_user_config_path();
    fname = util_concat(cfg, "/", mname, ".vsf", NULL);
#if 0
    lib_free(cfg);
#endif
    debug_gtk3("quicksnap_filename = %s.", fname);
    return fname;
}


/** \brief  Show dialog to save a snapshot
 */
static void save_snapshot_dialog(void)
{
    GtkWidget *dialog;
    GtkWidget *extra;
    GtkWidget *roms_widget;
    GtkWidget *disks_widget;
    gint response_id;
    int save_roms;
    int save_disks;

    dialog = gtk_file_chooser_dialog_new("Save snapshot file",
            ui_get_active_window(),
            GTK_FILE_CHOOSER_ACTION_SAVE,
            "Save", GTK_RESPONSE_ACCEPT,
            "Cancel", GTK_RESPONSE_CANCEL,
            NULL);

    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),
            create_file_chooser_filter(file_chooser_filter_snapshot, FALSE));

    /* create extras widget */
    extra = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(extra), 16);

    disks_widget = gtk_check_button_new_with_label("Save attached disks");
    roms_widget = gtk_check_button_new_with_label("Save attached ROMs");
    gtk_grid_attach(GTK_GRID(extra), disks_widget, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(extra), roms_widget, 1, 0, 1, 1);
    gtk_widget_show_all(extra);

    gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(dialog), extra);

    response_id = gtk_dialog_run(GTK_DIALOG(dialog));
    save_roms = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(roms_widget));
    save_disks = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(disks_widget));

    debug_gtk3("response_id = %d.", response_id);
    debug_gtk3("save disks = %s.", save_disks ? "YES" : "NO");
    debug_gtk3("save ROMs = %s.", save_roms ? "YES" : "NO");

    if (response_id == GTK_RESPONSE_ACCEPT) {
        gchar *filename;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (filename != NULL) {
            char *fname_copy;
            char buffer[1024];

            fname_copy = util_add_extension_const(filename, "vsf");

            if (machine_write_snapshot(fname_copy, save_roms, save_disks, 0) < 0) {
                snapshot_display_error();
                g_snprintf(buffer, 1024, "Failed to save snapshot '%s'",
                        fname_copy);
            } else {
                debug_gtk3("Wrote snapshot file '%s'.", fname_copy);
                g_snprintf(buffer, 1024, "Saved snapshot '%s'", fname_copy);
            }
            lib_free(fname_copy);
            g_free(filename);
        }
    }
    gtk_widget_destroy(dialog);
}


/*****************************************************************************
 *                              CPU trap handlers                            *
 ****************************************************************************/


/** \brief  CPU trap handler for the load snapshot dialog
 *
 * \param[in]   addr    memory address (unused)
 * \param[in]   data    unused
 */
static void load_snapshot_trap(uint16_t addr, void *data)
{
    const char *filters[] = { "*.vsf", NULL };
    gchar *filename;
    gchar buffer[1024];

    vsync_suspend_speed_eval();

    filename = vice_gtk3_open_file_dialog("Open snapshot file",
            "Snapshot files", filters, NULL);
    if (filename != NULL) {
        /* load snapshot */
        if (machine_read_snapshot(filename, 0) < 0) {
            snapshot_display_error();
            g_snprintf(buffer, 1024, "Failed to load snapshot '%s'", filename);
        } else {
            g_snprintf(buffer, 1024, "Loaded snapshot '%s'", filename);
        }
        g_free(filename);
    }
}


/** \brief  CPU trap handler to trigger the Save dialog
 *
 * \param[in]   addr    memory address (unused)
 * \param[in]   data    unused
 */
static void save_snapshot_trap(uint16_t addr, void *data)
{
    vsync_suspend_speed_eval();
    save_snapshot_dialog();
}


/** \brief  CPU trap handler for the QuickLoad snapshot menu item
 *
 * \param[in]   addr    memory address (unused)
 * \param[in]   data    quickload snapshot filename
 */
static void quickload_snapshot_trap(uint16_t addr, void *data)
{
    char *filename = (char *)data;

    vsync_suspend_speed_eval();

    debug_gtk3("Quickloading file '%s'.", filename);
    if (machine_read_snapshot(filename, 0) < 0) {
        snapshot_display_error();
    }
    lib_free(filename);
}


/** \brief  CPU trap handler for the QuickSave snapshot menu item
 *
 * \param[in]   addr    memory address (unused)
 * \param[in]   data    quicksave snapshot filename
 */
static void quicksave_snapshot_trap(uint16_t addr, void *data)
{
    char *filename = (char *)data;

    vsync_suspend_speed_eval();

    debug_gtk3("Quicksaving file '%s'.", filename);
    if (machine_write_snapshot(filename, TRUE, TRUE, 0) < 0) {
        snapshot_display_error();
    }
    lib_free(filename);
}


/*****************************************************************************
 *                              Public functions                             *
 ****************************************************************************/


/** \brief  Display UI to load a snapshot file
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 *
 * \return  TRUE
 */
gboolean uisnapshot_open_file(GtkWidget *parent, gpointer user_data)
{
    if (!ui_pause_active()) {
        interrupt_maincpu_trigger_trap(load_snapshot_trap, NULL);
    } else {
        load_snapshot_trap(0, NULL);
    }
    return TRUE;
}


/** \brief  Display UI to save a snapshot file
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 *
 * \return  TRUE
 */
gboolean uisnapshot_save_file(GtkWidget *parent, gpointer user_data)
{
    if (!ui_pause_active()) {
        interrupt_maincpu_trigger_trap(save_snapshot_trap, NULL);
    } else {
        save_snapshot_trap(0, NULL);
    }
    return TRUE;
}


/** \brief  Gtk event handler for the QuickLoad menu item
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 */
gboolean uisnapshot_quickload_snapshot(GtkWidget *parent, gpointer user_data)
{
    char *fname = quicksnap_filename();

    interrupt_maincpu_trigger_trap(quickload_snapshot_trap, (void *)fname);
    return TRUE;
}


/** \brief  Gtk event handler for the QuickSave menu item
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 *
 * \return  TRUE
 */
gboolean uisnapshot_quicksave_snapshot(GtkWidget *parent, gpointer user_data)
{
    char *fname = quicksnap_filename();

    interrupt_maincpu_trigger_trap(quicksave_snapshot_trap, (void *)fname);
    return TRUE;
}


#if 0
/** \brief  Gtk event handler for the "Select history directory" menu item
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 */
void uisnapshot_history_select_dir(GtkWidget *parent, gpointer user_data)
{
    char *filename;
    const char *current;

    if (resources_get_string("EventSnapshotDir", &current) < 0) {
        debug_gtk3("failed to get current history directory, using NULL.");
        current = NULL;
    }

    filename = vice_gtk3_select_directory_dialog("Select history directory",
            NULL, TRUE, current);
    if (filename != NULL) {
        debug_gtk3("Setting EventSnapshotDir to '%s'.", filename);
        if (resources_set_string("EventSnapshotDir", filename) < 0) {
            vice_gtk3_message_error("VICE core error",
                    "Failed to set history directory.");
        }
        g_free(filename);
    }
}
#endif


/** \brief  Gtk event handler for the "Start recording events" menu item
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 *
 * \return  TRUE
 */
gboolean uisnapshot_history_record_start(GtkWidget *parent, gpointer user_data)
{
    event_record_start();
    ui_display_recording(1);
    return TRUE;
}


/** \brief  Gtk event handler for the "Stop recording events" menu item
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 *
 * \return  TRUE
 */
gboolean uisnapshot_history_record_stop(GtkWidget *parent, gpointer user_data)
{
    event_record_stop();
    ui_display_recording(0);
    return TRUE;
}


/** \brief  Gtk event handler for the "Start playing back events" menu item
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 *
 * \return  TRUE
 */
gboolean uisnapshot_history_playback_start(GtkWidget *parent, gpointer user_data)
{
    event_playback_start();
    return TRUE;
}



/** \brief  Gtk event handler for the "Stop playing back events" menu item
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 *
 * \return  TRUE
 */
gboolean uisnapshot_history_playback_stop(GtkWidget *parent, gpointer user_data)
{
    event_playback_stop();
    return TRUE;
}


/** \brief  Gtk event handler for the "Set recording milestone" menu item
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 *
 * \return  TRUE
 */
gboolean uisnapshot_history_milestone_set(GtkWidget *parent, gpointer user_data)
{
    event_record_set_milestone();
    return TRUE;
}


/** \brief  Gtk event handler for the "Return to milestone" menu item
 *
 * \param[in]   parent      parent widget
 * \param[in]   user_data   unused
 *
 * \return  TRUE
 */
gboolean uisnapshot_history_milestone_reset(GtkWidget *parent, gpointer user_data)
{
    event_record_reset_milestone();
    return TRUE;
}
