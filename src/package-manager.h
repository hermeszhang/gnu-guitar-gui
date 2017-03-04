#ifndef GNUITAR_PACKAGE_MANAGER_H
#define GNUITAR_PACKAGE_MANAGER_H

#include "package.h"

/* for size_t */
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct GnuitarPackageManager {
    char *packages_dir;
    struct GnuitarPackage *packages;
    size_t packages_count;
};

void gnuitar_package_manager_init(struct GnuitarPackageManager *manager);

void gnuitar_package_manager_done(struct GnuitarPackageManager *manager);

struct GnuitarPackage * gnuitar_package_manager_find(const struct GnuitarPackageManager *manager, const char *name);

struct GnuitarPackage * gnuitar_package_manager_get(const struct GnuitarPackageManager *manager, size_t index);

size_t gnuitar_package_manager_get_count(const struct GnuitarPackageManager *manager);

int gnuitar_package_manager_open_package(struct GnuitarPackageManager *manager, const char * package_path);

int gnuitar_package_manager_refresh_packages(struct GnuitarPackageManager *manager);

int gnuitar_package_manager_set_packages_dir(struct GnuitarPackageManager *manager, const char *packages_dir);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_PACKAGE_MANAGER_H */

