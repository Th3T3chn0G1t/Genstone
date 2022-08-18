// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genargs.h
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
      * 
      */
     size_t* short_argument_indices;
     /**
      * 
      */
     const char** short_argument_parameters;
     /**
      * 
      */
     size_t short_argument_count;

     /**
      * 
      */
     size_t* long_argument_indices;
     /**
      * 
      */
     const char** long_argument_parameters;
     /**
      * 
      */
     size_t long_argument_count;

     /**
      * 
      */
     size_t* raw_argument_indices;
     /**
      * 
      */
     size_t raw_argument_count;
} gen_arguments_parsed_t;

/**
 * Parses UNIX-style arguments.
 * @param[in] arguments An array of arguments.
 * @param[in] argument_count The number of arguments in `arguments`.
 * @param[in] short_arguments An array of short arguments.
 * @param[in] short_argument_count The number of short arguments in `short_arguments`.
 * @param[in] long_arguments An array of long arguments.
 * @param[in] long_argument_lengths An array of lengths for the long arguments.
 * @param[in] long_argument_count The number of long arguments in `long_arguments` and the number of lengths in `long_arguments_lengths`.
 * @param[out] out_parsed A pointer to storage for the parsed arguments' information. Should be passed to `gen_arguments_free_parsed` once used.
 * @return An error code.
 */
extern gen_error_t gen_arguments_parse(const char* const restrict* const restrict arguments, const size_t argument_count, const char* const restrict short_arguments, const size_t short_argument_count, const char* const restrict * const restrict long_arguments, const size_t long_argument_lengths, const size_t long_argument_count, gen_arguments_parsed_t* const restrict out_parsed);

#endif
