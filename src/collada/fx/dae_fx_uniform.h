/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libassetkit__dae_fx_uniform_h_
#define __libassetkit__dae_fx_uniform_h_

#include "../../../include/ak/assetkit.h"
#include "../dae_common.h"

AkResult _assetkit_hide
ak_dae_fxBindUniform(AkXmlState * __restrict xst,
                     void * __restrict memParent,
                     AkBindUniform ** __restrict dest);

#endif /* __libassetkit__dae_fx_uniform_h_ */
