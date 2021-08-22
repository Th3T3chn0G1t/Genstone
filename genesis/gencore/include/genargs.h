// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file genargs.h
 * Utility for parsing UNIX-style arguments
 * Defines basic types used for managing the parser
 */

#ifndef GEN_ARGS_H
#define GEN_ARGS_H

#include "generrors.h"
#include "gencommon.h"

/**
 * Argument types to be passed to parser handler
 */
typedef enum {
    /**
     * A short argument
     */
    GEN_ARG_SHORT,
    /**
     * A long argument
     */
    GEN_ARG_LONG,
    /**
     * A raw argument
     */
    GEN_ARG_RAW
} gen_arg_type_t;

/**
 * Handler for parsed arguments
 */
typedef void (*gen_arg_handler_t)(const gen_arg_type_t, const size_t, const char* const restrict, void* restrict);

/**
 * Parses UNIX-style arguments into an easy-to-interpret format for a handler
 * Similar to the argp parsing library
 * @param argc the number of arguments to parse
 * @param argv an array of arguments
 * @param handler the handler to be called for each parsed argument
 * @param n_short_args the number of short arguments
 * @param short_args an array of short arguments
 * @param n_long_args the number of long arguments
 * @param long_args an array of long arguments
 * @param passthrough a passthrough argument for the handler
 * @return an error code
 */
extern gen_error_t gen_parse_args(const int argc, const char* const restrict * const restrict argv, const gen_arg_handler_t handler, const size_t n_short_args, const char* const restrict short_args, const size_t n_long_args, const char* const restrict * const restrict long_args, void* restrict passthrough);

#endif
