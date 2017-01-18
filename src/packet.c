#include "packet.h"

const gnuitar_sample_t gnuitar_sample_max = GNUITAR_SAMPLE_MAX;

const gnuitar_sample_t gnuitar_sample_min = GNUITAR_SAMPLE_MIN;

void
gnuitar_packet_mul(gnuitar_packet_t *packet, float n)
{
    unsigned int i;
    for (i = 0; i < packet->len; i++) {
        packet->data[i] *= n;
    }
}

void
gnuitar_packet_div(gnuitar_packet_t *packet, float n)
{
    unsigned int i;
    for (i = 0; i < packet->len; i++) {
        packet->data[i] /= n;
    }
}

