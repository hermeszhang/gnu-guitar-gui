#ifndef GNUITAR_PACKET_H
#define GNUITAR_PACKET_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup libgnuitar-packet Audio Packets
 * The API related to audio packets.
 */

/* get rid of __restrict__ type qualifier for MS Visual C */

#ifdef _MSC_VER
#define __restrict__
#endif

/** The maximum value of an audio sample
 * @ingroup libgnuitar-packet
 */

#define GNUITAR_SAMPLE_MAX INT32_MAX

/** The minimum value of an audio sample
 * @ingroup libgnuitar-packet
 */

#define GNUITAR_SAMPLE_MIN INT32_MIN

/** The maximum value of an audio sample, as a variable
 * @ingroup libgnuitar-packet
 */

extern const int32_t gnuitar_sample_max;

/** The minimum value of an audio sample, as a variable
 * @ingroup libgnuitar-packet
 */

extern const int32_t gnuitar_sample_min;

/** An audio packet.
 * Audio packets are preferably manipulated, instead of samples,
 * so that effects and audio drivers are not dependent on the sample type.
 * @ingroup libgnuitar-packet
 */

struct GnuitarPacket {
    /** Audio samples in the packet.
     * The samples are interleaved. */
    int32_t * __restrict__ data;
    /** A swap buffer for the packet.
     * The samples are interleaved. */
    int32_t * __restrict__ data_swap;
    /** The number of frames in the packet. */
    size_t len;
    /** The number of frames reserved for the packet */
    size_t res;
    /** The number of channels per frame. */
    size_t channels;
    /** The number of frames per second. */
    size_t rate;
};

void gnuitar_packet_init(struct GnuitarPacket *packet);

void gnuitar_packet_free(struct GnuitarPacket *packet);

size_t gnuitar_packet_get_length(const struct GnuitarPacket *packet);

size_t gnuitar_packet_import_s32le(struct GnuitarPacket *packet, const void * in, size_t in_size);

size_t gnuitar_packet_export_s32le(const struct GnuitarPacket *packet, void * out, size_t out_size);

void gnuitar_packet_set_si(struct GnuitarPacket *packet, size_t index, int32_t n);

int32_t gnuitar_packet_get_si(const struct GnuitarPacket *packet, size_t index);

void gnuitar_packet_mul_si(struct GnuitarPacket *packet, int32_t n);

void gnuitar_packet_div_si(struct GnuitarPacket *packet, int32_t n);

int gnuitar_packet_resize(struct GnuitarPacket *packet, size_t res);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_PACKET_H */

