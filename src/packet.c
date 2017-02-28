#include "packet.h"

#include "conversion.h"

#include <errno.h>
#include <string.h>

const int32_t gnuitar_sample_max = GNUITAR_SAMPLE_MAX;

const int32_t gnuitar_sample_min = GNUITAR_SAMPLE_MIN;

void
gnuitar_packet_init(struct GnuitarPacket *packet)
{
    packet->data = NULL;
    packet->data_swap = NULL;
    packet->len = 0;
    packet->res = 0;
    packet->channels = 0;
    packet->rate = 0;
}

/** Frees the memory allocated by the packet.
 * @param packet An initialized packet.
 * @ingroup libgnuitar-packet
 */

void
gnuitar_packet_done(struct GnuitarPacket *packet)
{
    if (packet == NULL) {
        free(packet->data);
        free(packet->data_swap);
        packet->data = NULL;
        packet->data_swap = NULL;
        packet->len = 0;
        packet->res = 0;
    }
}

/** Returns the number of samples in the packet.
 * @param packet An initialized packet.
 * @returns the number of samples in the packet.
 * @ingroup libgnuitar-packet
 */

size_t
gnuitar_packet_get_length(const struct GnuitarPacket *packet)
{
    return packet->len;
}

/** Reads an array of signed, 32-bit samples into the packet.
 * If there is data in the packet, it is discarded.
 * The packet is not automatically resized to fit all of the samples.
 * Use @ref gnuitar_packet_reserve to do this.
 * @param packet An initialized packet.
 * @param src An array of signed, 32-bit samples.
 * @param src_size The number of samples that can fit in @p src.
 * @returns The number of samples from @p src that were read from @p src.
 * @ingroup libgnuitar-packet
 */

size_t
gnuitar_packet_import_s32le(struct GnuitarPacket *packet, const void *src, size_t src_size)
{
    size_t i;
    int32_t sample;
    const unsigned char *src8;
    src8 = (const unsigned char *)(src);
    for (i = 0; (i < packet->len) && (i < src_size); i++){
        sample = gnuitar_s32le_to_si(&src8[i * 4]);
        gnuitar_packet_set_si(packet, i, sample);
    }
    return i;
}

/** Writes the audio samples in the packet on to an array.
 * The audio samples are encoded as signed, integer values (little endian).
 * @param packet An initialized packet.
 * @param dst A destination array
 * @param dst_size The number of samples that can fit into the destionation
 *  array.
 * @returns The number of samples from @p packet that were written to @p dst.
 * @ingroup libgnuitar-packet
 */

size_t
gnuitar_packet_export_s32le(const struct GnuitarPacket *packet, void *dst, size_t dst_size)
{
    size_t i;
    int32_t sample;
    unsigned char *dst8;
    dst8 = (unsigned char *)(dst);
    for (i = 0; (i < packet->len) && (i < dst_size); i++){
        sample = gnuitar_packet_get_si(packet, i);
        gnuitar_si_to_s32le(sample, &dst8[i * 4]);
    }
    return i;
}

/** Sets a sample in the packet to an integer value.
 * @param packet An initialized packet.
 * @param index The index of the sample within the packet.
 *  This value is range-checked.
 * @param n The value to set the sample to.
 *  The value maximum is INT32_MAX, the minimum is INT32_MIN.
 * @ingroup libgnuitar-packet
 */

void
gnuitar_packet_set_si(struct GnuitarPacket *packet, size_t index, int32_t n)
{
    if (index < packet->len) {
        packet->data[index] = n;
    }
}

void
gnuitar_packet_set_df(struct GnuitarPacket *packet, size_t index, double n)
{
    if (n > 1.0)
        n = 1.0;
    else if (n < -1.0)
        n = -1.0;
    if (index < packet->len)
        packet->data[index] = GNUITAR_SAMPLE_MAX * n;
}

/** Gets the value of a sample within a packet.
 * @param packet An initialized packet.
 * @param index The index of the packet.
 * @returns The value of the packet.
 *  If the index specified is out of range, zero is returned.
 * @ingroup libgnuitar-packet
 */

