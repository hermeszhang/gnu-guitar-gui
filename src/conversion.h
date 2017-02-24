#ifndef GNUITAR_CONVERSION_H
#define GNUITAR_CONVERSION_H

#include <stdint.h>

void gnuitar_si_to_s32le(int32_t in, void *out);

void gnuitar_si_to_s32be(int32_t in, void *out);

int32_t gnuitar_s32le_to_si(const void *in);

int32_t gnuitar_s32be_to_si(const void *in);

int32_t gnuitar_s24x3le_to_si(const void *in);

int32_t gnuitar_s24x3be_to_si(const void *in);

int32_t gnuitar_s16le_to_si(const void *in);

int32_t gnuitar_s16be_to_si(const void *in);

int32_t gnuitar_s8_to_si(const void *in);

#endif /* GNUITAR_CONVERSION_H */

