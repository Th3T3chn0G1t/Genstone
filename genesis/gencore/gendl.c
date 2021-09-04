// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/gendl.h"

#include "include/gencommon.h"
#include "include/gentooling.h"

GEN_ERRORABLE_RETURN gen_dylib_load(gen_dylib_t* const restrict output_dylib, const char* const restrict lib_name) {
	GEN_FRAME_BEGIN(gen_dylib_load);

	// The static analyser has a bit of an aneurism about this function
	// Just ignore it for now

	if(!output_dylib) return GEN_INVALID_PARAMETER;
	if(!lib_name) return GEN_INVALID_PARAMETER;

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
	if(error) return gen_convert_errno(error);
	error = memcpy_s(lib_file_name + (sizeof(lib_prefix) - 1), lib_file_name_len - (sizeof(lib_prefix) - 1), lib_name, lib_name_len);
	if(error) return gen_convert_errno(error);
	error = memcpy_s(lib_file_name + (sizeof(lib_prefix) - 1) + lib_name_len, lib_file_name_len - ((sizeof(lib_prefix) - 1) + lib_name_len), lib_suffix, (sizeof(lib_suffix) - 1));
	if(error) return gen_convert_errno(error);

	lib_file_name[lib_file_name_len - 1] = '\0';

#if PLATFORM == WIN
	if(!(*output_dylib = LoadLibraryA(lib_file_name))) {
		return gen_convert_winerr(GetLastError());
	}
#else
	if(!(*output_dylib = dlopen(lib_file_name, 0))) {
		glogf(ERROR, "Failed to load library %s: %s", lib_file_name, dlerror());
		return gen_convert_errno(errno);
	}
#endif

	return GEN_OK;
}

GEN_ERRORABLE_RETURN gen_dylib_symbol(void* restrict * const restrict output_address, const gen_dylib_t dylib, const char* const restrict symname) {
	if(!output_address) return GEN_INVALID_PARAMETER;
	if(!dylib) return GEN_INVALID_PARAMETER;
	if(!symname) return GEN_INVALID_PARAMETER;

#if PLATFORM == WIN
	if(!(*output_address = GetProcAddress(dylib, symname))) {
		return gen_convert_winerr(GetLastError());
	}
#else
	if(!(*output_address = dlsym(dylib, symname))) {
		glogf(ERROR, "Failed to locate symbol %s: %s", symname, dlerror());
		return gen_convert_errno(errno);
	}
#endif

	return GEN_OK;
}

GEN_ERRORABLE_RETURN gen_dylib_unload(const gen_dylib_t dylib) {
	if(!dylib) return GEN_INVALID_PARAMETER;

#if PLATFORM == WIN
	if(!FreeLibrary(dylib)) {
		return gen_convert_winerr(GetLastError());
	}
#else
	if(dlclose(dylib)) {
		glogf(ERROR, "Failed to unload library: %s", dlerror());
		return gen_convert_errno(errno);
	}
#endif

	return GEN_OK;
}
