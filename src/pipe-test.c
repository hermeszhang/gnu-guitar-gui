#include "pipe.h"

#include <assert.h>
#include <stdlib.h>

void
test_request(void)
{
    struct GnuitarPipe server_pipe;
    struct GnuitarPipe client_pipe;

    struct GnuitarRequest server_request;
    struct GnuitarRequest client_request;

    gnuitar_pipe_init(&server_pipe, GNUITAR_PIPE_SERVER);
    gnuitar_pipe_init(&client_pipe, GNUITAR_PIPE_CLIENT);

    client_request.type = GNUITAR_REQUEST_START;
    assert(gnuitar_pipe_send_request(&client_pipe, &client_request) == 0);

    assert(gnuitar_pipe_recv_request(&server_pipe, &server_request) == 0);
    assert(server_request.type == GNUITAR_REQUEST_START);

    gnuitar_pipe_done(&server_pipe);
    gnuitar_pipe_done(&client_pipe);
}

int
main(void)
{
    test_request();
    return EXIT_SUCCESS;
}

