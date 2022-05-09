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

GEN_INTERNAL_ERRORABLE gen_internal_vk_evaluate_physical_device(const gen_gfx_context_t* const restrict context, const VkPhysicalDevice device, size_t* const restrict out_score) {
	GEN_FRAME_BEGIN(gen_internal_vk_evaluate_physical_device);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(device);

	*out_score = 0;

	VkPhysicalDeviceProperties device_properties = {0};
	vkGetPhysicalDeviceProperties(device, &device_properties);
	if(device_properties.apiVersion < VK_API_VERSION_1_3) GEN_ALL_OK;
	switch(device_properties.deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER: {
			*out_score += 1;
			break;
		}
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
			*out_score += 3;
			break;
		}
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
			*out_score += 4;
			break;
		}
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: {
			*out_score += 2;
			break;
		}
		case VK_PHYSICAL_DEVICE_TYPE_CPU: {
			*out_score += 1;
			break;
		}
		case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM: GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "Unknown GPU type");
	}

	glogf(NOTE, "Device `%s` evaulated with score of %zu", device_properties.deviceName, *out_score);

	// VkPhysicalDeviceLimits              limits;
	// VkPhysicalDeviceSparseProperties    sparseProperties;

	// VkPhysicalDeviceFeatures device_features = {0};
	// vkGetPhysicalDeviceFeatures(device, &device_features);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_context_create(gen_gfx_context_t* const restrict out_context) {
	GEN_FRAME_BEGIN(gen_gfx_context_create);

	GEN_NULL_CHECK(out_context);

#if MODE == DEBUG
	const char* const layers[] = {
		"VK_LAYER_KHRONOS_validation"};
#endif
	const char* const extensions[] = {
		"VK_KHR_surface",
		"VK_KHR_xcb_surface"
#if MODE == DEBUG
		,
		"VK_EXT_debug_utils"
#endif
	};
	const VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO, NULL, "Genstone Application", 0, "Genstone", 0, VK_API_VERSION_1_3};
#if MODE == DEBUG
	const VkInstanceCreateInfo instance_create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, NULL, 0, &app_info, sizeof(layers) / sizeof(layers[0]), layers, sizeof(extensions) / sizeof(extensions[0]), extensions};
#else
	const VkInstanceCreateInfo instance_create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, NULL, 0, &app_info, 0, NULL, sizeof(extensions) / sizeof(extensions[0]), extensions};
