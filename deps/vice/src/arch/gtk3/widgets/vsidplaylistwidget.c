/** \file   vsidplaylistwidget.c
 * \brief   GTK3 playlist widget for VSID
 *
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

/*
 * Icons used by this file:
 *
 * $VICEICON    actions/media-skip-backward
 * $VICEICON    actions/media-seek-backward
 * $VICEICON    actions/media-seek-forward
 * $VICEICON    actions/media-skip-forward
 * $VICEICON    status/media-playlist-repeat
 * $VICEICON    status/media-playlist-shuffle
 * $VICEICON    actions/list-add
 * $VICEICON    actions/list-remove
 * $VICEICON    actions/document-open
 * $VICEICON    actions/document-save
 * $VICEICON    actions/edit-clear-all
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
#include <string.h>

#include "vice_gtk3.h"
#include "debug_gtk3.h"
#include "filechooserhelpers.h"
#include "hvsc.h"
#include "resources.h"
#include "uivsidwindow.h"

/* TODO: move and rename `convert_to_utf8()` */
#include "vsidtuneinfowidget.h"
#include "vsidplaylistadddialog.h"

#include "vsidplaylistwidget.h"


/** \brief  Control button types
 */
enum {
    CTRL_ACTION,    /**< action button: simple push button */
    CTRL_TOGGLE     /**< toggle button: for 'repeat' and 'shuffle' */
};


/** \brief  Playlist control button struct
 */
typedef struct plist_ctrl_button_s {
    const char *icon_name;  /**< icon-name in the Gtk3 theme */
    int         type;       /**< type of button (action/toggle) */
    void        (*callback)(GtkWidget *, gpointer); /**< callback function */
    const char *tooltip;    /**< tooltip text */
} plist_ctrl_button_t;


/** \brief  Playlist state object
 *
 * This can be used to get the current playlist state without going through
 * some of the boilerplate hell of Gtk's tree/list models/views
 */
typedef struct plist_state_s {
    GtkTreeModel *      model;
    GtkTreeSelection *  selection;
    GtkTreeIter         iter;
    GtkTreePath *       path;
    gchar *             path_str;
} plist_state_t;


/** \brief  Type of context menu item types
 */
typedef enum {
    CTX_MENU_ACTION,    /**< action */
    CTX_MENU_SEP        /**< menu separator */
} ctx_menu_item_type_t;


/** \brief  Context menu item object
 */
typedef struct ctx_menu_item_s {
    const char *            text;   /**< displayed text */
    /**< item callback */
    gboolean                (*callback)(GtkWidget *, gpointer);
    ctx_menu_item_type_t    type;   /**< menu item type, \see ctx_menu_item_type_t */
} ctx_menu_item_t;


/** \brief  VSID Hotkey info object
 */
typedef struct vsid_hotkey_s {
    guint keyval;                                   /**< GDK key value */
    guint modifiers;                                /**< GDK modifiers */
    gboolean (*callback)(GtkWidget *, gpointer);    /**< hotkey callback */
} vsid_hotkey_t;


/*
 * Forward declarations
 */


static gboolean delete_all_rows(GtkWidget *widget, gpointer data);
static gboolean delete_selected_rows(GtkWidget *widget, gpointer data);
static gboolean open_add_dialog(GtkWidget *widget, gpointer data);

static void on_playlist_append_clicked(GtkWidget *widget, gpointer data);
static void on_playlist_remove_clicked(GtkWidget *widget, gpointer data);
static void on_playlist_next_clicked(GtkWidget *widget, gpointer data);
static void on_playlist_prev_clicked(GtkWidget *widget, gpointer data);
static void on_playlist_first_clicked(GtkWidget *widget, gpointer data);
static void on_playlist_last_clicked(GtkWidget *widget, gpointer data);
static void on_playlist_clear_clicked(GtkWidget *widget, gpointer data);


/** \brief  List of playlist controls
 */
