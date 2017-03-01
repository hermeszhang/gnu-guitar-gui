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

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <pthread.h>
#endif /* _WIN32 */

struct GnuitarMutex {
#ifdef _WIN32
    HANDLE handle;
#else /* _WIN32 */
    pthread_mutex_t handle;
#endif /* _WIN32 */
};

void gnuitar_mutex_init(struct GnuitarMutex *mutex);

void gnuitar_mutex_done(struct GnuitarMutex *mutex);

int gnuitar_mutex_lock(struct GnuitarMutex *mutex);

int gnuitar_mutex_unlock(struct GnuitarMutex *mutex);

#endif /* GNUITAR_UTILS_H */

