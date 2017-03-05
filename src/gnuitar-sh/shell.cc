#include "shell.h"

#include "string.h"

#ifndef GNUITAR_PROMPT
#define GNUITAR_PROMPT "gnuitar-sh: "
#endif

#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

namespace Gnuitar {

Shell::Shell(void) noexcept
{
    this->input = stdin;
    this->output = stdout;
    this->error = stderr;

    gnuitar_track_init(&track);

    gnuitar_package_manager_init(&package_manager);
    auto packages_dir = std::getenv("GNUITAR_PACKAGES_PATH");
    if (packages_dir != NULL)
        gnuitar_package_manager_set_packages_dir(&package_manager, packages_dir);
    gnuitar_package_manager_refresh_packages(&package_manager);
}

Shell::~Shell(void)
{
    gnuitar_track_done(&this->track);
    gnuitar_package_manager_done(&package_manager);
}

int
Shell::add_effect(void) noexcept
{
/*
    struct GnuitarEffect effect;
    struct GnuitarMap effect_map;

    fprintf(this->output, "effect name: ");

    std::string effect_name;
    if (readline(effect_name) != 0) {
        log_error("failed to read effect name");
        return -1;
    }

    if (gnuitar_package_init_effect(&this->package, effect_name.c_str(), &effect) != 0) {
        log_error("failed to initialize effect");
        return -2;
    }

    gnuitar_map_init(&effect_map);

    if (gnuitar_effect_get_map(&effect, &effect_map) == 0){
        Shell::prompt_map(&effect_map);
        if (gnuitar_effect_set_map(&effect, &effect_map) != 0){
            log_error("failed to set effect parameters");
            gnuitar_map_done(&effect_map);
            return -3;
        }
    }

    gnuitar_map_done(&effect_map);

    if (gnuitar_track_add_effect(&this->track, &effect) != 0) {
        log_error("failed to add effect to effects chain");
        gnuitar_effect_done(&effect);
        return -3;
    }
*/
    return 0;
}

void
Shell::help(void) noexcept
{
    fprintf(output, "commands:\n");
    fprintf(output, " add-effect\n");
    fprintf(output, " help\n");
    fprintf(output, " list-effects\n");
    fprintf(output, " list-drivers\n");
    fprintf(output, " list-packages\n");
    fprintf(output, " quit\n");
    fprintf(output, " exit (same as quit)\n");
}

void
Shell::list_drivers(void) noexcept
{
    auto count = gnuitar_package_manager_get_count(&package_manager);
    for (decltype(count) i = 0; i < count; i++) {
        auto package = gnuitar_package_manager_get(&package_manager, i);
        if (package == nullptr)
            continue;
        list_drivers(package);
    }
}

void
Shell::list_drivers(GnuitarPackage *package_ptr) noexcept
{
    Gnuitar::Package package(package_ptr);

    auto package_name = package.get_name();
    fprintf(output, "drivers from %s\n", package_name);

    auto count = package.get_driver_count();
    for (decltype(count) i = 0; i < count; i++) {
        auto name = package.get_driver_name(i);
        if (name == nullptr)
            continue;
        fprintf(output, " - %s\n", name);
    }
}

void
Shell::list_effects(void) noexcept
{
    auto count = gnuitar_package_manager_get_count(&package_manager);
    for (decltype(count) i = 0; i < count; i++) {
        auto package = gnuitar_package_manager_get(&package_manager, i);
        if (package == nullptr)
            continue;
        list_effects(package);
    }
}

void
Shell::list_effects(GnuitarPackage *package_) noexcept
{
    Package package(package_);

    fprintf(output, "effects from %s:\n", package.get_name());
    auto count = package.get_effect_count();
    for (decltype(count) i = 0; i < count; i++) {
        auto name = package.get_effect_name(i);
        if (name == nullptr)
            continue;
        fprintf(output, " - %s\n", name);
    }
}

void
Shell::list_packages(void) noexcept
{

    fprintf(output, "known packages:\n");

    gnuitar_package_manager_refresh_packages(&package_manager);

    auto count = gnuitar_package_manager_get_count(&package_manager);
    for (decltype(count) i = 0; i < count; i++){
        auto package = gnuitar_package_manager_get(&package_manager, i);
        if (package == NULL)
            continue;
        auto name = gnuitar_package_get_name(package);
        if (name == NULL)
            continue;
        fprintf(output, " - %s\n", name);
    }
}

int
Shell::loop(void) noexcept
{
    for (;;) {

        fprintf(this->output, "%s", GNUITAR_PROMPT);

        std::string cmd;
        auto err = readline(cmd);
        if (err != 0) {
            return err;
        }

        if (cmd == "add-effect") {
            add_effect();
        } else if (cmd == "help") {
            help();
        } else if (cmd == "list-drivers") {
            list_drivers();
        } else if (cmd == "list-effects") {
            list_effects();
        } else if (cmd == "list-packages") {
            list_packages();
        } else if (cmd == "set-driver") {
            set_driver();
        } else if (cmd == "start") {
            start();
        } else if (cmd == "stop") {
            stop();
        } else if ((cmd == "quit") || (cmd == "exit")) {
            break;
        } else {
            log_error("unknown command '" + cmd + "'");
        }
    }

    return 0;
}

int
Shell::readline(std::string& line) noexcept
{
    int c;

    for (;;) {
        c = fgetc(this->input);
        if (c == EOF) {
            return -1;
        } else if (c == '\n') {
            break;
        }
        line.push_back(c);
    }

    return 0;
}

int
Shell::prompt(const std::string& field_name, std::string& field) noexcept
{
    fprintf(output, "%s: ", field_name.c_str());
    return readline(field);
}

int
Shell::set_driver(void) noexcept
{
    std::string package_name;
    if (prompt("package name", package_name) != 0)
        return -1;

    auto package = gnuitar_package_manager_find(&package_manager, package_name.c_str());
    if (package == NULL)
        return -2;

    return set_driver(package);
}

int Shell::set_driver(GnuitarPackage *package_ptr) noexcept
{
    std::string driver_name;
    if (prompt("driver name", driver_name) != 0)
        return -1;

    GnuitarDriver driver;

    Gnuitar::Package package(package_ptr);
    package.init_driver(driver_name.c_str(), &driver);

    GnuitarMap driver_map;
    gnuitar_map_init(&driver_map);
    if (gnuitar_driver_get_map(&driver, &driver_map) == 0) {
        prompt_map(&driver_map);
        gnuitar_driver_set_map(&driver, &driver_map);
    }
    gnuitar_map_done(&driver_map);

    gnuitar_track_set_driver(&track, &driver);

    return 0;
}

int
Shell::start(void) noexcept
{
    return gnuitar_track_start(&track);
}

int
Shell::stop(void) noexcept
{
    return gnuitar_track_stop(&track);
}

void
Shell::prompt_map(struct GnuitarMap *map) noexcept
{
    size_t i;
    size_t count;
    const char *entry_name;

    count = gnuitar_map_get_count(map);
    for (i = 0; i < count; i++){
        entry_name = gnuitar_map_get_name(map, i);
        if (entry_name == NULL)
            continue;
        prompt_map_entry(map, entry_name);
    }
}

void
Shell::prompt_map_entry(struct GnuitarMap *map, const char *entry_name) noexcept
{
    struct GnuitarMapEntry *map_entry;

    map_entry = gnuitar_map_find(map, entry_name);
    if (map_entry == NULL)
        return;

    fprintf(this->output, "%s: ", entry_name);

    std::string map_value;
    if (Shell::readline(map_value) != 0) {
        return;
    }

    gnuitar_map_set_as_string(map, entry_name, map_value.c_str());
}

void
Shell::log_error(const std::string& error)
{
#ifdef _WIN32
    auto console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console_handle != NULL)
        SetConsoleTextAttribute(console_handle, FOREGROUND_RED);
#endif /* _WIN32 */
    fprintf(this->error, "error");
#ifdef _WIN32
    if (console_handle != NULL)
        SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#endif /* _WIN32 */
    fprintf(this->error, ": %s\n", error.c_str());
}

} /* namespace Gnuitar */

