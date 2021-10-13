// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/gendl.h"

#include "include/gencommon.h"
#include "include/gentooling.h"

gen_error_t gen_dylib_load(gen_dylib_t* const restrict output_dylib, const char* const restrict lib_name) {
	GEN_FRAME_BEGIN(gen_dylib_load);

	// The static analyser has a bit of an aneurism about this function
	// Just ignore it for now

	if(!output_dylib) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_dylib` was NULL");
	if(!lib_name) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`lib_name` was invalid");

	const size_t lib_name_len = strlen(lib_name);

#if PLATFORM == WIN
	static const char lib_prefix[] = "";
	static const char lib_suffix[] = ".dll";
#elif PLATFORM == DWN
	static const char lib_prefix[] = "lib";
	static const char lib_suffix[] = ".dylib";
#else
	static const char lib_prefix[] = "lib";
	static const char lib_suffix[] = ".so";
#endif

	const size_t lib_file_name_len = (sizeof(lib_prefix) - 1) + lib_name_len + (sizeof(lib_suffix) - 1) + 1;
	char lib_file_name[lib_file_name_len];

	errno_t error = 0;
	error = memcpy_s(lib_file_name, lib_file_name_len, lib_prefix, (sizeof(lib_prefix) - 1));
	if(error) {
		error = errno;
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "`memcpy_s` failed: %s", strerror(error));
#endif
		GEN_ERROR_OUT(gen_convert_errno(error), "`memcpy_s` failed");
	}
	error = memcpy_s(lib_file_name + (sizeof(lib_prefix) - 1), lib_file_name_len - (sizeof(lib_prefix) - 1), lib_name, lib_name_len);
	if(error) {
		error = errno;
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "`memcpy_s` failed: %s", strerror(error));
#endif
		GEN_ERROR_OUT(gen_convert_errno(error), "`memcpy_s` failed");
	}
	error = memcpy_s(lib_file_name + (sizeof(lib_prefix) - 1) + lib_name_len, lib_file_name_len - ((sizeof(lib_prefix) - 1) + lib_name_len), lib_suffix, (sizeof(lib_suffix) - 1));
	if(error) {
		error = errno;
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "`memcpy_s` failed: %s", strerror(error));
#endif
		GEN_ERROR_OUT(gen_convert_errno(error), "`memcpy_s` failed");
	}

	lib_file_name[lib_file_name_len - 1] = '\0';

#if PLATFORM == WIN
	if(!(*output_dylib = LoadLibraryA(lib_file_name))) {
		unsigned long winerror = GetLastError();
#if GEN_GLOGGIFY_EH == ENABLED
		size_t size = 0;
		gen_winerr_as_string(NULL, &size, winerror);
		char winerror_string[size];
		gen_winerr_as_string(winerror_string, 0, winerror);
		glogf(ERROR, "Failed to load library '%s': %s", lib_file_name, winerror_string);
#endif
		GEN_ERROR_OUT(winerror, "`LoadLibraryA` failed");
	}
#else
	if(!(*output_dylib = dlopen(lib_file_name, RTLD_LAZY | RTLD_GLOBAL))) {
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "Failed to load library '%s': %s", lib_file_name, dlerror());
#endif
		GEN_ERROR_OUT(GEN_UNKNOWN, "`dlopen` failed");
	}
#endif

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_dylib_symbol(void* restrict * const restrict output_address, const gen_dylib_t dylib, const char* const restrict symname) {
	if(!output_address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_address` was NULL");
	if(!dylib) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`dylib` was NULL");
	if(!symname) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`symname` was NULL");
	if(!strlen(symname)) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`symname` was invalid (`strlen(symname)` < 0)");

#if PLATFORM == WIN
	GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wpedantic"
	if(!(*output_address = GetProcAddress(dylib, symname))) {
		unsigned long winerror = GetLastError();
#if GEN_GLOGGIFY_EH == ENABLED
		size_t size = 0;
		gen_winerr_as_string(NULL, &size, winerror);
		char winerror_string[size];
		gen_winerr_as_string(winerror_string, 0, winerror);
		glogf(ERROR, "Failed to load library '%s': %s", lib_file_name, winerror_string);
#endif
		GEN_ERROR_OUT(gen_convert_winerr(winerror), "`GetProcAddress` failed");
	}
	GEN_DIAG_REGION_END
#else
	if(!(*output_address = dlsym(dylib, symname))) {
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "Failed to locate symbol `%s`: %s", symname, dlerror());
#endif
		GEN_ERROR_OUT(GEN_UNKNOWN, "`dlsym` failed");
	}
#endif

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_dylib_unload(const gen_dylib_t dylib) {
	if(!dylib) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`dylib` was NULL");

#if PLATFORM == WIN
	if(!FreeLibrary(dylib)) {
		unsigned long winerror = GetLastError();
#if GEN_GLOGGIFY_EH == ENABLED
		size_t size = 0;
		gen_winerr_as_string(NULL, &size, winerror);
		char winerror_string[size];
		gen_winerr_as_string(winerror_string, 0, winerror);
		glogf(ERROR, "Failed to load library '%s': %s", lib_file_name, winerror_string);
#endif
		GEN_ERROR_OUT(gen_convert_winerr(winerror), "`FreeLibrary` failed");
	}
#else
	if(dlclose(dylib)) {
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "Failed to unload library: %s", dlerror());
#endif
		GEN_ERROR_OUT(GEN_UNKNOWN, "`dlclose` failed");
	}
#endif

	GEN_ERROR_OUT(GEN_OK, "");
}
