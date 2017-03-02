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
    gnuitar_track_init(&this->track, "ALSA");
    gnuitar_package_init(&this->package);
}

Shell::~Shell(void)
{
    gnuitar_track_done(&this->track);
    gnuitar_package_done(&this->package);
}

int
Shell::add_effect(void) noexcept
{
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

    return 0;
}

void
Shell::help(void) noexcept
{
    fprintf(this->output, "commands:\n");
    fprintf(this->output, " add-effect\n");
    fprintf(this->output, " help\n");
    fprintf(this->output, " list-effects\n");
    fprintf(this->output, " open-package\n");
    fprintf(this->output, " quit\n");
    fprintf(this->output, " exit (same as quit)\n");
}

void
Shell::list_effects(void) noexcept
{
    size_t i;
    size_t count;
    const char *name;

    fprintf(this->output, "known effects:\n");

    count = gnuitar_package_get_effect_count(&this->package);
    for (i = 0; i < count; i++) {
        name = gnuitar_package_get_effect_name(&this->package, i);
        if (name == NULL)
            continue;
        fprintf(this->output, " %s\n", name);
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
            Shell::add_effect();
        } else if (cmd == "help") {
            Shell::help();
        } else if (cmd == "list-effects") {
            Shell::list_effects();
        } else if (cmd == "open-package") {
            Shell::open_package();
        } else if (cmd == "start") {
            Shell::start();
        } else if (cmd == "stop") {
            Shell::stop();
        } else if ((cmd == "quit") || (cmd == "exit")) {
            break;
        } else {
            log_error("unknown command '" + cmd + "'");
        }
    }

    return 0;
}

int
Shell::open_package(void) noexcept
{
    fprintf(this->output, "package path: ");

    std::string package_path;
    if (readline(package_path) != 0) {
        log_error("failed to read package path");
        return -1;
    }

    /* free the current package */
    gnuitar_package_done(&this->package);

    if (gnuitar_package_open(&this->package, package_path.c_str()) != 0) {
        log_error("failed to open package");
        return -2;
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
Shell::start(void) noexcept
{
    return gnuitar_track_start(&this->track);
}

int
Shell::stop(void) noexcept
{
    return gnuitar_track_stop(&this->track);
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

    union {
        double a1;
    } variant;

    map_entry = gnuitar_map_find(map, entry_name);
    if (map_entry == NULL)
        return;

    fprintf(this->output, "%s: ", entry_name);

    std::string map_value;
    if (Shell::readline(map_value) != 0) {
        return;
    }

    if (map_entry->type == GNUITAR_MAP_TYPE_DOUBLE) {
        if (sscanf(map_value.c_str(), "%lf", &variant.a1) == 1) {
            gnuitar_map_set(map, entry_name, &variant.a1);
        }
    }
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

