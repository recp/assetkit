/*
 * Copyright (C) 2020 Recep Aslantas
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common.h"
#include <json/json.h>


int AK_HIDE
ak_enumpair_cmp(const void * a, const void * b) {
  return strcmp(((const ak_enumpair *)a)->key,
                ((const ak_enumpair *)b)->key);
}

int AK_HIDE
ak_enumpair_cmp2(const void * a, const void * b) {
  return strcmp(a, ((const ak_enumpair *)b)->key);
}

int AK_HIDE
ak_enumpair_json_val_cmp(const void * a, const void * b) {
  const char *s;
  
  if (!(s = json_string(a)))
    return -1;

  return strncmp(s, ((const ak_enumpair *)b)->key, ((json_t *)a)->valsize);
}
