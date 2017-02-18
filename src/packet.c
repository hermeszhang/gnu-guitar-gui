#include "packet.h"

const float gnuitar_sample_max = GNUITAR_SAMPLE_MAX;

const float gnuitar_sample_min = GNUITAR_SAMPLE_MIN;

void
gnuitar_packet_mul(struct GnuitarPacket *packet, float n)
{
    unsigned int i;
    for (i = 0; i < packet->len; i++) {
        packet->data[i] *= n;
    }
}

void
gnuitar_packet_div(struct GnuitarPacket *packet, float n)
{
    unsigned int i;
    for (i = 0; i < packet->len; i++) {
        packet->data[i] /= n;
    }
}

