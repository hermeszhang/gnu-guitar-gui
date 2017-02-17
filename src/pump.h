#ifndef GNUITAR_PUMP_H
#define GNUITAR_PUMP_H

#include "effect.h"
#include "error.h"

typedef struct gnuitar_pump {
    /** number of references */
    size_t ref_count;
    /** array of effects */
    struct GnuitarEffect **effects;
    /** number of effects in the pump */
    size_t n_effects;
} gnuitar_pump_t;

gnuitar_pump_t * gnuitar_pump_create(void);

void gnuitar_pump_incref(gnuitar_pump_t *pump);

void gnuitar_pump_decref(gnuitar_pump_t *pump);

gnuitar_error_t gnuitar_pump_add_effect(gnuitar_pump_t *pump, struct GnuitarEffect *effect);

gnuitar_error_t gnuitar_pump_erase_effect(gnuitar_pump_t *pump, unsigned int index);

gnuitar_error_t gnuitar_pump_move_effect(gnuitar_pump_t *pump, unsigned int src, unsigned int dst);

void gnuitar_pump_process(gnuitar_pump_t *pump, struct GnuitarPacket *packet);

extern volatile unsigned short write_track;

void     pump_sample(struct GnuitarPacket *db);
void     pump_start(void);
void     pump_stop(void);
void     save_pump(const char *fname);
void     load_pump(const char *fname);
void     load_settings(void);
void     save_settings(void);
void     load_initial_state(char **argv, int argc);
char    *discover_preset_path(void);

#endif /* GNUITAR_PUMP_H */

