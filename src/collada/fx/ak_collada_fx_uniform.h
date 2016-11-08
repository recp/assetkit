/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libassetkit__ak_collada_fx_uniform_h_
#define __libassetkit__ak_collada_fx_uniform_h_

#include "../../../include/assetkit.h"
#include "../ak_collada_common.h"

AkResult _assetkit_hide
ak_dae_fxBindUniform(AkXmlState * __restrict xst,
                     void * __restrict memParent,
                     AkBindUniform ** __restrict dest);

#endif /* __libassetkit__ak_collada_fx_uniform_h_ */
