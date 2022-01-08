// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gendl.h
 * Utility for managing dynamic libraries.
 */

#ifndef GEN_DL_H
#define GEN_DL_H

#include "gencommon.h"

#include <dlfcn.h>

/**
 * A handle for a dynamically loaded library.
 */
typedef void* gen_dylib_t;

/**
 * @example{lineno} example/gencore/gen_dylib.c
 * Example for how to use `gen_dylib_load` `gen_dylib_symbol` and `gen_dylib_unload`.
 * The `gen_dylib*` family of functions is used to manage dynamic libraries in a platform agnostic way.
 */

/**
 * Loads a dynamic library.
 * @param[out] output_dylib a pointer to storage for the loaded dylib handle.
 * @param[in] lib_name the name of the library to load.
 * @return an error code.
 * @note The library name should be provided as just the significant portion of the name. e.g. to load `libfoo.so`, `lib_name` would be `foo`. This is to ensure that the same name can be used to load libraries on multiple platforms (i.e. on macOS `foo` would load `libfoo.dylib`).
 */
GEN_ERRORABLE gen_dylib_load(gen_dylib_t* restrict output_dylib, const char* const restrict lib_name);
/**
 * Gets a symbol's address from a loaded dynamic library.
 * @param[out] output_address a pointer to storage for the loaded symbol address.
 * @param[in] dylib the library to get the symbol from.
 * @param[in] symname the name of the symbol to get.
 * @return an error code.
 */
GEN_ERRORABLE gen_dylib_symbol(void* restrict* const restrict output_address, const gen_dylib_t dylib, const char* const restrict symname);
/**
 * Unloads a dynamic library.
 * @param[in] dylib the library to unload.
 * @return an error code.
 * @note This may not immediately remove the library from memory depending on the dynamic linker and program state.
 */
GEN_ERRORABLE gen_dylib_unload(const gen_dylib_t dylib);

#endif
