#ifndef GNUITAR_PIPE_H
#define GNUITAR_PIPE_H

enum GnuitarRequestType {
    GNUITAR_REQUEST_START,
    GNUITAR_REQUEST_STOP,
    GNUITAR_REQUEST_RECORD,
    GNUITAR_REQUEST_RECORD_OFF,
    /** Quits the server */
    GNUITAR_REQUEST_QUIT
};

struct GnuitarRequest {
    /** The type of the request */
    enum GnuitarRequestType type;
    /** Contains data specific to the request */
    union {
        /** Data need to activate recording */
        struct {
            /** The file name to write the audio to*/
            char *filename;
        } record_data;
    } u;
};

enum GnuitarResponseType {
    GNUITAR_RESPONSE_GOOD,
    GNUITAR_RESPONSE_BAD
};

struct GnuitarResponse {
    enum GnuitarResponseType type;
};

enum GnuitarPipeType {
    GNUITAR_PIPE_SERVER,
    GNUITAR_PIPE_CLIENT
};

struct GnuitarPipe {
    enum GnuitarPipeType type;
    void *data;
    void (*done)(void *data);
    int (*send_request)(void *data, const struct GnuitarRequest *request);
    int (*send_response)(void *data, const struct GnuitarResponse *response);
    int (*recv_request)(void *data, struct GnuitarRequest *request);
    int (*recv_response)(void *data, struct GnuitarResponse *response);
};

int gnuitar_pipe_init(struct GnuitarPipe *pipe, enum GnuitarPipeType type);

void gnuitar_pipe_done(struct GnuitarPipe *pipe);

int gnuitar_pipe_send_request(struct GnuitarPipe *pipe, const struct GnuitarRequest *request);

int gnuitar_pipe_send_response(struct GnuitarPipe *pipe, const struct GnuitarResponse *response);

int gnuitar_pipe_recv_request(struct GnuitarPipe *pipe, struct GnuitarRequest *request);

int gnuitar_pipe_recv_response(struct GnuitarPipe *pipe, struct GnuitarResponse *response);

#endif /* GNUITAR_PIPE_H */

