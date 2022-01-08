// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gendl.h"

#include "include/gencommon.h"
#include "include/genfs.h"
#include "include/genstring.h"
#include "include/gentooling.h"

gen_error_t gen_dylib_load(gen_dylib_t* const restrict output_dylib, const char* const restrict lib_name) {
	GEN_FRAME_BEGIN(gen_dylib_load);

	// The static analyser has a bit of an aneurism about this function
	// Just ignore it for now

	GEN_INTERNAL_BASIC_PARAM_CHECK(output_dylib);
	if(!lib_name) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`lib_name` was invalid");

	size_t lib_name_len = 0;
	gen_error_t error = gen_string_length(lib_name, GEN_PATH_MAX + 1, &lib_name_len);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

#if PLATFORM == DWN
	static const char lib_prefix[] = "lib";
	static const char lib_suffix[] = ".dylib";
#else
	static const char lib_prefix[] = "lib";
	static const char lib_suffix[] = ".so";
#endif

	const size_t lib_file_name_len = (sizeof(lib_prefix) - 1) + lib_name_len + (sizeof(lib_suffix) - 1) + 1;
	char lib_file_name[lib_file_name_len];
	memset(lib_file_name, 0, lib_file_name_len);

	error = gen_string_append(lib_file_name, lib_file_name_len, lib_prefix, sizeof(lib_prefix), sizeof(lib_prefix) - 1);
	GEN_ERROR_OUT_IF(error, "`gen_string_append` failed");
	error = gen_string_append(lib_file_name, lib_file_name_len, lib_name, lib_name_len + 1, lib_name_len);
	GEN_ERROR_OUT_IF(error, "`gen_string_append` failed");
	error = gen_string_append(lib_file_name, lib_file_name_len, lib_suffix, sizeof(lib_suffix), sizeof(lib_suffix) - 1);
	GEN_ERROR_OUT_IF(error, "`gen_string_append` failed");

	if(!(*output_dylib = dlopen(lib_file_name, RTLD_LAZY | RTLD_GLOBAL))) {
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "Failed to load library '%s': %s", lib_file_name, dlerror());
#endif
		GEN_ERROR_OUT(GEN_UNKNOWN, "`dlopen` failed");
	}

	GEN_ALL_OK;
}

gen_error_t gen_dylib_symbol(void* restrict* const restrict output_address, const gen_dylib_t dylib, const char* const restrict symname) {
	GEN_FRAME_BEGIN(gen_dylib_symbol);

	GEN_INTERNAL_BASIC_PARAM_CHECK(output_address);
	GEN_INTERNAL_BASIC_PARAM_CHECK(dylib);
	GEN_INTERNAL_BASIC_PARAM_CHECK(symname);

	if(!(*output_address = dlsym(dylib, symname))) {
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "Failed to locate symbol `%s`: %s", symname, dlerror());
#endif
		GEN_ERROR_OUT(GEN_UNKNOWN, "`dlsym` failed");
	}

	GEN_ALL_OK;
}

gen_error_t gen_dylib_unload(const gen_dylib_t dylib) {
	GEN_INTERNAL_BASIC_PARAM_CHECK(dylib);

	if(dlclose(dylib)) {
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "Failed to unload library: %s", dlerror());
#endif
		GEN_ERROR_OUT(GEN_UNKNOWN, "`dlclose` failed");
	}

	GEN_ALL_OK;
}
