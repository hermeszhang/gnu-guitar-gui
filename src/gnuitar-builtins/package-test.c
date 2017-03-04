#include "../gnuitar.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <dlfcn.h>

int
main(void)
{
    struct GnuitarPackage package;
    int err;
    size_t i;
    size_t count;
    const char * effect_name;

    err = gnuitar_package_open(&package, "./" GNUITAR_BUILTINS_PACKAGE);
    if (err != 0)
        printf("dlerror: %s\n", dlerror());
    assert(err == 0);

    printf("builtin effects:\n");

    count = gnuitar_package_get_effect_count(&package);
    for (i = 0; i < count; i++) {
        effect_name = gnuitar_package_get_effect_name(&package, i);
        if (effect_name == NULL)
            continue;
        printf("%s\n", effect_name);
    }
    gnuitar_package_done(&package);

    return EXIT_SUCCESS;
}

