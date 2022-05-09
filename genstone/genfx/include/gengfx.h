// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gfx.h
 * Genstone graphics utilities.
 */

#ifndef GEN_GRAPHICS_H
#define GEN_GRAPHICS_H

#include <vulkan/vulkan.h>

#include "gengfxdefs.h"
#include "genuwin.h"

#include <gencommon.h>

/**
 * State object for a graphics context.
 */
typedef struct {
    VkInstance internal_instance;
    VkDebugUtilsMessengerEXT internal_debug_messenger;
    VkPhysicalDevice internal_physical_device;
} gen_gfx_context_t;

/**
 * Creates a graphics context.
 * @param[out] out_context pointer to storage for the created graphics context. Should be cleaned up using `gen_gfx_context_destroy`.
 * @note Only one graphics context should exist per-process.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_context_create(gen_gfx_context_t* const restrict out_context);

/**
 * Destroys a graphics context.
 * @param[in] context the context to destroy.
 */
GEN_ERRORABLE gen_gfx_context_destroy(gen_gfx_context_t* const restrict context);

GEN_ERRORABLE gen_internal_gfx_context_load_extensions(const gen_gfx_context_t* const restrict context);

#endif
