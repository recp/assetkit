/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_collada_fx_effect.h"

#include "../core/ak_collada_asset.h"
#include "../core/ak_collada_technique.h"
#include "../core/ak_collada_annotate.h"
#include "../core/ak_collada_param.h"

#include "ak_collada_fx_profile.h"

AkResult _assetkit_hide
ak_dae_effect(AkXmlState * __restrict xst,
              void * __restrict memParent,
              void ** __restrict  dest) {
  AkEffect     *effect;
  AkAnnotate   *last_annotate;
  AkNewParam   *last_newparam;
  AkProfile    *last_profile;
  AkXmlElmState xest;

  effect = ak_heap_calloc(xst->heap,
                          memParent,
                          sizeof(*effect));

  last_annotate = NULL;
  last_newparam = NULL;
  last_profile  = NULL;

  ak_xml_readid(xst, effect);
  effect->name = ak_xml_attr(xst, effect, _s_dae_name);

  ak_xest_init(xest, _s_dae_effect)

  do {
    if (ak_xml_begin(&xest))
      break;

    if (ak_xml_eqelm(xst, _s_dae_asset)) {
      AkAssetInf *assetInf;
      AkResult ret;

      assetInf = NULL;
      ret = ak_dae_assetInf(xst, effect, &assetInf);
      if (ret == AK_OK)
        effect->inf = assetInf;
    } else if (ak_xml_eqelm(xst, _s_dae_annotate)) {
      AkAnnotate *annotate;
      AkResult    ret;

      ret = ak_dae_annotate(xst, effect, &annotate);

      if (ret == AK_OK) {
        if (last_annotate)
          last_annotate->next = annotate;
        else
          effect->annotate = annotate;

        last_annotate = annotate;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_newparam)) {
      AkNewParam *newparam;
      AkResult    ret;

      ret = ak_dae_newparam(xst, effect, &newparam);

      if (ret == AK_OK) {
        if (last_newparam)
          last_newparam->next = newparam;
        else
          effect->newparam = newparam;

        last_newparam = newparam;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_prfl_common)
               || ak_xml_eqelm(xst, _s_dae_prfl_glsl)
               || ak_xml_eqelm(xst, _s_dae_prfl_gles2)
               || ak_xml_eqelm(xst, _s_dae_prfl_gles)
               || ak_xml_eqelm(xst, _s_dae_prfl_cg)
               || ak_xml_eqelm(xst, _s_dae_prfl_bridge)) {
      AkProfile *profile;
      AkResult   ret;

      ret = ak_dae_profile(xst, effect, &profile);

      if (ret == AK_OK) {
        if (last_profile)
          last_profile->next = profile;
        else
          effect->profile = profile;

        last_profile = profile;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_extra)) {
      xmlNodePtr nodePtr;
      AkTree   *tree;

      nodePtr = xmlTextReaderExpand(xst->reader);
      tree = NULL;

      ak_tree_fromXmlNode(xst->heap,
                          effect,
                          nodePtr,
                          &tree,
                          NULL);
      effect->extra = tree;

      ak_xml_skipelm(xst);
    } else {
      ak_xml_skipelm(xst);
    }

    /* end element */
    if (ak_xml_end(&xest))
      break;
  } while (xst->nodeRet);

  *dest = effect;

  return AK_OK;
}

AkResult _assetkit_hide
ak_dae_fxInstanceEffect(AkXmlState * __restrict xst,
                        void * __restrict memParent,
                        AkInstanceEffect ** __restrict dest) {
  AkInstanceEffect *instanceEffect;
  AkTechniqueHint  *last_techHint;
  AkSetParam       *last_setparam;
  AkXmlElmState     xest;

  instanceEffect = ak_heap_calloc(xst->heap,
                                  memParent,
                                  sizeof(*instanceEffect));

  ak_xml_readsid(xst, instanceEffect);

  instanceEffect->base.name = ak_xml_attr(xst,
                                          instanceEffect,
                                          _s_dae_name);

  ak_xml_attr_url(xst,
                  _s_dae_url,
                  instanceEffect,
                  &instanceEffect->base.url);

  last_techHint = NULL;
  last_setparam = NULL;

  ak_xest_init(xest, _s_dae_inst_effect)

  do {
    if (ak_xml_begin(&xest))
      break;

    if (ak_xml_eqelm(xst, _s_dae_technique_hint)) {
      AkTechniqueHint *techHint;

      techHint = ak_heap_calloc(xst->heap,
                                instanceEffect,
                                sizeof(*techHint));

      techHint->ref      = ak_xml_attr(xst, techHint, _s_dae_ref);
      techHint->profile  = ak_xml_attr(xst, techHint, _s_dae_profile);
      techHint->platform = ak_xml_attr(xst, techHint, _s_dae_platform);

      if (last_techHint)
        last_techHint->next = techHint;
      else
        instanceEffect->techniqueHint = techHint;

      last_techHint = techHint;
    } else if (ak_xml_eqelm(xst, _s_dae_setparam)) {
      AkSetParam *setparam;
      AkResult ret;

      ret = ak_dae_setparam(xst,
                            instanceEffect,
                            &setparam);

      if (ret == AK_OK) {
        if (last_setparam)
          last_setparam->next = setparam;
        else
          instanceEffect->setparam = setparam;

        last_setparam = setparam;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_extra)) {
      xmlNodePtr nodePtr;
      AkTree   *tree;

      nodePtr = xmlTextReaderExpand(xst->reader);
      tree = NULL;

      ak_tree_fromXmlNode(xst->heap,
                          instanceEffect,
                          nodePtr,
                          &tree,
                          NULL);
      instanceEffect->base.extra = tree;

      ak_xml_skipelm(xst);
    } else {
      ak_xml_skipelm(xst);
    }

    /* end element */
    if (ak_xml_end(&xest))
      break;
  } while (xst->nodeRet);

  *dest = instanceEffect;

  return AK_OK;
}
