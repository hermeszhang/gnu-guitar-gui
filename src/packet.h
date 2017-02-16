#ifndef GNUITAR_PACKET_H
#define GNUITAR_PACKET_H

#include <stdint.h>

/* get rid of __restrict__ type qualifier for MS Visual C */

#ifdef _MSC_VER
#define __restrict__
#endif

#define MAX_SAMPLE (32767 << 8)

#define GNUITAR_SAMPLE_MAX (INT16_MAX * (1 << 8))

#define GNUITAR_SAMPLE_MIN (INT16_MIN * (1 << 8))

extern const float gnuitar_sample_max;

extern const float gnuitar_sample_min;

struct GnuitarPacket {
    float * __restrict__ data;
    float * __restrict__ data_swap;
    unsigned int len;
    unsigned int channels;
    unsigned long int rate;
};

void gnuitar_packet_mul(struct GnuitarPacket *packet, float n);

void gnuitar_packet_div(struct GnuitarPacket *packet, float n);

#endif /* GNUITAR_PACKET_H */

