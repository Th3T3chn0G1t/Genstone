// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file genmath.h
 * Includes mathematics utilities.
 */

#ifndef GEN_MATH_H
#define GEN_MATH_H

#include "gencommon.h"

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
 * Compares `float`s taking the type epsilon into account.
 * @param[in] a The first `float` to compare.
 * @param[in] b The second `float` to compare.
 * @param[out] out_equal A pointer to storage for the result of the comparison.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_math_compare_fuzzy_float(const float a, const float b, gen_bool_t* const restrict out_equal);

/**
 * Compares `double`s taking the type epsilon into account.
 * @param[in] a The first `double` to compare.
 * @param[in] b The second `double` to compare.
 * @param[out] out_equal A pointer to storage for the result of the comparison.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_math_compare_fuzzy_double(const double a, const double b, gen_bool_t* const restrict out_equal);

/**
 * Compares `long double`s taking the type epsilon into account.
 * @param[in] a The first `long double` to compare.
 * @param[in] b The second `long double` to compare.
 * @param[out] out_equal A pointer to storage for the result of the comparison.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_math_compare_fuzzy_long_double(const long double a, const long double b, gen_bool_t* const restrict out_equal);

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