#endif
	VkResult result = vkCreateInstance(&instance_create_info, out_context->internal_allocator, &out_context->internal_instance);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateInstance);

	gen_error_t error = gen_internal_gfx_context_load_extensions(out_context);
	GEN_ERROR_OUT_IF(error, "`gen_internal_gfx_context_load_extensions` failed");

	const VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT, NULL, 0, VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT, gen_internal_vk_debug_callback, NULL};
	result = vkCreateDebugUtilsMessengerEXT(out_context->internal_instance, &debug_messenger_create_info, out_context->internal_allocator, &out_context->internal_debug_messenger);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateDebugUtilsMessengerEXT);

	uint32_t physical_device_count = 0;
	result = vkEnumeratePhysicalDevices(out_context->internal_instance, &physical_device_count, NULL);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkEnumeratePhysicalDevices);
	VkPhysicalDevice* physical_devices = NULL;
	error = gzalloc((void**) &physical_devices, physical_device_count, sizeof(VkPhysicalDevice));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
	result = vkEnumeratePhysicalDevices(out_context->internal_instance, &physical_device_count, physical_devices);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkEnumeratePhysicalDevices);

	VkPhysicalDevice best_device = VK_NULL_HANDLE;
	size_t best_score = 0;

	for(size_t i = 0; i < physical_device_count; ++i) {
		const VkPhysicalDevice device = physical_devices[i];
		size_t score = 0;
		error = gen_internal_vk_evaluate_physical_device(out_context, device, &score);
		GEN_ERROR_OUT_IF(error, "`gen_internal_vk_evaluate_physical_device` failed");

		if(!score) continue;
		if(score > best_score) {
			best_device = device;
			best_score = score;
		}
	}

	if(!best_device) GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "No suitable physical device could be resolved");
	out_context->internal_physical_device = best_device;
	error = gfree(physical_devices);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(out_context->internal_physical_device, &queue_family_count, NULL);
	VkQueueFamilyProperties* queue_family_properties = NULL;
	error = gzalloc((void**) &queue_family_properties, queue_family_count, sizeof(VkQueueFamilyProperties));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
	vkGetPhysicalDeviceQueueFamilyProperties(out_context->internal_physical_device, &queue_family_count, queue_family_properties);

	uint32_t graphics_queue_family_index = UINT32_MAX;
	uint32_t compute_queue_family_index = UINT32_MAX;
	uint32_t transfer_queue_family_index = UINT32_MAX;
	uint32_t sparse_binding_queue_family_index = UINT32_MAX;
	uint32_t protected_queue_family_index = UINT32_MAX;
	uint32_t video_decode_queue_family_index = UINT32_MAX;
	uint32_t video_encode_queue_family_index = UINT32_MAX;

	(void) compute_queue_family_index;
	(void) transfer_queue_family_index;
	(void) sparse_binding_queue_family_index;
	(void) protected_queue_family_index;
	(void) video_decode_queue_family_index;
	(void) video_encode_queue_family_index;

	for(uint32_t i = 0; i < queue_family_count; ++i) {
		const VkQueueFamilyProperties properties = queue_family_properties[i];

		if(properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) graphics_queue_family_index = i;
		if(properties.queueFlags & VK_QUEUE_COMPUTE_BIT) compute_queue_family_index = i;
		if(properties.queueFlags & VK_QUEUE_TRANSFER_BIT) transfer_queue_family_index = i;
		if(properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) sparse_binding_queue_family_index = i;
		if(properties.queueFlags & VK_QUEUE_PROTECTED_BIT) protected_queue_family_index = i;
		if(properties.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) video_decode_queue_family_index = i;
		if(properties.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) video_encode_queue_family_index = i;
	}

	error = gfree(queue_family_properties);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");
	if(graphics_queue_family_index == UINT32_MAX) GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "No graphics queue family available on device");

	const VkDeviceQueueCreateInfo device_queue_create_infos[] = {{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, NULL, 0, graphics_queue_family_index, 1, (float[]){1.0f}}};
	const VkPhysicalDeviceFeatures physical_device_features = {0};
	const char* const device_extensions[] = {"VK_KHR_swapchain"};
	const VkDeviceCreateInfo device_create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, NULL, 0, sizeof(device_queue_create_infos) / sizeof(device_queue_create_infos[0]), device_queue_create_infos, sizeof(layers) / sizeof(layers[0]), layers, sizeof(device_extensions) / sizeof(device_extensions[0]), device_extensions, &physical_device_features};
	result = vkCreateDevice(out_context->internal_physical_device, &device_create_info, out_context->internal_allocator, &out_context->internal_device);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateDevice);

	vkGetDeviceQueue(out_context->internal_device, graphics_queue_family_index, 0, &out_context->internal_graphics_queue);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_context_destroy(gen_gfx_context_t* const restrict context) {
	GEN_FRAME_BEGIN(gen_gfx_context_destroy);

	GEN_NULL_CHECK(context);

	vkDestroyDevice(context->internal_device, context->internal_allocator);
	vkDestroyDebugUtilsMessengerEXT(context->internal_instance, context->internal_debug_messenger, context->internal_allocator);
	vkDestroyInstance(context->internal_instance, context->internal_allocator);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_targeted_create(gen_gfx_context_t* const restrict context, gen_window_system_t* const restrict window_system, gen_window_t* const restrict window, gen_gfx_targeted_t* const restrict out_targeted) {
	GEN_FRAME_BEGIN(gen_gfx_targeted_create);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(window);
	GEN_NULL_CHECK(out_targeted);

	const VkXcbSurfaceCreateInfoKHR surface_create_info = {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR, NULL, 0, window_system->internal_connection, window->window};
	VkResult result = vkCreateXcbSurfaceKHR(context->internal_instance, &surface_create_info, context->internal_allocator, &out_targeted->internal_surface);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateXcbSurfaceKHR);

	GEN_ALL_OK;
}
gen_error_t gen_gfx_targeted_destroy(gen_gfx_context_t* const restrict context, gen_window_system_t* const restrict window_system, gen_window_t* const restrict window, gen_gfx_targeted_t* const restrict targeted) {
	GEN_FRAME_BEGIN(gen_gfx_targeted_destroy);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(window);
	GEN_NULL_CHECK(targeted);

	vkDestroySurfaceKHR(context->internal_instance, targeted->internal_surface, context->internal_allocator);

	GEN_ALL_OK;
}