static const plist_ctrl_button_t controls[] = {
    { "media-skip-backward", CTRL_ACTION,
        on_playlist_first_clicked,
        "Go to start of playlist" },
    { "media-seek-backward", CTRL_ACTION,
        on_playlist_prev_clicked,
        "Go to previous tune" },
    { "media-seek-forward", CTRL_ACTION,
        on_playlist_next_clicked,
        "Go to next tune" },
    { "media-skip-forward", CTRL_ACTION,
        on_playlist_last_clicked,
        "Go to end of playlist" },
    { "media-playlist-repeat", CTRL_TOGGLE,
        NULL,
        "Repeat playlist" },
    { "media-playlist-shuffle", CTRL_TOGGLE,
        NULL,
        "Shuffle playlist" },
    { "list-add", CTRL_ACTION,
        on_playlist_append_clicked,
        "Add tune to playlist" },
    { "list-remove", CTRL_ACTION,
        on_playlist_remove_clicked,
        "Remove selected tune from playlist" },
    { "document-open", CTRL_ACTION,
        NULL,
        "Open playlist file" },
    { "document-save", CTRL_ACTION,
        NULL,
        "Save playlist file" },
    { "edit-clear-all", CTRL_ACTION,
        on_playlist_clear_clicked,
        "Clear playlist" },
    { NULL, 0, NULL, NULL }
};


/** \brief  Playlist context menu items
 */
static const ctx_menu_item_t cmenu_items[] = {
    { "Play", NULL, CTX_MENU_ACTION },
    { "Delete selected item(s)", delete_selected_rows, CTX_MENU_ACTION },
    { "---", NULL, CTX_MENU_SEP },
    { "Export binary", NULL, CTX_MENU_ACTION },
    { NULL, NULL, -1 }
};


/** \brief  Playlist hotkeys
 */
static const vsid_hotkey_t hotkeys[] = {
    { GDK_KEY_Insert,   0,              open_add_dialog },
    { GDK_KEY_Delete,   0,              delete_selected_rows },
    { GDK_KEY_Delete,   GDK_SHIFT_MASK, delete_all_rows },
    { 0, 0, NULL }
};



/** \brief  Reference to the playlist model
 */
static GtkListStore *playlist_model;

/** \brief  Reference to the playlist view
 */
static GtkWidget *playlist_view;


static GtkWidget *title_widget;


/** \brief  Initialize playlist \a state object
 *
 * Initializes \a state to an invalid/empty state
 *
 * \param[out]  state   playlist state object
 */
static void playlist_state_init(plist_state_t *state)
{
    state->selection = NULL;
    state->model = NULL;

    /* TODO: init iter to some initial state */

    state->path = NULL;
    state->path_str = NULL;
}


/** \brief  Get current playlist state
 *
 * Get the Gtk tree model, selection, iterator, path and all that jazz of the
 * playlist.
 *
 * \param[out]  state   playlist state
 *
 * \return  TRUE if valid iterator and thus state.
 */
static gboolean playlist_state_get(plist_state_t *state)
{
    playlist_state_init(state);

    state->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(playlist_view));
    if (gtk_tree_selection_get_selected(state->selection,
                                        &(state->model),
                                        &(state->iter))) {
            /* got proper iter */
            state->path = gtk_tree_model_get_path(state->model, &(state->iter));
            state->path_str = gtk_tree_path_to_string(state->path);
            return TRUE;
    }
    return FALSE;
}


/** \brief  Free members of \a state and reset \a state for next use
 *
 * \param[in,out]   state   playlist state object
 */
static void playlist_state_free(plist_state_t *state)
{
    if (state->path != NULL) {
        gtk_tree_path_free(state->path);
    }
    if (state->path_str != NULL) {
        g_free(state->path_str);
    }

    /* reset for future use */
    playlist_state_init(state);
}


/** \brief  Add SID files to the playlist
 *
 * \param[in,out]   files   List of selected files
 *
 * WARNING: this function frees its argument and probably shouldn't do that!
 *
 */
static void add_files_callback(GSList *files)
{
    GSList *pos = files;

    if (files == NULL) {
        return;
    }

    do {
        const char *path = (const char *)(pos->data);
        debug_gtk3("Adding SID: %s", path);
        vsid_playlist_widget_append_file(path);
        pos = g_slist_next(pos);
    } while (pos != NULL);
    g_slist_free(files);
}


/** \brief  Delete selected rows
 *
 * \param[in]   widget  widget triggering the event
 * \param[in]   data    extra event data (unused)
 *
 * \return  FALSE on error
 */
