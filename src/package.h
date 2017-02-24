#ifndef GNUITAR_PACKAGE_H
#define GNUITAR_PACKAGE_H

#include "track.h"
#include "effect.h"

#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup libgnuitar-package Package
 * The API related to Gnuitar packages (plugins)
 */

/** An effect that's part of a package.
 * @ingroup libgnuitar-package
 */

struct GnuitarPackageEffect {
    /** The name of the effect */
    const char * name;
    /** The method to create the effect */
    int (*init)(struct GnuitarEffect *);
} gnuitar_package_effect_t;

/** An audio track that's part of a package.
 * @ingroup libgnuitar-package
 */

struct GnuitarPackageTrack {
    /** The name of the audio track */
    char * name;
    /** The method used to create the audio track */
    int (*init)(struct GnuitarTrack *);
};

struct GnuitarPackage {
#ifdef _WIN32
    /** A library handle returned by LoadLibrary */
    HMODULE handle;
#else /* _WIN32 */
    /** A library handle returned by dlopen */
    void * handle;
#endif /* _WIN32 */
    /** The name of the package ('gnuitar' is the builtin package) */
    char * name;
    /** An array of effects from the package */
    struct GnuitarPackageEffect * effects;
    /** The number of effects in the package */
    size_t effects_count;
    /** An array of audio tracks from the package */
    struct GnuitarPackageTrack * tracks;
    /** The number of audio tracks in the package */
    size_t tracks_count;
};

int gnuitar_package_open(struct GnuitarPackage *package, const char *path);

void gnuitar_package_done(struct GnuitarPackage *package);

int gnuitar_package_add_effect(struct GnuitarPackage *package, const struct GnuitarPackageEffect *package_effect);

int gnuitar_package_find_effect(const struct GnuitarPackage *package, const char *name, size_t *index);

size_t gnuitar_package_get_effect_count(const struct GnuitarPackage *package);

const char * gnuitar_package_get_effect_name(const struct GnuitarPackage *package, size_t index);

int gnuitar_package_init_effect(struct GnuitarPackage *package, const char *name, struct GnuitarEffect *effect);

int gnuitar_package_init_track(struct GnuitarPackage *package, const char *name, struct GnuitarTrack *track);

int gnuitar_package_entry(struct GnuitarPackage *package);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_PACKAGE_H */

