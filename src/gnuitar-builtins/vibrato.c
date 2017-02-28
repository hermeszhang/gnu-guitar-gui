/*
 * GNUitar
 * Tremolo effect
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
 * Revision 1.40  2006/08/06 20:14:55  alankila
 * - split pump.h into several domain-specific headers to reduce file
 *   interdependencies (everyone included pump.h). New files are:
 *   - effect.h for effect definitions
 *   - audio-driver.h for work relating to audio drivers
 *   - audio-midi.h for MIDI interaction.
 *
 * Revision 1.39  2006/08/02 19:11:18  alankila
 * - add missing static declarations
 *
 * Revision 1.38  2006/07/29 12:04:36  alankila
 * - effect lickup:
 *   * stereo phaser has less severe phase cancellation effects
 *   * rotary speaker can go even faster
 * - improve performance of sin_lookup, add cos_lookup
 *
 * Revision 1.37  2006/07/28 19:41:51  alankila
 * - save amplitude too
 *
 * Revision 1.36  2006/07/27 19:24:42  alankila
 * - aligned memory needs aligned free operation.
 *
 * Revision 1.35  2006/07/08 16:28:16  alankila
 * - extend hilbert transform with channel information for effects that could
 *   be used on channels separately. We've already allocated space in biquads
 *   for them.
 *
 * Revision 1.34  2006/06/16 14:43:42  alankila
 * - add static base pitch shift
 *
 * Revision 1.33  2006/05/29 23:46:02  alankila
 * - move _GNU_SOURCE into Makefile
 * - align memory for x86-32; x86-64 already aligned memory for us in glibc
 *   so we didn't crash. This is done through new gnuitar_memalign().
 * - cater to further restrictions in SSE instructions for x86 arhictecture:
 *   it appears that mulps memory must be aligned to 16 too. This crashed
 *   all biquad-using functions and tubeamp. :-(
 *
 * Revision 1.32  2006/05/20 16:59:19  alankila
 * - more exciting tremolo bar effect with pitch shifting.
 * - if you want to have old-style amplitude modulation, try using
 *   rotary speaker instead.
 *
 * Revision 1.31  2005/09/04 23:05:17  alankila
 * - delete the repeated toggle_foo functions, use one global from gui.c
 *
 * Revision 1.30  2005/09/04 19:45:12  alankila
 * - replace "Speed 1/ms" with "Period ms" which is easier to understand
 *
 * Revision 1.29  2005/09/04 14:40:17  alankila
 * - get rid of effect->id and associated enumeration
 *
 * Revision 1.28  2005/09/04 12:12:36  alankila
 * - make create() and done() symmetric in memory allocation/free
 *
 * Revision 1.27  2005/09/04 11:16:59  alankila
 * - destroy passthru function, move the toggle logic higher up
 *
 * Revision 1.26  2005/09/04 01:51:09  alankila
 * - GKeyFile-based preset load/save
 * - still need locale-immune %lf for printf and sscanf
 *
 * Revision 1.25  2005/09/02 11:58:49  alankila
 * - remove #ifdef HAVE_GTK2 entirely from all effect code
 *
 * Revision 1.24  2005/09/01 23:52:15  alankila
 * - make window delete event do something useful
 *
 * Revision 1.23  2005/09/01 22:41:08  alankila
 * - simplifications and fixes
 *
 * Revision 1.22  2005/09/01 22:09:02  alankila
 * decrypt parameters
 *
 * Revision 1.21  2005/09/01 17:31:40  alankila
 * - various small fixes for multichannel / gui
 *
 * Revision 1.20  2005/08/18 23:54:32  alankila
 * - use GTK_WINDOW_DIALOG instead of TOPLEVEL, however #define them the same
 *   for GTK2.
 *
 * Revision 1.19  2005/08/13 12:06:08  alankila
 * - removed bunch of #ifdef HAVE_GTK/HAVE_GTK2 regarding window type
 *
 * Revision 1.18  2005/08/13 11:38:15  alankila
 * - some final warning fixups and removal of MYGTK_TEXT hack
 *
 * Revision 1.17  2005/08/12 17:56:16  alankila
 * use one global sin lookup table
 *
 * Revision 1.16  2005/08/11 17:57:22  alankila
 * - add some missing headers & fix all compiler warnings on gcc 4.0.1+ -Wall
 *
 * Revision 1.15  2005/08/08 18:34:45  alankila
 * - rename effects:
 *   * vibrato -> tremolo
 *   * tremolo -> vibrato
 *   * distort2 -> overdrive
 *
 * Revision 1.14  2005/08/08 17:26:19  alankila
 * - fix vibrato to not do any snapping sounds any more.
 * - correct fubared _load() and _save() functions
 * - change the "vibrato phase buffer size" that's really the size of the
 *   sin lookup table to something semi-reasonable like 16384
 *   (at later point I'll merge all the sin lookup tables together)
 *
 * Revision 1.13  2005/04/29 11:24:42  fonin
 * Better presets
 *
 * Revision 1.12  2004/08/10 15:21:16  fonin
 * Support processing in float/int - type gnuitar_sample_t
 *
 * Revision 1.10  2004/07/07 19:18:42  fonin
 * GTK2 port
 *
 * Revision 1.9  2003/03/09 20:53:16  fonin
 * Meaningful params "speed" and "amplitude". Structures were redesigned
 * for new feature of on-the-fly change of sampling params.
 *
 * Revision 1.8  2003/02/03 11:39:25  fonin
 * Copyright year changed.
 *
 * Revision 1.6  2003/02/01 19:13:44  fonin
 * Changed the units of slider bars from computer ones to ms and %
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
 * Revision 1.2  2001/03/25 12:10:50  fonin
 * Effect window control ignores delete event.
 *
 * Revision 1.1.1.1  2001/01/11 13:22:31  fonin
 * Version 0.1.0 Release 1 beta
 *
 */

