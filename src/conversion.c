#include "conversion.h"

void gnuitar_si_to_s32le(int32_t in, void *out){
    unsigned char *out8;
    out8 = (unsigned char *)(out);
    out8[0] = in >> 0x00;
    out8[1] = in >> 0x08;
    out8[2] = in >> 0x10;
    out8[3] = in >> 0x18;
}

void gnuitar_si_to_s32be(int32_t in, void *out){
    unsigned char *out8;
    out8 = (unsigned char *)(out);
    out8[0] = in >> 0x18;
    out8[1] = in >> 0x10;
    out8[2] = in >> 0x08;
    out8[3] = in >> 0x00;
}

int32_t gnuitar_s32le_to_si(const void *in){
    const unsigned char *in8;
    int32_t out32;
    in8 = (const unsigned char *)(in);
    out32 = 0;
    out32 |= (in8[0] << 0x00);
    out32 |= (in8[1] << 0x08);
    out32 |= (in8[2] << 0x10);
    out32 |= (in8[3] << 0x18);
    return out32;
}

int32_t gnuitar_s32be_to_si(const void *in){
    const unsigned char *in8;
    int32_t out32;
    in8 = (const unsigned char *)(in);
    out32 = 0;
    out32 |= (in8[0] << 0x18);
    out32 |= (in8[1] << 0x10);
    out32 |= (in8[2] << 0x08);
    out32 |= (in8[3] << 0x00);
    return out32;
}

int32_t gnuitar_s24x3le_to_si(const void *in){
    const unsigned char *in8;
    int32_t out32;
    in8 = (const unsigned char *)(in);
    out32 = 0;
    out32 |= (in8[0] << 0x08);
    out32 |= (in8[1] << 0x10);
    out32 |= (in8[2] << 0x18);
    return out32;
}

int32_t gnuitar_s24x3be_to_si(const void *in){
    const unsigned char *in8;
    int32_t out32;
    in8 = (const unsigned char *)(in);
    out32 = 0;
    out32 |= (in8[0] << 0x18);
    out32 |= (in8[1] << 0x10);
    out32 |= (in8[2] << 0x08);
    return out32;
}

int32_t gnuitar_s16le_to_si(const void *in){
    const unsigned char *in8;
    int32_t out32;
    in8 = (const unsigned char *)(in);
    out32 = 0;
    out32 |= (in8[0] << 0x10);
    out32 |= (in8[1] << 0x18);
    return out32;
}

int32_t gnuitar_s16be_to_si(const void *in){
    const unsigned char *in8;
    int32_t out32;
    in8 = (const unsigned char *)(in);
    out32 = 0;
    out32 |= (in8[0] << 0x18);
    out32 |= (in8[1] << 0x10);
    return out32;
}

int32_t gnuitar_s8_to_si(const void *in){
    const unsigned char *in8 = (const unsigned char*)(in);
    return (in8[0] << 0x18);
}

