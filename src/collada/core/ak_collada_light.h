/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libassetkit__ak_collada_light__h_
#define __libassetkit__ak_collada_light__h_

#include "../ak_collada_common.h"

AkResult _assetkit_hide
ak_dae_light(AkXmlState * __restrict xst,
             void * __restrict memParent,
             AkLight ** __restrict  dest);

#endif /* __libassetkit__ak_collada_light__h_ */
