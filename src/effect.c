#include "effect.h"

#include <errno.h>

/** Initializes the members of the effect structure to zero or NULL.
 * This may be useful to avoid invalid calls to @ref gnuitar_effect_done.
 * This function doesn't initialize an actual effect.
 * @param effect An unitialized effect
 * @ingroup libgnuitar-effect
 */

void
gnuitar_effect_init(struct GnuitarEffect *effect)
{
    effect->params = NULL;
    effect->init = NULL;
    effect->done = NULL;
    effect->process = NULL;
    effect->get_map = NULL;
    effect->set_map = NULL;
    effect->toggle = 0;
}

/** Releases resources allocated by effect structure.
 * @param effect An initialized gnuitar effect
 * @ingroup libgnuitar-effect
 */

void
gnuitar_effect_done(struct GnuitarEffect *effect)
{
    if (effect == NULL)
        return;
    if (effect->done)
        effect->done(effect);
}

/** Runs the effect on an audio packet.
 * @param effect An initialized effect
 * @param packet The audio packet to process
 * @returns On success, zero.
 *  If a memory allocation failure occurs, ENOMEM is returned.
 *  If the processing callback wasn't set, EFAULT is returned.
 * @ingroup libgnuitar-effect
 */

int
gnuitar_effect_process(struct GnuitarEffect *effect, struct GnuitarPacket *packet)
{
    if (effect->toggle)
        return effect->process(effect, packet);

    return EFAULT;
}

/** Gets the parameter map of an effect.
 * @param effect An initialized effect
 * @param map The map to fill with the effect parameters
 * @returns On success, zero.
 *  If a memory allocation failure occurs, ENOMEM is returned.
 *  If the map access callback wasn't set, EFAULT is returned.
 * @ingroup libgnuitar-effect
 */

int
gnuitar_effect_get_map(const struct GnuitarEffect *effect, struct GnuitarMap *map)
{
    if (effect->get_map)
        return effect->get_map(effect, map);

    return EFAULT;
}

/** Sets the parameter map of an effect.
 * @param effect An initialized effect
 * @param map The map with the effect parameters to set
 * @returns On success, zero.
 *  If a memory allocation failure occurs, ENOMEM is returned.
 *  If the map access callback wasn't set, EFAULT is returned.
 * @ingroup libgnuitar-effect
 */

int
gnuitar_effect_set_map(struct GnuitarEffect *effect, const struct GnuitarMap *map)
{
    if (effect->set_map)
        return effect->set_map(effect, map);

    return EFAULT;
}

