#ifndef GNUITAR_PIPE_H
#define GNUITAR_PIPE_H

/** @defgroup libgnuitar-pipe Pipe and Communications
 * The API of the pipe that is used to communicate across server and client.
 */

/** The type of request.
 * @ingroup libgnuitar-pipe
 */

enum GnuitarRequestType {
    /** Starts the audio driver */
    GNUITAR_REQUEST_START,
    /** Stops the audio driver */
    GNUITAR_REQUEST_STOP,
    /** Enables recording */
    GNUITAR_REQUEST_RECORD,
    /** Disables recording */
    GNUITAR_REQUEST_RECORD_OFF,
    /** Quits the server */
    GNUITAR_REQUEST_QUIT
};

/** A request to the server to do something.
 * @ingroup gnuitar-pipe
 */

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

/** The type of response from the server.
 * @ingroup libgnuitar-pipe
 */

enum GnuitarResponseType {
    /** The server handled the request without error */
    GNUITAR_RESPONSE_GOOD,
    /** The server could not properly handle the request */
    GNUITAR_RESPONSE_BAD
};

/** The response from the server of the client's last request.
 * @ingroup libgnuitar-pipe
 */

struct GnuitarResponse {
    /** The type of response */
    enum GnuitarResponseType type;
};

/** The type of pipe.
 * It may either be a server type or a client type pipe.
 * @ingroup libgnuitar-pipe
 */

enum GnuitarPipeType {
    /** The pipe will be used by a server */
    GNUITAR_PIPE_SERVER,
    /** The pipe will be used by a client */
    GNUITAR_PIPE_CLIENT
};

/** A communication pipe for a server and client communication channel.
 * This pipe may be either used by the server or by a client.
 * @ingroup libgnuitar-pipe
 */

struct GnuitarPipe {
    /** The type of pipe */
    enum GnuitarPipeType type;
    /** The implementation specific data */
    void *data;
    /** Release resources allocated by the pipe implementation */
    void (*done)(void *data);
    /** Sends a request to the server */
    int (*send_request)(void *data, const struct GnuitarRequest *request);
    /** Sends a response to the client */
    int (*send_response)(void *data, const struct GnuitarResponse *response);
    /** Receives a request from the client */
    int (*recv_request)(void *data, struct GnuitarRequest *request);
    /** Recieves a response from the server */
    int (*recv_response)(void *data, struct GnuitarResponse *response);
};

int gnuitar_pipe_init(struct GnuitarPipe *pipe, enum GnuitarPipeType type);

void gnuitar_pipe_done(struct GnuitarPipe *pipe);

int gnuitar_pipe_send_request(struct GnuitarPipe *pipe, const struct GnuitarRequest *request);

int gnuitar_pipe_send_response(struct GnuitarPipe *pipe, const struct GnuitarResponse *response);

int gnuitar_pipe_recv_request(struct GnuitarPipe *pipe, struct GnuitarRequest *request);

int gnuitar_pipe_recv_response(struct GnuitarPipe *pipe, struct GnuitarResponse *response);

#endif /* GNUITAR_PIPE_H */

