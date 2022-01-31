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
#define GEN_STRING_NO_BOUND SIZE_MAX

#define GEN_STRING_FOREACH(member, members_length, container) for(register __typeof__((container)[0])* member = &(container)[0]; (size_t) ((member) - (container)) < (members_length); ++(member))

/**
 * Compares 2 strings.
 * @param[in] a the first string.
 * @param[in] a_bound the first string's bounds.
 * @param[in] b the second string.
 * @param[in] b_bound the second string's bounds.
 * @param[in] limit the number of characters to compare.
 * @param[out] out_equal pointer to storage for the result of the comparison.
 * @return an error code.
 */
GEN_ERRORABLE gen_string_compare(const char* const restrict a, const size_t a_bound, const char* const restrict b, const size_t b_bound, const size_t limit, bool* const restrict out_equal);
/**
 * Copies the contents of a string into another.
 * @param[in] destination the destination for the copied data.
 * @param[in] destination_bound the bounds for the destination buffer.
 * @param[in] source the string to copy.
 * @param[in] source_bound the bounds of the string to copy.
 * @param[in] limit the number of characters to copy.
 * @return an error code.
 */
GEN_ERRORABLE gen_string_copy(char* const restrict destination, const size_t destination_bound, const char* const restrict source, const size_t source_bound, const size_t limit);
/**
 * Appends one string to another.
 * @param[in] destination the destination to append to.
 * @param[in] destination_bound the bounds for the destination buffer.
 * @param[in] source the string to append.
 * @param[in] source_bound the bounds of the string to append.
 * @param[in] limit the number of characters to append.
 * @return an error code.
 */
GEN_ERRORABLE gen_string_append(char* const restrict destination, const size_t destination_bound, const char* const restrict source, const size_t source_bound, const size_t limit);
/**
 * Gets the length of a string.
 * @param[in] string the string whose length should be found.
 * @param[in] string_bound the bounds of the string whose length should be found.
 * @param[out] out_length pointer to storage for the length of the string.
 * @return an error code.
 */
GEN_ERRORABLE gen_string_length(const char* const restrict string, const size_t string_bound, const size_t limit, size_t* const restrict out_length);
/**
 * Duplicates a string onto the heap.
 * @param[in] string the string to duplicate.
 * @param[in] string_bound the bounds of the string to duplicate.
 * @param[in] limit the number of characters to duplicate.
 * @param[out] out_duplicated pointer to storage for a pointer to the duplicated string. Must be freed.
 * @return an error code.
 */
GEN_ERRORABLE gen_string_duplicate(const char* const restrict string, const size_t string_bound, const size_t limit, char* restrict* const restrict out_duplicated);
/**
 * Finds the first occurrence of a character in a string.
 * @param[in] string the string to search in.
 * @param[in] string_bound the bounds of the string to seach in.
 * @param[in] character the character to search for.
 * @param[in] limit the number of characters in string from its beginning to search in.
 * @param[out] out_found pointer to storage for a pointer to the found occurrence, or NULL if the character was not found.
 * @return an error code.
 */
GEN_ERRORABLE gen_string_character_first(const char* const restrict string, const size_t string_bound, const char character, const size_t limit, const char* restrict* const restrict out_found);
/**
 * Finds the last occurrence of a character in a string.
 * @param[in] string the string to search in.
 * @param[in] string_bound the bounds of the string to seach in.
 * @param[in] character the character to search for.
 * @param[in] limit the number of characters in string from its beginning to search in.
 * @param[out] out_found pointer to storage for a pointer to the found occurrence, or NULL if the character was not found.
 * @return an error code.
 */
GEN_ERRORABLE gen_string_character_last(const char* const restrict string, const size_t string_bound, const char character, const size_t limit, const char* restrict* const restrict out_found);

/**
 * Creates a number from a numeric string.
 * @param[in] string the numeric string to create from.
 * @param[in] string_bound the bounds of the numeric string.
 * @param[out] out_number a pointer to storage for the created number.
 * @return an error code.
 */
GEN_ERRORABLE gen_string_number(const char* const restrict string, const size_t string_bound, const size_t limit, size_t* const restrict out_number);

#endif
