// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gfx.h
 * Genstone graphics utilities.
 */

#ifndef GEN_GRAPHICS_H
#define GEN_GRAPHICS_H

#include <gencommon.h>

#include "gengfxdefs.h"
#include "genuwin.h"

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>
#include <spirv_reflect.h>
GEN_DIAG_REGION_END

/**
 * State object for a graphics context.
 */
typedef struct {
    VkInstance internal_instance;
    VkDebugUtilsMessengerEXT internal_debug_messenger;
    VkPhysicalDevice internal_physical_device;
    VkDevice internal_device;
    VkAllocationCallbacks* internal_allocator;

    VkQueue internal_graphics_queue;
} gen_gfx_context_t;

/**
 * State object for a targeted graphics instance.
 */
typedef struct {
    VkSurfaceKHR internal_surface;
    VkSwapchainKHR internal_swapchain;
    uint32_t internal_swapchain_image_count;
    VkImage* internal_swapchain_images;
    VkImageView* internal_swapchain_image_views;
    gen_gfx_format_t internal_swapchain_image_format;
    gint2 internal_swapchain_image_extent;
    VkViewport internal_viewport;
    VkRect2D internal_scissor;
} gen_gfx_targeted_t;

/**
 * Type for shader programs.
 */
typedef enum {
    GEN_GFX_SHADER_TYPE_VERTEX,
    GEN_GFX_SHADER_TYPE_FRAGMENT,
    GEN_GFX_SHADER_TYPE_GEOMETRY,
    GEN_GFX_SHADER_TYPE_TESSELATION,
    GEN_GFX_SHADER_TYPE_COMPUTE
} gen_gfx_shader_type_t;

/**
 * Representation of a shader program.
 */
typedef struct {
    /**
     * The type of this shader program.
     */
    gen_gfx_shader_type_t type;
    /**
     * SPIR-V bytecode data.
     */
    uint8_t* data;
    /**
     * The size of the SPIR-V bytecode data.
     */
    size_t data_size;
    VkShaderModule internal_shader_module;
} gen_gfx_shader_t;

/**
 * State object for a graphics pipeline.
 */
typedef struct {
    VkRenderPass internal_render_pass;
    VkPipelineLayout internal_layout;
    VkPipeline internal_pipeline;
} gen_gfx_pipeline_t;

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

/**
 * Creates a targeted graphics instance for a window.
 * @param[in,out] context the graphics context from which to create the targeted instance.
 * @param[in,out] window_system the window system to which the targeted window belongs.
 * @param[in,out] window the targeted window.
 * @param[out] out_targeted pointer to storage for the created targeted instance. Must be cleaned up with `gen_gfx_targeted_destroy`.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_targeted_create(gen_gfx_context_t* const restrict context, gen_window_system_t* const restrict window_system, gen_window_t* const restrict window, gen_gfx_targeted_t* const restrict out_targeted);

/**
 * Binds a graphics pipeline to a targeted graphics instance.
 * @param[in,out] context the graphics context from which the targeted instance was created.
 * @param[in,out] targeted the targeted graphics instance to bind to.
 * @param[in,out] pipeline the pipeline to bind.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_targeted_bind_pipeline(gen_gfx_context_t* const restrict context, gen_gfx_targeted_t* const restrict targeted, gen_gfx_pipeline_t* const restrict pipeline);

/**
 * Unbinds a graphics pipeline from a targeted graphics instance.
 * @param[in,out] context the graphics context from which the targeted instance was created.
 * @param[in,out] targeted the targeted graphics instance to which the pipeline is bound.
 * @param[in,out] pipeline the pipeline to unbind.
 */
GEN_ERRORABLE gen_gfx_targeted_unbind_pipeline(gen_gfx_context_t* const restrict context, gen_gfx_targeted_t* const restrict targeted, gen_gfx_pipeline_t* const restrict pipeline);

/**
 * Destroys a targeted graphics instance.
 * @param[in,out] context the graphics context from which the targeted instance was created.
 * @param[in,out] window_system the window system to which the targeted window belongs.
 * @param[in,out] window the targeted window.
 * @param[in,out] targeted the targeted graphics instance to destroy.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_targeted_destroy(gen_gfx_context_t* const restrict context, gen_window_system_t* const restrict window_system, gen_window_t* const restrict window, gen_gfx_targeted_t* const restrict targeted);

/**
 * Creates a graphics pipeline.
 * @param[in,out] context the graphics context from which to create the pipeline.
 * @param[out] out_pipeline pointer to storage for the created pipeline.
 * @param[in] shaders the shaders from which to construct the pipeline.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_pipeline_create(gen_gfx_context_t* const restrict context, gen_gfx_pipeline_t* const restrict out_pipeline, const gen_gfx_shader_t* const restrict shaders, const size_t shaders_length);

/**
 * Destroys a graphics pipeline.
 * @param[in,out] context the graphics context from which the graphics pipeline was created.
 * @param[in,out] pipeline the pipeline to destroy.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_pipeline_destroy(gen_gfx_context_t* const restrict context, gen_gfx_pipeline_t* const restrict pipeline);

/**
 * Creates a shader program.
 * @param[in,out] context the graphics context from which to create the shader program.
 * @param[out] out_shader pointer to storage for the created shader program.
 * @param[in] shader_data the SPIR-V bytecode data from which to create the shader program.
 * @param[in] shader_data_size the size of the SPIR-V bytecode data from which to create the shader program.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_shader_create(gen_gfx_context_t* const restrict context, gen_gfx_shader_t* const restrict out_shader, const uint8_t* shader_data, const size_t shader_data_size);

/**
 * Destroys a shader program.
 * @param[in,out] context the graphics context from which the shader was created.
 * @param[in,out] shader the shader program to destroy.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_shader_destroy(gen_gfx_context_t* const restrict context, gen_gfx_shader_t* const restrict shader);

GEN_ERRORABLE gen_internal_gfx_context_load_extensions(const gen_gfx_context_t* const restrict context);

#endif
