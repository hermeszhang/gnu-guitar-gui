#include "package-manager.h"

#ifdef _MSC_VER
#define gnuitar_strdup _strdup
#else /* _MSC_VER */
#define gnuitar_strdup strdup
#endif /* _MSC_VER */

#ifndef GNUITAR_PLUGINS_PATH
#define GNUITAR_PLUGINS_PATH "/usr/local/lib/gnuitar"
#endif /* GNUITAR_PLUGINS_PATH */

#ifdef _WIN32
#else /* _WIN32 */
#include <glob.h>
#endif /* _WIN32 */
#ifdef __unix__
#endif /* __unix__ */

#include <errno.h>

static int
pkgcmp(const void *name_ptr, const void *package_ptr)
{
    const char *name;
    const struct GnuitarPackage *package;

    name = (const char *)(name_ptr);
    package = (const struct GnuitarPackage *)(package_ptr);

    if (name == NULL)
        return -1;
    if (package->name == NULL)
        return 1;

    return strcmp(name, package->name);
}

void
gnuitar_package_manager_init(struct GnuitarPackageManager *manager)
{
    manager->packages_dir = NULL;
    manager->packages = NULL;
    manager->packages_count = 0;
}

void
gnuitar_package_manager_done(struct GnuitarPackageManager *manager)
{
    size_t i;
    if (manager == NULL)
        return;
    for (i = 0; i < manager->packages_count; i++){
        gnuitar_package_done(&manager->packages[i]);
    }
}

struct GnuitarPackage *
gnuitar_package_manager_find(const struct GnuitarPackageManager *manager, const char *name)
{
    return bsearch(name,
                   manager->packages,
                   manager->packages_count,
                   sizeof(manager->packages[0]),
                   pkgcmp);
}

struct GnuitarPackage *
gnuitar_package_manager_get(const struct GnuitarPackageManager *manager, size_t index)
{
    if (index >= manager->packages_count)
        return NULL;
    return &manager->packages[index];
}

size_t
gnuitar_package_manager_get_count(const struct GnuitarPackageManager *manager)
{
    return manager->packages_count;
}

int
gnuitar_package_manager_open_package(struct GnuitarPackageManager *manager, const char *package_path)
{
    struct GnuitarPackage *tmp;
    size_t tmp_size;
    struct GnuitarPackage package;

    tmp_size = sizeof(manager->packages[0]);
    tmp_size *= manager->packages_count + 1;

    tmp = realloc(manager->packages, tmp_size);
    if (tmp == NULL)
        return ENOMEM;
    manager->packages = tmp;

    gnuitar_package_init(&package);

    if (gnuitar_package_open(&package, package_path) != 0){
        gnuitar_package_done(&package);
        return ENOENT;
    }

    manager->packages[manager->packages_count] = package;
    manager->packages_count++;

    return 0;
}

int
gnuitar_package_manager_refresh_packages(struct GnuitarPackageManager *manager)
{
#ifdef _WIN32
    (void) manager;
#else /* _WIN32 */

    const char *search_dir;
    char *pattern;
    size_t pattern_size;
    size_t i;
    glob_t libs;

    if (manager->packages_dir != NULL)
        search_dir = manager->packages_dir;
    else
        search_dir = GNUITAR_PLUGINS_PATH;

    pattern_size = sizeof("/*.so");
    pattern_size += strlen(search_dir);

    pattern = malloc(pattern_size + 1);
    if (pattern == NULL)
        return ENOMEM;

    strcpy(pattern, search_dir);
    strcat(pattern, "/*.so");

    if (glob(pattern, 0, NULL, &libs) != 0) {
        free(pattern);
        return EINVAL;
    }

    free(pattern);

    for (i = 0; i < libs.gl_pathc; i++) {
        gnuitar_package_manager_open_package(manager, libs.gl_pathv[i]);
    }

    globfree(&libs);

#endif /* _WIN32 */
    return 0;
}

int
gnuitar_package_manager_set_packages_dir(struct GnuitarPackageManager *manager, const char *packages_dir)
{
    char *tmp;
    if (packages_dir == NULL)
        return EFAULT;
    tmp = gnuitar_strdup(packages_dir);
    if (tmp == NULL)
        return ENOMEM;
    free(manager->packages_dir);
    manager->packages_dir = tmp;
    return 0;
}