static gboolean delete_selected_rows(GtkWidget *widget, gpointer data)
{
    GtkTreeModel *model;
    GtkTreeSelection *selection;
    GList *rows;
    GList *elem;

    /* get model in the correct type for gtk_tree_model_get_iter(),
     * taking the address of GTK_TREE_MODEL(M) doesn't work.
     */
    model = GTK_TREE_MODEL(playlist_model);
    /* get current selection */
    selection = gtk_tree_view_get_selection(
            GTK_TREE_VIEW(playlist_view));
    /* get rows in the selection, which is a GList of GtkTreePath *'s */
    rows = gtk_tree_selection_get_selected_rows(
            selection,
            &model);

    /* iterate the list of rows in reverse order to avoid
     * invalidating the GtkTreePath*'s in the list
     */
    for (elem = g_list_last(rows); elem != NULL; elem = elem->prev) {
        GtkTreePath *path;
        GtkTreeIter iter;

        /* delete row */
        path = elem->data;
        if (gtk_tree_model_get_iter(
                    GTK_TREE_MODEL(playlist_model),
                    &iter,
                    path)) {
            gtk_list_store_remove(playlist_model, &iter);
        } else {
            return FALSE;
        }
    }
    g_list_free(rows);
    return TRUE;
}


/** \brief  Delete the entire playlist
 *
 * \param[in]   widget  widget triggering the callback (unused)
 * \param[in]   data    event reference (unused)
 *
 * \return  TRUE
 */
static gboolean delete_all_rows(GtkWidget *widget, gpointer data)
{
    gtk_list_store_clear(playlist_model);
    return TRUE;
}


/** \brief  Callback for the Insert hotkey
 *
 * \param[in]   widget  widget triggering the callback (unused)
 * \param[in]   data    event reference (unused)
 *
 * \return  TRUE
 */
static gboolean open_add_dialog(GtkWidget *widget, gpointer data)
{
    vsid_playlist_add_dialog_exec(add_files_callback);
    return TRUE;
}



/** \brief  Update title of the widget with number of entries in the list
 */
static void update_title(void)
{
    gchar buffer[256];
    gint rows;

    /* get number of top level items by using NULL as an iter */
    rows = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(playlist_model), NULL);

    g_snprintf(buffer, 256, "<b>Playlist (%d)</b>", rows);
    gtk_label_set_markup(GTK_LABEL(title_widget), buffer);
}


/*
 * Event handlers
 */


/** \brief  Event handler for the 'destroy' event of the playlist widget
 *
 * \param[in]   widget  playlist widget
 * \param[in    data    extra event data (unused)
 */
static void on_destroy(GtkWidget *widget, gpointer data)
{
    vsid_playlist_add_dialog_free();
}


/** \brief  Event handler for the 'row-activated' event of the view
 *
 * Triggered by double-clicking on a SID file in the view
 *
 * \param[in,out]   view    the GtkTreeView instance
 * \param[in]       path    the path to the activated row
 * \param[in]       column  the column in the \a view (unused)
 * \param[in]       data    extra event data (unused)
 */
static void on_row_activated(GtkTreeView *view,
                             GtkTreePath *path,
                             GtkTreeViewColumn *column,
                             gpointer data)
{
    GtkTreeIter iter;
    GValue value = G_VALUE_INIT;
    const gchar *filename;

    if (!gtk_tree_model_get_iter(GTK_TREE_MODEL(playlist_model), &iter, path)) {
        debug_gtk3("error: failed to get tree iter.");
        return;
    }

    gtk_tree_model_get_value(GTK_TREE_MODEL(playlist_model), &iter, 2, &value);
    filename = g_value_get_string(&value);
    debug_gtk3("got filename '%s'.", filename);

    ui_vsid_window_load_psid(filename);

    g_value_unset(&value);
}



/** \brief  Event handler for the 'add SID' button
 *
 * \param[in]   widget  button triggering the event (unused)
 * \param[in]   data    extra event data (unused)
 */
static void on_playlist_append_clicked(GtkWidget *widget, gpointer data)
{
    vsid_playlist_add_dialog_exec(add_files_callback);
}


