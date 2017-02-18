#include "packet.h"

const double gnuitar_sample_max = GNUITAR_SAMPLE_MAX;

const double gnuitar_sample_min = GNUITAR_SAMPLE_MIN;

void
gnuitar_packet_mul(struct GnuitarPacket *packet, double n)
{
    unsigned int i;
    for (i = 0; i < packet->len; i++) {
        packet->data[i] *= n;
    }
}

void
gnuitar_packet_div(struct GnuitarPacket *packet, double n)
{
    unsigned int i;
    for (i = 0; i < packet->len; i++) {
        packet->data[i] /= n;
    }
}

