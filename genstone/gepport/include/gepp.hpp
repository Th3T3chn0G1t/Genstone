// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gepp.hpp
 * Includes C++ backporting utilities
 */

#ifndef GEN_PORT_H
#define GEN_PORT_H

extern "C" {
    #include "gendiag.h"

    GEN_DIAG_REGION_BEGIN
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #pragma clang diagnostic ignored "-Wreserved-macro-identifier"

    #define _LIBCPP_COMPLEX_H
    #define _LIBCPP_COMPLEX
    #define _COMPLEX_H
    #define COMPLEX_H

    #define _LIBCPP_UCHAR_H
    #define _LIBCPP_UCHAR
    #define _UCHAR_H
    #define UCHAR_H

    #define _LIBCPP_TGMATH_H
    #define _LIBCPP_TGMATH
    #define _TGMATH_H
    #define TGMATH_H

    #define _LIBCPP_UTILITY_H
    #define _LIBCPP_UTILITY
    #define _UTILITY_H
    #define UTILITY_H

    #define MIMALLOC_H
    GEN_DIAG_REGION_END

    #include "gencommon.h"
}

#endif
