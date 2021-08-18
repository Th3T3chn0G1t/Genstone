// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file gendl.h
 * Utility for managing dynamic libraries
 */

#ifndef GEN_DL_H
#define GEN_DL_H

#include "generrors.h"

#if PLATFORM == WIN
#include <Windows.h>

typedef HMODULE gen_dylib_t;
#else
#include <dlfcn.h>

/**
 * A handle for a dynamically loaded library
 */
typedef void* gen_dylib_t;
#endif

/**
 * Loads a dynamic library
 * @param output_dylib a pointer to storage for the loaded dylib handle
 * @param lib_name the name of the library to load
 * @return an error code
 * @note The library name should be provided as just the significant portion of the name. e.g. to load `libfoo.so`, `lib_name` would be `foo`. This is to ensure that the same name can be used to load libraries on multiple platforms (i.e. on Windows `foo` would load `foo.dll`)
 */
extern gen_error_t gen_dylib_load(gen_dylib_t* restrict output_dylib, const char* const restrict lib_name);
/**
 * Gets a symbol's address from a loaded dynamic library
 * @param output_address a pointer to storage for the loaded symbol address
 * @param dylib the library to get the symbol from
 * @param symname the name of the symbol to get
 * @return an error code
 */
extern gen_error_t gen_dylib_symbol(void* restrict * const restrict output_address, const gen_dylib_t dylib, const char* const restrict symname);
/**
 * Unloads a dynamic library
 * @param dylib the library to unload
 * @note This may not immediately remove the library from memory depending on the dynamic linker and program state 
 */
extern gen_error_t gen_dylib_unload(const gen_dylib_t dylib);

#endif