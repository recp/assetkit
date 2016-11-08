/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_collada_fx_lambert.h"
#include "ak_collada_fx_color_or_tex.h"
#include "ak_collada_fx_float_or_param.h"

#define k_s_dae_emission            1
#define k_s_dae_ambient             2
#define k_s_dae_diffuse             3
#define k_s_dae_reflective          5
#define k_s_dae_transparent         6
#define k_s_dae_reflectivity        8
#define k_s_dae_transparency        9
#define k_s_dae_index_of_refraction 10

static ak_enumpair lambertMap[] = {
  {_s_dae_emission,            k_s_dae_emission},
  {_s_dae_ambient,             k_s_dae_ambient},
  {_s_dae_diffuse,             k_s_dae_diffuse},
  {_s_dae_reflective,          k_s_dae_reflective},
  {_s_dae_transparent,         k_s_dae_transparent},
  {_s_dae_reflectivity,        k_s_dae_reflectivity},
  {_s_dae_transparency,        k_s_dae_transparency},
  {_s_dae_index_of_refraction, k_s_dae_index_of_refraction}
};

static size_t lambertMapLen = 0;

AkResult _assetkit_hide
ak_dae_fxLambert(AkXmlState * __restrict xst,
                 void * __restrict memParent,
                 AkLambert ** __restrict dest) {
  AkLambert *lambert;

  lambert = ak_heap_calloc(xst->heap,
                           memParent,
                           sizeof(*lambert),
                           false);

  if (lambertMapLen == 0) {
    lambertMapLen = AK_ARRAY_LEN(lambertMap);
    qsort(lambertMap,
          lambertMapLen,
          sizeof(lambertMap[0]),
          ak_enumpair_cmp);
  }

  do {
    const ak_enumpair *found;

    if (ak_xml_beginelm(xst, _s_dae_lambert))
      break;

    found = bsearch(xst->nodeName,
                    lambertMap,
                    lambertMapLen,
                    sizeof(lambertMap[0]),
                    ak_enumpair_cmp2);

    switch (found->val) {
      case k_s_dae_emission:
      case k_s_dae_ambient:
      case k_s_dae_diffuse:
      case k_s_dae_reflective:
      case k_s_dae_transparent: {
        AkFxColorOrTex *colorOrTex;
        AkResult ret;

        ret = ak_dae_colorOrTex(xst,
                                lambert,
                                (const char *)xst->nodeName,
                                &colorOrTex);
        if (ret == AK_OK) {
          switch (found->val) {
            case k_s_dae_emission:
              lambert->emission = colorOrTex;
              break;
            case k_s_dae_ambient:
              lambert->ambient = colorOrTex;
              break;
            case k_s_dae_diffuse:
              lambert->diffuse = colorOrTex;
              break;
            case k_s_dae_reflective:
              lambert->reflective = colorOrTex;
              break;
            case k_s_dae_transparent:
              lambert->transparent = colorOrTex;
              break;
            default: break;
          }
        }

        break;
      }
      case k_s_dae_reflectivity:
      case k_s_dae_transparency:
      case k_s_dae_index_of_refraction: {
        AkFxFloatOrParam * floatOrParam;
        AkResult ret;

        ret = ak_dae_floatOrParam(xst,
                                  lambert,
                                  (const char *)xst->nodeName,
                                  &floatOrParam);

        if (ret == AK_OK) {
          switch (found->val) {
            case k_s_dae_reflectivity:
              lambert->reflectivity = floatOrParam;
              break;
            case k_s_dae_transparency:
              lambert->transparency = floatOrParam;
              break;
            case k_s_dae_index_of_refraction:
              lambert->indexOfRefraction = floatOrParam;
              break;
            default: break;
          }
        }

        break;
      }
      default:
        ak_xml_skipelm(xst);;
        break;
    }

    /* end element */
    ak_xml_endelm(xst);;
  } while (xst->nodeRet);
  
  *dest = lambert;
  
  return AK_OK;
}
