/*
 * GNUitar
 * Autowah effect
 * Copyright (C) 2000,2001 Max Rudensky         <fonin@ziet.zhitomir.ua>
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
 *
 * $Log$
 * Revision 1.4  2003/01/29 19:34:00  fonin
 * Win32 port.
 *
 * Revision 1.3  2001/06/02 14:05:58  fonin
 * Added GNU disclaimer.
 *
 * Revision 1.2  2001/03/25 12:10:49  fonin
 * Effect window control ignores delete event.
 *
 * Revision 1.1.1.1  2001/01/11 13:21:05  fonin
 * Version 0.1.0 Release 1 beta
 *
 */

#include "autowah.h"
#include "gui.h"
#include <math.h>
#include <stdlib.h>
#ifndef _WIN32
#    include <unistd.h>
#else
#    include <io.h>
#endif
#include <string.h>
#include <gtk/gtk.h>

double          amplitude,
                amplitude2;

void
                autowah_filter(struct effect *p, struct data_block *db);


void
update_wah_speed(GtkAdjustment * adj, struct autowah_params *params)
{
    params->speed = (float) adj->value;
    params->df = (float) adj->value;
}

void
update_wah_freqlow(GtkAdjustment * adj, struct autowah_params *params)
{
    params->freq_low = (float) adj->value;
    params->f = params->freq_low;

    RC_setup(10, 1.5, params->fd);
    RC_set_freq(params->f, params->fd);
}

void
update_wah_freqhi(GtkAdjustment * adj, struct autowah_params *params)
{
    params->freq_high = (float) adj->value;
    RC_setup(10, 1.5, params->fd);
    RC_set_freq(params->f, params->fd);
}

void
toggle_mix(GtkWidget * bullshit, unsigned short *mixx)
{
    if (*mixx == 1) {
	*mixx = 0;
    } else {
	*mixx = 1;
    }
}

void
toggle_wah(GtkWidget * bullshit, struct effect *p)
{
    if (p->toggle == 1) {
	p->proc_filter = passthru;
	p->toggle = 0;
    } else {
	p->proc_filter = autowah_filter;
	p->toggle = 1;
    }
}


void
autowah_init(struct effect *p)
{
    struct autowah_params *pautowah;

    GtkWidget      *speed;
    GtkWidget      *speed_label;
    GtkObject      *adj_speed;

    GtkWidget      *freq_low;
    GtkWidget      *freqlow_label;
    GtkObject      *adj_freqlow;

    GtkWidget      *freq_high;
    GtkWidget      *freqhi_label;
    GtkObject      *adj_freqhi;

    GtkWidget      *button;
    GtkWidget      *mix;
    GtkWidget      *parmTable;

    pautowah = (struct autowah_params *) p->params;

    RC_setup(10, 1.5, pautowah->fd);
    RC_set_freq(pautowah->f, pautowah->fd);

    /*
     * GUI Init
     */
    p->control = gtk_window_new(GTK_WINDOW_DIALOG);

    rnd_window_pos(GTK_WINDOW(p->control));

    gtk_signal_connect(GTK_OBJECT(p->control), "delete_event",
		       GTK_SIGNAL_FUNC(delete_event), NULL);

    parmTable = gtk_table_new(4, 8, FALSE);

    adj_speed = gtk_adjustment_new(pautowah->speed,
				   0.0, 30.0, 0.1, 1.0, 1.0);
    speed_label = gtk_label_new("Speed");
    gtk_table_attach(GTK_TABLE(parmTable), speed_label, 0, 1, 0, 1,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);

    gtk_signal_connect(GTK_OBJECT(adj_speed), "value_changed",
		       GTK_SIGNAL_FUNC(update_wah_speed), pautowah);

    speed = gtk_vscale_new(GTK_ADJUSTMENT(adj_speed));
    gtk_range_set_update_policy(GTK_RANGE(speed), GTK_UPDATE_DELAYED);
    gtk_table_attach(GTK_TABLE(parmTable), speed, 0, 1, 1, 2,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);


    adj_freqlow = gtk_adjustment_new(pautowah->freq_low,
				     150.0, 300.0, 1.0, 1.0, 1.0);
    freqlow_label = gtk_label_new("Freq. low");
    gtk_table_attach(GTK_TABLE(parmTable), freqlow_label, 3, 4, 0, 1,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);


    gtk_signal_connect(GTK_OBJECT(adj_freqlow), "value_changed",
		       GTK_SIGNAL_FUNC(update_wah_freqlow), pautowah);

    freq_low = gtk_vscale_new(GTK_ADJUSTMENT(adj_freqlow));
    gtk_range_set_update_policy(GTK_RANGE(freq_low), GTK_UPDATE_DELAYED);
    gtk_table_attach(GTK_TABLE(parmTable), freq_low, 3, 4, 1, 2,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);

    adj_freqhi = gtk_adjustment_new(pautowah->freq_high,
				    500.0, 3500.0, 1.0, 1.0, 1.0);
    freqhi_label = gtk_label_new("Freq. hi");
    gtk_table_attach(GTK_TABLE(parmTable), freqhi_label, 5, 6, 0, 1,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);


    gtk_signal_connect(GTK_OBJECT(adj_freqhi), "value_changed",
		       GTK_SIGNAL_FUNC(update_wah_freqhi), pautowah);

    freq_high = gtk_vscale_new(GTK_ADJUSTMENT(adj_freqhi));
    gtk_range_set_update_policy(GTK_RANGE(freq_high), GTK_UPDATE_DELAYED);
    gtk_table_attach(GTK_TABLE(parmTable), freq_high, 5, 6, 1, 2,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);


    button = gtk_check_button_new_with_label("On");
    gtk_signal_connect(GTK_OBJECT(button), "toggled",
		       GTK_SIGNAL_FUNC(toggle_wah), p);

    gtk_table_attach(GTK_TABLE(parmTable), button, 3, 4, 3, 4,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);
    if (p->toggle == 1) {
	p->toggle = 0;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    }

    mix = gtk_check_button_new_with_label("Mix");
    gtk_signal_connect(GTK_OBJECT(mix), "toggled",
		       GTK_SIGNAL_FUNC(toggle_mix), &(pautowah->mixx));

    gtk_table_attach(GTK_TABLE(parmTable), mix, 3, 4, 5, 6,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);
    if (pautowah->mixx == 1) {
	pautowah->mixx = 0;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mix), TRUE);
    }


    gtk_window_set_title(GTK_WINDOW(p->control), (gchar *) ("Wah-wah"));
    gtk_container_add(GTK_CONTAINER(p->control), parmTable);

    gtk_widget_show_all(p->control);
}

