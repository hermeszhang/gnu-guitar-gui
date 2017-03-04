#ifndef GNUITAR_PACKAGE_H
#define GNUITAR_PACKAGE_H

#include "driver.h"
#include "effect.h"

#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef _WIN32
#define GNUITAR_BUILTINS_PACKAGE "gnuitar-builtins.dll"
#else /* _WIN32 */
#define GNUITAR_BUILTINS_PACKAGE "libgnuitar-builtins.so"
#endif /* _WIN32 */

/** @defgroup libgnuitar-package Package
 * The API related to Gnuitar packages (plugins)
 */

/** An effect that's part of a package.
 * @ingroup libgnuitar-package
 */

struct GnuitarPackageEffect {
    /** The name of the effect */
    const char *name;
    /** The method to create the effect */
    int (*init)(struct GnuitarEffect *);
};

/** An audio driver that's part of a package.
 * @ingroup libgnuitar-package
 */

struct GnuitarPackageDriver {
    /** The name of the audio driver */
    const char *name;
    /** The method used to create the audio driver */
    int (*init)(struct GnuitarDriver *);
};

struct GnuitarPackage {
#ifdef _WIN32
    /** A library handle returned by LoadLibrary */
    HMODULE handle;
#else /* _WIN32 */
    /** A library handle returned by dlopen */
    void *handle;
#endif /* _WIN32 */
    /** The name of the package ('gnuitar' is the builtin package) */
    char *name;
    /** An array of effects from the package */
    struct GnuitarPackageEffect *effects;
    /** The number of effects in the package */
    size_t effects_count;
    /** An array of audio drivers from the package */
    struct GnuitarPackageDriver *drivers;
    /** The number of audio drivers in the package */
    size_t drivers_count;
};

int gnuitar_package_open(struct GnuitarPackage *package, const char *path);

void gnuitar_package_init(struct GnuitarPackage *package);

void gnuitar_package_done(struct GnuitarPackage *package);

int gnuitar_package_add_effect(struct GnuitarPackage *package, const struct GnuitarPackageEffect *package_effect);

int gnuitar_package_add_driver(struct GnuitarPackage *package, const struct GnuitarPackageDriver *driver);

int gnuitar_package_find_effect(const struct GnuitarPackage *package, const char *name, size_t *index);

size_t gnuitar_package_get_driver_count(const struct GnuitarPackage *package);

size_t gnuitar_package_get_effect_count(const struct GnuitarPackage *package);

const char * gnuitar_package_get_driver_name(const struct GnuitarPackage *package, size_t index);

const char * gnuitar_package_get_effect_name(const struct GnuitarPackage *package, size_t index);

const char * gnuitar_package_get_name(const struct GnuitarPackage *package);

int gnuitar_package_init_effect(struct GnuitarPackage *package, const char *name, struct GnuitarEffect *effect);

int gnuitar_package_init_driver(struct GnuitarPackage *package, const char *name, struct GnuitarDriver *driver);

int gnuitar_package_set_name(struct GnuitarPackage *package, const char *name);

int gnuitar_package_entry(struct GnuitarPackage *package);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_PACKAGE_H */

