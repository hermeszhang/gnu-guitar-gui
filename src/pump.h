#ifndef GNUITAR_PUMP_H
#define GNUITAR_PUMP_H

#include "effect.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct GnuitarPump {
    /** number of references */
    size_t ref_count;
    /** array of effects */
    struct GnuitarEffect **effects;
    /** number of effects in the pump */
    size_t n_effects;
};

struct GnuitarPump * gnuitar_pump_create(void);

void gnuitar_pump_incref(struct GnuitarPump *pump);

void gnuitar_pump_decref(struct GnuitarPump *pump);

gnuitar_error_t gnuitar_pump_add_effect(struct GnuitarPump *pump, struct GnuitarEffect *effect);

gnuitar_error_t gnuitar_pump_erase_effect(struct GnuitarPump *pump, unsigned int index);

gnuitar_error_t gnuitar_pump_move_effect(struct GnuitarPump *pump, unsigned int src, unsigned int dst);

void gnuitar_pump_process(struct GnuitarPump *pump, struct GnuitarPacket *packet);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_PUMP_H */

