#include <gnuitar-sh/shell.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(void)
{
    Gnuitar::Shell shell;
    auto err = shell.loop();
    if (err != 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

