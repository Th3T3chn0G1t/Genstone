// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genarguments.h
 * Utility for parsing UNIX-style arguments.
 * Defines basic types used for managing the parser.
 */

#ifndef GEN_ARGUMENTS_H
#define GEN_ARGUMENTS_H

#include "gencommon.h"

/**
 * Parsed out argument information.
 */
typedef struct {
    /**
     * Indices into the `short_arguments` array passed to `gen_arguments_parse` parsed out of the argument array.
     */
    size_t* short_argument_indices;

    /**
     * Parameters to short arguments in `short_argument_indices` parsed out of the argument array passed to `gen_arguments_parse`.
     * Each entry corresponds to an entry in `short_argument_indices` - for arguments which recieved no parameter, the entry will be `NULL`.
     */
    const char** short_argument_parameters;

    /**
     * The lengths of parameters in `short_argument_parameters`.
     */
    size_t* short_argument_parameter_lengths;

    /**
     * The number of indices in `short_argument_indices`, the number of parameters in `short_argument_parameters`, and the number of lengths in `short_argument_parameter_lengths`.
     */
    size_t short_argument_count;

    /**
     * Indices into the `long_arguments` array passed to `gen_arguments_parse` parsed out of the argument array.
     */
    size_t* long_argument_indices;

    /**
     * Parameters to short arguments in `long_argument_indices` parsed out of the argument array passed to `gen_arguments_parse`.
     * Each entry corresponds to an entry in `long_argument_indices` - for arguments which recieved no parameter, the entry will be `NULL`.
     */
    const char** long_argument_parameters;

    /**
     * The lengths of parameters in `long_argument_parameters`.
     */
    size_t* long_argument_parameter_lengths;

    /**
     * The number of indices in `long_argument_indices`, the number of parameters in `long_argument_parameters`, and the number of lengths in `long_argument_parameter_lengths`.
     */
    size_t long_argument_count;

    /**
     * Indices into the argument array passed to `gen_arguments_parse` for raw arguments.
     */
    size_t* raw_argument_indices;

    /**
     * The number of indices in raw_argument_indices.
     */
    size_t raw_argument_count;
} gen_arguments_parsed_t;

/**
 * Parses UNIX-style arguments.
 * @param[in] arguments An array of arguments.
 * @param[in] argument_lengths An array of lengths for `arguments`.
 * @param[in] argument_count The number of arguments in `arguments`, and the number of lengths in `argument_lengths`.
 * @param[in] short_arguments An array of short arguments.
 * @param[in] short_argument_count The number of short arguments in `short_arguments`.
 * @param[in] long_arguments An array of long arguments.
 * @param[in] long_argument_lengths An array of lengths for `long_arguments`.
 * @param[in] long_argument_count The number of long arguments in `long_arguments` and the number of lengths in `long_arguments_lengths`.
 * @param[out] out_parsed A pointer to storage for the parsed arguments' information. Should be passed to `gen_arguments_free_parsed` once used.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_arguments_parse(const char* const restrict* const restrict arguments, const size_t* const restrict argument_lengths, const size_t argument_count, const char* const restrict short_arguments, const size_t short_argument_count, const char* const restrict * const restrict long_arguments, const size_t* const restrict long_argument_lengths, const size_t long_argument_count, gen_arguments_parsed_t* const restrict out_parsed);

/**
 * Frees parsed arguments' information.
 * @param[in,out] parsed The parsed arguments' information to free.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_arguments_free_parsed(gen_arguments_parsed_t* const restrict parsed);

#endif
