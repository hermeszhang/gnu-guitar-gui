/*
 * GNUitar
 * ALSA parts
 * Copyright (C) 2005 Antti Lankila  <alankila@bel.fi>
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
 * Revision 1.6  2006/08/10 18:52:07  alankila
 * - declare prototypes properly
 * - hide some accidentally global methods
 *
 * Revision 1.5  2006/08/06 20:14:54  alankila
 * - split pump.h into several domain-specific headers to reduce file
 *   interdependencies (everyone included pump.h). New files are:
 *   - effect.h for effect definitions
 *   - audio-driver.h for work relating to audio drivers
 *   - audio-midi.h for MIDI interaction.
 *
 * Revision 1.4  2006/07/26 23:09:09  alankila
 * - DirectSound may be buggy; MMSystem at least worked in mingw build.
 * - remove some sound-specific special cases in gui and main code.
 * - create thread in windows driver.
 * - remove all traces of "program states" variable.
 * - remove snd_open mutex: it is now unnecessary. Concurrency is handled
 *   through joining/waiting threads where necessary. (We assume JACK
 *   does its own locking, though.)
 *
 * Revision 1.3  2005/09/03 20:20:42  alankila
 * - create audio_driver type and write all the driver stuff into it. This
 *   faciliates carrying configuration data about the capabilities of
 *   a specific audio driver and uses less global variables.
 *
 * Revision 1.2  2005/08/27 18:11:35  alankila
 * - support 32-bit sampling
 * - use 24-bit precision in integer arithmetics
 * - fix effects that contain assumptions about absolute sample values
 *
 * Revision 1.1  2005/08/24 21:44:44  alankila
 * - split sound drivers off main.c
 * - add support for alsa
 * - rework thread locking
 * - in this version, sound drivers are chosen at compile time
 * - windows driver is probably broken
 *
 *
 */

#ifndef GNUITAR_AUDIO_ALSA_H
#define GNUITAR_AUDIO_ALSA_H

#ifdef HAVE_ALSA

#include "audio-driver.h"

#include <alsa/asoundlib.h>
#include <pthread.h>

typedef struct gnuitar_alsa_driver {
    /** The name of the input PCM */
    char * input_name;
    /** The input PCM */
    snd_pcm_t * input_pcm;
    /** The number of channels from the input */
    unsigned int input_channels;
    /** The number of bits per sample from the input */
    unsigned int input_bits;
    /** The name of the output PCM */
    char * output_name;
    /** The output PCM */
    snd_pcm_t * output_pcm;
    /** The number of channels from the output */
    unsigned int output_channels;
    /** The number of bits per sample from the output */
    unsigned int output_bits;
    /** The number of periods to use */
    unsigned int period_size;
    /** The number of frames in a period */
    unsigned int periods;
    /** The number of frames per second */
    unsigned long int rate;
    /** The thread that runs the audio threw the pump */
    pthread_t thread;
    /** A flag to decide whether or not to keep to the thread running */
    unsigned int keep_thread_running;
} gnuitar_alsa_driver_t;

gnuitar_audio_driver_t * gnuitar_alsa_driver_create(void);

int gnuitar_alsa_available(void);

extern gnuitar_audio_driver_t *global_alsa_driver;

#endif /* HAVE_ALSA */

#endif /* GNUITAR_AUDIO_ALSA_H */