int32_t
gnuitar_packet_get_si(const struct GnuitarPacket *packet, size_t index)
{
    if (index < packet->len) {
        return packet->data[index];
    }
    return 0;
}

double
gnuitar_packet_get_df(const struct GnuitarPacket *packet, size_t index)
{
    double sample = 0.0;
    if (index < packet->len) {
        sample = packet->data[index];
	sample /= GNUITAR_SAMPLE_MAX;
    }
    return sample;
}

/** Multiplies the samples in the packet by a signed integer.
 * This may cause the samples to clip.
 * @param packet An initialized packet.
 * @param n A signed integer multiplying factor.
 *  This number is clipped if it is greater
 *  than @ref GNUITAR_SAMPLE_MAX or less than
 *  @ref GNUITAR_SAMPLE_MIN.
 * @ingroup libgnuitar-packet
 */

void
gnuitar_packet_mul_si(struct GnuitarPacket *packet, int32_t n)
{
    size_t i;
    int64_t product;

#if GNUITAR_SAMPLE_MAX != INT32_MAX
    if (n > GNUITAR_SAMPLE_MAX)
        n = GNUITAR_SAMPLE_MAX;
#endif /* GNUITAR_SAMPLE_MAX != INT32_MAX */

#if GNUITAR_SAMPLE_MIN != INT32_MIN
    if (n < GNUITAR_SAMPLE_MIN)
        n = GNUITAR_SAMPLE_MIN;
#endif /* GNUITAR_SAMPLE_MIN != INT32_MIN */

    for (i = 0; i < packet->len; i++) {
        product = packet->data[i];
    product *= n;
    if (product > GNUITAR_SAMPLE_MAX)
        packet->data[i] = GNUITAR_SAMPLE_MAX;
    else if (product < GNUITAR_SAMPLE_MIN)
        packet->data[i] = GNUITAR_SAMPLE_MIN;
    else
        packet->data[i] = product;
    }
}

/** Divides the samples in the packet by a signed integer.
 * @param packet An initialized packet
 * @param n A signed integer divisor
 *  This number is clipped if it is greater
 *  than @ref GNUITAR_SAMPLE_MAX or less than
 *  @ref GNUITAR_SAMPLE_MIN.
 * @ingroup libgnuitar-packet
 */

void
gnuitar_packet_div_si(struct GnuitarPacket *packet, int32_t n)
{
    size_t i;

#if GNUITAR_SAMPLE_MAX != INT32_MAX
    if (n > GNUITAR_SAMPLE_MAX)
        n = GNUITAR_SAMPLE_MAX;
#endif /* GNUITAR_SAMPLE_MAX != INT32_MAX */

#if GNUITAR_SAMPLE_MIN != INT32_MIN
    if (n < GNUITAR_SAMPLE_MIN)
        n = GNUITAR_SAMPLE_MIN;
#endif /* GNUITAR_SAMPLE_MIN != INT32_MIN */

    for (i = 0; i < packet->len; i++) {
        packet->data[i] /= n;
    }
}

/** Reserves memory for the packet.
 * @param packet An initialized packet.
 * @param res The number of samples to reserve in the packet.
 * @returns On success, zero.
 *  On failure, the number indicated the error is returned.
 * @ingroup libgnuitar-packet
 */

int
gnuitar_packet_reserve(struct GnuitarPacket *packet, size_t res)
{
    void *tmp1, *tmp2;
    size_t memcpy_size;

    tmp1 = malloc(res * sizeof(packet->data[0]));
    if ((tmp1 == NULL) && (res > 0))
        return ENOMEM;

    tmp2 = malloc(res * sizeof(packet->data_swap[0]));
    if ((tmp2 == NULL) && (res > 0)) {
        free(tmp1);
        return ENOMEM;
    }

    if (packet->len < res)
        memcpy_size = packet->len;
    else
        memcpy_size = res;

    memcpy(tmp1, packet->data, memcpy_size);
    memcpy(tmp2, packet->data_swap, memcpy_size);

    packet->res = res;

    if (packet->res < packet->len)
        packet->len = packet->res;

    free(packet->data);
    free(packet->data_swap);

    packet->data = tmp1;
    packet->data_swap = tmp2;

    return 0;
}

