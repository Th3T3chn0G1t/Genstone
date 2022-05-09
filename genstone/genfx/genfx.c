// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gengfx.h"

static gen_error_t gen_internal_vk_convert_result(const VkResult result) {
	switch(result) {
		case VK_SUCCESS: return GEN_OK;
		case VK_NOT_READY:
			glog(WARNING, "Recieved VK_NOT_READY");
			[[fallthrough]];
		case VK_TIMEOUT:
			glog(WARNING, "Recieved VK_TIMEOUT");
			[[fallthrough]];
		case VK_EVENT_SET:
			glog(WARNING, "Recieved VK_EVENT_SET");
			[[fallthrough]];
		case VK_EVENT_RESET:
			glog(WARNING, "Recieved VK_EVENT_RESET");
			[[fallthrough]];
		case VK_INCOMPLETE:
			glog(WARNING, "Recieved VK_INCOMPLETE");
			[[fallthrough]];
		case VK_PIPELINE_COMPILE_REQUIRED:
			glog(WARNING, "Recieved VK_PIPELINE_COMPILE_REQUIRED");
			[[fallthrough]];
		case VK_SUBOPTIMAL_KHR:
			glog(WARNING, "Recieved VK_SUBOPTIMAL_KHR");
			[[fallthrough]];
		case VK_THREAD_IDLE_KHR:
			glog(WARNING, "Recieved VK_THREAD_IDLE_KHR");
			[[fallthrough]];
		case VK_THREAD_DONE_KHR:
			glog(WARNING, "Recieved VK_THREAD_DONE_KHR");
			[[fallthrough]];
		case VK_OPERATION_DEFERRED_KHR:
			glog(WARNING, "Recieved VK_OPERATION_DEFERRED_KHR");
			[[fallthrough]];
		case VK_OPERATION_NOT_DEFERRED_KHR:
			glog(WARNING, "Recieved VK_OPERATION_NOT_DEFERRED_KHR");
			return GEN_OK;

		case VK_ERROR_OUT_OF_HOST_MEMORY: return GEN_OUT_OF_MEMORY;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: return GEN_OUT_OF_MEMORY;
		case VK_ERROR_INITIALIZATION_FAILED: return GEN_BAD_OPERATION;
		case VK_ERROR_DEVICE_LOST: return GEN_NO_SUCH_OBJECT;
		case VK_ERROR_MEMORY_MAP_FAILED: return GEN_OUT_OF_MEMORY;
		case VK_ERROR_LAYER_NOT_PRESENT: return GEN_NO_SUCH_OBJECT;
		case VK_ERROR_EXTENSION_NOT_PRESENT: return GEN_NOT_IMPLEMENTED;
		case VK_ERROR_FEATURE_NOT_PRESENT: return GEN_NOT_IMPLEMENTED;
		case VK_ERROR_INCOMPATIBLE_DRIVER: return GEN_WRONG_OBJECT_TYPE;
		case VK_ERROR_TOO_MANY_OBJECTS: return GEN_TOO_LONG;
		case VK_ERROR_FORMAT_NOT_SUPPORTED: return GEN_INVALID_PARAMETER;
		case VK_ERROR_FRAGMENTED_POOL: return GEN_OUT_OF_MEMORY;
		case VK_ERROR_UNKNOWN: return GEN_UNKNOWN;
		case VK_ERROR_OUT_OF_POOL_MEMORY: return GEN_OUT_OF_MEMORY;
		case VK_ERROR_INVALID_EXTERNAL_HANDLE: return GEN_INVALID_PARAMETER;
		case VK_ERROR_FRAGMENTATION: return GEN_OUT_OF_MEMORY;
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return GEN_INVALID_PARAMETER;
		case VK_ERROR_SURFACE_LOST_KHR: return GEN_NO_SUCH_OBJECT;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return GEN_IN_USE;
		case VK_ERROR_OUT_OF_DATE_KHR: return GEN_INVALID_PARAMETER;
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return GEN_INVALID_PARAMETER;
		case VK_ERROR_VALIDATION_FAILED_EXT: return GEN_OPERATION_FAILED;
		case VK_ERROR_INVALID_SHADER_NV: return GEN_INVALID_PARAMETER;
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return GEN_INVALID_PARAMETER;
		case VK_ERROR_NOT_PERMITTED_KHR: return GEN_PERMISSION;
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return GEN_OPERATION_FAILED;

		case VK_RESULT_MAX_ENUM: return GEN_UNKNOWN;
	}
}

