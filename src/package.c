#include "package.h"

#include "amp.h"
#include "autowah.h"
#include "phasor.h"
#include "chorus.h"
#include "delay.h"
#include "echo.h"
#include "tremolo.h"
#include "vibrato.h"
#include "distort.h"
#include "distort2.h"
#include "tubeamp.h"
#include "sustain.h"
#include "reverb.h"
#include "rotary.h"
#include "noise.h"
#include "eqbank.h"
#include "pitch.h"
#include "tuner.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif /* _WIN32 */

#include <stdlib.h>

const gnuitar_package_effect_t effect_list[] = {
    { "Digital Amp", amp_create },
    { "Autowah", autowah_create },
    { "Distort", distort_create },
    { "Delay", delay_create },
    { "Reverb", reverb_create },
    { "Tremolo bar", vibrato_create },
    { "Chorus / Flanger", chorus_create },
    { "Echo", echo_create },
    { "Phaser", phasor_create },
    { "Tremolo", tremolo_create },
    { "Sustain", sustain_create },
    { "Overdrive", distort2_create },
    { "Tube amplifier", tubeamp_create },
    { "Rotary speaker", rotary_create },
    { "Noise gate", noise_create },
    { "Eq bank", eqbank_create },
    { "Pitch shift", pitch_create },
    { "Tuner", tuner_create }
};

#ifdef _MSC_VER
#define gnuitar_strdup _strdup
#else /* _MSC_VER */
#define gnuitar_strdup strdup
#endif /* _MSC_VER */

gnuitar_package_t *builtin_package = NULL;

static int gnuitar_package_add_builtins(gnuitar_package_t *package);

/** Opens up a package, specified by the path.
 * The package is created with a reference count of one.
 * To close the package, see @ref gnuitar_package_decref.
 * To increase the package reference count, see @ref gnuitar_package_incref
 * @param path The path of the library.
 *  If this parameter is NULL, the builtin package is opened.
 * @returns A package structure is always returned, unless an
 *  error occurs allocating space for the structure (then it returns NULL).
 * @ingroup gnuitar-package
 */

gnuitar_package_t *
gnuitar_package_open(const char *path)
{
    gnuitar_package_t *package = NULL;

    package = malloc(sizeof(*package));
    if (package == NULL) {
        return NULL;
    }

    package->ref_count = 1;
    package->handle = NULL;
    package->name = NULL;
    package->effects = NULL;
    package->effects_count = 0;
    package->drivers = NULL;
    package->drivers_count = 0;

    if ((path == NULL) || (path[0] == 0)) {
        package->handle = NULL;
        gnuitar_package_add_builtins(package);
    } else {
#ifdef _WIN32
        package->handle = LoadLibrary(path);
#else /* _WIN32 */
        package->handle = dlopen(path, RTLD_LAZY);
        if (package->handle == NULL) {
            return package;
        }
#endif /* _WIN32 */
    }

    return package;
}

/** Increases the reference count of the package.
 * @param package A package created by @ref gnuitar_package_open
 * @ingroup gnuitar-package
 */

void
gnuitar_package_incref(gnuitar_package_t *package)
{
    package->ref_count++;
}

/** Decreases the reference count of the package.
 * If the reference count reaches zero, the package is closed.
 * If the reference count is already zero, nothing happens.
 * @param package A package created by @ref gnuitar_package_open
 * @ingroup gnuitar-package
 */

void
gnuitar_package_decref(gnuitar_package_t *package)
{
    if (package->ref_count == 0) {
        return;
    }
    package->ref_count--;
    if (package->ref_count > 0){
        return;
    }
    if (package->handle != NULL) {
#ifdef _WIN32
        FreeLibrary(package->handle);
#else /* _WIN32 */
        dlclose(package->handle);
#endif /* _WIN32 */
    }
    free(package->name);
}

gnuitar_effect_t *
gnuitar_package_create_effect(gnuitar_package_t *package, const char *name)
{
    gnuitar_error_t error;
    gnuitar_effect_t *effect;
    unsigned int index;

    error = gnuitar_package_find_effect(package, name, &index);
    if (error)
        return NULL;

    effect = package->effects[index].create();
    if (effect == NULL)
        return NULL;

    effect->ref_count = 1;
    effect->toggle = 0;
    effect->proc_init(effect);

    return effect;
}

/** Locates the index of an effect.
 * @param package The package to search the effect for.
 * @param name The name of the effect
 * @param index The index of the effect.
 *  If the effect is not found, this parameter is not accessed.
 *  This parameter may be NULL.
 * @returns If the effect is found, @ref GNUITAR_ERROR_NONE is returned.
 *  If the effect is not found, @ref GNUITAR_ERROR_ENOENT is returned.
 * @ingroup gnuitar-package
 */

gnuitar_error_t
gnuitar_package_find_effect(const gnuitar_package_t *package, const char *name, unsigned int *index)
{
    unsigned int i;
    for (i = 0; i < package->effects_count; i++) {
        if (strcmp(package->effects[i].name, name) == 0)
            break;
    }
    if (i >= package->effects_count)
        return GNUITAR_ERROR_ENOENT;
    if (index != NULL)
        *index = i;
    return GNUITAR_ERROR_NONE;
}

/** Gets the number of effects in the package.
 * @param package A package created by @gnuitar_package_open
 * @returns The number of effects in the package.
 * @ingroup gnuitar-package
 */

unsigned int
gnuitar_package_get_effect_count(const gnuitar_package_t *package)
{
	return package->effects_count;
}

/** Gets the name of an effect.
 * @param package A package created by @ref gnuitar_package_open
 * @param index The index of the effect in the package
 * @returns On success, the name of the effect.
 *  On failure, returns NULL.
 * @ingroup gnuitar-package
 */

const char *
gnuitar_package_get_effect_name(const gnuitar_package_t *package, unsigned int index)
{
    if (index >= package->effects_count) {
        return NULL;
    }
    return package->effects[index].name;
}

/** Adds builtin effects and audio drivers to the package.
 * @param package A package created by @gnuitar_package_open
 * @returns Zero on success, a negative number otherwise.
 */

static int
gnuitar_package_add_builtins(gnuitar_package_t *package)
{
    /* 18 effects */
    const unsigned int effects_count = sizeof(effect_list) / sizeof(effect_list[0]);
    unsigned int i;

    package->name = gnuitar_strdup("GNUitar");

    package->effects = malloc(effects_count * sizeof(gnuitar_package_effect_t));
    if (package->effects == NULL) {
        return -1;
    }
    package->effects_count = effects_count;

    for (i = 0; i < effects_count; i++){
        package->effects[i].name = strdup(effect_list[i].name);
        package->effects[i].create = effect_list[i].create;
    }

    return 0;
}

