#include "../gnuitar.h"

#include "shell.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(void)
{
    int err;
    struct GnuitarShell shell;

    gnuitar_shell_init(&shell);

    err = gnuitar_shell_loop(&shell);

    gnuitar_shell_done(&shell);

    if (err != 0)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

