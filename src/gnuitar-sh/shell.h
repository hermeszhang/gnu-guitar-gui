#ifndef GNUITAR_SHELL_H
#define GNUITAR_SHELL_H

#include <cstdio>
#include <string>

#include "../gnuitar.h"

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
    /** the shell's package */
    struct GnuitarPackage package;
public:
    Shell(void) noexcept;
    ~Shell(void);
    int loop(void) noexcept;
    int add_effect(void) noexcept;
    void help(void) noexcept;
    void list_effects(void) noexcept;
    int open_package(void) noexcept;
    int readline(std::string& line) noexcept;
    int start(void) noexcept;
    int stop(void) noexcept;
protected:
    void prompt_map(struct GnuitarMap *map) noexcept;
    void prompt_map_entry(struct GnuitarMap *map, const char *name) noexcept;
}; /* class Shell */

} /* namespace Gnuitar */

#endif /* GNUITAR_SHELL_H */

