// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/gendl.h"
#include "include/gencommon.h"

gen_error_t gen_dylib_load(gen_dylib_t* output_dylib, const char* lib_name) {
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
    memcpy(lib_file_name, lib_prefix, (sizeof(lib_prefix) - 1));
    memcpy(lib_file_name + (sizeof(lib_prefix) - 1), lib_name, lib_name_len);
    memcpy(lib_file_name + (sizeof(lib_prefix) - 1) + lib_name_len, lib_suffix, (sizeof(lib_suffix) - 1));
    lib_file_name[lib_file_name_len - 1] = '\0';

#if PLATFORM == WIN
    if(!(*output_dylib = LoadLibraryA(lib_file_name)))
        return gen_convert_winerr(GetLastError());
#else
    if(!(*output_dylib = dlopen(lib_file_name, 0)))
        return gen_convert_errno(errno);
#endif

    return GEN_OK;
}

gen_error_t gen_dylib_symbol(void** output_address, const gen_dylib_t dylib, const char* symname) {
    if(!output_address) return GEN_INVALID_PARAMETER;
    if(!dylib) return GEN_INVALID_PARAMETER; // This presumes that a valid dylib handle will always be truthy
    if(!symname) return GEN_INVALID_PARAMETER;

#if PLATFORM == WIN
    if(!(*output_address = GetProcAddress(dylib, symname)))
        return gen_convert_winerr(GetLastError());
#else
    if(!(*output_address = dlsym(dylib, symname)))
        return gen_convert_errno(errno);
#endif

    return GEN_OK;
}

gen_error_t gen_dylib_unload(const gen_dylib_t dylib) {
    if(!dylib) return GEN_INVALID_PARAMETER;

#if PLATFORM == WIN
    if(!FreeLibrary(dylib))
        return gen_convert_winerr(GetLastError());
#else
    if(dlclose(dylib))
        return gen_convert_errno(errno);
#endif

    return GEN_OK;
}

