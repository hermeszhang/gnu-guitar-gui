/*
 * GNUitar
 * Pump module - processeing sound
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
 * Revision 1.90  2006/10/27 21:54:46  alankila
 * - new source file: audio-midi.c. Do some data abstraction, prepare to
 *   support multiple midi continuous controls.
 *
 * Revision 1.89  2006/08/15 15:46:00  alankila
 * - move typecast to not overflow gnuitar_sample_t if it is integer. Shit.
 *
 * Revision 1.88  2006/08/10 16:18:36  alankila
 * - improve const correctness and make gnuitar compile cleanly under
 *   increasingly pedantic warning models.
 *
 * Revision 1.87  2006/08/08 21:05:31  alankila
 * - optimize gnuitar: this breaks dsound, I'll fix it later
 *
 * Revision 1.86  2006/08/07 22:05:53  alankila
 * - store 0 as effects number if effect list is empty
 *
 * Revision 1.85  2006/08/07 20:01:50  alankila
 * - move all modifications of effect list structures into effect.c.
 *
 * Revision 1.84  2006/08/07 13:20:42  alankila
 * - group all effects through effect.h rather than enumerating them in
 *   pump.c.
 *
 * Revision 1.83  2006/08/07 12:55:30  alankila
 * - construct audio-driver.c to hold globals and provide some utility
 *   functions to its users. This slashes interdependencies somewhat.
 *
 * Revision 1.82  2006/08/06 20:14:54  alankila
 * - split pump.h into several domain-specific headers to reduce file
 *   interdependencies (everyone included pump.h). New files are:
 *   - effect.h for effect definitions
 *   - audio-driver.h for work relating to audio drivers
 *   - audio-midi.h for MIDI interaction.
 *
 * Revision 1.81  2006/08/02 19:21:23  alankila
 * - clarify that amp is a digital amplification, not analog simulation
 *
 * Revision 1.80  2006/07/30 11:14:06  alankila
 * - rename n variable to effects_n -- n is too short for a global
 *
 * Revision 1.79  2006/07/30 10:56:23  alankila
 * - W32 mkdir() doesn't have permissions
 *
 * Revision 1.78  2006/07/29 15:16:28  alankila
 * - remember presets between gnuitar invocations
 * - remember effect settings between gnuitar invocations
 *
 * Revision 1.77  2006/07/29 12:04:36  alankila
 * - effect lickup:
 *   * stereo phaser has less severe phase cancellation effects
 *   * rotary speaker can go even faster
 * - improve performance of sin_lookup, add cos_lookup
 *
 * Revision 1.76  2006/07/28 19:42:15  alankila
 * - fix compat library to not crash on invalid free
 *
 * Revision 1.75  2006/07/28 19:23:41  alankila
 * - reduce lock contention when switching between presets
 *
 * Revision 1.74  2006/07/28 19:08:40  alankila
 * - add midi event listeners into JACK and ALSA
 * - make gui listen to midi events and switch bank
 * - fix a bug involving "add pressed"
 * - change documentation of "Switch preset" to "Cycle presets" which is what
 *   it does now.
 *
 * Revision 1.73  2006/07/27 19:49:39  alankila
 * - blah, forgot one }
 *
 * Revision 1.72  2006/07/27 19:48:46  alankila
 * - remove debug print
 *
 * Revision 1.71  2006/07/27 19:15:35  alankila
 * - split windows driver architecture now compiles and runs.
 *
 * Revision 1.70  2006/07/27 18:31:15  alankila
 * - split dsound and winmm into separate drivers.
 *
 * Revision 1.69  2006/07/16 20:43:32  alankila
 * - use non-white triangular noise for slightly better dithering.
 *
 * Revision 1.68  2006/07/15 23:21:23  alankila
 * - show input/output vu meters separately
 *
 * Revision 1.67  2006/07/15 23:02:45  alankila
 * - remove the bits control -- just use the best available on every driver.
 *
 * Revision 1.66  2006/07/15 21:15:47  alankila
 * - implement triangular dithering on the sound drivers. Triangular dithering
 *   places more noise at the nyquist frequency so the noise floor is made
 *   smaller elsewhere.
 *
 * Revision 1.65  2006/07/15 20:42:56  alankila
 * - remove global dithering code for now. JACK does its own dithering if
 *   configured, so the decision to dither in pump is no longer appropriate.
 *
 * Revision 1.64  2006/06/20 20:41:07  anarsoul
 * Added some kind of status window. Now we can use gnuitar_printf(char *fmt, ...) that redirects debug information in this window.
 *
 * Revision 1.63  2006/06/16 12:32:19  alankila
 * - the reasonably trivial vibrato effect is now reinstated as tremolo
 *   in a file with proper name. Old vibrato can be achieved through 1-voice
 *   100% wet chorus.
 *
 * Revision 1.62  2006/05/24 20:06:15  alankila
 * - save some CPU by avoiding the very expensive pow() call on every input.
 *   Use floats because their accuracy suffices for the work.
 *
 * Revision 1.61  2006/05/20 17:32:02  alankila
 * - rename Tremolo effect as Tremolo Bar.
 * - I think about changing Vibrato into what Tremolo used to be. This way the
 *   source file names again make sense.
 *
 * Revision 1.60  2006/05/20 09:56:58  alankila
 * - move audio_driver_str and audio_driver_enabled into driver structure
 * - Win32 drivers are ugly, with the need to differentiate between
 *   DirectX and MMSystem operation through dsound variable. The driver
 *   should probably be split with dsound-specific parts in its own driver.
 *
 * Revision 1.59  2006/05/19 15:12:54  alankila
 * I keep on getting rattles with ALSA playback, seems like ALSA doesn't
 * know when to swap buffers or allows write to go on too easily. I
 * performed a major overhaul/cleanup in trying to kill this bug for good.
 *
 * - fix confusion about what "buffer_size" really means and how many bytes
 *   it takes.
 *   - buffer size is ALWAYS the fragment size in all audio drivers
 *     (ALSA, OSS, Win32 driver)
 *   - it follows that memory needed is buffer_size times maximum
 *     frame size. (32-bit samples, 4 channels, max buffer size.)
 *   - latency calculation updated, but it may be incorrect yet
 * - add write buffer for faster ALSA read-write cycle. (Hopefully this
 *   reduces buffer underruns and rattles and all sort of ugliness we
 *   have with ALSA)
 * - redesign the ALSA configuration code. Now we let ALSA choose the
 *   parameters during the adjustment phase, then we try same for playback.
 * - some bugs squashed in relation to this, variables renamed, etc.
 * - if opening audio driver fails, do not kill the user's audio_driver
 *   choice. (It makes configuring bits, channels, etc. difficult.)
 *   We try to track whether processing is on/off through new variable,
 *   audio_driver_enabled.
 *
 * Note: all the GUI code related to audio is in need of a major overhaul.
 * Several variables should be renamed, variable visibility better controlled.
 *
 * Revision 1.58  2006/05/19 10:17:04  alankila
 * - avoid floating point in input path, especially denormal numbers
 *
 * Revision 1.57  2006/05/13 10:57:57  alankila
 * - We can get better dynamic range if we don't clip everywhere, but only at
 *   the master gain adjustment, just before output is produced.
 *
 * Revision 1.56  2006/05/13 10:16:53  alankila
 * - produce 2 output channels by default rather than 1 to allow new effects
 *   such as rotary speaker to work
 *
 * Revision 1.55  2006/05/13 10:09:52  alankila
 * - allow sin_lookup(1.0) to work
 *
 * Revision 1.54  2006/05/13 07:44:50  alankila
 * - remove pump's noise reduction -- we'll have the real effect soon enough
 * - add rotary speaker simulator based on hilbert transform
 *
 * Revision 1.53  2006/05/07 13:22:12  alankila
 * - new bare bones distortion effect: tubeamp
 *
 * Revision 1.52  2006/05/01 10:23:54  anarsoul
 * Alsa device is selectable and input volume is adjustable now. Added new filter - amp.
 *
 * Revision 1.51  2005/11/05 12:18:38  alankila
 * - pepper the code with static declarations for all private funcs and vars
 *
 * Revision 1.50  2005/10/02 08:23:50  fonin
 * Master volume is stored in the preset file
 *
 * Revision 1.49  2005/09/28 19:52:47  fonin
 * Load and save windows settings correctly
 *
 * Revision 1.48  2005/09/12 09:42:25  fonin
 * - MSVC compatibility fixes
 *
 * Revision 1.47  2005/09/11 12:54:14  alankila
 * - add a tentative pitch shift effect
 *
 * Revision 1.46  2005/09/09 20:22:17  alankila
 * - phasor reimplemented according to a popular algorithm that simulates
 *   high-impedance isolated varying capacitors
 *
 * Revision 1.45  2005/09/05 17:42:07  alankila
 * - fix some small memory leaks
 *
 * Revision 1.44  2005/09/04 20:45:01  alankila
 * - store audio driver into config
 * - make it possible to start gnuitar with invalid audio driver and enter
 *   options and correct the driver. Some rough edges still remain with
 *   the start/stop button, mutexes, etc.
 *
 * Revision 1.43  2005/09/04 16:06:59  alankila
 * - first multichannel effect: delay
 * - need to use surround40 driver in alsa
 * - introduce new buffer data_swap so that effects need not reserve buffers
 * - correct off-by-one error in multichannel adapting
 *
 * Revision 1.42  2005/09/04 14:40:17  alankila
 * - get rid of effect->id and associated enumeration
 *
 * Revision 1.41  2005/09/04 12:12:36  alankila
 * - make create() and done() symmetric in memory allocation/free
 *
 * Revision 1.40  2005/09/04 11:21:48  alankila
 * - one warning is really an error
 *
 * Revision 1.39  2005/09/04 11:16:59  alankila
 * - destroy passthru function, move the toggle logic higher up
 *
 * Revision 1.38  2005/09/04 01:51:09  alankila
 * - GKeyFile-based preset load/save
 * - still need locale-immune %lf for printf and sscanf
 *
 * Revision 1.37  2005/09/03 23:46:04  alankila
 * - add some release polish
 *
 * Revision 1.36  2005/09/03 22:13:56  alankila
 * - make multichannel processing selectable
 * - new GUI (it sucks as much as the old one and I'll need to grok GTK
 *   tables first before it gets better)
 * - make pump.c do the multichannel adapting bits
 * - effects can now change channel counts
 *
 * Revision 1.35  2005/09/01 00:35:35  alankila
 * - add support for GKeyFile into glib 1.2
 *
 * Revision 1.34  2005/08/28 21:45:12  fonin
 * Portability: introduced new functions for mutexes
 *
 * Revision 1.33  2005/08/28 12:42:27  alankila
 * move write_track flag properly into pump.h, make it volatile because it's
 * shared by threads
 *
 * Revision 1.32  2005/08/28 12:39:01  alankila
 * - make audio_lock a real mutex
 * - fix mutex cleanup at exit
 *
 * Revision 1.31  2005/08/27 19:05:43  alankila
 * - introduce int16_t and SAMPLE32 types, and switch
 *
 * Revision 1.30  2005/08/27 18:11:35  alankila
 * - support 32-bit sampling
 * - use 24-bit precision in integer arithmetics
 * - fix effects that contain assumptions about absolute sample values
 *
 * Revision 1.29  2005/08/24 21:44:44  alankila
 * - split sound drivers off main.c
 * - add support for alsa
 * - rework thread locking
 * - in this version, sound drivers are chosen at compile time
 * - windows driver is probably broken
 *
 * Revision 1.28  2005/08/23 22:01:34  alankila
 * - add -Wall to ease developing
 * - fix warnings
 *
 * Revision 1.27  2005/08/22 11:09:13  alankila
 * - close fd at end
 *
 * Revision 1.26  2005/08/22 11:07:27  alankila
 * - move last bits of tracker support off main.c to pump.c
 * - add settings loader/saver for GTK2, now needs GTK+ 2.6 in minimum
 *
 * Revision 1.25  2005/08/21 23:44:13  alankila
 * - use libsndfile on Linux to write audio as .wav
 * - move clipping tests into pump.c to save writing it in tracker and 3 times
 *   in main.c
 * - give default name to .wav from current date and time (in ISO format)
 * - there's a weird bug if you cancel the file dialog, it pops up again!
 *   I have to look into what's going on.
 *
 * Revision 1.24  2005/08/12 17:56:16  alankila
 * use one global sin lookup table
 *
 * Revision 1.23  2005/08/12 11:21:38  alankila
 * - add master volume widget
 * - reimplement bias computation to use true average
 *
 * Revision 1.22  2005/08/10 17:52:40  alankila
 * - don't test foo < 0 for unsigned
 *
 * Revision 1.21  2005/08/10 10:54:39  alankila
 * - add output VU meter. The scale is logarithmic, resolves down to -96 dB
 *   although it's somewhat wasteful at the low end.
 * - add bias elimination code -- what I want is just a long-term average
 *   for estimating the bias. Right now it's a bad estimation of average but
 *   I'll improve it later on.
 * - the vu-meter background flashes red if clipping. (I couldn't make the
 *   bar itself flash red, the colour is maybe not FG or something.)
 * - add *experimental* noise reduction code. This code will be moved into
 *   a separate NR effect later on. Right now it's useful for testing, and
 *   should not perceptibly degrade the signal anyway.
 *
 * Revision 1.20  2005/08/08 18:34:45  alankila
 * - rename effects:
 *   * vibrato -> tremolo
 *   * tremolo -> vibrato
 *   * distort2 -> overdrive
 *
 * Revision 1.19  2005/08/07 12:53:42  alankila
 * - new tuner plugin / effect
 * - some gcc -Wall shutups
 * - added the entry required for gnuitar.vcproj as well but I can't test it
 * - changed pump.h to use enum instead of bunch-of-defines. Hopefully it's
 *   better that way.
 *
 * Revision 1.18  2005/04/24 19:11:22  fonin
 * Optimized for zero input (after the noise filter) to avoid the extra calcs
 *
 * Revision 1.17  2005/04/15 14:32:39  fonin
 * Few improvements with the effects save/load; fixed nasty bug with CR/LF translation when saving preset files on Win32
 *
 * Revision 1.16  2004/08/10 15:07:31  fonin
 * Support processing in float/int - type gnuitar_sample_t
 *
 * Revision 1.15  2003/12/28 10:16:08  fonin
 * Code lickup
 *
 * Revision 1.14  2003/12/21 08:40:36  dexterus
 * biquad files amd eqbank working
 *
 * Revision 1.13  2003/04/12 20:02:10  fonin
 * New noise gate effect.
 *
 * Revision 1.12  2003/04/11 18:34:36  fonin
 * Added distort2 effect.
 *
 * Revision 1.11  2003/03/28 19:56:08  fonin
 * Sampling rate is 44100 by default.
 *
 * Revision 1.10  2003/03/09 21:12:41  fonin
 * New variables for new "change sampling params" feature.
 *
 * Revision 1.9  2003/02/05 21:10:10  fonin
 * Cleanup before release.
 *
 * Revision 1.8  2003/02/03 17:23:26  fonin
 * One more newline after the effects were loaded by pump_start().
 *
 * Revision 1.7  2003/02/03 11:39:25  fonin
 * Copyright year changed.
 *
 * Revision 1.6  2003/01/29 19:34:00  fonin
 * Win32 port.
 *
 * Revision 1.5  2001/06/02 14:05:59  fonin
 * Added GNU disclaimer.
 *
 * Revision 1.4  2001/03/25 17:42:32  fonin
 * open() can overwrite existing files from now, because program switches back to real user priorities after start.
 *
 * Revision 1.3  2001/03/25 12:10:06  fonin
 * Text messages begin from newline rather than end with it.
 *
 * Revision 1.2  2001/01/13 10:02:35  fonin
 * Fix: setuid root program shouldnt overwrite existing files.
 *
 * Revision 1.1.1.1  2001/01/11 13:22:01  fonin
 * Version 0.1.0 Release 1 beta
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#ifndef _WIN32
#    include <unistd.h>
#else
#    include <io.h>
#    include <windows.h>
#endif
#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include "pump.h"
#include "gui.h"
#include "glib12-compat.h"
#include "audio-midi.h"
#include "audio-driver.h"
#include "effect.h"
#include "tracker.h"
#include "utils.h"

gnuitar_pump_t *
gnuitar_pump_create(void)
{
    gnuitar_pump_t *pump;
    pump = malloc(sizeof(*pump));
    if (pump == NULL)
        return NULL;
    pump->ref_count = 1;
    pump->effects = NULL;
    pump->n_effects = 0;
    return pump;
}

void
gnuitar_pump_incref(gnuitar_pump_t *pump)
{
    pump->ref_count++;
}

void
gnuitar_pump_decref(gnuitar_pump_t *pump)
{
    size_t i;
    if (pump->ref_count <= 0) {
        return;
    }
    pump->ref_count--;
    if (pump->ref_count > 0) {
        return;
    }
    for (i = 0; i < pump->n_effects; i++) {
        gnuitar_effect_decref(pump->effects[i]);
    }
    free(pump->effects);
}

gnuitar_error_t
gnuitar_pump_add_effect(gnuitar_pump_t *pump, gnuitar_effect_t *effect)
{
    gnuitar_effect_t ** tmp;
    size_t tmp_size;

    tmp_size = sizeof(*tmp) * (pump->n_effects + 1);
    tmp = realloc(pump->effects, tmp_size);
    if (tmp == NULL)
        return GNUITAR_ERROR_MALLOC;

    gnuitar_effect_incref(effect);

    tmp[pump->n_effects] = effect;
    pump->effects = tmp;
    pump->n_effects++;
    return GNUITAR_ERROR_NONE;
}

gnuitar_error_t
gnuitar_pump_erase_effect(gnuitar_pump_t *pump, unsigned int index)
{
    if (index >= pump->n_effects)
        return GNUITAR_ERROR_ENOENT;

    gnuitar_effect_decref(pump->effects[index]);

    memmove(&pump->effects[index],
            &pump->effects[index + 1],
            (pump->n_effects - (index + 1)) * sizeof(*pump->effects));

    pump->n_effects--;

    return GNUITAR_ERROR_NONE;
}

gnuitar_error_t
gnuitar_pump_move_effect(gnuitar_pump_t *pump, unsigned int src, unsigned int dst)
{
    unsigned int i;
    gnuitar_effect_t *swap;

    if ((src >= pump->n_effects)
     || (dst >= pump->n_effects)){
        return GNUITAR_ERROR_ENOENT;
    }

    swap = pump->effects[src];
    if (src < dst) {
        for (i = src; i < dst; i++)
            pump->effects[i] = pump->effects[i + 1];
    } else if (src > dst) {
        for (i = src; i > dst; i--)
            pump->effects[i] = pump->effects[i - 1];
    }
    pump->effects[dst] = swap;

    return GNUITAR_ERROR_NONE;
}

static void bias_elimination(gnuitar_packet_t *packet);

static float vu_meter(gnuitar_packet_t *packet);

static void adjust_master_volume(gnuitar_packet_t *packet);

static void adjust_input_volume(gnuitar_packet_t *packet);

static void adapt_to_output(gnuitar_packet_t *packet);

void
gnuitar_pump_process(gnuitar_pump_t *pump, gnuitar_packet_t *packet)
{
    unsigned int i;

    bias_elimination(packet);
    adjust_input_volume(packet);
    set_vumeter_in_value(vu_meter(packet));

    for (i = 0; i < pump->n_effects; i++) {
        if (!pump->effects[i]->toggle)
            continue;
        gnuitar_effect_process(pump->effects[i], packet);
    }

    adjust_master_volume(packet);
    set_vumeter_out_value(vu_meter(packet));
    adapt_to_output(packet);

    if (write_track)
        track_write(packet->data, packet->len);
}

/* flag for whether we are creating .wav */
volatile unsigned short  write_track = 0;
/* sin table */
float sin_lookup_table[SIN_LOOKUP_SIZE + 1];