/** \brief  Event handler for the 'remove' button
 *
 * \param[in]   widget  button triggering the event
 * \param[in]   data    extra event data (unused)
 */
static void on_playlist_remove_clicked(GtkWidget *widget, gpointer data)
{
    GtkTreeSelection *selection;
    GtkTreeIter iter;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(playlist_view));
    if (gtk_tree_selection_get_selected(selection,
                                        NULL,
                                        &iter)) {
        debug_gtk3("got valid iter.");
        gtk_list_store_remove(playlist_model, &iter);
    }
}


static void on_playlist_first_clicked(GtkWidget *widget, gpointer data)
{
    plist_state_t state;

    debug_gtk3("clicked!");

    if (playlist_state_get(&state)) {
        debug_gtk3("got valid iterator: %s", state.path_str);

        if (gtk_tree_model_get_iter_first(state.model, &(state.iter))) {

            GValue value = G_VALUE_INIT;
            const gchar *filename;

            gtk_tree_selection_select_iter(state.selection, &(state.iter));
            gtk_tree_model_get_value(GTK_TREE_MODEL(state.model),
                    &(state.iter), 2, &value);
            filename = g_value_get_string(&value);
            debug_gtk3("got filename '%s'.", filename);

            ui_vsid_window_load_psid(filename);

            g_value_unset(&value);

            playlist_state_free(&state);
        }
    }
}


static void on_playlist_last_clicked(GtkWidget *widget, gpointer data)
{
    plist_state_t state;

    debug_gtk3("clicked!");

    if (playlist_state_get(&state)) {

        GValue value = G_VALUE_INIT;
        const gchar *filename;
        GtkTreeIter temp;

        debug_gtk3("got valid iterator: %s", state.path_str);

        /* This is complete bollocks, there is no gtk_tree_model_iter_last() */
        temp = state.iter;
        while (gtk_tree_model_iter_next(state.model, &(state.iter))) {
            temp = state.iter;
        }

        gtk_tree_selection_select_iter(state.selection, &temp);
        gtk_tree_model_get_value(GTK_TREE_MODEL(state.model),
                &(temp), 2, &value);
        filename = g_value_get_string(&value);
        debug_gtk3("got filename '%s'.", filename);

        ui_vsid_window_load_psid(filename);

        g_value_unset(&value);

        playlist_state_free(&state);
    }
}

static void on_playlist_clear_clicked(GtkWidget *widget, gpointer data)
{
    debug_gtk3("Called!");
    delete_all_rows(NULL, NULL);
}


static void on_playlist_next_clicked(GtkWidget *widget, gpointer data)
{
#if 0
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkTreePath *path;
    gchar *pstr;
#endif

    plist_state_t state;

    debug_gtk3("clicked!");

    if (playlist_state_get(&state)) {
        debug_gtk3("got valid iterator: %s", state.path_str);

        if (gtk_tree_model_iter_next(state.model, &(state.iter))) {

            GValue value = G_VALUE_INIT;
            const gchar *filename;

            gtk_tree_selection_select_iter(state.selection, &(state.iter));

            gtk_tree_model_get_value(GTK_TREE_MODEL(state.model),
                    &(state.iter), 2, &value);
            filename = g_value_get_string(&value);
            debug_gtk3("got filename '%s'.", filename);

            ui_vsid_window_load_psid(filename);

            g_value_unset(&value);

            playlist_state_free(&state);
        }
    }
}


/** \brief  Go to previous entry in the playlist
 *
 * \param[in]   widget  button triggering the event (unused)
 * \param[in]   data    extra event data (unused)
 */
static void on_playlist_prev_clicked(GtkWidget *widget, gpointer data)
{
#if 0
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkTreePath *path;
    gchar *pstr;
#endif

    plist_state_t state;

    debug_gtk3("clicked!");

    if (playlist_state_get(&state)) {
        debug_gtk3("got valid iterator: %s", state.path_str);

        if (gtk_tree_model_iter_previous(state.model, &(state.iter))) {

            GValue value = G_VALUE_INIT;
            const gchar *filename;

            gtk_tree_selection_select_iter(state.selection, &(state.iter));

            gtk_tree_model_get_value(GTK_TREE_MODEL(state.model),
                    &(state.iter), 2, &value);
            filename = g_value_get_string(&value);
            debug_gtk3("got filename '%s'.", filename);

            ui_vsid_window_load_psid(filename);

            g_value_unset(&value);

            playlist_state_free(&state);
        }
    }
}


