#include "package.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif /* _WIN32 */

#include <stdlib.h>
#include <errno.h>

#ifdef _MSC_VER
#define gnuitar_strdup _strdup
#else /* _MSC_VER */
#define gnuitar_strdup strdup
#endif /* _MSC_VER */

/** Opens up a package, specified by the path.
 * To close the package, see @ref gnuitar_package_done.
 * @param package An unitialized package.
 * @param path The path of the library.
 *  This parameter may not be NULL.
 * @returns On success, zero.
 *  Otherwise, the error number is returned.
 * @ingroup gnuitar-package
 */

int
gnuitar_package_open(struct GnuitarPackage *package, const char *path)
{
#ifdef _WIN32
#else /* _WIN32 */
    int (*package_init)(struct GnuitarPackage *package);
    int err;
#endif /* _WIN32 */
    gnuitar_package_init(package);

    if (path == NULL)
        return EFAULT;

#ifdef _WIN32
    package->handle = LoadLibrary(path);
#else /* _WIN32 */
    package->handle = dlopen(path, RTLD_LAZY);
    if (package->handle == NULL)
        return ENOENT;
    package_init = dlsym(package->handle, "gnuitar_package_entry");
    if (package_init == NULL) {
        dlclose(package->handle);
        return ENOENT;
    }
    err = package_init(package);
    if (err != 0) {
        dlclose(package->handle);
        return err;
    }
#endif /* _WIN32 */

    return 0;
}

void
gnuitar_package_init(struct GnuitarPackage *package)
{
    package->handle = NULL;
    package->name = NULL;
    package->effects = NULL;
    package->effects_count = 0;
    package->drivers = NULL;
    package->drivers_count = 0;
}

/** Closes a package.
 * @param package A package created by @ref gnuitar_package_open
 * @ingroup gnuitar-package
 */

void
gnuitar_package_done(struct GnuitarPackage *package)
{
    if (package->handle != NULL) {
#ifdef _WIN32
        FreeLibrary(package->handle);
#else /* _WIN32 */
        dlclose(package->handle);
#endif /* _WIN32 */
    }
    free(package->effects);
    free(package->drivers);
    free(package->name);

    gnuitar_package_init(package);
}

int
gnuitar_package_add_effect(struct GnuitarPackage *package, const struct GnuitarPackageEffect *package_effect)
{
    struct GnuitarPackageEffect *tmp;
    size_t tmp_size;

    tmp_size = sizeof(*tmp) * (package->effects_count + 1);

    tmp = realloc(package->effects, tmp_size);
    if (tmp == NULL)
        return ENOMEM;

    package->effects = tmp;
    package->effects[package->effects_count] = *package_effect;
    package->effects_count++;

    return 0;
}

int
gnuitar_package_init_effect(struct GnuitarPackage *package, const char *name, struct GnuitarEffect *effect)
{
    int err;
    size_t index;

    err = gnuitar_package_find_effect(package, name, &index);
    if (err)
        return err;

    err = package->effects[index].init(effect);
    if (err != 0)
        return err;

    return 0;
}

/** Locates the index of an effect.
 * @param package The package to search the effect for.
 * @param name The name of the effect
 * @param index The index of the effect.
 *  If the effect is not found, this parameter is not accessed.
 *  This parameter may be NULL.
 * @returns If the effect is found, zero is returned.
 *  If the effect is not found, ENOENT is returned.
 * @ingroup gnuitar-package
 */

int
gnuitar_package_find_effect(const struct GnuitarPackage *package, const char *name, size_t *index)
{
    size_t i;

    for (i = 0; i < package->effects_count; i++) {
        if (strcmp(package->effects[i].name, name) == 0)
            break;
    }

    if (i >= package->effects_count)
        return ENOENT;

    if (index != NULL)
        *index = i;

    return 0;
}

/** Gets the number of effects in the package.
 * @param package A package created by @gnuitar_package_open
 * @returns The number of effects in the package.
 * @ingroup gnuitar-package
 */

size_t
gnuitar_package_get_effect_count(const struct GnuitarPackage *package)
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
gnuitar_package_get_effect_name(const struct GnuitarPackage *package, size_t index)
{
    if (index >= package->effects_count) {
        return NULL;
    }
    return package->effects[index].name;
}

const char *
gnuitar_package_get_name(const struct GnuitarPackage *package)
{
    return package->name;
}

int
gnuitar_package_set_name(struct GnuitarPackage *package, const char *name)
{
    char *tmp;

    tmp = gnuitar_strdup(name);
    if (tmp == NULL)
        return ENOMEM;

    if (package->name != NULL)
        free(package->name);

    package->name = tmp;

    return 0;
}

