/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../ak_common.h"
#include "../ak_memory_common.h"
#include <math.h>
#include <cglm.h>
#include <stdio.h>

AK_EXPORT
void
ak_coordCvtTransform(AkCoordSys *oldCoordSystem,
                     AkFloat4x4  oldTransform,
                     AkCoordSys *newCoordSystem,
                     AkFloat4x4  newTransform) {
  mat4           rot;
  mat4           scale = GLM_MAT4_IDENTITY;
  vec3           scalev, angles, tmp;
  ivec3          scalevs;
  AkAxisAccessor a0, a1;
  char           eulerNew[3];
  const char     eulerXYZ[3] = {0, 1, 2};

  a0.s_up    = AK_GET_SIGN(oldCoordSystem->up);
  a0.s_right = AK_GET_SIGN(oldCoordSystem->right);
  a0.s_fwd   = AK_GET_SIGN(oldCoordSystem->fwd);
  a0.up      = abs(oldCoordSystem->up)    - 1;
  a0.right   = abs(oldCoordSystem->right) - 1;
  a0.fwd     = abs(oldCoordSystem->fwd)   - 1;

  a1.s_up    = AK_GET_SIGN(newCoordSystem->up);
  a1.s_right = AK_GET_SIGN(newCoordSystem->right);
  a1.s_fwd   = AK_GET_SIGN(newCoordSystem->fwd);
  a1.up      = abs(newCoordSystem->up)    - 1;
  a1.right   = abs(newCoordSystem->right) - 1;
  a1.fwd     = abs(newCoordSystem->fwd)   - 1;

#define AK_CVT_VEC_TO(X0, X1)                                                 \
  X1[a1.up]    = X0[a0.up]    * a0.s_up    * a1.s_up;                         \
  X1[a1.right] = X0[a0.right] * a0.s_right * a1.s_right;                      \
  X1[a1.fwd]   = X0[a0.fwd]   * a0.s_fwd   * a1.s_fwd;

#define AK_CVT_VEC_NOSIGN_TO(X0, X1)                                          \
  X1[a1.up]    = X0[a0.up];                                                   \
  X1[a1.right] = X0[a0.right];                                                \
  X1[a1.fwd]   = X0[a0.fwd];

#define AK_CVT_VEC(X)                                                         \
  AK_CVT_VEC_TO(X, tmp)                                                       \
  glm_vec_dup(tmp, X);

#define AK_CVT_VEC_NOSIGN(X)                                                  \
  AK_CVT_VEC_NOSIGN_TO(X, tmp)                                                \
  glm_vec_dup(tmp, X);

  /* convert rotation */
  glm_decompose_rotation(oldTransform, rot);

  /* extract angles */
  glm_euler_angles(rot, angles);
  AK_CVT_VEC(angles);

  /* apply new rotation */
  AK_CVT_VEC_NOSIGN_TO(eulerXYZ, eulerNew);
  glm_euler_by_order(angles,
                     glm_euler_order(eulerNew),
                     rot);

  /* convert scaling */
  glm_decompose_scalev(oldTransform, scalev, scalevs);

  AK_CVT_VEC_NOSIGN(scalev);

  /* apply scaling */
  scale[0][0] = scalev[0] * scalevs[0];
  scale[1][1] = scalev[1] * scalevs[1];
  scale[2][2] = scalev[2] * scalevs[2];

  glm_mul(rot, scale, newTransform);

  /* convert translation */
  AK_CVT_VEC_TO(oldTransform[3], newTransform[3])

  /* duplicate w item directly to new coordinate sys without normalization */
  newTransform[0][3] = 0;
  newTransform[1][3] = 0;
  newTransform[2][3] = 0;
  newTransform[3][3] = oldTransform[3][3];
}
