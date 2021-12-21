#if PLATFORM == DWN
#define HAVE_TOWUPPER
#define HAVE_TOWLOWER
#endif

#ifndef __cplusplus
#if PLATFORM != DWN
#define restrict __restrict
#endif
#else
#define restrict __restrict
#endif

#define HAVE_STDINT_H

#define SIZEOF_SIZE_T 8
#define SIZEOF_TIME_T 8
#define HAVE_X86INTRIN_H 1
#define HAVE_XMMINTRIN_H 1

#define HAVE___BUILTIN_CONSTANT_P 1
#define HAVE___BUILTIN_CTZ 1
#define HAVE___BUILTIN_OBJECT_SIZE 1

#define SAFECLIB_HAVE_C99 1

#define SIZEOF_TIME_T 8
#define SIZEOF_WCHAR_T 4

#define HAVE_WCHAR_H

#define STDC_HEADERS 1

#define TIME_WITH_SYS_TIME 1


#define RSIZE_MAX_MEM (256UL << 20)

#define RSIZE_MAX_STR (4UL << 10)

#define SAFECLIB_STR_NULL_SLACK 1

#define HAVE___BUILTIN_OBJECT_SIZE 1

#define HAVE___BUILTIN_CONSTANT_P 1

#define SAFECLIB_HAVE_C99 1

#define STRTOK_DELIM_MAX_LEN 16

#include <stdio.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>

typedef int errno_t;

typedef size_t rsize_t;

#define RSIZE_MAX (~(rsize_t) 0)

#define __attribute_format__(type, index, check) __attribute__((format(type, index, check)))
#define __attribute_format_wprintf(index, check)
#define __attribute_format_wscanf(index, check)

#define ESNULLP (400)
#define ESZEROL (401)
#define ESLEMIN (402)
#define ESLEMAX (403)
#define ESOVRLP (404)
#define ESEMPTY (405)
#define ESNOSPC (406)
#define ESUNTERM (407)
#define ESNODIFF (408)
#define ESNOTFND (409)
#define ESLEWRNG (410)
#define ESLAST ESLEWRNG

#ifndef EOK
#define EOK (0)
#endif
