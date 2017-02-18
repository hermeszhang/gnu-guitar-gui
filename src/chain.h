#ifndef GNUITAR_CHAIN_H
#define GNUITAR_CHAIN_H

#include "effect.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup libgnuitar-chain Effects Chain
 * The API for manipulating effect chains.
 */

/** An effects chain.
 * @ingroup libgnuitar-chain
 */

struct GnuitarChain {
    /** number of references */
    size_t ref_count;
    /** array of effects */
    struct GnuitarEffect **effects;
    /** number of effects in the chain */
    size_t n_effects;
};

struct GnuitarChain * gnuitar_chain_create(void);

void gnuitar_chain_incref(struct GnuitarChain *chain);

void gnuitar_chain_decref(struct GnuitarChain *chain);

int gnuitar_chain_add_effect(struct GnuitarChain *chain, struct GnuitarEffect *effect);

int gnuitar_chain_erase_effect(struct GnuitarChain *chain, unsigned int index);

int gnuitar_chain_move_effect(struct GnuitarChain *chain, unsigned int src, unsigned int dst);

void gnuitar_chain_process(struct GnuitarChain *chain, struct GnuitarPacket *packet);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_CHAIN_H */

