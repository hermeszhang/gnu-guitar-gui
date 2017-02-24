/*
 * GNUitar
 * Echo effect
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
 * Revision 1.26  2006/09/04 14:25:10  alankila
 * - hide multichannel GUI if input channels != 1
 *
 * Revision 1.25  2006/08/02 19:07:57  alankila
 * - add missing static declarations
 *
 * Revision 1.24  2005/09/16 20:40:51  alankila
 * - reduce total voice count to spare memory and constrain effect
 * - complete circular mixing and increase cross-mixing attenuation a lot
 *
 * Revision 1.23  2005/09/06 23:21:40  alankila
 * - tabularize pow, set some constants, do crossmixing, better defaults
 *
 * Revision 1.22  2005/09/06 14:54:32  alankila
 * - set button states at loadup
 * - make echo multichannel aware. Echo currently can do almost everything
 *   reverb can do, so we could remove reverb.
 *
 * Revision 1.21  2005/09/04 23:05:17  alankila
 * - delete the repeated toggle_foo functions, use one global from gui.c
 *
 * Revision 1.20  2005/09/04 14:40:17  alankila
 * - get rid of effect->id and associated enumeration
 *
 * Revision 1.19  2005/09/04 12:12:35  alankila
 * - make create() and done() symmetric in memory allocation/free
 *
 * Revision 1.18  2005/09/04 11:16:59  alankila
 * - destroy passthru function, move the toggle logic higher up
 *
 * Revision 1.17  2005/09/04 01:51:09  alankila
 * - GKeyFile-based preset load/save
 * - still need locale-immune %lf for printf and sscanf
 *
 * Revision 1.16  2005/09/03 16:36:51  alankila
 * - reworked effect to use backbuff
 * - removed some output scaling that made this effect almost inaudible
 * - reinstated a decay based on delay % and echo delay
 * - echo_size adjustment works again
 *
 * Revision 1.15  2005/09/02 11:58:49  alankila
 * - remove #ifdef HAVE_GTK2 entirely from all effect code
 *
 * Revision 1.14  2005/09/01 23:52:15  alankila
 * - make window delete event do something useful
 *
 * Revision 1.13  2005/08/18 23:54:32  alankila
 * - use GTK_WINDOW_DIALOG instead of TOPLEVEL, however #define them the same
 *   for GTK2.
 *
 * Revision 1.12  2005/08/13 12:06:08  alankila
 * - removed bunch of #ifdef HAVE_GTK/HAVE_GTK2 regarding window type
 *
 * Revision 1.11  2004/08/10 15:07:31  fonin
 * Support processing in float/int - type gnuitar_sample_t
 *
 * Revision 1.10  2004/07/07 19:18:42  fonin
 * GTK2 port
 *
 * Revision 1.9  2003/03/14 19:20:31  fonin
 * Unreferenced variables fixed.
 *
 * Revision 1.8  2003/03/12 20:53:54  fonin
 * - meaningful sliders measure units;
 * - code cleanup.
 *
 * Revision 1.7  2003/02/03 11:39:25  fonin
 * Copyright year changed.
 *
 * Revision 1.6  2003/02/01 19:15:12  fonin
 * Use sizeof(variable) instead sizeof(type) in load/save procedures,
 * when reading/writing from file.
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
 * Revision 1.1.1.1  2001/01/11 13:21:41  fonin
 * Version 0.1.0 Release 1 beta
 *
 */

#include "echo.h"

#include <errno.h>

int
gnuitar_echo_init(struct GnuitarEffect *effect)
{
    struct GnuitarEcho *echo;

    echo = malloc(sizeof(*echo));
    if (echo == NULL)
        return ENOMEM;
    gnuitar_packet_init(&echo->history);
    echo->echo_delay = 250;
    echo->echo_count = 2;

    effect->params = echo;
    effect->done = gnuitar_echo_done;
    effect->get_map = gnuitar_echo_get_map;
    effect->set_map = gnuitar_echo_set_map;
    effect->process = gnuitar_echo_process;

    return 0;
}

void
gnuitar_echo_done(struct GnuitarEffect *effect)
{
    struct GnuitarEcho *echo;

    echo = effect->params;
    if (echo != NULL) {
        gnuitar_packet_done(&echo->history);
	free(echo);
	effect->params = NULL;
    }
}

int
gnuitar_echo_process(struct GnuitarEffect *effect, struct GnuitarPacket *packet)
{
    (void) effect;
    (void) packet;
    return 0;
}

int
gnuitar_echo_get_map(const struct GnuitarEffect *effect, struct GnuitarMap *map)
{
    int err;
    const struct GnuitarEcho *echo;

    echo = effect->params;
    if (echo == NULL)
        return EFAULT;

    err = gnuitar_map_define(map, "Delay", GNUITAR_MAP_TYPE_SIZE);
    if (err != 0)
        return err;

    err = gnuitar_map_set(map, "Delay", &echo->echo_delay);
    if (err != 0)
        return err;

    err = gnuitar_map_define(map, "Echo Count", GNUITAR_MAP_TYPE_SIZE);
    if (err != 0)
        return err;

    err = gnuitar_map_set(map, "Echo Count", &echo->echo_count);
    if (err != 0)
        return err;

    return 0;
}

int
gnuitar_echo_set_map(struct GnuitarEffect *effect, const struct GnuitarMap *map)
{
    (void) effect;
    (void) map;
    return 0;
}

