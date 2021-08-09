// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file genargs.h
 * Utility for parsing UNIX-style arguments
 * Defines basic types used for managing the parser
 * @note does not use stdlib
 */

#ifndef GEN_ARGS_H
#define GEN_ARGS_H

/**
 * Return values for gen_process_args
 */
typedef enum {
    /**
     * Arguments were parsed with no errors
     */
    GEN_ARG_OK = 0,
    /**
     * A nonexistent argument was passed
     */
    GEN_ARG_NO_EXIST
} gen_arg_error_t;

/**
 * Argument types to be passed to parser callback
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
typedef void (*gen_arg_handler_t)(const gen_arg_type_t, const unsigned long, const char*, void*);

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
extern gen_arg_error_t gen_parse_args(const int argc, const char** argv, const gen_arg_handler_t handler, unsigned long n_short_args, char* short_args, unsigned long n_long_args, char** long_args, void* passthrough);

#endif
