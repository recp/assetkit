/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libassetio__aio_collada_fx_pass_h_
#define __libassetio__aio_collada_fx_pass_h_

#include "../../../include/assetio.h"

typedef struct _xmlNode xmlNode;

int _assetio_hide
aio_dae_fxPass(xmlNode * __restrict xml_node,
               aio_pass ** __restrict dest);

#endif /* __libassetio__aio_collada_fx_pass_h_ */
