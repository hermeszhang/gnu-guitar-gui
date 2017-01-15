#include "session.h"

gnuitar_session_t * global_session;

gnuitar_session_t *
gnuitar_session_create(void)
{
    gnuitar_session_t * session;
    session = malloc(sizeof(*session));
    if (session == NULL) {
        return NULL;
    }
    session->ref_count = 1;
    session->pump = NULL;
    session->driver = NULL;
    session->packages = NULL;
    session->packages_count = 0;

    /* TODO create pump */

    return session;
}

void
gnuitar_session_incref(gnuitar_session_t *session)
{
    session->ref_count++;
}

void
gnuitar_session_decref(gnuitar_session_t *session)
{
    unsigned int i;
    if ((session == NULL) || (session->ref_count == 0)){
        return;
    } else {
        session->ref_count--;
        if (session->ref_count > 0) {
            return;
        }
    }
    for (i = 0; i < session->packages_count; i++) {
        gnuitar_package_decref(session->packages[i]);
    }
    free(session->packages);
    free(session);
}

