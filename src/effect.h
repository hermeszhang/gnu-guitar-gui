/*
 * GNUitar
 * Common definitions for all effects.
 * Copyright (C) 2000,2001,2003 Max Rudensky         <fonin@ziet.zhitomir.ua>
 *           (c) 2005,2006 Antti S. Lankila          <alankila@bel.fi>
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

#ifndef GNUITAR_EFFECT_H
#define GNUITAR_EFFECT_H

#include "packet.h"
#include "map.h"

/** @defgroup libgnuitar-effect Audio Effects
 * The API that's associated with audio effects.
 */

/** An audio effect structure.
 * @ingroup libgnuitar-effect
 */

struct GnuitarEffect {
    /** Effect specific data */
    void *params;
    /** Initializes the effect */
    void (*init) (struct GnuitarEffect *);
    /** Releases resources allocated by the effect */
    void (*done) (struct GnuitarEffect *);
    /** The processing callback of the effect */
    int (*process) (struct GnuitarEffect *, struct GnuitarPacket *);
    /** Gets the parameter map of the effect */
    int (*get_map) (const struct GnuitarEffect *, struct GnuitarMap *);
    /** Sets the parameter map of the effect */
    int (*set_map) (struct GnuitarEffect *, const struct GnuitarMap *);
    /** Toggles the effect on and off */
    short toggle;
};

void gnuitar_effect_init(struct GnuitarEffect *effect);

void gnuitar_effect_done(struct GnuitarEffect *effect);

int gnuitar_effect_process(struct GnuitarEffect *effect, struct GnuitarPacket *packet);

int gnuitar_effect_get_map(const struct GnuitarEffect *effect, struct GnuitarMap *map);

int gnuitar_effect_set_map(struct GnuitarEffect *effect, const struct GnuitarMap *map);

/* left for compatibility */

#ifdef __MINGW32__
#include <malloc.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __GNUC__
#define unlikely(x) __builtin_expect((x), 0)
#else
#warning "no unlikely"
#define unlikely(x) !!(x)
#endif

#define CLIP_SAMPLE(sm) \
    if (unlikely(sm > (typeof(sm)) MAX_SAMPLE)) \
        sm = (typeof(sm)) MAX_SAMPLE; \
    if (unlikely(sm < (typeof(sm)) -MAX_SAMPLE)) \
        sm = (typeof(sm)) -MAX_SAMPLE;

/* these macros are used to save my sanity */
#define SAVE_ARGS \
    GKeyFile *preset, gchar *group
#define LOAD_ARGS \
    GKeyFile *preset, gchar *group, GError **error
#define SAVE_DOUBLE(name, param) \
    g_key_file_set_double(preset, group, name, param);
#define LOAD_DOUBLE(name, param) \
    param = g_key_file_get_double(preset, group, name, error); \
    if (*error != NULL) { \
	gnuitar_printf("warning: couldn't read '%s', '%s'\n", group, name); \
	*error = NULL; \
    }
#define SAVE_INT(name, param) \
    g_key_file_set_integer(preset, group, name, param);
#define LOAD_INT(name, param) \
    param = g_key_file_get_integer(preset, group, name, error); \
    if (*error != NULL) { \
	gnuitar_printf("warning: couldn't read '%s', '%s'\n", group, name); \
	*error = NULL; \
    }

/* tabularised sin() */
#define SIN_LOOKUP_SIZE         65536
extern float sin_lookup_table[SIN_LOOKUP_SIZE+1];

static inline float
sin_lookup(float pos) {
    return sin_lookup_table[(int) (pos * (float) SIN_LOOKUP_SIZE)];
}

static inline float
cos_lookup(float pos) {
    if (pos >= 0.75f)
        return sin_lookup(pos - 0.75f);
    else
        return sin_lookup(pos + 0.25f);
}

/* FreeBSD's malloc is aligned by 16 */
#if defined(__SSE__) && !defined(__FreeBSD__)

/* for SSE we need aligned memory */
static inline void *
gnuitar_memalign(size_t num, size_t bytes) {
    void *mem = NULL;
#ifndef __MINGW32__
    if (posix_memalign(&mem, 16, num * bytes))
        return NULL;
#else /* __MINGW32__ */
    mem = __mingw_aligned_malloc(num * bytes, 16);
    if (mem == NULL)
        return NULL;
#endif /* __MINGW32__ */
    memset(mem, 0, num * bytes);
    return mem;
}

static inline void
gnuitar_free(void *memory) {
#ifndef __MINGW32__
    free(memory);
#else /* __MINGW32__ */
    __mingw_aligned_free(memory);
#endif /* __MINGW32__ */
}

#else /* defined(__SSE__) && !defined(__FreeBSD__) */

/* without SSE we just wrap calloc */

static inline void *
gnuitar_memalign(unsigned int num, size_t bytes)
{
    void *mem = calloc(num, bytes);
    if (mem == NULL) {
        fprintf(stderr, "failed to allocate aligned memory.\n");
        exit(1);
    }
    return mem;
}

static inline void
gnuitar_free(void *memory) {
    free(memory);
}

#endif /* defined(__SSE__) && !defined(__FreeBSD__) */

#endif /* GNUITAR_EFFECT_H */

