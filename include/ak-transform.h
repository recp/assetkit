/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_transform_h
#define ak_transform_h

/*!
 * @brief build skew matrix from AkSkew
 *
 * @param skew   skew element
 * @param matrix skew matrix (must be aligned 16)
 */
AK_EXPORT
void
ak_transformSkewMatrix(AkSkew * __restrict skew,
                       float  * matrix);

/*!
 * @brief combines all node's transform elements
 *
 * if there is no transform then this returns identity matrix
 *
 * @param node   node
 * @param matrix combined transform (must be aligned 16)
 */
AK_EXPORT
void
ak_transformCombine(AkNode * __restrict node,
                    float  * matrix);

#endif /* ak_transform_h */