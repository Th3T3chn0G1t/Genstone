// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_BACKENDS_H
#define GEN_BACKENDS_H

// I went down the route of attempting static, symbol-based resolution of backends
// Following several hours of painful struggles with symbol aliases, weak symbols and
// DYLD weirdness - I've decided that dynamic resolution is a better solution, even if it's
// Less optimal.

extern void* gen_backends_resolve_deferred(const char* const restrict symbol);

#define GEN_BACKENDS_CALL_LAYER(level, function_top_level) ((__typeof__(&gen_##function_top_level)) gen_backends_resolve_deferred("gen_backends_" level "_" #function_top_level))

// We can just presume that there will never be a need for more than 4 backends at once

#ifdef GEN_BACKENDS_QUATERNARY
#define GEN_BACKENDS_CALL(function_top_level) \
    ({ \
        __typeof__(&gen_##function_top_level) primary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_PRIMARY, function_top_level); \
        __typeof__(&gen_##function_top_level) secondary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_SECONDARY, function_top_level); \
        __typeof__(&gen_##function_top_level) tertiary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_TERTIARY, function_top_level); \
        __typeof__(&gen_##function_top_level) quaternary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_QUATERNARY, function_top_level); \
        primary ? primary : \
        secondary ? secondary : \
        tertiary ? tertiary : \
        quaternary ? quaternary /* This should never be unresolved at this point given we have a valid backend */ : ((__typeof__(&gen_##function_top_level)) &gen_error_abort); \
    })
// Unfortunately avoiding `#elifdef` here as VSC has an aneurism (just for my own convenience mostly)
// We should probably file an issue for this or otherwise just check in once C23 is properly supported
// To see if it's resolved
// #elifdef GEN_BACKENDS_TERTIARY
#elif defined(GEN_BACKENDS_TERTIARY)
#define GEN_BACKENDS_CALL(function_top_level) \
    ({ \
        __typeof__(&gen_##function_top_level) primary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_PRIMARY, function_top_level); \
        __typeof__(&gen_##function_top_level) secondary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_SECONDARY, function_top_level); \
        __typeof__(&gen_##function_top_level) tertiary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_TERTIARY, function_top_level); \
        primary ? primary : \
        secondary ? secondary : \
        tertiary ? tertiary : ((__typeof__(&gen_##function_top_level)) &gen_error_abort); \
    })
#elif defined(GEN_BACKENDS_SECONDARY)
#define GEN_BACKENDS_CALL(function_top_level) \
    ({ \
        __typeof__(&gen_##function_top_level) primary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_PRIMARY, function_top_level); \
        __typeof__(&gen_##function_top_level) secondary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_SECONDARY, function_top_level); \
        primary ? primary : \
        secondary ? secondary : ((__typeof__(&gen_##function_top_level)) &gen_error_abort); \
    })
#elif defined(GEN_BACKENDS_PRIMARY)
#define GEN_BACKENDS_CALL(function_top_level) \
    ({ \
        __typeof__(&gen_##function_top_level) primary = GEN_BACKENDS_CALL_LAYER(GEN_BACKENDS_PRIMARY, function_top_level); \
        primary ? primary : ((__typeof__(&gen_##function_top_level)) &gen_error_abort); \
    })
#else
#error "No backends selected"
#endif

#endif
