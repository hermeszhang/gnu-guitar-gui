#include "../gnuitar.h"

#include "../package.inl"

#include "echo.h"
#include "tremolo.h"
#include "vibrato.h"

const struct GnuitarPackageEffect builtin_effects[] = {
    { "Echo", gnuitar_echo_init },
    { "Tremolo", gnuitar_tremolo_init },
    { "Vibrato", gnuitar_vibrato_init }
};

int
gnuitar_package_entry(struct GnuitarPackage *package_ptr)
{
    Gnuitar::Package package(package_ptr);

    auto err = package.set_name("Builtins");
    if (err != 0)
        return err;

    auto builtin_effects_count = sizeof(builtin_effects);
    builtin_effects_count /= sizeof(builtin_effects[0]);

    for (decltype(builtin_effects_count) i = 0; i < builtin_effects_count; i++) {
        err = package.add_effect(&builtin_effects[i]);
        if (err != 0)
            return err;
    }

    return 0;
}

