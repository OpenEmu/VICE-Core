/** \file   crtpreviewwidget.h
 * \brief   Widget to show CRT cart image data before attaching - header
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

#ifndef VICE_CRT_PREVIEW_WIDGET_H
#define VICE_CRT_PREVIEW_WIDGET_H

#include <gtk/gtk.h>

#include <stdio.h>
#include "crt.h"
#include "cartridge.h"

GtkWidget *crt_preview_widget_create(void);
void crt_preview_widget_set_open_func(FILE *(*func)(const char *, crt_header_t *));
void crt_preview_widget_set_chip_func(int (*func)(crt_chip_header_t *, FILE *));
void crt_preview_widget_update(const char *filename);

#endif

