// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genargs.h
 * Utility for parsing UNIX-style arguments.
 * Defines basic types used for managing the parser.
 */

#ifndef GEN_ARGS_H
#define GEN_ARGS_H

#include "gencommon.h"

/**
 * Argument types to be passed to parser handler.
 */
typedef enum
{
	/**
     * A short argument.
     */
	GEN_ARG_SHORT,
	/**
     * A long argument.
     */
	GEN_ARG_LONG,
	/**
     * A raw argument.
     */
	GEN_ARG_RAW
} gen_arg_type_t;

/**
 * Handler for parsed arguments.
 */
typedef void (*gen_arg_handler_t)(const gen_arg_type_t, const size_t, const char* const restrict, void* const restrict);

/**
 * Parses UNIX-style arguments into an easy-to-interpret format for a handler.
 * Similar to the argp parsing library.
 * @param[in] argc the number of arguments to parse.
 * @param[in] argv an array of arguments.
 * @param[in] handler the handler to be called for each parsed argument.
 * @param[in] short_args_length the number of short arguments.
 * @param[in] short_args an array of short arguments.
 * @param[in] long_args_length the number of long arguments.
 * @param[in] long_args an array of long arguments.
 * @param[in] passthrough a passthrough argument for the handler.
 * @return an error code.
 */
GEN_ERRORABLE gen_parse_args(const int argc, const char* const restrict* const restrict argv, const gen_arg_handler_t handler, const size_t short_args_length, const char* const restrict short_args, const size_t long_args_length, const char* const restrict* const restrict long_args, void* const restrict passthrough);
/**
 * @example{lineno} example/gencore/gen_parse_args.c
 * Example for how to use `gen_parse_args`.
 * `gen_parse_args` is used to manage arguments using a nicer API than manual string manipulation, using a similar API to the argp argument parser.
 */

#endif
