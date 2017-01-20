/*
 * GNUitar
 * Graphics user interface: definitions
 * Copyright (C) 2000,2001,2003 Max Rudensky         <fonin@ziet.zhitomir.ua>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id$
 */

#ifndef GNUITAR_GUI_H
#define GNUITAR_GUI_H

#include <gtk/gtk.h>
#include "effect.h"
#include "error.h"
#include "glib12-compat.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup gnuitar-gui GNUitar GUI
 * Contains information on the GUI API
 */

/** The graphical interface for the GNUitar project.
 * @ingroup gnuitar-gui
 */

typedef struct gnuitar_gui {
    GtkAccelGroup *accel_group;

    /** The top level window */
    GtkWidget *mainWnd;
    /** Tooltips for all windows */
    GtkTooltips *tooltips;

    GtkWidget *menuBar;
    GtkWidget *tbl;
    GtkItemFactory *item_factory;
} gnuitar_gui_t;

/** The graphical version of the pump.
 * Contains a list of the effects in the pump.
 */

typedef struct gnuitar_processor {
    GtkWidget *widget;
} gnuitar_processor_t;

gnuitar_error_t gnuitar_processor_init(gnuitar_processor_t *processor);

void gnuitar_processor_init_lazy(gnuitar_processor_t *processor);

void gnuitar_processor_done(gnuitar_processor_t *processor);

typedef struct gnuitar_volume_control {
    float master_volume;
    float input_volume;
} gnuitar_volume_control_t;

int gnuitar_gui_init(gnuitar_gui_t * gui);

void gnuitar_gui_done(gnuitar_gui_t * gui);

int gnuitar_gui_set_size(gnuitar_gui_t * gui, unsigned int width, unsigned int height);

#ifdef __cplusplus
#define __GTKATTACHOPTIONS GtkAttachOptions
#else
#define __GTKATTACHOPTIONS
#endif

#define VERSION "0.3.2"
#define DISCLAIMER  "This program is free software; you can redistribute it and/or modify\n" \
		    "it under the terms of the GNU General Public License as published by\n" \
		    "the Free Software Foundation; either version 2, or (at your option)\n" \
		    "any later version.\n\n" \
		    "This program makes usage of Glib and GTK+ libraries that are distributed\n" \
		    "under Library GNU Public License (LGPL).\n\n"
#define COPYRIGHT   "GNUitar "VERSION"\n" \
		    "Copyright (C) 2000,2001,2003,2004 Max Rudensky <fonin@omnistaronline.com>\n" \
		    "http://www.omnistaronline.com/~fonin/\n"

#define LICENSE     "GPL license"

void		init_gui(void);
gint            delete_event(GtkWidget *, GdkEvent *, gpointer);
void            toggle_effect(GtkWidget *, effect_t *);
void            bank_append_entry(gchar *entry);
void            set_vumeter_in_value(float power);
void            set_vumeter_out_value(float power);
void            gnuitar_printf(const char *fmt, ...);

extern GtkWidget *processor;
extern float master_volume;
extern float input_volume;
extern GtkObject* adj_input;
extern GtkObject* adj_master;

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_GUI_H */

