#include "package.h"

#include "autowah.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif /* _WIN32 */

#include <stdlib.h>

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
gnuitar_package_get_name(const gnuitar_package_t *package, unsigned int index)
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
    const unsigned int effects_count = 1;

    package->name = gnuitar_strdup("GNUitar");

    package->effects = malloc(effects_count * sizeof(gnuitar_effect_t));
    if (package->effects == NULL) {
        return -1;
    }
    package->effects_count = effects_count;

    package->effects[0].name = gnuitar_strdup("Autowah");
    package->effects[0].create = autowah_create;

    return 0;
}

