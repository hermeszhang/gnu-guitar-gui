#ifndef GNUITAR_DRIVER_INL
#define GNUITAR_DRIVER_INL

#include "driver.h"

namespace Gnuitar {

class Driver {
    GnuitarDriver *driver;
public:
    inline Driver(GnuitarDriver *driver_ptr) noexcept
    {
        driver = driver_ptr;
    }
    inline ~Driver(void)
    {

    }
    inline int start(GnuitarChain *chain) noexcept
    {
        return gnuitar_driver_start(driver, chain);
    }
    inline int stop(void) noexcept
    {
        return gnuitar_driver_stop(driver);
    }
    inline int get_map(GnuitarMap *map) const noexcept
    {
        return gnuitar_driver_get_map(driver, map);
    }
    inline int set_map(const GnuitarMap *map) noexcept
    {
        return gnuitar_driver_set_map(driver, map);
    }
}; /* class Driver */

} /* namespace Gnuitar */

#endif /* GNUITAR_DRIVER_INL */