/* If the long-term average of input data does not exactly equal to 0,
 * compensate. Some soundcards would also need highpass filtering ~20 Hz
 * or so. */
static void
bias_elimination(gnuitar_packet_t *db) {
    static gnuitar_sample_t       bias_s[MAX_CHANNELS] = { 0, 0, 0, 0 };
    static int_least32_t    bias_n[MAX_CHANNELS] = { 10, 10, 10, 10 };
    uint_fast16_t i, curr_channel = 0;
    gnuitar_sample_t biasadj = bias_s[curr_channel] / bias_n[curr_channel];
    
    for (i = 0; i < db->len; i += 1) {
        bias_s[curr_channel] += db->data[i];
        bias_n[curr_channel] += 1;
        db->data[i] -= biasadj;
        curr_channel = (curr_channel + 1) % db->channels;
    }
    /* keep bias within limits of shortest type (int_least32_t) */
    for (i = 0; i < MAX_CHANNELS; i += 1) {
        if (fabs(bias_s[i]) > (gnuitar_sample_t) 1E10 || bias_n[i] > (int_least32_t) 1E10) {
            bias_s[i] /= (gnuitar_sample_t) 2;
            bias_n[i] /= 2;
        }
    }
}

/* accumulate power estimate and monitor clipping */
static float
vu_meter(gnuitar_packet_t *db) {
    unsigned int i;
    gnuitar_sample_t sample;
    float power = 0;

    for (i = 0; i < db->len; i += 1) {
        sample = db->data[i];
        power += (float) sample * (float) sample;
    }
    /* energy per sample scaled down to 0.0 - 1.0 */
    return power / (float) db->len / (float) MAX_SAMPLE / (float) MAX_SAMPLE;
}

