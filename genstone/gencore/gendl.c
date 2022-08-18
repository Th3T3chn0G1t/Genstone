// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gendl.h"

#include "include/gencommon.h"
#include "include/genfs.h"
#include "include/genstring.h"
#include "include/gentooling.h"

gen_error_t gen_dylib_load(gen_dylib_t* const restrict output_dylib, const char* const restrict lib_name) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_dylib_load", (void*) gen_dylib_load, GEN_FILENAME);
	if(error.type) return error;

	// The static analyser has a bit of an aneurism about this function
	// Just ignore it for now

	GEN_NULL_CHECK(output_dylib);
	if(!lib_name) gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`lib_name` was invalid");

	size_t lib_name_length = 0;
	gen_error_t error = gen_string_length(lib_name, GEN_PATH_MAX + 1, GEN_PATH_MAX, &lib_name_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

#if PLATFORM == DWN
	static const char lib_prefix[] = "lib";
	static const char lib_suffix[] = ".dylib";
#else
	static const char lib_prefix[] = "lib";
	static const char lib_suffix[] = ".so";
#endif

	const size_t lib_file_name_length = (sizeof(lib_prefix) - 1) + lib_name_length + (sizeof(lib_suffix) - 1) + 1;
	char* lib_file_name = NULL;
	error = gzalloc((void**) &lib_file_name, lib_file_name_length, sizeof(char));
	gen_error_attach_backtrace_IF(error, "`gzalloc` failed");

	error = gen_string_append(lib_file_name, lib_file_name_length, lib_prefix, sizeof(lib_prefix), sizeof(lib_prefix) - 1);
	if(error != GEN_OK) {
		gen_error_t free_error = gfree(lib_file_name);
		gen_error_attach_backtrace_IF(free_error, "`gfree` failed");

		gen_error_attach_backtrace(error, "`gen_string_append` failed");
	}
	error = gen_string_append(lib_file_name, lib_file_name_length, lib_name, lib_name_length + 1, lib_name_length);
	if(error != GEN_OK) {
		gen_error_t free_error = gfree(lib_file_name);
		gen_error_attach_backtrace_IF(free_error, "`gfree` failed");

		gen_error_attach_backtrace(error, "`gen_string_append` failed");
	}
	error = gen_string_append(lib_file_name, lib_file_name_length, lib_suffix, sizeof(lib_suffix), sizeof(lib_suffix) - 1);
	if(error != GEN_OK) {
		gen_error_t free_error = gfree(lib_file_name);
		gen_error_attach_backtrace_IF(free_error, "`gfree` failed");

		gen_error_attach_backtrace(error, "`gen_string_append` failed");
	}

	if(!(*output_dylib = dlopen(lib_file_name, RTLD_LAZY | RTLD_GLOBAL))) {
		glogf(ERROR, "Failed to load library '%s': %s", lib_file_name, dlerror());
		gen_error_t free_error = gfree(lib_file_name);
		gen_error_attach_backtrace_IF(free_error, "`gfree` failed");

		gen_error_attach_backtrace(GEN_UNKNOWN, "`dlopen` failed");
	}

	gen_error_t free_error = gfree(lib_file_name);
	gen_error_attach_backtrace_IF(free_error, "`gfree` failed");

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_dylib_symbol(void* restrict* const restrict output_address, const gen_dylib_t dylib, const char* const restrict symname) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_dylib_symbol", (void*) gen_dylib_symbol, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(output_address);
	GEN_NULL_CHECK(dylib);
	GEN_NULL_CHECK(symname);

	if(!(*output_address = dlsym(dylib, symname))) {
		glogf(ERROR, "Failed to locate symbol `%s`: %s", symname, dlerror());
		gen_error_attach_backtrace(GEN_UNKNOWN, "`dlsym` failed");
	}

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_dylib_unload(const gen_dylib_t dylib) {
	GEN_NULL_CHECK(dylib);

	if(dlclose(dylib)) {
		glogf(ERROR, "Failed to unload library: %s", dlerror());
		gen_error_attach_backtrace(GEN_UNKNOWN, "`dlclose` failed");
	}

	return (gen_error_t){GEN_OK, ""};
}
