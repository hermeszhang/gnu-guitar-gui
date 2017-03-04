#ifndef GNUITAR_PACKAGE_INL
#define GNUITAR_PACKAGE_INL

#include "package.h"

#include <string>

namespace Gnuitar {

class Package {
    GnuitarPackage *package;
public:
    inline Package(GnuitarPackage *package_) noexcept
    {
        package = package_;
    }
    inline ~Package(void)
    {

    }
    inline int add_effect(const GnuitarPackageEffect *effect) noexcept
    {
        return gnuitar_package_add_effect(package, effect);
    }
    inline int add_driver(const GnuitarPackageDriver *driver) noexcept
    {
        return gnuitar_package_add_driver(package, driver);
    }
    inline size_t get_effect_count(void) const noexcept
    {
        return gnuitar_package_get_effect_count(package);
    }
    inline size_t get_driver_count(void) const noexcept
    {
        return gnuitar_package_get_driver_count(package);
    }
    inline const char * get_driver_name(size_t index) const noexcept
    {
        return gnuitar_package_get_driver_name(package, index);
    }
    inline const char * get_effect_name(size_t index) const noexcept
    {
        return gnuitar_package_get_effect_name(package, index);
    }
    inline const char * get_name(void) noexcept
    {
        return gnuitar_package_get_name(package);
    }
    inline int set_name(const char *name) noexcept
    {
        return gnuitar_package_set_name(package, name);
    }
    inline int set_name(const std::string& name) noexcept
    {
        return set_name(name.c_str());
    }
}; /* class Package */

} /* namespace Gnuitar */

#endif /* GNUITAR_PACKAGE_INL */

