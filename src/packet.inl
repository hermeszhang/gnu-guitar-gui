#ifndef GNUITAR_PACKET_INL
#define GNUITAR_PACKET_INL

#include "packet.h"

namespace Gnuitar {

class Packet final {
    GnuitarPacket *packet;
public:
    inline Packet(GnuitarPacket *packet_ptr) noexcept
    {
        packet = packet_ptr;
    }
    inline ~Packet(void)
    {

    }
    inline double get_df(size_t index) const
    {
        return gnuitar_packet_get_df(packet, index);
    }
    inline int32_t get_si(size_t index) const
    {
        return gnuitar_packet_get_si(packet, index);
    }
    inline void set(size_t index, double n)
    {
        gnuitar_packet_set_df(packet, index, n);
    }
    inline void set(size_t index, int32_t n)
    {
        gnuitar_packet_set_si(packet, index, n);
    }
}; /* class Packet */

} /* namespace Gnuitar */

#endif /* GNUITAR_PACKET_INL */

