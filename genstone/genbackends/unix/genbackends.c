// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genbackends.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <dlfcn.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

#ifdef RTLD_SELF // BSD-ish specific
static void* gen_backends_internal_selfsym_handle = RTLD_SELF;
#else
static void* gen_backends_internal_selfsym_handle = RTLD_DEFAULT;
#endif

void* gen_backends_resolve_deferred(const char* const restrict symbol) {
    void* ptr = dlsym(gen_backends_internal_selfsym_handle, symbol);
    char* error = dlerror();
    if(!error) return ptr;
    return GEN_NULL;
}
