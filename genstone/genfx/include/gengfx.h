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

#ifndef GEN_GFX_FRAMES_IN_FLIGHT
/**
 * The number of frames which can be processed concurrently while the GPU handles previous inbound frames.
 * @note Large values may cause latency issues, and smaller values may cause stuttering.
 */
#define GEN_GFX_FRAMES_IN_FLIGHT 3
#endif

/**
 * State object for a graphics context.
 */
typedef struct {
    VkInstance internal_instance;
    VkDebugUtilsMessengerEXT internal_debug_messenger;
    VkPhysicalDevice internal_physical_device;
    VkDevice internal_device;
    VkAllocationCallbacks* internal_allocator;

    uint32_t internal_graphics_queue_index;
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
    VkFramebuffer* internal_framebuffers;
    gen_gfx_format_t internal_swapchain_image_format;
    gint2 internal_swapchain_image_extent;
    VkViewport internal_viewport;
    VkRect2D internal_scissor;
    VkCommandPool internal_command_pool;
    VkCommandBuffer internal_command_buffers[GEN_GFX_FRAMES_IN_FLIGHT];
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

    size_t internal_current_frame;

    VkSemaphore internal_image_available_semaphores[GEN_GFX_FRAMES_IN_FLIGHT];
    VkSemaphore internal_render_finished_semaphores[GEN_GFX_FRAMES_IN_FLIGHT];
    VkFence internal_in_flight_fences[GEN_GFX_FRAMES_IN_FLIGHT];
} gen_gfx_pipeline_t;

/**
 * State object for a frame.
 */
typedef struct {
    uint32_t internal_image_index;
} gen_gfx_frame_t;

/**
 * Creates a graphics context.
 * @param[out] out_context pointer to storage for the created graphics context. Should be cleaned up using `gen_gfx_context_destroy`.
 * @note Only one graphics context should exist per-process.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_context_create(gen_gfx_context_t* const restrict out_context);

/**
 * Waits for the device to complete operations preceeding a shutdown.
 * @param[in,out] context the graphics context to wait on.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_shutdown_pre(gen_gfx_context_t* const restrict context);

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
 * Updates a targeted graphics instance and associated pipelines to an updated underlying window geometry for the targeted instance.
 * @param[in,out] context the graphics context from which the targeted instance was created.
 * @param[in,out] window_system the window system to which the targeted window belongs.
 * @param[in,out] window the window to update the targeted instance and pipelines' geometry from.
 * @param[in,out] targeted the targeted graphics instance whose geometry should be adjusted according to `window`.
 * @param[in,out] pipelines a buffer of pointers to pipelines whose geometry should be adjusted according to `window` and the new state of `targeted`.
 * @param[in] pipelines_length the number of pipeline pointers in the `pipelines` buffer.
 * @note Should be called after window resizes to keep gen gfx structures in sync with genuwin.
 * @return an error code.
 */
// GEN_ERRORABLE gen_gfx_targeted_geometry_update(gen_gfx_context_t* const restrict context, gen_window_system_t* const restrict window_system, gen_window_t* const restrict window, gen_gfx_targeted_t* const restrict targeted, gen_gfx_pipeline_t** const restrict pipelines, const size_t pipelines_length);

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
 * Creates a graphics pipeline associated with a targeted graphics instance.
 * @param[in,out] context the graphics context from which to create the pipeline.
 * @param[in,out] targeted the targeted instance to associate the pipeline with.
 * @param[out] out_pipeline pointer to storage for the created pipeline.
 * @param[in] shaders the shaders from which to construct the pipeline.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_pipeline_create(gen_gfx_context_t* const restrict context, const gen_gfx_targeted_t* const restrict targeted, gen_gfx_pipeline_t* const restrict out_pipeline, const gen_gfx_shader_t* const restrict shaders, const size_t shaders_length);

/**
 * Starts a graphics frame using a graphics pipeline to target a targeted graphics instance.
 * @param[in,out] context the graphics context from which the graphics pipeline was created.
 * @param[in,out] targeted the targeted instance the pipeline was associated with.
 * @param[in,out] pipeline the pipeline to begin the frame on.
 * @param[in] clear_color the clear color to start the frame with.
 * @param[out] out_frame pointer to storage for a frame state object.
 * @note the frame must be terminated with a call to `gen_gfx_pipeline_frame_end`.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_pipeline_frame_begin(gen_gfx_context_t* const restrict context, gen_gfx_targeted_t* const restrict targeted, gen_gfx_pipeline_t* const restrict pipeline, const gfloat4 clear_color, gen_gfx_frame_t* const restrict out_frame);

/**
 * Starts a graphics frame using a graphics pipeline to target a targeted graphics instance.
 * @param[in,out] context the graphics context from which the graphics pipeline was created.
 * @param[in,out] targeted the targeted instance the pipeline was associated with.
 * @param[in,out] pipeline the pipeline to begin the frame on.
 * @param[in] frame the frame state object from the corresponding `gen_gfx_pipeline_frame_begin` call.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_pipeline_frame_end(gen_gfx_context_t* const restrict context, gen_gfx_targeted_t* const restrict targeted, gen_gfx_pipeline_t* const restrict pipeline, const gen_gfx_frame_t* const restrict frame);

/**
 * Destroys a graphics pipeline.
 * @param[in,out] context the graphics context from which the graphics pipeline was created.
 * @param[in,out] targeted the targeted instance the pipeline was associated with.
 * @param[in,out] pipeline the pipeline to destroy.
 * @return an error code.
 */
GEN_ERRORABLE gen_gfx_pipeline_destroy(gen_gfx_context_t* const restrict context, gen_gfx_targeted_t* const restrict targeted, gen_gfx_pipeline_t* const restrict pipeline);

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
