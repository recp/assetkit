/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_collada_triangles.h"
#include "ak_collada_enums.h"
#include "../../ak_array.h"

AkResult _assetkit_hide
ak_dae_triangles(AkXmlState * __restrict xst,
                 void * __restrict memParent,
                 const char * elm,
                 AkTriangleMode mode,
                 AkTriangles ** __restrict dest) {
  AkDoc       *doc;
  AkTriangles *triangles;
  AkInput     *last_input;

  doc       = ak_heap_data(xst->heap);
  triangles = ak_heap_calloc(xst->heap,
                             memParent,
                             sizeof(*triangles),
                             false);

  triangles->mode = mode;
  triangles->base.type = AK_MESH_PRIMITIVE_TYPE_TRIANGLES;

  _xml_readAttr(triangles, triangles->base.name, _s_dae_name);
  _xml_readAttr(triangles, triangles->base.material, _s_dae_material);
  _xml_readAttrUsingFnWithDef(triangles->count,
                              _s_dae_count,
                              0,
                              strtoul, NULL, 10);

  last_input = NULL;

  do {
    if (ak_xml_beginelm(xst, elm))
      break;

    if (_xml_eqElm(_s_dae_input)) {
      AkInput *input;

      input = ak_heap_calloc(xst->heap, triangles, sizeof(*input), false);

      _xml_readAttr(input, input->base.semanticRaw, _s_dae_semantic);

      ak_url_from_attr(xst->reader,
                       _s_dae_source,
                       input,
                       &input->base.source);

      if (!input->base.semanticRaw || !input->base.source.url)
        ak_free(input);
      else {
        AkEnum inputSemantic;
        inputSemantic = ak_dae_enumInputSemantic(input->base.semanticRaw);

        if (inputSemantic < 0)
          inputSemantic = AK_INPUT_SEMANTIC_OTHER;

        input->base.semantic = inputSemantic;
      }

      _xml_readAttrUsingFn(input->offset,
                           _s_dae_offset,
                           (AkUInt)strtoul, NULL, 10);

      _xml_readAttrUsingFn(input->set,
                           _s_dae_set,
                           (AkUInt)strtoul, NULL, 10);

      if (last_input)
        last_input->base.next = &input->base;
      else
        triangles->base.input = input;

      last_input = input;

      triangles->base.inputCount++;

      /* attach vertices for convenience */
      if (input->base.semantic == AK_INPUT_SEMANTIC_VERTEX)
        triangles->base.vertices = ak_getObjectByUrl(&input->base.source);
    } else if (_xml_eqElm(_s_dae_p)) {
      AkUIntArray *uintArray;
      char *content;

      content = ak_xml_rawval(xst);
      if (content) {
        AkResult ret;
        ret = ak_strtoui_array(xst->heap, triangles, content, &uintArray);
        if (ret == AK_OK)
          triangles->base.indices = uintArray;

        xmlFree(content);
      }

    } else if (_xml_eqElm(_s_dae_extra)) {
      xmlNodePtr nodePtr;
      AkTree   *tree;

      nodePtr = xmlTextReaderExpand(xst->reader);
      tree = NULL;
      
      ak_tree_fromXmlNode(xst->heap,
                          triangles,
                          nodePtr,
                          &tree,
                          NULL);
      triangles->base.extra = tree;
      
      ak_xml_skipelm(xst);;
    } else {
      ak_xml_skipelm(xst);;
    }
    
    /* end element */
    ak_xml_endelm(xst);;
  } while (xst->nodeRet);
  
  *dest = triangles;
  
  return AK_OK;
}
