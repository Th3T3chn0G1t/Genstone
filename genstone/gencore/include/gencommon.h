// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_COMMON_H
#define GEN_COMMON_H

#define GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN clang diagnostic push
#define GEN_PRAGMA_DIAGNOSTIC_REGION_END clang diagnostic pop
#define GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE(w) clang diagnostic ignored w
#define GEN_STRINGIFY(s) #s
#define GEN_EXPAND(x) x
#define GEN_FULL_STRINGIFY(s) GEN_STRINGIFY(s)
#define GEN_PRAGMA(p) _Pragma(GEN_STRINGIFY(p))

#define GEN_FILE_NAME __FILE__
#define GEN_LINE_NUMBER __LINE__
#define GEN_LINE_STRING GEN_FULL_STRINGIFY(__LINE__)
#define GEN_FUNCTION_NAME __func__

#define GEN_NORETURN _Noreturn
#define GEN_THREAD_LOCAL _Thread_local
#define GEN_ALIGNAS(x) _Alignas(x)
#define GEN_ALIGNOF(x) _Alignof(x)
#define GEN_GENERIC _Generic

#define GEN_TYPEOF(x) __typeof__(x)

#define GEN_UNUSED __attribute__((unused))
#define GEN_USED __attribute__((used))
#define GEN_PACKED __attribute__((packed))
#define GEN_INITIALIZER __attribute__((constructor))
#define GEN_FALLTHROUGH __attribute__((fallthrough))
#define GEN_CLEANUP_FUNCTION(function) \
    GEN_UNUSED \
    __attribute__((cleanup(function)))
#define GEN_UNREACHABLE __builtin_unreachable()
#define GEN_FORCE_INLINE \
    __attribute__((always_inline)) __attribute__((artificial)) inline
#define GEN_FLAG_ENUM __attribute__((enum_extensibility(closed), flag_enum))
#define GEN_NODISCARD __attribute__((nodiscard))
#define GEN_LOCAL_ALIAS(s) __attribute__((alias(s)))
#define GEN_GLOBAL_ALIAS(s) asm(s)

typedef _Bool gen_bool_t;
#define gen_true (1)
#define gen_false (0)

typedef unsigned long long gen_size_t;
#define GEN_SIZE_MAX (~0ull)

typedef unsigned long long gen_uintptr_t;
#define GEN_UINTPTR_MAX (~0ull)

typedef long long gen_ssize_t;
#define GEN_SSIZE_MAX ((gen_ssize_t) (0x7FFFFFFFFFFFFFFF))

typedef unsigned long long gen_uint64_t;
#define GEN_UINT64_MAX (~0ull)

typedef unsigned long long gen_ulong_t;
#define GEN_ULONG_MAX (~0ull)

typedef unsigned int gen_uint32_t;
#define GEN_UINT32_MAX (~0u)

typedef unsigned int gen_uint_t;
#define GEN_UINT_MAX (~0u)

typedef unsigned short gen_uint16_t;
#define GEN_UINT16_MAX ((gen_uint16_t) (~0u))

typedef unsigned short gen_ushort_t;
#define GEN_USHORT_MAX ((gen_ushort_t) (~0u))

typedef unsigned char gen_uint8_t;
#define GEN_UINT8_MAX ((gen_uint8_t) (~0u))

typedef unsigned char gen_uchar_t;
#define GEN_UCHAR_MAX ((gen_uchar_t) (~0u))

#define GEN_NULL ((void*) (0))

typedef __builtin_va_list gen_variadic_list_t;
#define gen_variadic_list_argument(list, type) __builtin_va_arg(list, type)
#define gen_variadic_list_start(list, param) __builtin_va_start(list, param)
#define gen_variadic_list_end(list) __builtin_va_end(list)
#define gen_variadic_list_copy(to, from) __builtin_va_copy(to, from)

static GEN_FORCE_INLINE void gen_internal_variadic_list_auto_end(
                                    gen_variadic_list_t* list) {

    gen_variadic_list_end(*list);
}

#define GEN_VARIADIC_LIST_AUTO \
                GEN_CLEANUP_FUNCTION(gen_internal_variadic_list_auto_end)

#define gen_inline_variadic_list_pack __builtin_va_arg_pack()
#define gen_inline_variadic_list_pack_length __builtin_va_arg_pack_len()

typedef gen_uintptr_t gen_jump_point_t;
#define gen_set_jump_point(p) __builtin_setjmp(p)
// NOTE: Apparently `__builtin_longjmp` only accepts 1 as the retval?
#define gen_goto_jump_point(p) __builtin_longjmp(p, 1)

#define GEN_ASM_BLOCK(ops, ...) __asm__ volatile(ops __VA_ARGS__)
#define GEN_ASM(...) #__VA_ARGS__ "\n"

#define GEN_MEMBER_SIZE(type, member) (sizeof(((type*) GEN_NULL)->member))
#define GEN_ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))

#define GEN_MINIMUM(a, b) ((a) < (b) ? (a) : (b))
#define GEN_MAXIMUM(a, b) ((a) > (b) ? (a) : (b))

#define GEN_NEXT_NEAREST(x, multiple) \
                ((((x) + (multiple) - 1) / (multiple)) * (multiple))

#define GEN_LEADING_ZEROES(x) ((gen_size_t) __builtin_clzll((x)))

#include "generror.h"
#include "gentooling.h"

#endif
