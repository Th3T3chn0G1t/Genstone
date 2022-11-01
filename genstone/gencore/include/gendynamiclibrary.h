// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file gendynamiclibrary.h
 * Utility for managing dynamic libraries.
 */

#ifndef GEN_DYNAMIC_LIBRARY_H
#define GEN_DYNAMIC_LIBRARY_H

#include "gencommon.h"

/**
 * A handle for a dynamically loaded library.
 */
typedef void* gen_dynamic_library_handle_t;

/**
 * Opens a dynamic library handle.
 * @note The library name should be provided as just the significant portion of the name. e.g. to load `libfoo.so`, `library_name` would be `foo`. This is to ensure that the same name can be used to load libraries on multiple platforms (i.e. on macOS `foo` would load `libfoo.dylib`).
 * @param[in] library_name The name of the library to open.
 * @param[in] library_name_length The length of the name of the library to open.
 * @param[out] out_dynamic_library A pointer to storage for the opened dynamic library handle.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_dynamic_library_handle_open(const char* const restrict library_name, const gen_size_t library_name_length, gen_dynamic_library_handle_t* const restrict out_dynamic_library);

/**
 * Closes a dynamic library handle.
 * @param[in,out] dynamic_library The dynamic library handle to unload.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_dynamic_library_handle_close(const gen_dynamic_library_handle_t* const restrict dynamic_library);

/**
 * Gets a symbol's address from a dynamic library.
 * @param[in] dynamic_library The dynamic library to get a symbol from.
 * @param[in] symbol_name The symbol to get.
 * @param[in] symbol_name_length The length of the symbol to get.
 * @param[out] out_address A pointer to storage for the symbol's address.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_dynamic_library_handle_get_symbol(const gen_dynamic_library_handle_t* const restrict dynamic_library, const char* const restrict symbol_name, gen_size_t symbol_name_length, void* restrict* const restrict out_address);

#endif