/* adjust master volume according to the main window slider and clip */
static void
adjust_master_volume(gnuitar_packet_t *db) {
    unsigned int i;
    float volume = pow(10, master_volume / 20.0);

    for (i = 0; i < db->len; i += 1) {
	float val = db->data[i] * volume;
        CLIP_SAMPLE(val);
        db->data[i] = val;
    }
}

static void
adjust_input_volume(gnuitar_packet_t *db) {
    unsigned int i;
    float	volume = pow(10, input_volume / 20.0);

    for (i = 0; i < db->len; i += 1)
        db->data[i] = db->data[i] * volume;
}


static void
adapt_to_output(gnuitar_packet_t *db)
{
    int i;
    int size = db->len;
    gnuitar_format_t format;
    unsigned int output_channels;
    gnuitar_sample_t *s = db->data;

    if (gnuitar_audio_driver_get_format(audio_driver, &format) < 0) {
        output_channels = 2;
    } else {
        output_channels = format.output_channels;
    }

    assert(db->channels <= output_channels);

    /* nothing to do */
    if (db->channels == output_channels)
        return;
    /* clone 1 to 2 */
    if (db->channels == 1 && output_channels == 2) {
        for (i = size - 1; i >= 0; i -= 1) {
            s[i*2+1] = s[i];
            s[i*2  ] = s[i];
        }
        db->channels = 2;
        db->len *= 2;
        return;
    }
    /* clone 1 to 4, mute channels 2 & 3 (the rear channels) */
    if (db->channels == 1 && output_channels == 4) {
        for (i = size - 1; i >= 0; i -= 1) {
            s[i*4+3] = 0;
            s[i*4+2] = 0;
            s[i*4+1] = s[i];
            s[i*4  ] = s[i];
        }
        db->channels = 4;
        db->len *= 4;
        return;
    }
    /* clone 2 to 4, mute channels 2 & 3 */
    if (db->channels == 2 && output_channels == 4) {
        for (i = size/2-1; i >= 0; i -= 1) {
            s[i*4+3] = 0;
            s[i*4+2] = 0;
            s[i*4+1] = s[i*2+1];
            s[i*4+0] = s[i*2+0];
        }
        db->channels = 4;
        db->len *= 2;
        return;
    }
    /* we shouldn't have more than 2 channels coming in, and we don't support
     * generating to 5 channels, so error out */

    gnuitar_printf( "unknown channel combination: %d in and %d out",
            db->channels, output_channels);
}

