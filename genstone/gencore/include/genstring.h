// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genstr.h
 * String manipulation utilities.
 */

#ifndef GEN_STRING_H
#define GEN_STRING_H

#include "gencommon.h"

/**
 * A value which can be used in place of string bounds in the event that no bounding should be performed.
 */
#define GEN_STRING_NO_BOUNDS SIZE_MAX

/**
 * Compares 2 strings.
 * @param[in] a The first string.
 * @param[in] a_bounds The first string's bounds.
 * @param[in] b The second string.
 * @param[in] b_bounds The second string's bounds.
 * @param[in] limit The number of characters to compare.
 * @param[out] out_equal Pointer to storage for the result of the comparison.
 * @return An error code.
 */
extern gen_error_t gen_string_compare(const char* const restrict a, const size_t a_bounds, const char* const restrict b, const size_t b_bounds, const size_t limit, bool* const restrict out_equal);
/**
 * Copies the contents of a string into another.
 * @param[in] destination The destination for the copied data.
 * @param[in] destination_bounds The bounds for the destination buffer.
 * @param[in] source The string to copy.
 * @param[in] source_bounds The bounds of the string to copy.
 * @param[in] limit The number of characters to copy.
 * @return An error code.
 */
extern gen_error_t gen_string_copy(char* const restrict destination, const size_t destination_bounds, const char* const restrict source, const size_t source_bounds, const size_t limit);
/**
 * Appends one string to another.
 * @param[in] destination The destination to append to.
 * @param[in] destination_bounds The bounds for the destination buffer.
 * @param[in] source The string to append.
 * @param[in] source_bounds The bounds of the string to append.
 * @param[in] limit The number of characters to append.
 * @return An error code.
 */
extern gen_error_t gen_string_append(char* const restrict destination, const size_t destination_bounds, const char* const restrict source, const size_t source_bounds, const size_t limit);
/**
 * Gets the length of a string.
 * @param[in] string The string whose length should be found.
 * @param[in] string_bounds The bounds of the string whose length should be found.
 * @param[out] out_length A pointer to storage for the length of the string.
 * @return An error code.
 */
extern gen_error_t gen_string_length(const char* const restrict string, const size_t string_bounds, const size_t limit, size_t* const restrict out_length);
/**
 * Duplicates a string onto the heap.
 * @param[in] string The string to duplicate.
 * @param[in] string_bounds The bounds of the string to duplicate.
 * @param[in] limit The number of characters to duplicate.
 * @param[out] out_duplicated A pointer to storage for a pointer to the duplicated string. Must be freed.
 * @return An error code.
 */
extern gen_error_t gen_string_duplicate(const char* const restrict string, const size_t string_bounds, const size_t limit, char* restrict* const restrict out_duplicated);
/**
 * Finds the first occurrence of a character in a string.
 * @param[in] string The string to search in.
 * @param[in] string_bounds The bounds of the string to seach in.
 * @param[in] character The character to search for.
 * @param[in] limit The number of characters in string from its beginning to search in.
 * @param[out] out_found A pointer to storage for a pointer to the found occurrence.
 * @return An error code.
 */
extern gen_error_t gen_string_character_first(const char* const restrict string, const size_t string_bounds, const char character, const size_t limit, const char** const restrict out_found);
/**
 * Finds the last occurrence of a character in a string.
 * @param[in] string The string to search in.
 * @param[in] string_bounds The bounds of the string to seach in.
 * @param[in] character The character to search for.
 * @param[in] limit The number of characters in string from its beginning to search in.
 * @param[out] out_found A pointer to storage for a pointer to the found occurrence.
 * @return An error code.
 */
extern gen_error_t gen_string_character_last(const char* const restrict string, const size_t string_bounds, const char character, const size_t limit, const char** const restrict out_found);

/**
 * Creates a number from a numeric string.
 * @param[in] string The numeric string to create from.
 * @param[in] string_bounds The bounds of the numeric string.
 * @param[out] out_number A pointer to storage for the created number.
 * @return An error code.
 */
extern gen_error_t gen_string_number(const char* const restrict string, const size_t string_bounds, const size_t limit, size_t* const restrict out_number);

/**
 * Formats a string to a buffer.
 * @param[out] out_buffer A pointer to storage for the formatted string.
 */
extern gen_error_t gen_string_format(char* const restrict out_buffer, size_t* out_length, const char* const restrict format, ...);

#endif
