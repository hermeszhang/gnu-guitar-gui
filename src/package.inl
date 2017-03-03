#ifndef GNUITAR_PACKAGE_INL
#define GNUITAR_PACKAGE_INL

#include "package.h"

#include <string>

namespace Gnuitar {

class Package {
    GnuitarPackage *package;
public:
    Package(GnuitarPackage *package_) noexcept
    {
        package = package_;
    }
    ~Package(void)
    {

    }
    int add_effect(const struct GnuitarPackageEffect *effect) noexcept
    {
        return gnuitar_package_add_effect(package, effect);
    }
    const char * get_name(void) noexcept
    {
        return gnuitar_package_get_name(package);
    }
    int set_name(const char *name) noexcept
    {
        return gnuitar_package_set_name(package, name);
    }
    int set_name(const std::string& name) noexcept
    {
        return set_name(name.c_str());
    }
}; /* class Package */

} /* namespace Gnuitar */

#endif /* GNUITAR_PACKAGE_INL */

