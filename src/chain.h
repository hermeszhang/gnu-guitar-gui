#ifndef GNUITAR_CHAIN_H
#define GNUITAR_CHAIN_H

#include "effect.h"
#include "mutex.h"

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
    /** array of effects */
    struct GnuitarEffect *effects;
    /** number of effects in the chain */
    size_t n_effects;
    /** the chain lock */
    struct GnuitarMutex mutex;
};

void gnuitar_chain_init(struct GnuitarChain *chain);

void gnuitar_chain_done(struct GnuitarChain *chain);

int gnuitar_chain_lock(struct GnuitarChain *chain);

int gnuitar_chain_unlock(struct GnuitarChain *chain);

int gnuitar_chain_add_effect(struct GnuitarChain *chain, struct GnuitarEffect *effect);

int gnuitar_chain_erase_effect(struct GnuitarChain *chain, unsigned int index);

int gnuitar_chain_move_effect(struct GnuitarChain *chain, unsigned int src, unsigned int dst);

void gnuitar_chain_process(struct GnuitarChain *chain, struct GnuitarPacket *packet);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* GNUITAR_CHAIN_H */

