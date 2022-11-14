// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <gendynamiclibrary.h>
#include <genstring.h>
#include <genmemory.h>

#include <gendynamiclibrary_be.h>
#include "include/gendynamiclibrary_be.h"

#include <gendynamiclibrary_be_libname.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <dlfcn.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

static void gen_backends_dynamic_library_internal_handle_open_cleanup_library_file_name(char** library_name) {
    gen_error_t* error = gen_memory_free((void**) library_name);
	if(error) gen_error_abort_with_error(error, "gendynamiclibrary");
}

extern gen_error_t* gen_backends_unix_dynamic_library_handle_open(const char* const restrict library_name, const gen_size_t library_name_bounds, GEN_UNUSED const gen_size_t library_name_length, gen_dynamic_library_handle_t* const restrict out_dynamic_library);
gen_error_t* gen_backends_unix_dynamic_library_handle_open(const char* const restrict library_name, const gen_size_t library_name_bounds, GEN_UNUSED const gen_size_t library_name_length, gen_dynamic_library_handle_t* const restrict out_dynamic_library) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_dynamic_library_handle_open, GEN_FILE_NAME);
    if(error) return error;

    // TODO: Library versioning
    // TODO: "Version" structure API

	gen_size_t library_file_name_length = 0;
    error = gen_string_format(GEN_STRING_NO_BOUNDS, GEN_NULL, &library_file_name_length, "%tz%tz%tz", sizeof("%tz%tz%tz"), sizeof("%tz%tz%tz") - 1,
            gen_backends_dynamic_library_name_prefix, sizeof(gen_backends_dynamic_library_name_prefix),
            library_name, library_name_bounds,
            gen_backends_dynamic_library_name_suffix, sizeof(gen_backends_dynamic_library_name_suffix));
	if(error) return error;
    
	GEN_CLEANUP_FUNCTION(gen_backends_dynamic_library_internal_handle_open_cleanup_library_file_name) char* library_file_name = GEN_NULL;
    error = gen_memory_allocate_zeroed((void**) &library_file_name, library_file_name_length + 1, sizeof(char));
	if(error) return error;

    error = gen_string_format(GEN_STRING_NO_BOUNDS, library_file_name, GEN_NULL, "%tz%tz%tz", sizeof("%tz%tz%tz"), sizeof("%tz%tz%tz") - 1,
            gen_backends_dynamic_library_name_prefix, sizeof(gen_backends_dynamic_library_name_prefix),
            library_name, library_name_bounds,
            gen_backends_dynamic_library_name_suffix, sizeof(gen_backends_dynamic_library_name_suffix));
	if(error) return error;

    void* handle = dlopen(library_file_name, RTLD_LAZY | RTLD_GLOBAL);
	if(!handle) return gen_error_attach_backtrace_formatted(GEN_ERROR_UNKNOWN, GEN_LINE_NUMBER, "Failed to open library `%tz`: %t", library_file_name, library_file_name_length, dlerror());

    out_dynamic_library->native->handle = handle;

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_dynamic_library_handle_close(const gen_dynamic_library_handle_t* const restrict dynamic_library);
gen_error_t* gen_backends_unix_dynamic_library_handle_close(const gen_dynamic_library_handle_t* const restrict dynamic_library) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_dynamic_library_handle_close, GEN_FILE_NAME);
    if(error) return error;    

	int result = dlclose(dynamic_library->native->handle);
    if(result == -1) return gen_error_attach_backtrace_formatted(GEN_ERROR_UNKNOWN, GEN_LINE_NUMBER, "Failed to close library: %t", dlerror());

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_dynamic_library_handle_get_symbol(const gen_dynamic_library_handle_t* const restrict dynamic_library, const char* const restrict symbol_name, GEN_UNUSED const gen_size_t symbol_name_bounds, const gen_size_t symbol_name_length, void* restrict* const restrict out_address);
gen_error_t* gen_backends_unix_dynamic_library_handle_get_symbol(const gen_dynamic_library_handle_t* const restrict dynamic_library, const char* const restrict symbol_name, GEN_UNUSED const gen_size_t symbol_name_bounds, const gen_size_t symbol_name_length, void* restrict* const restrict out_address) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_dynamic_library_handle_get_symbol, GEN_FILE_NAME);
    if(error) return error;

    dlerror(); // Clear the `dlfcn` error buffer.
    void* address = dlsym(dynamic_library->native->handle, symbol_name);
    char* result = dlerror();
    if(result) return gen_error_attach_backtrace_formatted(GEN_ERROR_NO_SUCH_OBJECT, GEN_LINE_NUMBER, "Failed to locate symbol `%tz`: %t", symbol_name, symbol_name_length, result);

    *out_address = address;

    return GEN_NULL;
}

