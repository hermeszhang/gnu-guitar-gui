#ifndef GNUITAR_PACKET_H
#define GNUITAR_PACKET_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* get rid of __restrict__ type qualifier for MS Visual C */

#ifdef _MSC_VER
#define __restrict__
#endif

#define GNUITAR_SAMPLE_MAX INT32_MAX

#define GNUITAR_SAMPLE_MIN INT32_MIN

extern const int32_t gnuitar_sample_max;

extern const int32_t gnuitar_sample_min;

struct GnuitarPacket {
    int32_t * __restrict__ data;
    int32_t * __restrict__ data_swap;
    unsigned int len;
    unsigned int channels;
    unsigned long int rate;
};

void gnuitar_packet_mul(struct GnuitarPacket *packet, int32_t n);

void gnuitar_packet_div(struct GnuitarPacket *packet, int32_t n);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_PACKET_H */

