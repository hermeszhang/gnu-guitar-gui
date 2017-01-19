#include "effect.h"

void
gnuitar_effect_incref(gnuitar_effect_t *effect)
{
    effect->ref_count++;
}

void
gnuitar_effect_decref(gnuitar_effect_t *effect)
{
    if ((effect == NULL) || (effect->ref_count == 0))
        return;
    effect->ref_count--;
    if (effect->ref_count > 0)
        return;
    if (effect->proc_done)
        effect->proc_done(effect);
}

void
gnuitar_effect_process(gnuitar_effect_t *effect, gnuitar_packet_t *packet)
{
    if (effect->toggle)
        effect->proc_filter(effect, packet);
}

