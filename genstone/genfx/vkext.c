// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gengfx.h"

GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wunused-macros"
#define GEN_INTERNAL_VK_EXT_LOAD(name) \
	gen_internal_##name##_ptr = (__typeof__(gen_internal_##name##_ptr)) vkGetInstanceProcAddr(context->internal_instance, #name); \
	if(!gen_internal_##name##_ptr) GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "Failed to load extension proc `" #name "`")

#define GEN_INTERNAL_VK_EXT_PROC(name, ret, ...) \
	static ret (*gen_internal_##name##_ptr)(__VA_ARGS__) = NULL; \
	ret name(__VA_ARGS__) {return gen_internal_##name##_ptr(

#define GEN_INTERNAL_VK_EXT_VOID_PROC(name, ...) \
	static void (*gen_internal_##name##_ptr)(__VA_ARGS__) = NULL; \
	void name(__VA_ARGS__) {gen_internal_##name##_ptr(

#define GEN_INTERNAL_VK_EXT_PROC_TAIL ); \
	}
GEN_DIAG_REGION_END

// clang-format off

#if MODE == DEBUG
GEN_INTERNAL_VK_EXT_PROC(
	vkCreateDebugUtilsMessengerEXT,
	VkResult,
	VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pMessenger
)
    instance, pCreateInfo, pAllocator, pMessenger
GEN_INTERNAL_VK_EXT_PROC_TAIL

GEN_INTERNAL_VK_EXT_VOID_PROC(
    vkDestroyDebugUtilsMessengerEXT,
    VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *pAllocator
)
    instance, messenger, pAllocator
GEN_INTERNAL_VK_EXT_PROC_TAIL
#endif

	// clang-format on

	gen_error_t gen_internal_gfx_context_load_extensions(const gen_gfx_context_t *const restrict context) {
	GEN_FRAME_BEGIN(gen_internal_gfx_context_load_extensions);

	GEN_NULL_CHECK(context);

#if MODE == DEBUG
	GEN_INTERNAL_VK_EXT_LOAD(vkCreateDebugUtilsMessengerEXT);
	GEN_INTERNAL_VK_EXT_LOAD(vkDestroyDebugUtilsMessengerEXT);
#endif

	GEN_ALL_OK;
}
