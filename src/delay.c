/*
 * GNUitar
 * Delay effect
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
 *
 * $Log$
 * Revision 1.17  2005/09/01 14:18:54  alankila
 * - drop fractions in delay count
 *
 * Revision 1.16  2005/09/01 14:09:56  alankila
 * - multichannel work: delay independent of nchannels; uses backbuf instead
 *   of doing it all on its own. Also fixes bugs with delay load/save.
 *
 * Revision 1.15  2005/08/18 23:54:32  alankila
 * - use GTK_WINDOW_DIALOG instead of TOPLEVEL, however #define them the same
 *   for GTK2.
 *
 * Revision 1.14  2005/08/13 12:06:08  alankila
 * - removed bunch of #ifdef HAVE_GTK/HAVE_GTK2 regarding window type
 *
 * Revision 1.13  2005/08/11 17:57:21  alankila
 * - add some missing headers & fix all compiler warnings on gcc 4.0.1+ -Wall
 *
 * Revision 1.12  2004/08/10 15:07:31  fonin
 * Support processing in float/int - type DSP_SAMPLE
 *
 * Revision 1.11  2004/07/07 19:18:42  fonin
 * GTK2 port
 *
 * Revision 1.10  2003/03/14 19:20:31  fonin
 * Unreferenced variables fixed.
 *
 * Revision 1.9  2003/03/12 20:53:54  fonin
 * - meaningful sliders measure units;
 * - code cleanup.
 *
 * Revision 1.8  2003/02/03 11:39:25  fonin
 * Copyright year changed.
 *
 * Revision 1.7  2003/02/01 19:15:12  fonin
 * Use sizeof(variable) instead sizeof(type) in load/save procedures,
 * when reading/writing from file.
 *
 * Revision 1.6  2003/01/31 19:45:45  fonin
 * Better effect presets.
 *
 * Revision 1.5  2003/01/30 21:35:29  fonin
 * Got rid of rnd_window_pos().
 *
 * Revision 1.4  2003/01/29 19:34:00  fonin
 * Win32 port.
 *
 * Revision 1.3  2001/06/02 14:05:59  fonin
 * Added GNU disclaimer.
 *
 * Revision 1.2  2001/03/25 12:10:49  fonin
 * Effect window control ignores delete event.
 *
 * Revision 1.1.1.1  2001/01/11 13:21:26  fonin
 * Version 0.1.0 Release 1 beta
 *
 */

#include "backbuf.h"
#include "delay.h"
#include "gui.h"
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
#    include <unistd.h>
#else
#    include <io.h>
#endif

void            delay_filter(struct effect *p, struct data_block *db);


void
update_delay_decay(GtkAdjustment * adj, struct delay_params *params)
{
    int             i;
    params->delay_decay = (int) adj->value * 10;
    for (i = 0; i < MAX_CHANNELS; i += 1)
        params->history[i]->clear(params->history[i]);
}

void
update_delay_time(GtkAdjustment * adj, struct delay_params *params)
{
    int             i;
    params->delay_time = (int) adj->value * sample_rate / 1000;
    for (i = 0; i < MAX_CHANNELS; i += 1)
        params->history[i]->clear(params->history[i]);
}

void
update_delay_repeat(GtkAdjustment * adj, struct delay_params *params)
{
    params->delay_count = (int) adj->value;
}

void
toggle_delay(void *bullshit, struct effect *p)
{
    if (p->toggle == 1) {
	p->proc_filter = passthru;
	p->toggle = 0;
    } else {
	p->proc_filter = delay_filter;
	p->toggle = 1;
    }
}


