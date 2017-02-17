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

#ifndef GNUITAR_UTILS_H
#define GNUITAR_UTILS_H

#include <math.h>

#ifdef _WIN32

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef  R_OK
#define  R_OK 04
#endif

#ifndef  X_OK
#define  X_OK 01
#endif

#ifndef  F_OK
#define  F_OK 00
#endif

#define FILESEP "\\"

#ifndef isnan
#define isnan _isnan
#endif

#include <windows.h>

struct GnuitarMutex {
    HANDLE handle;
};

#else /* _WIN32 */

#define FILESEP "/"

#include <pthread.h>

struct GnuitarMutex {
    pthread_t handle;
};

#endif /* _WIN32 */

void gnuitar_mutex_init(struct GnuitarMutex *mutex);

void gnuitar_mutex_done(struct GnuitarMutex *mutex);

void gnuitar_mutex_lock(struct GnuitarMutex *mutex);

void gnuitar_mutex_unlock(struct GnuitarMutex *mutex);

#endif /* GNUITAR_UTILS_H */

