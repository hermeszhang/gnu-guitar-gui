#ifndef GNUITAR_SHELL_H
#define GNUITAR_SHELL_H

#include <stdio.h>

#include "../gnuitar.h"

#include "string.h"

struct GnuitarShell {
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
};

void gnuitar_shell_init(struct GnuitarShell *shell);

void gnuitar_shell_done(struct GnuitarShell *shell);

int gnuitar_shell_loop(struct GnuitarShell *shell);

int gnuitar_shell_add_effect(struct GnuitarShell *shell);

void gnuitar_shell_help(struct GnuitarShell *shell);

void gnuitar_shell_list_effects(struct GnuitarShell *shell);

int gnuitar_shell_open_package(struct GnuitarShell *shell);

int gnuitar_shell_readline(struct GnuitarShell *shell, struct GnuitarString *line);

#endif /* GNUITAR_SHELL */

