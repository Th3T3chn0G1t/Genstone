// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file genmathtypes.h
 * Includes mathematics utilities.
 */

#ifndef GEN_MATH_TYPES_H
#define GEN_MATH_TYPES_H

#include "gencommon.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <math.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)


/**
 * Signed 128-bit integer.
 */
typedef __int128 gen_int128_t;

/**
 * Unsigned 128-bit integer.
 */
typedef unsigned __int128 gen_uint128_t;

#ifdef __INTELLISENSE
typedef float gen_float16_t;
#else
/**
 * 16-bit floating point storage type.
 */
typedef __fp16 gen_float16_t;
#endif

/**
 * Performs fuzzy comparison of floating point types.
 * @param[in] a The first number.
 * @param[in] b The second number.
 * @param[in] epsilon The error or "fuzziness" for the comparison. Set to -1 to use the machine epsilon for the provided precision of `a`.
 * @returns Whether `a` and `b` are fuzzily equal.
 */
#define GEN_MATH_FLOAT_FUZZY_COMPARE(a, b, epsilon) \
		(((a) == (b)) \
		|| (fabs((a) - (b)) < (GEN_GENERIC((a), \
				float : (((epsilon) == -1.0f) ? FLT_EPSILON : (epsilon)), \
				double : (((epsilon) == -1.0) ? DBL_EPSILON : (epsilon)), \
				long double : (((epsilon) == -1.0L) ? LDBL_EPSILON : (epsilon)))))) \

/**
 * Type declarator for swizzleable clang vectors (OpenCL-style).
 * @param[in] base The base type of the vector.
 * @param[in] n The number of elements in the vector.
 * @param[in] name The name to apply to the created type.
 */
#define GEN_MATH_VECTOR_TYPE(base, n, name) typedef base name __attribute__((ext_vector_type(n)))

/**
 * Type declarator for clang matrices.
 * @param[in] base the base type of the matrix.
 * @param[in] i the number of elements along the matrix's x dimension.
 * @param[in] j the number of elements along the matrix's y dimension.
 * @param[in] name The name to apply to the created type.
 */
#define GEN_MATH_MATRIX_TYPE(base, i, j, name) typedef base name __attribute__((matrix_type(i, j)))

#define GEN_MATH_INTERNAL_VECTOR_TYPE(base, n) GEN_MATH_VECTOR_TYPE(base, n, gen_vector_##base##n)

#define GEN_MATH_INTERNAL_MATRIX_TYPE(base, i, j) GEN_MATH_MATRIX_TYPE(base, i, j, gen_matrix_##base##i##x##j)

#ifndef GEN_MATH_COMMON_TYPES
/**
 * Enables the default definition of common vector and matrix types.
 */
#define GEN_MATH_COMMON_TYPES GEN_ENABLED
#endif

#if GEN_MATH_COMMON_TYPES == GEN_ENABLED
GEN_MATH_INTERNAL_VECTOR_TYPE(float, 2);
GEN_MATH_INTERNAL_VECTOR_TYPE(float, 3);
GEN_MATH_INTERNAL_VECTOR_TYPE(float, 4);

GEN_MATH_INTERNAL_MATRIX_TYPE(float, 3, 3);
GEN_MATH_INTERNAL_MATRIX_TYPE(float, 4, 4);

GEN_MATH_INTERNAL_VECTOR_TYPE(int, 2);
GEN_MATH_INTERNAL_VECTOR_TYPE(int, 3);
GEN_MATH_INTERNAL_VECTOR_TYPE(int, 4);

GEN_MATH_INTERNAL_MATRIX_TYPE(int, 3, 3);
GEN_MATH_INTERNAL_MATRIX_TYPE(int, 4, 4);
#endif

#endif
