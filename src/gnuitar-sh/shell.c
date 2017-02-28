#include "shell.h"

#include "string.h"

#ifndef GNUITAR_PROMPT
#define GNUITAR_PROMPT "gnuitar-sh: "
#endif

void
gnuitar_shell_init(struct GnuitarShell *shell)
{
    shell->input = stdin;
    shell->output = stdout;
    shell->error = stderr;
    gnuitar_track_init(&shell->track, "ALSA");
    gnuitar_package_init(&shell->package);
}

void
gnuitar_shell_done(struct GnuitarShell *shell)
{
    gnuitar_track_done(&shell->track);
    gnuitar_package_done(&shell->package);
}

int
gnuitar_shell_add_effect(struct GnuitarShell *shell)
{
    struct GnuitarString effect_name;
    struct GnuitarEffect effect;

    gnuitar_string_init(&effect_name);

    fprintf(shell->output, "effect name: ");

    if (gnuitar_shell_readline(shell, &effect_name) != 0) {
        fprintf(shell->error, "failed to read effect name\n");
        gnuitar_string_done(&effect_name);
        return -1;
    }

    if (gnuitar_package_init_effect(&shell->package, effect_name.buf, &effect) != 0) {
        fprintf(shell->error, "failed to initialize effect\n");
        gnuitar_string_done(&effect_name);
        return -2;
    }

    gnuitar_string_done(&effect_name);

    if (gnuitar_track_add_effect(&shell->track, &effect) != 0) {
        fprintf(shell->error, "failed to add effect to effects chain\n");
        gnuitar_effect_done(&effect);
        return -3;
    }

    return 0;
}

void
gnuitar_shell_help(struct GnuitarShell *shell)
{
    fprintf(shell->output, "commands:\n");
    fprintf(shell->output, " help\n");
    fprintf(shell->output, " list-effects\n");
    fprintf(shell->output, " add-effect\n");
    fprintf(shell->output, " open-package\n");
    fprintf(shell->output, " quit\n");
    fprintf(shell->output, " exit (same as quit)\n");
}

void
gnuitar_shell_list_effects(struct GnuitarShell *shell)
{
    size_t i;
    size_t count;
    const char *name;

    fprintf(shell->output, "known effects:\n");

    count = gnuitar_package_get_effect_count(&shell->package);
    for (i = 0; i < count; i++) {
        name = gnuitar_package_get_effect_name(&shell->package, i);
        if (name == NULL)
            continue;
        fprintf(shell->output, " %s\n", name);
    }
}

int
gnuitar_shell_loop(struct GnuitarShell *shell)
{
    int err;
    struct GnuitarString cmd;

    for (;;) {

        fprintf(shell->output, "%s", GNUITAR_PROMPT);

        gnuitar_string_init(&cmd);

        err = gnuitar_shell_readline(shell, &cmd);
        if (err != 0) {
            gnuitar_string_done(&cmd);
            return err;
        }

        if (gnuitar_string_cmp_literal(&cmd, "add-effect") == 0) {
            gnuitar_shell_add_effect(shell);
        } else if (gnuitar_string_cmp_literal(&cmd, "help") == 0) {
            gnuitar_shell_help(shell);
        } else if (gnuitar_string_cmp_literal(&cmd, "list-effects") == 0) {
            gnuitar_shell_list_effects(shell);
        } else if (gnuitar_string_cmp_literal(&cmd, "open-package") == 0) {
            gnuitar_shell_open_package(shell);
        } else if ((gnuitar_string_cmp_literal(&cmd, "quit") == 0)
                || (gnuitar_string_cmp_literal(&cmd, "exit") == 0)) {
            gnuitar_string_done(&cmd);
            break;
        } else {
            fprintf(shell->output, "unknown command: %s\n", cmd.buf);
        }

        gnuitar_string_done(&cmd);
    }

    return 0;
}

int
gnuitar_shell_open_package(struct GnuitarShell *shell)
{
    struct GnuitarString package_path;

    gnuitar_string_init(&package_path);

    fprintf(shell->output, "package path: ");

    if (gnuitar_shell_readline(shell, &package_path) != 0) {
        fprintf(shell->error, "failed to read package path\n");
        gnuitar_string_done(&package_path);
        return -1;
    }

    /* free the current package */
    gnuitar_package_done(&shell->package);

    if (gnuitar_package_open(&shell->package, package_path.buf) != 0) {
        fprintf(shell->error, "failed to open package\n");
        gnuitar_string_done(&package_path);
        return -2;
    }

    gnuitar_string_done(&package_path);

    return 0;
}

int
gnuitar_shell_readline(struct GnuitarShell *shell, struct GnuitarString *string)
{
    int c;

    gnuitar_string_init(string);

    for (;;) {
        c = fgetc(shell->input);
        if (c == EOF) {
            gnuitar_string_done(string);
            return -1;
        } else if (c == '\n') {
            break;
        } else if (gnuitar_string_add_char(string, c) != 0) {
            gnuitar_string_done(string);
            return -2;
        }
    }

    return 0;
}

