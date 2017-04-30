/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../ak_common.h"
#include "../../include/ak-bbox.h"
#include "../../include/ak-path.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define STBI_MALLOC(sz)           ak_malloc(NULL, sz)
#define STBI_REALLOC(p,newsz)     ak_realloc(NULL, p, newsz)
#define STBI_FREE(p)              ak_free(p)

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#pragma GCC diagnostic pop

void
ak_imageLoad(AkImage * __restrict image) {
  AkHeap        *heap;
  AkDoc         *doc;
  AkImageData   *idata;
  unsigned char *data;

  idata  = NULL;
  data   = NULL;
  heap   = ak_heap_getheap(image);
  doc    = ak_heap_data(heap);

  if (image->initFrom) {
    AkInitFrom *initFrom;
    int         x, y, ch;

    initFrom = image->initFrom;
    if (initFrom->ref) {
      char        pathbuf[PATH_MAX];
      const char *path;

      path = ak_fullpath(doc, initFrom->ref, pathbuf);
      data = stbi_load(path, &x, &y, &ch, 0);
      if (!data)
        return;

      idata = ak_heap_calloc(heap,
                             image,
                             sizeof(*idata));
      idata->width  = x;
      idata->height = y;
      idata->comp   = ch;
      idata->data   = data;

      image->data   = idata;
    } else if (initFrom->hex) {
      /* TODO: */
    }
  }
}
