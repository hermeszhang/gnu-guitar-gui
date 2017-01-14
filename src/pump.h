#ifndef GNUITAR_PUMP_H
#define GNUITAR_PUMP_H

#include "audio-driver.h"
#include "effect.h"

typedef struct gnuitar_pump {
    /** number of references */
    size_t ref_count;
    /** array of effects */
    gnuitar_effect_t * effects;
    /** number of effects in the pump */
    size_t n_effects;
} gnuitar_pump_t;

void gnuitar_pump_init(gnuitar_pump_t * pump);

void gnuitar_pump_ref(gnuitar_pump_t * pump);

void gnuitar_pump_unref(gnuitar_pump_t * pump);

extern volatile unsigned short write_track;

void     pump_sample(gnuitar_packet_t *db);
void     pump_start(void);
void     pump_stop(void);
void     save_pump(const char *fname);
void     load_pump(const char *fname);
void     load_settings(void);
void     save_settings(void);
void     load_initial_state(char **argv, int argc);
char    *discover_preset_path(void);

#endif /* GNUITAR_PUMP_H */

