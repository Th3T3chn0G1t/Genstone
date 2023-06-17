// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_BACKENDS_H
#define GEN_BACKENDS_H

#define GEN_BACKENDS_DEFER_NOGEN(func, ret, current, target, retblock) \
    extern ret gen_internal_##current##_##func() \
        GEN_GLOBAL_ALIAS(target); \
    GEN_USED ret gen_##current##_##func() { \
        retblock gen_internal_##current##_##func(); }

#define GEN_BACKENDS_DEFER(func, ret, current, target, retblock) \
    extern ret gen_internal_##current##_##func() \
        GEN_GLOBAL_ALIAS("gen_" target "_" #func); \
    GEN_USED ret gen_##current##_##func() { \
        retblock gen_internal_##current##_##func(); }

#define GEN_BACKENDS_PROC(func, ret) \
    ret gen_backends_##func() GEN_GLOBAL_ALIAS("gen_" GEN_BACKEND "_" #func);

#endif
