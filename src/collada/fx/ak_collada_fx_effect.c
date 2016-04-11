/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_collada_fx_effect.h"

#include "../ak_collada_asset.h"
#include "../ak_collada_technique.h"
#include "../ak_collada_annotate.h"
#include "../ak_collada_param.h"

#include "ak_collada_fx_profile.h"

AkResult _assetkit_hide
ak_dae_effect(void * __restrict memParent,
               xmlTextReaderPtr reader,
               AkEffect ** __restrict  dest) {
  AkEffect    *effect;
  ak_annotate *last_annotate;
  ak_newparam *last_newparam;
  AkProfile   *last_profile;
  const xmlChar *nodeName;
  int            nodeType;
  int            nodeRet;

  effect = ak_calloc(memParent, sizeof(*effect), 1);

  last_annotate = NULL;
  last_newparam = NULL;
  last_profile  = NULL;
  
  _xml_readAttr(effect, effect->id, _s_dae_id);
  _xml_readAttr(effect, effect->name, _s_dae_name);

  do {
    _xml_beginElement(_s_dae_effect);

    if (_xml_eqElm(_s_dae_asset)) {
      ak_assetinf *assetInf;
      AkResult ret;

      assetInf = NULL;
      ret = ak_dae_assetInf(effect, reader, &assetInf);
      if (ret == AK_OK)
        effect->inf = assetInf;
    } else if (_xml_eqElm(_s_dae_annotate)) {
      ak_annotate *annotate;
      AkResult     ret;

      ret = ak_dae_annotate(effect, reader, &annotate);

      if (ret == AK_OK) {
        if (last_annotate)
          last_annotate->next = annotate;
        else
          effect->annotate = annotate;

        last_annotate = annotate;
      }
    } else if (_xml_eqElm(_s_dae_newparam)) {
      ak_newparam *newparam;
      AkResult     ret;

      ret = ak_dae_newparam(effect, reader, &newparam);

      if (ret == AK_OK) {
        if (last_newparam)
          last_newparam->next = newparam;
        else
          effect->newparam = newparam;

        last_newparam = newparam;
      }
    } else if (_xml_eqElm(_s_dae_prfl_common)
               || _xml_eqElm(_s_dae_prfl_glsl)
               || _xml_eqElm(_s_dae_prfl_gles2)
               || _xml_eqElm(_s_dae_prfl_gles)
               || _xml_eqElm(_s_dae_prfl_cg)
               || _xml_eqElm(_s_dae_prfl_bridge)) {
      AkProfile *profile;
      AkResult   ret;

      ret = ak_dae_profile(effect, reader, &profile);

      if (ret == AK_OK) {
        if (last_profile)
          last_profile->next = profile;
        else
          effect->profile = profile;

        last_profile = profile;
      }
    } else if (_xml_eqElm(_s_dae_extra)) {
      xmlNodePtr nodePtr;
      AkTree   *tree;

      nodePtr = xmlTextReaderExpand(reader);
      tree = NULL;

      ak_tree_fromXmlNode(effect, nodePtr, &tree, NULL);
      effect->extra = tree;

      _xml_skipElement;
    } else {
      _xml_skipElement;
    }

    /* end element */
    _xml_endElement;
  } while (nodeRet);

  *dest = effect;

  return AK_OK;
}

AkResult _assetkit_hide
ak_dae_fxInstanceEffect(void * __restrict memParent,
                         xmlTextReaderPtr reader,
                         AkInstanceEffect ** __restrict dest) {
  AkInstanceEffect *instanceEffect;
  AkTechniqueHint  *last_techHint;
  ak_setparam      *last_setparam;
  const xmlChar *nodeName;
  int            nodeType;
  int            nodeRet;

  instanceEffect = ak_calloc(memParent, sizeof(*instanceEffect), 1);

  _xml_readAttr(instanceEffect, instanceEffect->url, _s_dae_url);
  _xml_readAttr(instanceEffect, instanceEffect->sid, _s_dae_sid);
  _xml_readAttr(instanceEffect, instanceEffect->name, _s_dae_name);

  last_techHint = NULL;
  last_setparam = NULL;

  if (!xmlTextReaderIsEmptyElement(reader)) {
    do {
      _xml_beginElement(_s_dae_inst_effect);

      if (_xml_eqElm(_s_dae_technique_hint)) {
        AkTechniqueHint *techHint;

        techHint = ak_calloc(instanceEffect, sizeof(*techHint), 1);

        _xml_readAttr(techHint, techHint->ref, _s_dae_ref);
        _xml_readAttr(techHint, techHint->profile, _s_dae_profile);
        _xml_readAttr(techHint, techHint->platform, _s_dae_platform);

        if (last_techHint)
          last_techHint->next = techHint;
        else
          instanceEffect->techniqueHint = techHint;

        last_techHint = techHint;
      } else if (_xml_eqElm(_s_dae_setparam)) {
        ak_setparam *setparam;
        AkResult ret;

        ret = ak_dae_setparam(instanceEffect, reader, &setparam);

        if (ret == AK_OK) {
          if (last_setparam)
            last_setparam->next = setparam;
          else
            instanceEffect->setparam = setparam;

          last_setparam = setparam;
        }
      } else if (_xml_eqElm(_s_dae_extra)) {
        xmlNodePtr nodePtr;
        AkTree   *tree;

        nodePtr = xmlTextReaderExpand(reader);
        tree = NULL;

        ak_tree_fromXmlNode(instanceEffect, nodePtr, &tree, NULL);
        instanceEffect->extra = tree;

        _xml_skipElement;
      } else {
        _xml_skipElement;
      }

      /* end element */
      _xml_endElement;
    } while (nodeRet);
  }

  *dest = instanceEffect;
  
  return AK_OK;
}
