// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genbackends.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <dlfcn.h>
// #include <stdio.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

void* gen_backends_resolve_deferred(const char* const restrict symbol) {
    // printf("Resolving `%s`...\n", symbol);
    void* ptr = dlsym(RTLD_SELF, symbol);
    char* error = dlerror();
    if(!error) return ptr;
    // printf("Got error: `%s`\n", error);
    return GEN_NULL;
}
