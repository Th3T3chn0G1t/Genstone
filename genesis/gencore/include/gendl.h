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

extern gen_error_t gen_dylib_load(gen_dylib_t* output_dylib, const char* lib_name);
extern gen_error_t gen_dylib_symbol(void** output_address, const gen_dylib_t dylib, const char* symname);
extern gen_error_t gen_dylib_unload(const gen_dylib_t dylib);

#endif
