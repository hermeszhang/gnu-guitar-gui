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

extern const double gnuitar_sample_max;

extern const double gnuitar_sample_min;

struct GnuitarPacket {
    double * __restrict__ data;
    double * __restrict__ data_swap;
    unsigned int len;
    unsigned int channels;
    unsigned long int rate;
};

void gnuitar_packet_mul(struct GnuitarPacket *packet, double n);

void gnuitar_packet_div(struct GnuitarPacket *packet, double n);

#endif /* GNUITAR_PACKET_H */

