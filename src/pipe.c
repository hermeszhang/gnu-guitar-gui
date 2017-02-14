#include "pipe.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct GnuitarPipeData {
    FILE * in;
    FILE * out;
};

static void
done(void *data_ptr)
{
    free(data_ptr);
}

static int
send_request(void *data_ptr, const struct GnuitarRequest *request)
{
    struct GnuitarPipeData * data;

    data = (struct GnuitarPipeData *)(data_ptr);
    if (data == NULL)
        return EINVAL;

    if (request == NULL)
        return EINVAL;

    switch (request->type){
    case GNUITAR_REQUEST_START:
        fprintf(data->out, "a\n");
        break;
    case GNUITAR_REQUEST_STOP:
        fprintf(data->out, "b\n");
        break;
    default:
        return EPROTO;
    }

    return 0;
}

static int
recv_request(void *data_ptr, struct GnuitarRequest *request)
{
    struct GnuitarPipeData * data;
    int c;

    data = (struct GnuitarPipeData *)(data_ptr);
    if (data == NULL)
        return EINVAL;

    if (request == NULL)
        return EINVAL;

    c = fgetc(data->in);
    switch (c) {
    case 'a':
        request->type = GNUITAR_REQUEST_START;
        break;
    case 'b':
        request->type = GNUITAR_REQUEST_STOP;
	break;
    default:
        return EPROTO;
    }

    if (fgetc(data->in) != '\n')
        return EPROTO;

    return 0;
}

static int
recv_response(void *data_ptr, struct GnuitarResponse *response)
{
    char c;
    struct GnuitarPipeData * data;

    data = (struct GnuitarPipeData*)(data_ptr);
    if (data == NULL)
        return EINVAL;

    if (response == NULL)
        return EINVAL;

    c = fgetc(data->in);

    switch (c) {
    case EOF:
        return EPIPE;
    case 'b':
        response->type = GNUITAR_RESPONSE_BAD;
        return 0;
    case 'g':
        response->type = GNUITAR_RESPONSE_GOOD;
        return 0;
    default:
        break;
    }

    return EPROTO;
}

/**
 * @returns 0 on success.
 *  If type is not a valid pipe type, EINVAL is returned.
 *  If a memory allocation fails, ENOMEM is returned.
 */

int
gnuitar_pipe_init(struct GnuitarPipe *pipe, enum GnuitarPipeType type)
{
    struct GnuitarPipeData * data;

    data = malloc(sizeof(*data));
    if (data == NULL)
        return ENOMEM;

    pipe->data = data;
    pipe->done = done;

    data->in = stdin;
    data->out = stdout;

    switch (type){
    case GNUITAR_PIPE_CLIENT:
        pipe->send_request = send_request;
        pipe->send_response = NULL;
        pipe->recv_request = NULL;
        pipe->recv_response = recv_response;
        break;
    case GNUITAR_PIPE_SERVER:
        pipe->send_request = NULL;
        pipe->send_response = NULL;
        pipe->recv_request = recv_request;
        pipe->recv_response = NULL;
        break;
    default:
        return EINVAL;
    }

    return 0;
}

void
gnuitar_pipe_done(struct GnuitarPipe *pipe)
{
    if (pipe->data == NULL) {
        /* TODO : write a debug warning here about double free */
        return;
    }

    if (pipe->done == NULL) {
        /* TODO : write a debug warning here about a NULL done function */
        return;
    }

    pipe->done(pipe->data);

    pipe->data = NULL;
}

int
gnuitar_pipe_send_request(struct GnuitarPipe *pipe, const struct GnuitarRequest *request)
{
    if (pipe->data == NULL) {
        /* TODO write debug warning here */ 
        return EINVAL;
    } else if (pipe->send_request == NULL) {
        /* TODO write debug warning here */
        return EINVAL;
    }

    return pipe->send_request(pipe->data, request);
}

int
gnuitar_pipe_recv_request(struct GnuitarPipe *pipe, struct GnuitarRequest *request)
{
    if (pipe->data == NULL) {
        /* TODO write debug warning here */ 
        return EINVAL;
    } else if (pipe->recv_request == NULL) {
        /* TODO write debug warning here */
        return EINVAL;
    }

    return pipe->recv_request(pipe->data, request);
}

