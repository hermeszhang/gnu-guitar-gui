/*
 * GNUitar
 * Utility functions
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

#ifndef _UTILS_H_
#define _UTILS_H_ 1

#define ERR_NOERROR		0
#define ERR_THREAD		1	/* cannot create audio thread */
#define ERR_DEMOEXPIRED		2	/* demo version is expired */
#define ERR_WAVEINOPEN		3	/* cannot open wave in device */
#define ERR_WAVEOUTOPEN		4	/* cannot open wave out device */
#define ERR_WAVEFRAGMENT	5	/* cannot set fragments */
#define ERR_WAVEDUPLEX		6	/* cannot open device in duplex
					 * mode */
#define ERR_WAVENOTDUPLEX	7	/* device is not full-duplex
					 * capable */
#define ERR_WAVEGETCAPS		8	/* cannot get device capabilities */
#define ERR_WAVESETBIT		9	/* cannot set 8/16bits */
#define ERR_WAVESETCHANNELS	10	/* cannot set mono/stereo mode */
#define ERR_WAVESETRATE		11	/* cannot set sampling rate */
#define ERR_WAVEOUTHDR		12	/* error preparing write header */
#define ERR_WAVEINHDR		13	/* error preparing record header */
#define ERR_WAVEINQUEUE		14	/* error queuing record header */
#define ERR_WAVEINRECORD	15	/* recording error */

#include "pump.h"
#ifdef _WIN32
#    define M_PI 3.14159265358979323846E0
#endif

extern short    log2(int);

#endif