static void
init_sin_lookup_table(void) {
    int i = 0;
    for (i = 0; i < SIN_LOOKUP_SIZE + 1; i += 1)
        sin_lookup_table[i] = sin(2 * M_PI * i / SIN_LOOKUP_SIZE);
}

gchar *
discover_preset_path(void) {
    return g_strdup_printf("%s" FILESEP "%s", g_get_home_dir(), ".gnuitar_presets");
}

void
load_initial_state(char **argv, int argc)
{
    char *path = discover_preset_path();
    DIR *dir;
    struct dirent *ent;
    GList *list = NULL;
    GList *cur;

    (void) argv;
    (void) argc;

    dir = opendir(path);
    if (dir == NULL) {
        if (errno == ENOENT) {
            /* doesn't exist, create it */
#ifdef _WIN32
            mkdir(path);
#else
            mkdir(path, 0777);
#endif
        }
        /* whether it succeeded or not doesn't matter, there's nothing to load */
        return;
    }

    /* scan the settings directory, load into preset control */
    while ((ent = readdir(dir)) != NULL) {
        gchar *entry;
        /* entry must terminate with .gnuitar to be considered */
        if (strstr(ent->d_name, ".gnuitar") == NULL)
            continue;
        /* construct full path to entry */
        entry = g_strdup_printf("%s" FILESEP "%s", path, ent->d_name);
        list = g_list_append(list, entry);
    }
    if (errno) {
        gnuitar_printf("Error reading presets from directory '%s': %s\n",
                       path, strerror(errno));
    }
    g_free(path);

    /* order the entries to guarantee same loading order anywhere */
    list = g_list_sort(list, (GCompareFunc) strcmp);

    /* iterate the entires now, add into preset list */
    for (cur = g_list_first(list); cur != NULL; cur = g_list_next(cur)) {
        if (strstr(cur->data, FILESEP "__default__.gnuitar") != NULL) {
            /* if the entry is the default entry, load it, don't append it.
             * If user gave cmdline arguments, load the effects from cmdline. */
            if (argc < 2)
                load_pump(cur->data);
        } else {
            /* add it to preset list */
            bank_append_entry(cur->data);
        }
        g_free(cur->data);
    }
    /* free GList memory */
    g_list_free(list);
}

void
load_settings(void) {
    /* deprecated */
}

void
save_settings(void) {
    /* deprecated */
}

void
pump_start(void)
{
    init_sin_lookup_table();

    master_volume = 0.0;
    input_volume = 0.0;
    write_track = 0;
}

void
pump_stop(void)
{
    if (write_track) {
        write_track = 0;
        tracker_done();
    }
}

void
save_pump(const char *fname)
{
    (void) fname;
}

void
load_pump(const char *fname)
{
    (void) fname;
}
