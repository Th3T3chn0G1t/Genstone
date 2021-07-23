#ifndef _UCHAR_H
#define _UCHAR_H

typedef unsigned short char16_t;
typedef unsigned char32_t;

#include <stddef.h>
#include <wchar.h>

size_t c16rtomb(char *__restrict, char16_t, mbstate_t *__restrict);
size_t mbrtoc16(char16_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict);

size_t c32rtomb(char *__restrict, char32_t, mbstate_t *__restrict);
size_t mbrtoc32(char32_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict);

#endif
