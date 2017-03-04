#include "../gnuitar.h"

#include "../package.inl"

#include "echo.h"
#include "tremolo.h"
#include "vibrato.h"

#ifdef GNUITAR_WITH_ALSA
#include "driver-alsa.h"
#endif /* GNUITAR_WITH_ALSA */

const struct GnuitarPackageEffect builtin_effects[] = {
    { "Echo", gnuitar_echo_init },
    { "Tremolo", gnuitar_tremolo_init },
    { "Vibrato", gnuitar_vibrato_init }
};

const struct GnuitarPackageDriver builtin_drivers[] = {
#ifdef GNUITAR_WITH_ALSA
    { "ALSA", gnuitar_alsa_driver_init }
#endif /* GNUITAR_WITH_ALSA */
};

int
gnuitar_package_entry(struct GnuitarPackage *package_ptr)
{
    Gnuitar::Package package(package_ptr);

    auto err = package.set_name("Builtins");
    if (err != 0)
        return err;

    auto count = sizeof(builtin_effects);
    count /= sizeof(builtin_effects[0]);
    for (decltype(count) i = 0; i < count; i++) {
        err = package.add_effect(&builtin_effects[i]);
        if (err != 0)
            return err;
    }

    count = sizeof(builtin_drivers);
    count /= sizeof(builtin_drivers[0]);
    for (decltype(count) i = 0; i < count; i++) {
        err = package.add_driver(&builtin_drivers[i]);
        if (err != 0)
            return err;
    }

    return 0;
}

