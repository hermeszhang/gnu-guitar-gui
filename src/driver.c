#include "driver.h"

#include <errno.h>

void
gnuitar_driver_init(struct GnuitarDriver *driver)
{
    driver->data = NULL;
    driver->done = NULL;
    driver->start = NULL;
    driver->stop = NULL;
    driver->get_map = NULL;
    driver->set_map = NULL;
}

void
gnuitar_driver_done(struct GnuitarDriver *driver)
{
    if (driver->done != NULL)
        driver->done(driver->data);
    gnuitar_driver_init(driver);
}

int
gnuitar_driver_start(struct GnuitarDriver *driver, struct GnuitarChain *chain)
{
    if (driver->start != NULL)
        return driver->start(driver->data, chain);
    return EFAULT;
}

int
gnuitar_driver_stop(struct GnuitarDriver *driver)
{
    if (driver->stop != NULL)
        return driver->stop(driver);
    return EFAULT;
}