#include "vibrato.h"

#include <errno.h>
#include <math.h>
#include <stdlib.h>

static int
vibrato_process(struct GnuitarEffect *effect, struct GnuitarPacket *packet)
{
    /* the vibrato effect is based on hilbert transform that allows us to reconstruct
     * shifted frequency spectra. However, the shifting effect does not preserve
     * frequency relationships because it implements f' = f - f_mod. On the other
     * hand, neither does the genuine guitar tremolo bar, so there is some emulation
     * accuracy.
     *
     * The f_mod is the speed of modulation, stored in variables speed. The speed
     * itself is modulated by the period and depth parameters. */
    
    size_t i;
    double sample;
    size_t count;
    size_t curr_channel;
    struct GnuitarVibrato *vibrato;

    vibrato = effect->params;

    i = 0;
    count = gnuitar_packet_get_length(packet);
    curr_channel = 0;

    for (i = 0; i < count; i++){

        double x0, x1;
        float sinval, cosval;
        sample = gnuitar_packet_get_df(packet, i);

        gnuitar_hilbert_transform(sample, &x0, &x1, &vibrato->hilbert, curr_channel);
        
        sinval = sin(vibrato->phase);
        cosval = cos(vibrato->phase);

        if (vibrato->vibrato_base > 0)
            sample = cosval * x0 + sinval * x1;
        else
            sample = cosval * x0 - sinval * x1;
        
        curr_channel = (curr_channel + 1) % packet->channels;
        if (curr_channel == 0) {
            vibrato->vibrato_phase += 1000.0 / vibrato->vibrato_speed / packet->rate;
            if (vibrato->vibrato_phase >= 1.0)
                vibrato->vibrato_phase -= 1.0;
            vibrato->phase += (fabs(vibrato->vibrato_base) + (vibrato->vibrato_amplitude + sin(vibrato->vibrato_phase) *
	    vibrato->vibrato_amplitude) / 2) / packet->rate;
            if (vibrato->phase >= 1.0)
                vibrato->phase -= 1.0;
        }

	gnuitar_packet_set_df(packet, i, sample);
    }

    return 0;
}

static void
vibrato_done(struct GnuitarEffect *effect)
{
    gnuitar_free(effect->params);
}

int
gnuitar_vibrato_init(struct GnuitarEffect *effect)
{
    struct GnuitarVibrato *vibrato;

    vibrato = gnuitar_memalign(1, sizeof(struct GnuitarVibrato));
    if (vibrato == NULL)
        return ENOMEM;
    gnuitar_hilbert_init(&vibrato->hilbert);
    vibrato->vibrato_base = 0;
    vibrato->vibrato_amplitude = 10;
    vibrato->vibrato_speed = 200;
    vibrato->vibrato_phase = 0;

    effect->params = vibrato;
    effect->done = vibrato_done;
    effect->process = vibrato_process;
    effect->get_map = NULL;
    effect->set_map = NULL;

    return 0;
}