void
delay_init(struct effect *p)
{
    struct delay_params *pdelay;

    GtkWidget      *decay;
    GtkWidget      *decay_label;
    GtkObject      *adj_decay;

    GtkWidget      *time;
    GtkWidget      *time_label;
    GtkObject      *adj_time;

    GtkWidget      *repeat;
    GtkWidget      *repeat_label;
    GtkObject      *adj_repeat;

    GtkWidget      *button;

    GtkWidget      *parmTable;


    pdelay = (struct delay_params *) p->params;


    /*
     * GUI Init
     */
    p->control = gtk_window_new(GTK_WINDOW_DIALOG);

    gtk_signal_connect(GTK_OBJECT(p->control), "delete_event",
		       GTK_SIGNAL_FUNC(delete_event), NULL);

    parmTable = gtk_table_new(2, 8, FALSE);

    adj_decay = gtk_adjustment_new(pdelay->delay_decay / 10,
				   10.0, 100.0, 1.0, 1.0, 0.0);
    decay_label = gtk_label_new("Decay\n%");
    gtk_table_attach(GTK_TABLE(parmTable), decay_label, 0, 1, 0, 1,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);


    gtk_signal_connect(GTK_OBJECT(adj_decay), "value_changed",
		       GTK_SIGNAL_FUNC(update_delay_decay), pdelay);

    decay = gtk_vscale_new(GTK_ADJUSTMENT(adj_decay));
#ifdef HAVE_GTK2
    gtk_widget_set_size_request(GTK_WIDGET(decay),0,100);
#endif

    gtk_table_attach(GTK_TABLE(parmTable), decay, 0, 1, 1, 2,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);


    adj_time =
	gtk_adjustment_new(pdelay->delay_time * 1000 / sample_rate, 1.0,
			   MAX_STEP * 1000 / (sample_rate * nchannels),
			   1.0, 1.0, 1.0);
    time_label = gtk_label_new("Time\nms");
    gtk_table_attach(GTK_TABLE(parmTable), time_label, 3, 4, 0, 1,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);


    gtk_signal_connect(GTK_OBJECT(adj_time), "value_changed",
		       GTK_SIGNAL_FUNC(update_delay_time), pdelay);
    time = gtk_vscale_new(GTK_ADJUSTMENT(adj_time));

    gtk_table_attach(GTK_TABLE(parmTable), time, 3, 4, 1, 2,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);


    adj_repeat = gtk_adjustment_new(pdelay->delay_count,
				    1.0, MAX_COUNT, 1.0, 1.0, 0.0);
    repeat_label = gtk_label_new("Repeat\ntimes");
    gtk_table_attach(GTK_TABLE(parmTable), repeat_label, 5, 6, 0, 1,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);


    gtk_signal_connect(GTK_OBJECT(adj_repeat), "value_changed",
		       GTK_SIGNAL_FUNC(update_delay_repeat), pdelay);

    repeat = gtk_vscale_new(GTK_ADJUSTMENT(adj_repeat));
    gtk_scale_set_digits(GTK_SCALE(repeat), 0);

    gtk_table_attach(GTK_TABLE(parmTable), repeat, 5, 6, 1, 2,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);

    button = gtk_check_button_new_with_label("On");
    gtk_signal_connect(GTK_OBJECT(button), "toggled",
		       GTK_SIGNAL_FUNC(toggle_delay), p);

    gtk_table_attach(GTK_TABLE(parmTable), button, 3, 4, 3, 4,
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK),
		     __GTKATTACHOPTIONS(GTK_FILL | GTK_EXPAND |
					GTK_SHRINK), 0, 0);
    if (p->toggle == 1) {
	p->toggle = 0;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    }

    gtk_window_set_title(GTK_WINDOW(p->control), (gchar *) ("Delay"));
    gtk_container_add(GTK_CONTAINER(p->control), parmTable);

    gtk_widget_show_all(p->control);

}

void
delay_filter(struct effect *p, struct data_block *db)
{
    struct delay_params *dp;
    int             i,
                    count,
                    current_delay = 0;
    double          current_decay;
    DSP_SAMPLE     *s, newval;
    int             curr_channel = 0;

    dp = (struct delay_params *) p->params;

    s = db->data;
    count = db->len;

    /* this is a simple mono version */
    while (count) {
        /* mix stuff from history to current data */
        newval = 0;
        current_delay = 0;
        current_decay = 1.0;
	
        for (i = 0; i < dp->delay_count; i++) {
            current_delay += dp->delay_time;
            current_decay *= dp->delay_decay / 1000.0;

            newval += dp->history[curr_channel]->get(dp->history[curr_channel], current_delay) * current_decay;
	}
        
        /* write to history, add decay to current sample, avoid overflow */
        dp->history[curr_channel]->add(dp->history[curr_channel], *s);
        *s = *s/2 + newval/2;

        curr_channel = (curr_channel + 1) % db->channels;
        
	s++;
	count--;
    }
}

void
delay_done(struct effect *p)
{
    struct delay_params *dp;
    int i;

    dp = (struct delay_params *) p->params;
    for (i = 0; i < MAX_CHANNELS; i += 1)
        del_Backbuf(dp->history[i]);

    free(p->params);
    gtk_widget_destroy(p->control);
    free(p);
}

void
delay_save(struct effect *p, int fd)
{
    struct delay_params *dp;

    dp = (struct delay_params *) p->params;

    write(fd, &dp->delay_decay, sizeof(dp->delay_decay));
    write(fd, &dp->delay_time, sizeof(dp->delay_time));
    write(fd, &dp->delay_count, sizeof(dp->delay_count));
}

void
delay_load(struct effect *p, int fd)
{
    struct delay_params *dp;

    dp = (struct delay_params *) p->params;

    read(fd, &dp->delay_decay, sizeof(dp->delay_decay));
    read(fd, &dp->delay_time, sizeof(dp->delay_time));
    read(fd, &dp->delay_count, sizeof(dp->delay_count));
    if (p->toggle == 0) {
	p->proc_filter = passthru;
    } else {
	p->proc_filter = delay_filter;
    }
}

void
delay_create(struct effect *p)
{
    struct delay_params *pdelay;
    int                 i;

    p->params = calloc(1, sizeof(struct delay_params));
    pdelay = p->params;
    p->proc_init = delay_init;
    p->proc_filter = passthru;
    p->toggle = 0;
    p->id = DELAY;
    p->proc_done = delay_done;
    p->proc_save = delay_save;
    p->proc_load = delay_load;

    /*
     * 
     * Brian May echo stuff delay_start=delay_step=35000 delay_count=2
     * 
     * Diablo town music delay_start=delay_step=25000 delay_count=3
     * 
     */

    pdelay->delay_decay = 550;
    pdelay->delay_time = 11300;
    pdelay->delay_count = 8;
    for (i = 0; i < MAX_CHANNELS; i += 1)
        pdelay->history[i] = new_Backbuf(MAX_SIZE);
}
