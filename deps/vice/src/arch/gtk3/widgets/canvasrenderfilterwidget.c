/** \file   canvasrenderfilterwidget.c
 * \brief   Widget to select the Cario/OpenGL render filter
 *
 *
 * \note    Normally I'd use 'gtkrenderfilterwidget.c' as the filename and
 *          gtk_render_filter_widget_' as the functions prefix, but that
 *          would invade the Gtk3 namespace, so 'canvas' it is for now.
 *
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

/*
 * $VICERES GTKFilter   -vsid
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
#include "vice_gtk3.h"

#include "canvasrenderfilterwidget.h"


/** \brief  List of Cairo/OpenGL render filters
 */
static const vice_gtk3_radiogroup_entry_t filters[] = {
    { "Nearest neighbor",   0 },
    { "Bilinear",           1 },
    { NULL,                 -1 }
};


/** \brief  Reference to the resource radio group widget
 */
static GtkWidget *resource_widget;


/** \brief  Create widget to select the Gtk render filter method
 *
 *
 * \return  GtkGrid
 */
GtkWidget *canvas_render_filter_widget_create(void)
{
    GtkWidget *grid;
    GtkWidget *header;
    GtkWidget *restart;

    grid = vice_gtk3_grid_new_spaced(VICE_GTK3_DEFAULT, VICE_GTK3_DEFAULT);

    header = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(header), "<b>Gtk render filter</b>");
    gtk_widget_set_halign(header, GTK_ALIGN_START);

    resource_widget = vice_gtk3_resource_radiogroup_new(
            "GTKFilter",
            filters,
            GTK_ORIENTATION_VERTICAL);
    g_object_set(resource_widget, "margin-left", 16, NULL);

    restart = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(restart), "<i>(requires restart)</i>");

    gtk_grid_attach(GTK_GRID(grid), header, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), resource_widget, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), restart, 0, 2, 1, 1);

    return grid;
}