#if 0
static void on_playlist_open_clicked(GtkWidget *widget, gpointer data)
{
}
#endif


/** \brief  Create context menu for the playlist
 *
 * \return  GtkMenu
 */
static GtkWidget *create_context_menu(void)
{
    GtkWidget *menu;
    GtkWidget *item;
    int i;

    menu = gtk_menu_new();

    for (i = 0; cmenu_items[i].text != NULL; i++) {

        if (cmenu_items[i].type == CTX_MENU_SEP) {
            item = gtk_separator_menu_item_new();
        } else {
            item = gtk_menu_item_new_with_label(cmenu_items[i].text);
            if (cmenu_items[i].callback != NULL) {
                g_signal_connect(
                        item,
                        "activate",
                        G_CALLBACK(cmenu_items[i].callback),
                        GINT_TO_POINTER(i));
            }
        }
        gtk_container_add(GTK_CONTAINER(menu), item);

    }
    gtk_widget_show_all(menu);
    return menu;
}



/** \brief  Event handler for button press events on the playlist
 *
 * Pops up a context menu on the playlist.
 *
 * \param[in,out]   view    playlist view widget
 * \param[in]       event   event reference
 * \param[in]       data    extra even data
 *
 * \return  TRUE when event consumed and no further propagation is needed
 */
static gboolean on_button_press_event(GtkWidget *view,
                                      GdkEvent *event,
                                      gpointer data)
{

    if (((GdkEventButton *)event)->button == GDK_BUTTON_SECONDARY) {
        GtkWidget *menu = create_context_menu();

        gtk_menu_popup_at_pointer(GTK_MENU(menu), event);
        return TRUE;
    } else {
        return FALSE;
    }
}


/** \brief  Event handler for key press events on the playlist
 *
 * Delete:  Delete selected rows
 *
 * \param[in,out]   view    playlist view widget
 * \param[in]       event   event reference
 * \param[in]       data    extra even data
 *
 * \return  TRUE when event consumed and no further propagation is needed
 */
static gboolean on_key_press_event(GtkWidget *view,
                                   GdkEvent *event,
                                   gpointer data)
{
    GdkEventType type = ((GdkEventKey *)event)->type;

    if (type == GDK_KEY_PRESS) {
        guint keyval = ((GdkEventKey *)event)->keyval;  /* key ID */
        guint modifiers = ((GdkEventKey *)event)->state;    /* modifiers */
        int i;

        for (i = 0; hotkeys[i].keyval != 0; i++) {
            if (hotkeys[i].keyval == keyval
                    && hotkeys[i].modifiers == modifiers) {
                if (hotkeys[i].callback != NULL) {
                    return hotkeys[i].callback(view, event);
                }
            }
        }
    }
    return FALSE;
}




/** \brief  Create playlist model
 */
static void vsid_playlist_model_create(void)
{
    playlist_model = gtk_list_store_new(
            3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
}


/** \brief  Create playlist view widget
 *
 */
static void vsid_playlist_view_create(void)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeSelection *selection;

    playlist_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(playlist_model));

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(
            "Title",
            renderer,
            "text", 0,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(playlist_view), column);

    column = gtk_tree_view_column_new_with_attributes(
            "Composer",
            renderer,
            "text", 1,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(playlist_view), column);

    column = gtk_tree_view_column_new_with_attributes(
            "Path",
            renderer,
            "text", 2,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(playlist_view), column);

    /* Allow selecting multiple items (for deletion) */
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(playlist_view));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);

    /*
     * Set event handlers
     */

    /* Enter/double-click */
    g_signal_connect(playlist_view, "row-activated",
            G_CALLBACK(on_row_activated), NULL);
    /* context menu (right-click, or left-click) */
    g_signal_connect(playlist_view, "button-press-event",
            G_CALLBACK(on_button_press_event), playlist_view);
    /* special keys (Del for now) */
    g_signal_connect(playlist_view, "key-press-event",
            G_CALLBACK(on_key_press_event), playlist_view);
}


