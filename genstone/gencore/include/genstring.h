// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_STRING_H
#define GEN_STRING_H

#include "gencommon.h"

/**
 * A value which can be used in place of string bounds in the event that no bounding should be performed.
 */
#define GEN_STRING_NO_BOUNDS GEN_SIZE_MAX

/**
 * Compares 2 strings.
 * @param[in] a The first string.
 * @param[in] a_length The length of `a`.
 * @param[in] a_bounds The bounds of `a`.
 * @param[in] b The second string.
 * @param[in] b_length The length of `b`.
 * @param[in] b_bounds The bounds of `b`.
 * @param[in] count The number of characters to compare.
 * @param[out] out_equal Pointer to storage for the result of the comparison.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_compare(const char* const restrict a, const gen_size_t a_bounds, const gen_size_t a_length, const char* const restrict b, const gen_size_t b_bounds, const gen_size_t b_length, const gen_size_t count, gen_bool_t* const restrict out_equal);

/**
 * Copies the contents of a string into another.
 * @param[in] destination The destination for the copied data.
 * @param[in] destination_bounds The bounds of `destination`.
 * @param[in] source The string to copy.
 * @param[in] source_bounds The bounds of `source`.
 * @param[in] source_length The length of `source`.
 * @param[in] count The number of characters to copy.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_copy(char* const restrict destination, const gen_size_t destination_bounds, const char* const restrict source, const gen_size_t source_bounds, const gen_size_t source_length, const gen_size_t count);

/**
 * Appends one string to another.
 * @param[in] destination The destination to append to.
 * @param[in] destination_bounds The bounds of `destination`.
 * @param[in] destination_length The length of `destination`.
 * @param[in] source The string to append.
 * @param[in] source_bounds The bounds of `source`.
 * @param[in] source_length The length of `source`.
 * @param[in] count The number of characters to append.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_append(char* const restrict destination, const gen_size_t destination_bounds, const gen_size_t destination_length, const char* const restrict source, const gen_size_t source_bounds, const gen_size_t source_length, const gen_size_t count);

/**
 * Gets the length of a string.
 * @param[in] string The string whose length should be found.
 * @param[in] string_bounds The bounds of `string`.
 * @param[in] limit The maximum length to output.
 * @param[out] out_length A pointer to storage for the length of the string.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_length(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t limit, gen_size_t* const restrict out_length);

/**
 * Duplicates a string onto the heap.
 * @param[in] string The string to duplicate.
 * @param[in] string_bounds The bounds of `string`.
 * @param[in] string_length The length of `string`.
 * @param[in] count The number of characters to duplicate.
 * @param[out] out_duplicated A pointer to storage for a pointer to the duplicated string. Must be freed.
 * @param[out] out_length A pointer to storage for the length of the duplicated string.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_duplicate(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const gen_size_t count, char* restrict* const restrict out_duplicated, gen_size_t* const restrict out_length);

/**
 * Finds the first occurrence of a character in a string.
 * @param[in] string The string to search in.
 * @param[in] string_bounds The bounds of `string`.
 * @param[in] string_length The length of `string`.
 * @param[in] character The character to search for.
 * @param[in] count The number of characters in string from its beginning to search in.
 * @param[out] out_found A pointer to storage for the index of the found occurrence, or `GEN_SIZE_MAX` if none was found.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_character_first(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const char character, const gen_size_t count, gen_size_t* const restrict out_found);

/**
 * Finds the last occurrence of a character in a string.
 * @param[in] string The string to search in.
 * @param[in] string_bounds The bounds of `string`.
 * @param[in] string_length The length of `string`.
 * @param[in] character The character to search for.
 * @param[in] count The number of characters in string from its beginning to search in.
 * @param[out] out_found A pointer to storage for the index of the found occurrence, or `GEN_SIZE_MAX` if none was found.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_character_last(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const char character, const gen_size_t count, gen_size_t* const restrict out_found);

/**
 * Creates a number from a numeric string.
 * @param[in] string The numeric string to create from.
 * @param[in] string_bounds The bounds of `string`.
 * @param[in] string_length The length of `string`.
 * @param[in] count The number of characters to process.
 * @param[out] out_number A pointer to storage for the created number.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_number(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const gen_size_t count, gen_size_t* const restrict out_number);

/**
 * Formats a string to a buffer.
 * @param[in] limit The maximum number of chars to format.
 * @param[out] out_buffer A pointer to storage for the formatted string.
 * @param[out] out_length A pointer to storage for the length of the formatted string.
 * @param[in] format The format specification to apply format arguments to.
 * @param[in] format_bounds The bounds of `format`.
 * @param[in] format_length The length of `format`.
 * @param[in] ... The format arguments to apply to `format`.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_format(const gen_size_t limit, char* const restrict out_buffer, gen_size_t* out_length, const char* const restrict format, const gen_size_t format_bounds, const gen_size_t format_length, ...);

/**
 * Formats a string to a buffer.
 * @param[in] limit The maximum number of chars to format.
 * @param[out] out_buffer A pointer to storage for the formatted string.
 * @param[out] out_length A pointer to storage for the length of the formatted string.
 * @param[in] format The format specification to apply format arguments to.
 * @param[in] format_bounds The bounds of `format`.
 * @param[in] format_length The length of `format`.
 * @param[in] list The format arguments to apply to `format`.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_formatv(const gen_size_t limit, char* const restrict out_buffer, gen_size_t* const restrict out_length, const char* const restrict format, const gen_size_t format_bounds, const gen_size_t format_length, gen_variadic_list_t list);

/**
 * Searches a string for another string.
 * @param[in] string The string to search in.
 * @param[in] string_bounds The bounds of `string`.
 * @param[in] string_length The length of `string`.
 * @param[in] target The string to search for.
 * @param[in] target_bounds The bounds of `target`.
 * @param[in] target_length The length of `target`.
 * @param[in] count The number of characters to search in.
 * @param[out] out_offset A pointer to storage for the index of the ocurrence, otherwise `GEN_SIZE_MAX`.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_string_contains(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const char* const restrict target, const gen_size_t target_bounds, const gen_size_t target_length, const gen_size_t count, gen_size_t* const restrict out_offset);

#endif
