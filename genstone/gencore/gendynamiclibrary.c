// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gendynamiclibrary.h"

#include "include/genmemory.h"
#include "include/genstring.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#if GEN_PLATFORM != GEN_WINDOWS
#include <dlfcn.h>
#endif
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

gen_error_t gen_dynamic_library_handle_open(const char* const restrict library_name, const size_t library_name_length, gen_dynamic_library_handle_t* const restrict out_dynamic_library) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_dynamic_library_handle_open, GEN_FILE_NAME);
	if(error.type) return error;

	if(!library_name) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`library_name` was `NULL`");
	if(!out_dynamic_library) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_dynamic_library` was `NULL`");

	if(library_name[library_name_length] != '\0') return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`library_file_name` was not NULL-terminated");

#if GEN_PLATFORM == GEN_LINUX
	static const char library_prefix[] = "lib";
	static const char library_suffix[] = ".so";
#elif GEN_PLATFORM == GEN_OSX
	static const char library_prefix[] = "lib";
	static const char library_suffix[] = ".dylib";
#elif GEN_PLATFORM == GEN_WINDOWS
	static const char library_prefix[] = "";
	static const char library_suffix[] = ".dll";
#endif

	char* library_file_name = NULL;
	const size_t library_file_name_length = (sizeof(library_prefix) - 1) + library_name_length + (sizeof(library_suffix) - 1);

	error = gen_memory_allocate_zeroed((void**) &library_file_name, library_file_name_length + 1, sizeof(char));
	if(error.type) return error;

	error = gen_string_append(library_file_name, library_file_name_length + 1, library_prefix, sizeof(library_prefix), sizeof(library_prefix) - 1);
	if(error.type) {
		gen_error_t free_error = gen_memory_free((void**) &library_file_name);
		if(free_error.type) return free_error;

		return error;
	}
	error = gen_string_append(library_file_name, library_file_name_length + 1, library_name, library_name_length + 1, library_name_length);
	if(error.type) {
		gen_error_t free_error = gen_memory_free((void**) &library_file_name);
		if(free_error.type) return free_error;

		return error;
	}
	error = gen_string_append(library_file_name, library_file_name_length, library_suffix, sizeof(library_suffix), sizeof(library_suffix) - 1);
	if(error.type) {
		gen_error_t free_error = gen_memory_free((void**) &library_file_name);
		if(free_error.type) return free_error;

		return error;
	}

	if(!(*out_dynamic_library = dlopen(library_file_name, RTLD_LAZY | RTLD_GLOBAL))) {
		error = gen_memory_free((void**) &library_file_name);
		if(error.type) return error;

		return gen_error_attach_backtrace_formatted(GEN_ERROR_UNKNOWN, GEN_LINE_NUMBER, "Failed to open library `%zs`: %s", library_file_name, library_file_name_length, dlerror());
	}

	error = gen_memory_free((void**) &library_file_name);
	if(error.type) return error;

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_dynamic_library_handle_close(const gen_dynamic_library_handle_t* const restrict dynamic_library) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_dynamic_library_handle_close, GEN_FILE_NAME);
	if(error.type) return error;

	if(!dynamic_library) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`dynamic_library` was `NULL`");

	if(dlclose(*dynamic_library)) {
		return gen_error_attach_backtrace_formatted(GEN_ERROR_UNKNOWN, GEN_LINE_NUMBER, "Failed to close library: %s", dlerror());
	}

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_dynamic_library_handle_get_symbol(const gen_dynamic_library_handle_t* const restrict dynamic_library, const char* const restrict symbol_name, size_t symbol_name_length, const void* restrict* const restrict out_address) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_dynamic_library_handle_get_symbol, GEN_FILE_NAME);
	if(error.type) return error;

	if(!dynamic_library) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`dynamic_library` was `NULL`");
	if(!symbol_name) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`symbol_name` was `NULL`");
	if(!out_address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_address` was `NULL`");

	if(symbol_name[symbol_name_length] != '\0') return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`symbol_name` was not NULL-terminated");

	if(!(*out_address = dlsym(*dynamic_library, symbol_name))) {
		return gen_error_attach_backtrace_formatted(GEN_ERROR_UNKNOWN, GEN_LINE_NUMBER, "Failed to locate symbol `%zs`: %s", symbol_name, symbol_name_length, dlerror());
	}

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}
