/*
 * GNUitar
 * Windows sound driver
 * Copyright (C) 2000,2001,2003 Max Rudensky         <fonin@ziet.zhitomir.ua>
 *           (C) 2005,2006 Antti S. Lankila          <alankila@bel.fi>
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
 * Revision 1.1  2006/07/27 18:31:15  alankila
 * - split dsound and winmm into separate drivers.
 *
 */

#ifndef _AUDIO_DSOUND_H_
#define _AUDIO_DSOUND_H_ 1
#ifdef HAVE_DSOUND

#include "pump.h"
audio_driver_t dsound_driver;

#endif
#endif