/** \brief  Create a grid with a list of buttons to control the playlist
 *
 * Most of the playlist should also be controllable via a context-menu
 * (ie mouse right-click), which is a big fat TODO now.
 *
 * \return  GtkGrid
 */
static GtkWidget *vsid_playlist_controls_create(void)
{
    GtkWidget *grid;
    int i;

    grid = vice_gtk3_grid_new_spaced(0, VICE_GTK3_DEFAULT);
    for (i = 0; controls[i].icon_name != NULL; i++) {
        GtkWidget *button;
        gchar buff[1024];

        g_snprintf(buff, 1024, "%s-symbolic", controls[i].icon_name);

        button = gtk_button_new_from_icon_name(
                buff,
                GTK_ICON_SIZE_LARGE_TOOLBAR);
        /* always show icon and don't grab focus on click/tab */
        gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);
        gtk_widget_set_can_focus(button, FALSE);

        gtk_grid_attach(GTK_GRID(grid), button, i, 0, 1, 1);
        if (controls[i].callback != NULL) {
            g_signal_connect(
                    button, "clicked",
                    G_CALLBACK(controls[i].callback),
                    (gpointer)(controls[i].icon_name));
        }
        if (controls[i].tooltip != NULL) {
            gtk_widget_set_tooltip_text(button, controls[i].tooltip);
        }
    }
    return grid;
}



/** \brief  Create main playlist widget
 *
 * \return  GtkGrid
 */
GtkWidget *vsid_playlist_widget_create(void)
{
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *scroll;

    vsid_playlist_model_create();
    vsid_playlist_view_create();

    grid = vice_gtk3_grid_new_spaced(VICE_GTK3_DEFAULT, VICE_GTK3_DEFAULT);

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<b>Playlist</b>");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    g_object_set(label, "margin-bottom", 8, NULL);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    title_widget = label;

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scroll, 400, 500);
    gtk_widget_set_hexpand(scroll, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_container_add(GTK_CONTAINER(scroll), playlist_view);

    gtk_grid_attach(GTK_GRID(grid), scroll, 0, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid),
            vsid_playlist_controls_create(),
            0, 2, 1, 1);

    g_signal_connect(grid, "destroy", G_CALLBACK(on_destroy), NULL);

    gtk_widget_show_all(grid);
    return grid;
}


/** \brief  Append \a path to the playlist
 *
 * \param[in[   path    path to SID file
 *
 * \return  TRUE on success, FALSE on failure
 */
gboolean vsid_playlist_widget_append_file(const gchar *path)
{
    GtkTreeIter iter;
    hvsc_psid_t psid;
    char name[HVSC_PSID_TEXT_LEN + 1];
    char author[HVSC_PSID_TEXT_LEN + 1];
    char *name_utf8;
    char *author_utf8;

    /* Attempt to parse sid header for title & composer */
    if (!hvsc_psid_open(path, &psid)) {
        vice_gtk3_message_error("VSID",
                "Failed to parse PSID header of '%s'.",
                path);
        return FALSE;
    }

    /* get SID name and author */
    memcpy(name, psid.name, HVSC_PSID_TEXT_LEN);
    name[HVSC_PSID_TEXT_LEN] = '\0';
    memcpy(author, psid.author, HVSC_PSID_TEXT_LEN);
    author[HVSC_PSID_TEXT_LEN] = '\0';

    name_utf8 = convert_to_utf8(name);
    author_utf8 = convert_to_utf8(author);

    /* append SID to playlist */
    gtk_list_store_append(playlist_model, &iter);
    gtk_list_store_set(playlist_model, &iter,
            0, name_utf8,
            1, author_utf8,
            2, path,
            -1);

    /* clean up */
    g_free(name_utf8);
    g_free(author_utf8);
    hvsc_psid_close(&psid);

    update_title();
    return TRUE;
}


/** \brief  Remove SID file at \a row from playlist
 *
 * \param[in]   row     row in playlist
 *
 * FIXME:   unlikely this will be used.
 */
void vsid_playlist_widget_remove_file(int row)
{
    if (row < 0) {
        debug_gtk3("got invalid row %d.", row);
        return;
    }
    /* TODO: actually remove item :) */
}

