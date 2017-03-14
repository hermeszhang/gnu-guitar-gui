#ifndef GNUITAR_SHELL_H
#define GNUITAR_SHELL_H

#include <cstdio>
#include <string>

#include <libgnuitar/driver-manager.h>

namespace Gnuitar {

class Shell final {
    /** standard shell input */
    FILE *input;
    /** standard shell output */
    FILE *output;
    /** standard shell error log */
    FILE *error;
    /** the shell's driver manager */
    DriverManager driver_manager;
public:
    Shell(void) noexcept;
    ~Shell(void);
    int loop(void) noexcept;
    int add_effect(void) noexcept;
    void help(void) noexcept;
    void list_effects(void) noexcept;
    int readline(std::string& line) noexcept;
    int prompt(const std::string& field_name, std::string& field) noexcept;
    int start(void) noexcept;
    int stop(void) noexcept;
protected:
    void log_error(const std::string& error);
    void log_info(const std::string& info);
}; /* class Shell */

} /* namespace Gnuitar */

#endif /* GNUITAR_SHELL_H */

