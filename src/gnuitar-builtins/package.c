#include "../gnuitar.h"

#include "echo.h"
#include "tremolo.h"
#include "vibrato.h"

const struct GnuitarPackageEffect builtin_effects[] = {
    { "Echo", gnuitar_echo_init },
    { "Tremolo", gnuitar_tremolo_init },
    { "Vibrato", gnuitar_vibrato_init }
};

int
gnuitar_package_entry(struct GnuitarPackage *package)
{
    size_t i;
    size_t builtin_effects_count;
    int err;

    builtin_effects_count = sizeof(builtin_effects);
    builtin_effects_count /= sizeof(builtin_effects[0]);

    for (i = 0; i < builtin_effects_count; i++) {
        err = gnuitar_package_add_effect(package, &builtin_effects[i]);
        if (err != 0)
            return err;
    }

    return 0;
}

