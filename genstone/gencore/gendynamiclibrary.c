// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/gendynamiclibrary.h"

#include "include/genfilesystem.h"
#include "include/genmemory.h"
#include "include/genstring.h"

#include <genbackends.h>
#include <gendynamiclibrary_be.h>

static void gen_dynamic_library_internal_handle_open_cleanup_native(gen_backends_dynamic_library_handle_t** native) {
    gen_error_t* error = gen_memory_free((void**) native);
    if(error) gen_error_abort_with_error(error, "gendynamiclibrary");
}

gen_error_t* gen_dynamic_library_handle_open(const char* const restrict library_name, const gen_size_t library_name_bounds, const gen_size_t library_name_length, gen_dynamic_library_handle_t* const restrict out_dynamic_library) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_dynamic_library_handle_open, GEN_FILE_NAME);
	if(error) return error;

	if(!library_name) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`library_name` was `GEN_NULL`");
	if(!out_dynamic_library) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_dynamic_library` was `GEN_NULL`");

    // Just taking the length of the string is a decent way to check that it's valid
    // As we presume the genstring backend takes responsibility for string bounding/length validity.
    gen_size_t library_name_length_checked = 0;
    error = gen_string_length(library_name, library_name_bounds, library_name_length, &library_name_length_checked);
	if(error) return error;

    gen_bool_t valid = gen_false;
    error = gen_filesystem_path_validate(library_name, library_name_bounds, library_name_length_checked, &valid);
	if(error) return error;
    if(!valid) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`library_name` `%tz` was invalid", library_name, library_name_bounds);

    error = gen_memory_allocate_zeroed((void**) &out_dynamic_library->native, 1, sizeof(gen_backends_dynamic_library_handle_t));
	if(error) return error;

    GEN_CLEANUP_FUNCTION(gen_dynamic_library_internal_handle_open_cleanup_native) gen_backends_dynamic_library_handle_t* native_scope_variable = out_dynamic_library->native;

    error = GEN_BACKENDS_CALL(dynamic_library_handle_open)(library_name, library_name_bounds, library_name_length_checked, out_dynamic_library);
	if(error) return error;

    native_scope_variable = GEN_NULL;

    return GEN_NULL;
}

gen_error_t* gen_dynamic_library_handle_close(gen_dynamic_library_handle_t* const restrict dynamic_library) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_dynamic_library_handle_close, GEN_FILE_NAME);
	if(error) return error;

	if(!dynamic_library) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`dynamic_library` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(dynamic_library_handle_close)(dynamic_library);
	if(error) return error;

    error = gen_memory_free((void**) &dynamic_library->native);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_dynamic_library_handle_get_symbol(const gen_dynamic_library_handle_t* const restrict dynamic_library, const char* const restrict symbol_name, const gen_size_t symbol_name_bounds, const gen_size_t symbol_name_length, void* restrict* const restrict out_address) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_dynamic_library_handle_get_symbol, GEN_FILE_NAME);
	if(error) return error;

	if(!dynamic_library) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`dynamic_library` was `GEN_NULL`");
	if(!symbol_name) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`symbol_name` was `GEN_NULL`");
	if(!out_address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_address` was `GEN_NULL`");

    gen_size_t symbol_name_length_checked = 0;
    error = gen_string_length(symbol_name, symbol_name_bounds, symbol_name_length, &symbol_name_length_checked);
	if(error) return error;

    error = GEN_BACKENDS_CALL(dynamic_library_handle_get_symbol)(dynamic_library, symbol_name, symbol_name_bounds, symbol_name_length_checked, out_address);
	if(error) return error;

	return GEN_NULL;
}
