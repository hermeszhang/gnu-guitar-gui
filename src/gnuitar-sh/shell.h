#ifndef GNUITAR_SHELL_H
#define GNUITAR_SHELL_H

#include <cstdio>
#include <string>

#include "../gnuitar.h"
#include "../package.inl"

namespace Gnuitar {

class Shell final {
    /** standard shell input */
    FILE *input;
    /** standard shell output */
    FILE *output;
    /** standard shell error log */
    FILE *error;
    /** the shell's track */
    struct GnuitarTrack track;
    /** the shell's package manager */
    struct GnuitarPackageManager package_manager;
public:
    Shell(void) noexcept;
    ~Shell(void);
    int loop(void) noexcept;
    int add_effect(void) noexcept;
    void help(void) noexcept;
    void list_drivers(void) noexcept;
    void list_drivers(GnuitarPackage *package) noexcept;
    void list_effects(void) noexcept;
    void list_effects(GnuitarPackage *package) noexcept;
    void list_packages(void) noexcept;
    int readline(std::string& line) noexcept;
    int prompt(const std::string& field_name, std::string& field) noexcept;
    int set_driver(void) noexcept;
    int set_driver(GnuitarPackage *package) noexcept;
    int start(void) noexcept;
    int stop(void) noexcept;
protected:
    void prompt_map(struct GnuitarMap *map) noexcept;
    void prompt_map_entry(struct GnuitarMap *map, const char *name) noexcept;
    void log_error(const std::string& error);
    void log_info(const std::string& info);
}; /* class Shell */

} /* namespace Gnuitar */

#endif /* GNUITAR_SHELL_H */

