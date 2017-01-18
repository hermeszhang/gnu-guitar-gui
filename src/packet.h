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

typedef float gnuitar_sample_t;

extern const gnuitar_sample_t gnuitar_sample_max;

extern const gnuitar_sample_t gnuitar_sample_min;

typedef struct gnuitar_packet {
    gnuitar_sample_t * __restrict__ data;
    gnuitar_sample_t * __restrict__ data_swap;
    unsigned int len;
    unsigned int channels;
    unsigned long int rate;
} gnuitar_packet_t;

void gnuitar_packet_mul(gnuitar_packet_t *packet, float n);

void gnuitar_packet_div(gnuitar_packet_t *packet, float n);

#endif /* GNUITAR_PACKET_H */

