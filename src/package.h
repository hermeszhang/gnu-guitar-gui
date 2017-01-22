#ifndef GNUITAR_PACKAGE_H
#define GNUITAR_PACKAGE_H

#include "audio-driver.h"
#include "effect.h"

#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

/** An effect that's part of a package */
typedef struct gnuitar_package_effect {
    /** The name of the effect */
    char * name;
    /** The method to create the effect */
    gnuitar_effect_t * (*create)(void);
} gnuitar_package_effect_t;

/** An audio driver that's part of a package  */

typedef struct gnuitar_package_driver {
    /** The name of the audio driver */
    char * name;
    /** The method used to create the audio driver */
    gnuitar_audio_driver_t * (*create_driver)(void);
} gnuitar_package_driver_t;

typedef struct gnuitar_package {
    /** The reference count of the package */
    unsigned int ref_count;
#ifdef _WIN32
    /** A library handle returned by LoadLibrary */
    HMODULE handle;
#else /* _WIN32 */
    /** A library handle returned by dlopen */
    void * handle;
#endif /* __linux__ */
    /** The name of the package ('gnuitar' is the builtin package) */
    char * name;
    /** An array of effects from the package */
    gnuitar_package_effect_t * effects;
    /** The number of effects in the package */
    unsigned int effects_count;
    /** An array of audio drivers from the package */
    gnuitar_package_driver_t * drivers;
    /** The number of audio drivers in the package */
    unsigned int drivers_count;
} gnuitar_package_t;

/* TODO get rid of this */

extern gnuitar_package_t * builtin_package;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

gnuitar_package_t * gnuitar_package_open(const char *path);

void gnuitar_package_incref(gnuitar_package_t *package);

void gnuitar_package_decref(gnuitar_package_t *package);

gnuitar_error_t gnuitar_package_find_effect(const gnuitar_package_t *package, const char *name, unsigned int *index);

unsigned int gnuitar_package_get_effect_count(const gnuitar_package_t *package);

const char * gnuitar_package_get_effect_name(const gnuitar_package_t *package, unsigned int index);

gnuitar_effect_t * gnuitar_package_create_effect(gnuitar_package_t *package, const char * name);

gnuitar_audio_driver_t * gnuitar_package_create_driver(gnuitar_package_t *package, const char * name);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_PACKAGE_H */