#define GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, proc) \
	do { \
		if(gen_internal_vk_convert_result(result)) { \
			glogf(ERROR, "%s: `%s` failed at %s:%i", gen_error_name(gen_internal_vk_convert_result(result)), #proc, __FILE__, __LINE__); \
			return gen_internal_vk_convert_result(result); \
		} \
	} while(0)

static VkBool32 gen_internal_vk_debug_callback(const VkDebugUtilsMessageSeverityFlagBitsEXT severity, const VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* data, __unused void* passthrough) {
	GEN_FRAME_BEGIN(gen_internal_vk_debug_callback);

	const char* prefix = NULL;
	if(type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
		prefix = "Debug Message";
	else if(type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
		prefix = "Validation Error";
	else if(type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
		glogf(PERFORMANCE, "%s", data->pMessage);

	if(severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
		glogf(TRACE, "%s: %s", prefix, data->pMessage);
	else if(severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		glogf(INFO, "%s: %s", prefix, data->pMessage);
	else if(severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		glogf(WARNING, "%s: %s", prefix, data->pMessage);
	else if(severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		glogf(FATAL, "%s: %s", prefix, data->pMessage);
		GEN_REQUIRE_NO_REACH;
	}

	return VK_FALSE;
}

gen_error_t gen_gfx_context_create(gen_gfx_context_t* const restrict out_context) {
	GEN_FRAME_BEGIN(gen_gfx_context_create);

	GEN_NULL_CHECK(out_context);

#if MODE == DEBUG
	const char* layers[] = {
		"VK_LAYER_KHRONOS_validation"};
#endif
	const char* extensions[] = {
		"VK_KHR_surface",
		"VK_KHR_xcb_surface"
#if MODE == DEBUG
		,
		"VK_EXT_debug_utils"
#endif
	};
	VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO, NULL, "Genstone Application", 0, "Genstone", 0, VK_API_VERSION_1_3};
#if MODE == DEBUG
	VkInstanceCreateInfo instance_create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, NULL, 0, &app_info, sizeof(layers) / sizeof(layers[0]), layers, sizeof(extensions) / sizeof(extensions[0]), extensions};
#else
	VkInstanceCreateInfo instance_create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, NULL, 0, &app_info, 0, NULL, sizeof(extensions) / sizeof(extensions[0]), extensions};
#endif
	VkResult result = vkCreateInstance(&instance_create_info, NULL, &out_context->internal_instance);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateInstance);

	gen_error_t error = gen_internal_gfx_context_load_extensions(out_context);
	GEN_ERROR_OUT_IF(error, "`gen_internal_gfx_context_load_extensions` failed");

	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT, NULL, 0, VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT, gen_internal_vk_debug_callback, NULL};
	result = vkCreateDebugUtilsMessengerEXT(out_context->internal_instance, &debug_messenger_create_info, NULL, &out_context->internal_debug_messenger);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateDebugUtilsMessengerEXT);

	// uint32_t physical_device_count = 0;
	// result = vkEnumeratePhysicalDevices(out_context->internal_instance, &physical_device_count, NULL);
	// GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkEnumeratePhysicalDevices);
	// VkPhysicalDevice* physical_devices = NULL;
	// error = gzalloc(&physical_devices, physical_device_count, sizeof(VkPhysicalDevice));
	// GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
	// result = vkEnumeratePhysicalDevices(out_context->internal_instance, NULL, physical_devices);
	// GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkEnumeratePhysicalDevices);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_context_destroy(gen_gfx_context_t* const restrict context) {
	GEN_FRAME_BEGIN(gen_gfx_context_destroy);

	GEN_NULL_CHECK(context);

	vkDestroyDebugUtilsMessengerEXT(context->internal_instance, context->internal_debug_messenger, NULL);
	vkDestroyInstance(context->internal_instance, NULL);

	GEN_ALL_OK;
}