void
autowah_filter(struct effect *p, struct data_block *db)
{
    struct autowah_params *ap;
    int             dry[BUFFER_SIZE];
    int             i;

    ap = (struct autowah_params *) p->params;


    if (ap->mixx == 1) {
	memcpy(dry, db->data, db->len * sizeof(int));
    }

    /*
     * Change this constant if autowah works bad 
     */
    if (amplitude2 / amplitude > 1.4) {
	ap->wah_count = 1;
	ap->f = ap->freq_low;
	ap->df = ap->speed;
    }

    /*
     * if (ap->wah_count != 0) { LC_filter(db->data, db->len, HIGHPASS,
     * ap->freq_high, ap->fd); } 
     */
    ap->f += ap->df;
    if (ap->f >= ap->freq_high) {
	ap->df = -ap->df;
	ap->wah_count = 2;
    }

    if (ap->f <= ap->freq_low && ap->wah_count == 2) {
	ap->wah_count = 0;
    }

    if (amplitude2 / amplitude > 1.2 && ap->wah_count == 0) {
	if (ap->df == 0) {
	    ap->df = ap->speed;
	    ap->f = ap->freq_low;
	}
    }

    if (ap->wah_count == 1) {
	ap->df = 0;
	ap->f = ap->freq_low;
	ap->wah_count = 0;
    }

    if (ap->df != 0)
	RC_bandpass(db->data, db->len, ap->fd);

    ap->f += ap->df;
    if (ap->f >= ap->freq_high || ap->f <= ap->freq_low) {
	ap->df = -ap->df;
	ap->wah_count += 2;

	if (ap->df != 0)
	    ap->wah_count++;
    }

    RC_set_freq(ap->f, ap->fd);

    if (ap->mixx == 1) {
	for (i = 0; i < db->len; i++)
	    db->data[i] = (db->data[i] + dry[i]) / 2;
    }
}

void
autowah_done(struct effect *p)
{
    struct autowah_params *ap;
    ap = (struct autowah_params *) p->params;

    free(ap->fd);
    free(p->params);
    gtk_widget_destroy(p->control);
    free(p);
    p = NULL;
}

void
autowah_save(struct effect *p, int fd)
{
    struct autowah_params *ap;

    ap = (struct autowah_params *) p->params;

    write(fd, &ap->freq_low, sizeof(float));
    write(fd, &ap->freq_high, sizeof(float));
    write(fd, &ap->speed, sizeof(float));
    write(fd, &ap->wah_count, sizeof(int));
    write(fd, &ap->f, sizeof(float));
    write(fd, &ap->df, sizeof(float));
    write(fd, &ap->mixx, sizeof(unsigned short));
}

void
autowah_load(struct effect *p, int fd)
{
    struct autowah_params *ap;

    ap = (struct autowah_params *) p->params;

    read(fd, &ap->freq_low, sizeof(float));
    read(fd, &ap->freq_high, sizeof(float));
    read(fd, &ap->speed, sizeof(float));
    read(fd, &ap->wah_count, sizeof(int));
    read(fd, &ap->f, sizeof(float));
    read(fd, &ap->df, sizeof(float));
    read(fd, &ap->mixx, sizeof(unsigned short));
    if (p->toggle == 0) {
	p->proc_filter = passthru;
    } else {
	p->proc_filter = autowah_filter;
    }
}

void
autowah_create(struct effect *p)
{
    struct autowah_params *ap;
    p->params =
	(struct autowah_params *) malloc(sizeof(struct autowah_params));
    ap = (struct autowah_params *) p->params;
    p->proc_init = autowah_init;
    p->proc_filter = passthru;
    p->toggle = 0;
    p->id = AUTOWAH;
    p->proc_done = autowah_done;
    p->proc_save = autowah_save;
    p->proc_load = autowah_load;
    ap->fd = (struct filter_data *) malloc(sizeof(struct filter_data));

    ap->freq_low = 150;
    ap->freq_high = 1000;
    ap->speed = 1;
    ap->wah_count = 0;
    ap->f = ap->freq_low;
    ap->df = ap->speed;
    ap->mixx = 0;
}
