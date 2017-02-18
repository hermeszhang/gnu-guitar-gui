#include "packet.h"

const int32_t gnuitar_sample_max = GNUITAR_SAMPLE_MAX;

const int32_t gnuitar_sample_min = GNUITAR_SAMPLE_MIN;

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

