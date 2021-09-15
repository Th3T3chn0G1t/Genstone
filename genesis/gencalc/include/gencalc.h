// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file gencalc.h
 * Includes mathematics utilities
 */

#ifndef GEN_CALC_H
#define GEN_CALC_H

#include <gencommon.h>

/**
 * Signed 128-bit integer
 */
typedef __int128 gint128;
/**
 * Unsigned 128-bit integer
 */
typedef unsigned __int128 guint128;

/**
 * 16-bit floating point storage type
 */
typedef __fp16 gfloat16;

/**
 * Internal type declarator for clang vectors (OpenCL-style)
 * @param base the base type of the vector
 * @param n the number of elements in the vector
 */
#define GEN_INTERNAL_VECTOR_TYPE(base, n) typedef base g##base##n __attribute__((ext_vector_type(n)))
/**
 * Internal type declarator for clang matrices
 * @param base the base type of the matrix
 * @param i the number of elements along the matrix's x dimension
 * @param j the number of elements along the matrix's y dimension
 */
#define GEN_INTERNAL_MATRIX_TYPE(base, i, j) typedef base g##base##i##x##j __attribute__((matrix_type(i, j)))

/**
 * Reasonably converts a vector between vector types
 * @param vector the vector to convert
 * @param new_type the vector type to convert to
 */
#define GEN_CONVERT_VECTOR(vector, new_type) __builtin_convertvector(vector, new_type)

/**
 * A 2-component vector of floats
 */
GEN_INTERNAL_VECTOR_TYPE(float, 2);
/**
 * A 3-component vector of floats
 */
GEN_INTERNAL_VECTOR_TYPE(float, 3);
/**
 * A 4-component vector of floats
 */
GEN_INTERNAL_VECTOR_TYPE(float, 4);

/**
 * A 3x3 matrix of floats
 */
GEN_INTERNAL_MATRIX_TYPE(float, 3, 3);
/**
 * A 4x4 matrix of floats
 */
GEN_INTERNAL_MATRIX_TYPE(float, 4, 4);

/**
 * A 2-component vector of ints
 */
GEN_INTERNAL_VECTOR_TYPE(int, 2);
/**
 * A 3-component vector of ints
 */
GEN_INTERNAL_VECTOR_TYPE(int, 3);
/**
 * A 4-component vector of ints
 */
GEN_INTERNAL_VECTOR_TYPE(int, 4);

/**
 * A 3x3 matrix of ints
 */
GEN_INTERNAL_MATRIX_TYPE(int, 3, 3);
/**
 * A 4x4 matrix of ints
 */
GEN_INTERNAL_MATRIX_TYPE(int, 4, 4);

#endif
