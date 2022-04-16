// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gfx.h
 * Genstone graphics utilities definitions.
 */

#ifndef GEN_GRAPHICS_DEFS_H
#define GEN_GRAPHICS_DEFS_H

#include <gencommon.h>

/**
 * Bitdepth values for graphics buffers.
 */
typedef enum {
    GEN_GFX_BITDEPTH_INHERIT = -1,
    GEN_GFX_BITDEPTH_8 = 8,
    GEN_GFX_BITDEPTH_12 = 12,
    GEN_GFX_BITDEPTH_14 = 14,
    GEN_GFX_BITDEPTH_16 = 16,
    GEN_GFX_BITDEPTH_24 = 24
} gen_gfx_bitdepth_t;

/**
 * Layouts for graphics buffers.
 */
typedef enum {
    GEN_GFX_LAYOUT_RGB,
    GEN_GFX_LAYOUT_RGBA,
    GEN_GFX_LAYOUT_ARGB,

    GEN_GFX_LAYOUT_BGR,
    GEN_GFX_LAYOUT_BGRA,
    GEN_GFX_LAYOUT_ABGR,

    GEN_GFX_LAYOUT_R,
    GEN_GFX_LAYOUT_RG,

    GEN_GFX_LAYOUT_DEPTH
} gen_gfx_layout_t;

/**
 * Graphics attribute/component types.
 */
typedef enum {
    GEN_GFX_TYPE_SIGNED_INT,
    GEN_GFX_TYPE_SIGNED_SCALED,
    GEN_GFX_TYPE_SIGNED_NORMALIZED,
    GEN_GFX_TYPE_SIGNED_RGB,

    GEN_GFX_TYPE_UNSIGNED_INT,
    GEN_GFX_TYPE_UNSIGNED_SCALED,
    GEN_GFX_TYPE_UNSIGNED_NORMALIZED
} gen_gfx_type_t;

/**
 * Format structure for grahpics buffers.
 */
typedef struct {
    /**
     * The type of members of a buffer.
     */
    gen_gfx_type_t type;
    /**
     * The layout of components within a buffer.
     */
    gen_gfx_layout_t layout;
    /**
     * The bitdepth of each layout component.
     */
    gen_gfx_bitdepth_t bitdepth[4];
} gen_gfx_format_t;

/**
 * A graphics buffer.
 */
typedef struct {
    /**
     * The buffer format.
     */
    gen_gfx_format_t format;
    /**
     * The contents of the buffer.
     */
    unsigned char* data;
} gen_gfx_buffer_t;

#endif
