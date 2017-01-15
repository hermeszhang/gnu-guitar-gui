#ifndef GNUITAR_SESSION_H
#define GNUITAR_SESSION_H

#include "package.h"
#include "pump.h"

typedef struct gnuitar_session {
    /** Reference count of the session */
    unsigned int ref_count;
    /** The audio driver in use */
    gnuitar_audio_driver_t *driver;
    /** The effects pump */
    gnuitar_pump_t *pump;
    /** The packages opened by the session */
    gnuitar_package_t **packages;
    /** The number of packages opened by the session */
    unsigned int packages_count;
} gnuitar_session_t;

extern gnuitar_session_t * global_session;

gnuitar_session_t * gnuitar_session_create(void);

void gnuitar_session_incref(gnuitar_session_t *session);

void gnuitar_session_decref(gnuitar_session_t *session);

int gnuitar_session_open_package(gnuitar_session_t *session, const char *name);

int gnuitar_session_insert_effect(gnuitar_session_t *session, const char *name, unsigned int index);

int gnuitar_session_remove_effect(gnuitar_session_t *session, unsigned int index);

int gnuitar_session_move_effect(gnuitar_session_t *session, unsigned int src, unsigned int dst);

int gnuitar_session_set_driver(gnuitar_session_t *session, const char *name);

#endif /* GNUITAR_SESSION_H */

