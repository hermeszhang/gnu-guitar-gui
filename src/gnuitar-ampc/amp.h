#ifndef GNUITAR_AMPC_AMP_H
#define GNUITAR_AMPC_AMP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct GnuitarAmp;

struct GnuitarAmp * gnuitar_amp_init (void);

float gnuitar_amp_process_signal (struct GnuitarAmp *amp, float signal);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_AMPC_AMP_H */

