// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gengfx.h"

static gen_error_t gen_internal_vk_convert_result(const VkResult result) {
	switch(result) {
		case VK_SUCCESS: return GEN_OK;
		case VK_NOT_READY: {
			glog(WARNING, "Recieved VK_NOT_READY");
			return GEN_OK;
		}
		case VK_TIMEOUT: {
			glog(WARNING, "Recieved VK_TIMEOUT");
			return GEN_OK;
		}
		case VK_EVENT_SET: {
			glog(WARNING, "Recieved VK_EVENT_SET");
			return GEN_OK;
		}
		case VK_EVENT_RESET: {
			glog(WARNING, "Recieved VK_EVENT_RESET");
			return GEN_OK;
		}
		case VK_INCOMPLETE: {
			glog(WARNING, "Recieved VK_INCOMPLETE");
			return GEN_OK;
		}
		case VK_PIPELINE_COMPILE_REQUIRED: {
			glog(WARNING, "Recieved VK_PIPELINE_COMPILE_REQUIRED");
			return GEN_OK;
		}
		case VK_SUBOPTIMAL_KHR: {
			glog(WARNING, "Recieved VK_SUBOPTIMAL_KHR");
			return GEN_OK;
		}
		case VK_THREAD_IDLE_KHR: {
			glog(WARNING, "Recieved VK_THREAD_IDLE_KHR");
			return GEN_OK;
		}
		case VK_THREAD_DONE_KHR: {
			glog(WARNING, "Recieved VK_THREAD_DONE_KHR");
			return GEN_OK;
		}
		case VK_OPERATION_DEFERRED_KHR: {
			glog(WARNING, "Recieved VK_OPERATION_DEFERRED_KHR");
			return GEN_OK;
		}
		case VK_OPERATION_NOT_DEFERRED_KHR: {
			glog(WARNING, "Recieved VK_OPERATION_NOT_DEFERRED_KHR");
			return GEN_OK;
		}

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
		case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: return GEN_OPERATION_FAILED;

		case VK_RESULT_MAX_ENUM: return GEN_UNKNOWN;
	}
}

static gen_error_t gen_internal_spv_convert_result(const SpvReflectResult result) {
	switch(result) {
		case SPV_REFLECT_RESULT_SUCCESS: return GEN_OK;
		case SPV_REFLECT_RESULT_NOT_READY: return GEN_BAD_TIMING;
		case SPV_REFLECT_RESULT_ERROR_PARSE_FAILED: return GEN_BAD_CONTENT;
		case SPV_REFLECT_RESULT_ERROR_ALLOC_FAILED: return GEN_OUT_OF_MEMORY;
		case SPV_REFLECT_RESULT_ERROR_RANGE_EXCEEDED: return GEN_TOO_LONG;
		case SPV_REFLECT_RESULT_ERROR_NULL_POINTER: return GEN_INVALID_PARAMETER;
		case SPV_REFLECT_RESULT_ERROR_INTERNAL_ERROR: return GEN_UNKNOWN;
		case SPV_REFLECT_RESULT_ERROR_COUNT_MISMATCH: return GEN_INVALID_PARAMETER;
		case SPV_REFLECT_RESULT_ERROR_ELEMENT_NOT_FOUND: return GEN_NO_SUCH_OBJECT;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_CODE_SIZE: return GEN_INVALID_PARAMETER;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_MAGIC_NUMBER: return GEN_BAD_CONTENT;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_EOF: return GEN_TOO_SHORT;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ID_REFERENCE: return GEN_INVALID_PARAMETER;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_SET_NUMBER_OVERFLOW: return GEN_TOO_LONG;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_STORAGE_CLASS: return GEN_BAD_CONTENT;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_RECURSION: return GEN_BAD_CONTENT;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_INSTRUCTION: return GEN_BAD_CONTENT;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_BLOCK_DATA: return GEN_BAD_CONTENT;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_BLOCK_MEMBER_REFERENCE: return GEN_BAD_CONTENT;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ENTRY_POINT: return GEN_BAD_CONTENT;
		case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_EXECUTION_MODE: return GEN_BAD_CONTENT;
	}
}

#define GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, proc) \
	do { \
		if(gen_internal_vk_convert_result(result)) { \
			glogf(ERROR, "%s: `%s` failed at %s:%i", gen_error_name(gen_internal_vk_convert_result(result)), #proc, __FILE__, __LINE__); \
			return gen_internal_vk_convert_result(result); \
		} \
	} while(0)

#define GEN_INTERNAL_ERROR_OUT_IF_SPVRESULT(result, proc) \
	do { \
		if(gen_internal_spv_convert_result(result)) { \
			glogf(ERROR, "%s: `%s` failed at %s:%i", gen_error_name(gen_internal_spv_convert_result(result)), #proc, __FILE__, __LINE__); \
			return gen_internal_spv_convert_result(result); \
		} \
	} while(0)

#if MODE == DEBUG
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
#endif

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

static gen_gfx_format_t gen_internal_gfx_format_from_vk_format(const VkFormat format) {
	switch(format) {
		case VK_FORMAT_UNDEFINED: return (gen_gfx_format_t){0};

		case VK_FORMAT_R4G4_UNORM_PACK8: return (gen_gfx_format_t){0};

		case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_R5G6B5_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_B5G6R5_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_R8_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8_SRGB: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_RGB, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_8};

		case VK_FORMAT_R8G8_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8_SRGB: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_RGB, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_8};

		case VK_FORMAT_R8G8B8_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8_SRGB: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_RGB, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_8};

		case VK_FORMAT_B8G8R8_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_BGR, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_BGR, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_BGR, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_BGR, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_BGR, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_BGR, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8_SRGB: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_RGB, GEN_GFX_LAYOUT_BGR, GEN_GFX_BITDEPTH_8};

		case VK_FORMAT_R8G8B8A8_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8A8_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8A8_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8A8_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8A8_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8A8_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_R8G8B8A8_SRGB: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_RGB, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_8};

		case VK_FORMAT_B8G8R8A8_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_BGRA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8A8_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_BGRA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8A8_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_BGRA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8A8_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_BGRA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8A8_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_BGRA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8A8_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_BGRA, GEN_GFX_BITDEPTH_8};
		case VK_FORMAT_B8G8R8A8_SRGB: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_RGB, GEN_GFX_LAYOUT_BGRA, GEN_GFX_BITDEPTH_8};

		case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A8B8G8R8_UINT_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A8B8G8R8_SINT_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return (gen_gfx_format_t){0};

		case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2R10G10B10_UINT_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2R10G10B10_SINT_PACK32: return (gen_gfx_format_t){0};

		case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2B10G10R10_UINT_PACK32: return (gen_gfx_format_t){0};
		case VK_FORMAT_A2B10G10R10_SINT_PACK32: return (gen_gfx_format_t){0};

		case VK_FORMAT_R16_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_16};

		case VK_FORMAT_R16G16_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_16};

		case VK_FORMAT_R16G16B16_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_16};

		case VK_FORMAT_R16G16B16A16_UNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_NORMALIZED, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16A16_SNORM: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_NORMALIZED, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16A16_USCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_SCALED, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16A16_SSCALED: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_SCALED, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16A16_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16A16_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_16};
		case VK_FORMAT_R16G16B16A16_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_16};

		case VK_FORMAT_R32_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_32};
		case VK_FORMAT_R32_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_32};
		case VK_FORMAT_R32_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_32};

		case VK_FORMAT_R32G32_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_32};
		case VK_FORMAT_R32G32_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_32};
		case VK_FORMAT_R32G32_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_32};

		case VK_FORMAT_R32G32B32_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_32};
		case VK_FORMAT_R32G32B32_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_32};
		case VK_FORMAT_R32G32B32_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_32};

		case VK_FORMAT_R32G32B32A32_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_32};
		case VK_FORMAT_R32G32B32A32_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_32};
		case VK_FORMAT_R32G32B32A32_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_32};

		case VK_FORMAT_R64_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_R, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64G64_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64G64_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64G64_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_RG, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64G64B64_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64G64B64_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64G64B64_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_RGB, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64G64B64A64_UINT: return (gen_gfx_format_t){GEN_GFX_TYPE_UNSIGNED_INT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64G64B64A64_SINT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_INT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_64};
		case VK_FORMAT_R64G64B64A64_SFLOAT: return (gen_gfx_format_t){GEN_GFX_TYPE_SIGNED_FLOAT, GEN_GFX_LAYOUT_RGBA, GEN_GFX_BITDEPTH_64};

		case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return (gen_gfx_format_t){0};

		case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return (gen_gfx_format_t){0};

		case VK_FORMAT_D16_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_X8_D24_UNORM_PACK32: return (gen_gfx_format_t){0};

		case VK_FORMAT_D32_SFLOAT: return (gen_gfx_format_t){0};

		case VK_FORMAT_S8_UINT: return (gen_gfx_format_t){0};

		case VK_FORMAT_D16_UNORM_S8_UINT: return (gen_gfx_format_t){0};

		case VK_FORMAT_D24_UNORM_S8_UINT: return (gen_gfx_format_t){0};

		case VK_FORMAT_D32_SFLOAT_S8_UINT: return (gen_gfx_format_t){0};

		case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_BC2_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_BC2_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_BC3_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_BC3_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_BC4_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_BC4_SNORM_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_BC5_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_BC5_SNORM_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_BC6H_UFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_BC6H_SFLOAT_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_BC7_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_BC7_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_EAC_R11_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_EAC_R11_SNORM_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_EAC_R11G11_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_EAC_R11G11_SNORM_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_8x8_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_8x8_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_10x5_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_10x5_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_10x6_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_10x6_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_10x8_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_10x8_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_10x10_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_10x10_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_12x10_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_12x10_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_12x12_UNORM_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_12x12_SRGB_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_G8B8G8R8_422_UNORM: return (gen_gfx_format_t){0};
		case VK_FORMAT_B8G8R8G8_422_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_R10X6_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_R10X6G10X6_UNORM_2PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_R12X4_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_R12X4G12X4_UNORM_2PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G16B16G16R16_422_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_B16G16R16G16_422_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM: return (gen_gfx_format_t){0};

		case VK_FORMAT_A4R4G4B4_UNORM_PACK16: return (gen_gfx_format_t){0};
		case VK_FORMAT_A4B4G4R4_UNORM_PACK16: return (gen_gfx_format_t){0};

		case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK: return (gen_gfx_format_t){0};
		case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK: return (gen_gfx_format_t){0};

		case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG: return (gen_gfx_format_t){0};
		case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG: return (gen_gfx_format_t){0};

		case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG: return (gen_gfx_format_t){0};
		case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG: return (gen_gfx_format_t){0};

		case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG: return (gen_gfx_format_t){0};
		case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG: return (gen_gfx_format_t){0};

		case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG: return (gen_gfx_format_t){0};
		case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG: return (gen_gfx_format_t){0};

		case VK_FORMAT_MAX_ENUM: return (gen_gfx_format_t){0};
	}
}

static size_t gen_internal_spv_format_size(const SpvReflectFormat format) {
	switch(format) {
		case SPV_REFLECT_FORMAT_UNDEFINED: return 0;
		case SPV_REFLECT_FORMAT_R32_UINT: return 4;
		case SPV_REFLECT_FORMAT_R32_SINT: return 4;
		case SPV_REFLECT_FORMAT_R32_SFLOAT: return 4;
		case SPV_REFLECT_FORMAT_R32G32_UINT: return 8;
		case SPV_REFLECT_FORMAT_R32G32_SINT: return 8;
		case SPV_REFLECT_FORMAT_R32G32_SFLOAT: return 8;
		case SPV_REFLECT_FORMAT_R32G32B32_UINT: return 12;
		case SPV_REFLECT_FORMAT_R32G32B32_SINT: return 12;
		case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT: return 12;
		case SPV_REFLECT_FORMAT_R32G32B32A32_UINT: return 16;
		case SPV_REFLECT_FORMAT_R32G32B32A32_SINT: return 16;
		case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT: return 16;
		case SPV_REFLECT_FORMAT_R64_UINT: return 8;
		case SPV_REFLECT_FORMAT_R64_SINT: return 8;
		case SPV_REFLECT_FORMAT_R64_SFLOAT: return 8;
		case SPV_REFLECT_FORMAT_R64G64_UINT: return 16;
		case SPV_REFLECT_FORMAT_R64G64_SINT: return 16;
		case SPV_REFLECT_FORMAT_R64G64_SFLOAT: return 16;
		case SPV_REFLECT_FORMAT_R64G64B64_UINT: return 24;
		case SPV_REFLECT_FORMAT_R64G64B64_SINT: return 24;
		case SPV_REFLECT_FORMAT_R64G64B64_SFLOAT: return 24;
		case SPV_REFLECT_FORMAT_R64G64B64A64_UINT: return 32;
		case SPV_REFLECT_FORMAT_R64G64B64A64_SINT: return 32;
		case SPV_REFLECT_FORMAT_R64G64B64A64_SFLOAT: return 32;
	}
}

__unused static VkFormat gen_internal_vk_format_from_gfx(const gen_gfx_format_t format) {
	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_RGB && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8_SRGB;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_RGB && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8_SRGB;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_RGB && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8_SRGB;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_BGR && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_BGR && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_BGR && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_BGR && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_BGR && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_BGR && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_RGB && format.layout == GEN_GFX_LAYOUT_BGR && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8_SRGB;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8A8_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8A8_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8A8_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8A8_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8A8_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8A8_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_RGB && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_R8G8B8A8_SRGB;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_BGRA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8A8_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_BGRA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8A8_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_BGRA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8A8_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_BGRA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8A8_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_BGRA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8A8_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_BGRA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8A8_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_RGB && format.layout == GEN_GFX_LAYOUT_BGRA && format.bitdepth == GEN_GFX_BITDEPTH_8) return VK_FORMAT_B8G8R8A8_SRGB;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16A16_UNORM;
	if(format.type == GEN_GFX_TYPE_SIGNED_NORMALIZED && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16A16_SNORM;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16A16_USCALED;
	if(format.type == GEN_GFX_TYPE_SIGNED_SCALED && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16A16_SSCALED;
	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16A16_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16A16_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_16) return VK_FORMAT_R16G16B16A16_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32G32_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32G32_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32G32_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32G32B32_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32G32B32_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32G32B32_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32G32B32A32_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32G32B32A32_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_32) return VK_FORMAT_R32G32B32A32_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_R && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64G64_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64G64_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_RG && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64G64_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64G64B64_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64G64B64_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_RGB && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64G64B64_SFLOAT;

	if(format.type == GEN_GFX_TYPE_UNSIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64G64B64A64_UINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_INT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64G64B64A64_SINT;
	if(format.type == GEN_GFX_TYPE_SIGNED_FLOAT && format.layout == GEN_GFX_LAYOUT_RGBA && format.bitdepth == GEN_GFX_BITDEPTH_64) return VK_FORMAT_R64G64B64A64_SFLOAT;

	return VK_FORMAT_UNDEFINED;
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

#if MODE == DEBUG
	const VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT, NULL, 0, VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT, gen_internal_vk_debug_callback, NULL};
	result = vkCreateDebugUtilsMessengerEXT(out_context->internal_instance, &debug_messenger_create_info, out_context->internal_allocator, &out_context->internal_debug_messenger);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateDebugUtilsMessengerEXT);
#endif

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
	if(transfer_queue_family_index == UINT32_MAX) {
		glog(WARNING, "No dedicated transfer queue family available on device, falling back to graphics queue");
		transfer_queue_family_index = graphics_queue_family_index;
	}
	out_context->internal_graphics_queue_index = graphics_queue_family_index;
	out_context->internal_transfer_queue_index = transfer_queue_family_index;

	const VkPhysicalDeviceFeatures physical_device_features = {0};
	const char* const device_extensions[] = {"VK_KHR_swapchain"};

	const VkDeviceQueueCreateInfo device_queue_create_infos[] = {{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, NULL, 0, graphics_queue_family_index, 1, (float[]){1.0f}}, {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, NULL, 0, transfer_queue_family_index, 1, (float[]){1.0f}}};
	const VkDeviceCreateInfo device_create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, NULL, 0, graphics_queue_family_index != transfer_queue_family_index ? 2 : 1, device_queue_create_infos, 0, NULL, sizeof(device_extensions) / sizeof(device_extensions[0]), device_extensions, &physical_device_features};
	result = vkCreateDevice(out_context->internal_physical_device, &device_create_info, out_context->internal_allocator, &out_context->internal_device);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateDevice);

	vkGetDeviceQueue(out_context->internal_device, graphics_queue_family_index, 0, &out_context->internal_graphics_queue);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_context_await(gen_gfx_context_t* const restrict context) {
	GEN_FRAME_BEGIN(gen_gfx_context_await);

	GEN_NULL_CHECK(context);

	VkResult result = vkDeviceWaitIdle(context->internal_device);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkDeviceWaitIdle);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_context_destroy(gen_gfx_context_t* const restrict context) {
	GEN_FRAME_BEGIN(gen_gfx_context_destroy);

	GEN_NULL_CHECK(context);

	vkDestroyDevice(context->internal_device, context->internal_allocator);
#if MODE == DEBUG
	vkDestroyDebugUtilsMessengerEXT(context->internal_instance, context->internal_debug_messenger, context->internal_allocator);
#endif
	vkDestroyInstance(context->internal_instance, context->internal_allocator);

	GEN_ALL_OK;
}

GEN_INTERNAL_ERRORABLE gen_internal_gfx_evaluate_surface_format(const gen_gfx_format_t format, size_t* const restrict out_score) {
	GEN_FRAME_BEGIN(gen_internal_gfx_evaluate_surface_format);

	*out_score = 0;

	switch(format.type) {
		case GEN_GFX_TYPE_UNKNOWN: break;
		case GEN_GFX_TYPE_SIGNED_INT: {
			*out_score += 5;
			break;
		}
		case GEN_GFX_TYPE_SIGNED_FLOAT: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_TYPE_SIGNED_SCALED: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_TYPE_SIGNED_NORMALIZED: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_TYPE_SIGNED_RGB: {
			*out_score += 20;
			break;
		}
		case GEN_GFX_TYPE_UNSIGNED_INT: {
			*out_score += 5;
			break;
		}
		case GEN_GFX_TYPE_UNSIGNED_FLOAT: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_TYPE_UNSIGNED_SCALED: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_TYPE_UNSIGNED_NORMALIZED: {
			*out_score += 10;
			break;
		}
	}

	switch(format.layout) {
		case GEN_GFX_LAYOUT_UNKNOWN: break;
		case GEN_GFX_LAYOUT_RGB: {
			*out_score += 5;
			break;
		}
		case GEN_GFX_LAYOUT_RGBA: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_LAYOUT_ARGB: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_LAYOUT_BGR: {
			*out_score += 5;
			break;
		}
		case GEN_GFX_LAYOUT_BGRA: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_LAYOUT_ABGR: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_LAYOUT_R: {
			*out_score += 1;
			break;
		}
		case GEN_GFX_LAYOUT_RG: {
			*out_score += 2;
			break;
		}
		case GEN_GFX_LAYOUT_DEPTH: {
			*out_score += 1;
			break;
		}
	}

	switch(format.bitdepth) {
		case GEN_GFX_BITDEPTH_INHERIT: break;
		case GEN_GFX_BITDEPTH_8: {
			*out_score += 2;
			break;
		}
		case GEN_GFX_BITDEPTH_12: {
			*out_score += 4;
			break;
		}
		case GEN_GFX_BITDEPTH_14: {
			*out_score += 6;
			break;
		}
		case GEN_GFX_BITDEPTH_16: {
			*out_score += 8;
			break;
		}
		case GEN_GFX_BITDEPTH_24: {
			*out_score += 10;
			break;
		}
		case GEN_GFX_BITDEPTH_32: {
			*out_score += 12;
			break;
		}
		case GEN_GFX_BITDEPTH_64: {
			*out_score += 14;
			break;
		}
	}

	GEN_ALL_OK;
}

GEN_INTERNAL_ERRORABLE gen_internal_vk_create_swapchain(gen_gfx_context_t* const restrict context, gen_window_system_t* const restrict window_system, gen_window_t* const restrict window, gen_gfx_targetable_t* const restrict targetable, const gint2 surface_extent) {
	GEN_FRAME_BEGIN(gen_internal_vk_create_swapchain);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(window);
	GEN_NULL_CHECK(targetable);

	const VkXcbSurfaceCreateInfoKHR surface_create_info = {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR, NULL, 0, window_system->internal_connection, window->window};
	VkResult result = vkCreateXcbSurfaceKHR(context->internal_instance, &surface_create_info, context->internal_allocator, &targetable->internal_surface);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateXcbSurfaceKHR);

	VkSurfaceCapabilitiesKHR surface_capabilities = {0};
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->internal_physical_device, targetable->internal_surface, &surface_capabilities);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);

	uint32_t surface_format_count = 0;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(context->internal_physical_device, targetable->internal_surface, &surface_format_count, NULL);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkGetPhysicalDeviceSurfaceFormatsKHR);
	VkSurfaceFormatKHR* surface_formats = NULL;
	gen_error_t error = gzalloc((void**) &surface_formats, surface_format_count, sizeof(VkSurfaceFormatKHR));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(context->internal_physical_device, targetable->internal_surface, &surface_format_count, surface_formats);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkGetPhysicalDeviceSurfaceFormatsKHR);

	bool found_format = false;
	VkSurfaceFormatKHR best_format = {0};
	size_t best_format_score = 0;
	for(size_t i = 0; i < surface_format_count; ++i) {
		const VkSurfaceFormatKHR format = surface_formats[i];
		if(format.colorSpace != VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) continue;

		size_t score = 0;
		error = gen_internal_gfx_evaluate_surface_format(gen_internal_gfx_format_from_vk_format(format.format), &score);
		GEN_ERROR_OUT_IF(error, "`gen_internal_gfx_evaluate_surface_format` failed");

		if(score > best_format_score) {
			found_format = true;
			best_format = format;
			best_format_score = score;
		}
	}

	error = gfree(surface_formats);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");
	if(!found_format) GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "No suitable surface format is supported by the selected physical device");

	uint32_t present_mode_count = 0;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(context->internal_physical_device, targetable->internal_surface, &present_mode_count, NULL);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkGetPhysicalDeviceSurfacePresentModesKHR);
	VkPresentModeKHR* present_modes = NULL;
	error = gzalloc((void**) &present_modes, present_mode_count, sizeof(VkPresentModeKHR));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(context->internal_physical_device, targetable->internal_surface, &present_mode_count, present_modes);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkGetPhysicalDeviceSurfacePresentModesKHR);

	VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;
	size_t best_mode_score = 0;

	for(size_t i = 0; i < present_mode_count; ++i) {
		const VkPresentModeKHR mode = present_modes[i];

		size_t score = 0;
		switch(mode) {
			case VK_PRESENT_MODE_IMMEDIATE_KHR: {
				score = 1;
				break;
			}
			case VK_PRESENT_MODE_MAILBOX_KHR: {
				score = 4;
				break;
			}
			case VK_PRESENT_MODE_FIFO_KHR: {
				score = 2;
				break;
			}
			case VK_PRESENT_MODE_FIFO_RELAXED_KHR: {
				score = 3;
				break;
			}
			case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR: break;
			case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR: break;
			case VK_PRESENT_MODE_MAX_ENUM_KHR: break;
		}

		if(score > best_mode_score) {
			best_mode = mode;
			best_mode_score = score;
		}
	}

	error = gfree(present_modes);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	const VkExtent2D extent = {(uint32_t) surface_extent.x, (uint32_t) surface_extent.y};

	uint32_t image_count = surface_capabilities.minImageCount + 1;
	if(surface_capabilities.maxImageCount && image_count > surface_capabilities.maxImageCount) image_count = surface_capabilities.maxImageCount;
	const VkSwapchainCreateInfoKHR swapchain_create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, NULL, 0, targetable->internal_surface, image_count, best_format.format, best_format.colorSpace, extent, 1, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE, 0, NULL, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, best_mode, VK_FALSE, VK_NULL_HANDLE};
	result = vkCreateSwapchainKHR(context->internal_device, &swapchain_create_info, context->internal_allocator, &targetable->internal_swapchain);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateSwapchainKHR);

	result = vkGetSwapchainImagesKHR(context->internal_device, targetable->internal_swapchain, &image_count, NULL);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkGetSwapchainImagesKHR);
	error = gzalloc((void**) &targetable->internal_swapchain_images, image_count, sizeof(VkImage));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
	error = gzalloc((void**) &targetable->internal_swapchain_image_views, image_count, sizeof(VkImageView));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
	result = vkGetSwapchainImagesKHR(context->internal_device, targetable->internal_swapchain, &image_count, targetable->internal_swapchain_images);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkGetSwapchainImagesKHR);

	targetable->internal_swapchain_image_format = gen_internal_gfx_format_from_vk_format(best_format.format);
	targetable->internal_swapchain_image_extent = surface_extent;
	targetable->internal_swapchain_image_count = (size_t) image_count;
	targetable->internal_viewport = (VkViewport){0.0f, 0.0f, (float) targetable->internal_swapchain_image_extent.x, (float) targetable->internal_swapchain_image_extent.y, 0.0f, 1.0f};
	targetable->internal_scissor = (VkRect2D){{0, 0}, extent};

	for(size_t i = 0; i < image_count; ++i) {
		const VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, NULL, 0, targetable->internal_swapchain_images[i], VK_IMAGE_VIEW_TYPE_2D, best_format.format, {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY}, {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}};
		result = vkCreateImageView(context->internal_device, &image_view_create_info, context->internal_allocator, &targetable->internal_swapchain_image_views[i]);
		GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateImageView);
	}

	const VkAttachmentDescription color_attachment = {0, best_format.format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};
	const VkAttachmentReference color_attachment_reference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
	const VkSubpassDescription subpass_description = {0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, NULL, 1, &color_attachment_reference, NULL, NULL, 0, NULL};
	const VkSubpassDependency subpass_dependency = {VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0};
	const VkRenderPassCreateInfo render_pass_create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, NULL, 0, 1, &color_attachment, 1, &subpass_description, 1, &subpass_dependency};
	result = vkCreateRenderPass(context->internal_device, &render_pass_create_info, context->internal_allocator, &targetable->internal_render_pass);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateRenderPass);

	error = gzalloc((void**) &targetable->internal_framebuffers, targetable->internal_swapchain_image_count, sizeof(VkFramebuffer));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");

	for(size_t i = 0; i < targetable->internal_swapchain_image_count; ++i) {
		const VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, NULL, 0, targetable->internal_render_pass, 1, &targetable->internal_swapchain_image_views[i], (uint32_t) targetable->internal_swapchain_image_extent.x, (uint32_t) targetable->internal_swapchain_image_extent.y, 1};
		result = vkCreateFramebuffer(context->internal_device, &framebuffer_create_info, context->internal_allocator, &targetable->internal_framebuffers[i]);
		GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateFramebuffer);
	}

	GEN_ALL_OK;
}

gen_error_t gen_gfx_targetable_create(gen_gfx_context_t* const restrict context, gen_window_system_t* const restrict window_system, gen_window_t* const restrict window, gen_gfx_targetable_t* const restrict out_targetable) {
	GEN_FRAME_BEGIN(gen_gfx_targetable_create);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(window);
	GEN_NULL_CHECK(out_targetable);

	gen_window_attribute_t attribute = {0};
	gen_error_t error = gen_window_fetch(window_system, window, GEN_WINDOW_ATTRIBUTE_EXTENT, &attribute);
	GEN_ERROR_OUT_IF(error, "`gen_window_fetch` failed");
	error = gen_internal_vk_create_swapchain(context, window_system, window, out_targetable, attribute.extent);
	GEN_ERROR_OUT_IF(error, "`gen_internal_vk_create_swapchain` failed");

	const VkCommandPoolCreateInfo command_pool_create_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, NULL, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, context->internal_graphics_queue_index};
	VkResult result = vkCreateCommandPool(context->internal_device, &command_pool_create_info, context->internal_allocator, &out_targetable->internal_command_pool);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateCommandPool);

	const VkCommandBufferAllocateInfo command_buffer_allocate_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, NULL, out_targetable->internal_command_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, GEN_GFX_FRAMES_IN_FLIGHT};
	result = vkAllocateCommandBuffers(context->internal_device, &command_buffer_allocate_info, out_targetable->internal_command_buffers);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkAllocateCommandBuffers);

	if(context->internal_transfer_queue_index != context->internal_graphics_queue_index) {
		const VkCommandPoolCreateInfo transfer_command_pool_create_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, NULL, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, context->internal_transfer_queue_index};
		result = vkCreateCommandPool(context->internal_device, &transfer_command_pool_create_info, context->internal_allocator, &out_targetable->internal_transfer_command_pool);
		GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateCommandPool);
	}
	else
		out_targetable->internal_transfer_command_pool = out_targetable->internal_command_pool;

	const VkCommandBufferAllocateInfo transfer_command_buffer_allocate_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, NULL, out_targetable->internal_transfer_command_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1};
	result = vkAllocateCommandBuffers(context->internal_device, &transfer_command_buffer_allocate_info, &out_targetable->internal_transfer_command_buffer);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkAllocateCommandBuffers);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_targetable_geometry_update(gen_gfx_context_t* const restrict context, gen_window_system_t* const restrict window_system, gen_window_t* const restrict window, gen_gfx_targetable_t* const restrict targetable, const gint2 extent) {
	GEN_FRAME_BEGIN(gen_gfx_targetable_geometry_update);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(window);
	GEN_NULL_CHECK(targetable);

	VkResult result = vkDeviceWaitIdle(context->internal_device);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkDeviceWaitIdle);

	for(size_t i = 0; i < targetable->internal_swapchain_image_count; ++i) {
		vkDestroyFramebuffer(context->internal_device, targetable->internal_framebuffers[i], context->internal_allocator);
	}

	gen_error_t error = gfree(targetable->internal_framebuffers);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	vkDestroyRenderPass(context->internal_device, targetable->internal_render_pass, context->internal_allocator);

	for(size_t i = 0; i < targetable->internal_swapchain_image_count; ++i) vkDestroyImageView(context->internal_device, targetable->internal_swapchain_image_views[i], context->internal_allocator);
	vkDestroySwapchainKHR(context->internal_device, targetable->internal_swapchain, context->internal_allocator);

	vkDestroySurfaceKHR(context->internal_instance, targetable->internal_surface, context->internal_allocator);

	error = gfree(targetable->internal_swapchain_images);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	error = gfree(targetable->internal_swapchain_image_views);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	error = gen_internal_vk_create_swapchain(context, window_system, window, targetable, extent);
	GEN_ERROR_OUT_IF(error, "`gen_internal_vk_create_swapchain` failed");

	GEN_ALL_OK;
}

gen_error_t gen_gfx_targetable_destroy(gen_gfx_context_t* const restrict context, gen_window_system_t* const restrict window_system, gen_window_t* const restrict window, gen_gfx_targetable_t* const restrict targetable) {
	GEN_FRAME_BEGIN(gen_gfx_targetable_destroy);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(window);
	GEN_NULL_CHECK(targetable);

	for(size_t i = 0; i < targetable->internal_swapchain_image_count; ++i) {
		vkDestroyFramebuffer(context->internal_device, targetable->internal_framebuffers[i], context->internal_allocator);
	}

	gen_error_t error = gfree(targetable->internal_framebuffers);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	vkDestroyRenderPass(context->internal_device, targetable->internal_render_pass, context->internal_allocator);

	for(size_t i = 0; i < targetable->internal_swapchain_image_count; ++i) vkDestroyImageView(context->internal_device, targetable->internal_swapchain_image_views[i], context->internal_allocator);

	error = gfree(targetable->internal_swapchain_images);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	error = gfree(targetable->internal_swapchain_image_views);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	vkDestroyCommandPool(context->internal_device, targetable->internal_command_pool, context->internal_allocator);
	if(context->internal_transfer_queue_index != context->internal_graphics_queue_index) vkDestroyCommandPool(context->internal_device, targetable->internal_transfer_command_pool, context->internal_allocator);

	vkDestroySwapchainKHR(context->internal_device, targetable->internal_swapchain, context->internal_allocator);
	vkDestroySurfaceKHR(context->internal_instance, targetable->internal_surface, context->internal_allocator);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_pipeline_create(gen_gfx_context_t* const restrict context, const gen_gfx_targetable_t* const restrict targetable, gen_gfx_pipeline_t* const restrict out_pipeline, const gen_gfx_shader_t** const restrict shaders, const size_t shaders_length) {
	GEN_FRAME_BEGIN(gen_gfx_pipeline_create);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(out_pipeline);
	GEN_NULL_CHECK(shaders);

	VkPipelineShaderStageCreateInfo* pipeline_shader_stage_create_infos = NULL;
	gen_error_t error = gzalloc((void**) &pipeline_shader_stage_create_infos, shaders_length, sizeof(VkPipelineShaderStageCreateInfo));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");

	for(size_t i = 0; i < shaders_length; ++i) {
		const gen_gfx_shader_t* const restrict shader = shaders[i];
		pipeline_shader_stage_create_infos[i] = (VkPipelineShaderStageCreateInfo){VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, NULL, 0, VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM, shader->internal_shader_module, "main", NULL};
		switch(shader->type) {
			case GEN_GFX_SHADER_TYPE_VERTEX: {
				pipeline_shader_stage_create_infos[i].stage = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			}
			case GEN_GFX_SHADER_TYPE_FRAGMENT: {
				pipeline_shader_stage_create_infos[i].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			}
			case GEN_GFX_SHADER_TYPE_GEOMETRY: {
				pipeline_shader_stage_create_infos[i].stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				break;
			}
			case GEN_GFX_SHADER_TYPE_TESSELATION: {
				pipeline_shader_stage_create_infos[i].stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				break;
			}
			case GEN_GFX_SHADER_TYPE_COMPUTE: {
				pipeline_shader_stage_create_infos[i].stage = VK_SHADER_STAGE_COMPUTE_BIT;
				break;
			}
		}
	}

	const gen_gfx_shader_t* vertex_shader = NULL;
	for(size_t i = 0; i < shaders_length; ++i)
		if(shaders[i]->type == GEN_GFX_SHADER_TYPE_VERTEX) vertex_shader = shaders[i];

	uint32_t input_count = 0;
	SpvReflectResult spv_result = spvReflectEnumerateInputVariables(&vertex_shader->internal_reflected_module, &input_count, NULL);
	GEN_INTERNAL_ERROR_OUT_IF_SPVRESULT(spv_result, spvReflectEnumerateInputVariables);

	SpvReflectInterfaceVariable** input_variables = NULL;
	error = gzalloc((void**) &input_variables, input_count, sizeof(SpvReflectInterfaceVariable*));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");

	VkVertexInputAttributeDescription* vertex_input_attribute_descriptions = NULL;
	error = gzalloc((void**) &vertex_input_attribute_descriptions, input_count, sizeof(VkVertexInputAttributeDescription));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");

	spv_result = spvReflectEnumerateInputVariables(&vertex_shader->internal_reflected_module, &input_count, input_variables);
	GEN_INTERNAL_ERROR_OUT_IF_SPVRESULT(spv_result, spvReflectEnumerateInputVariables);

	VkVertexInputBindingDescription vertex_input_binding_description = {0, 0, VK_VERTEX_INPUT_RATE_VERTEX};

	for(size_t i = 0; i < input_count; ++i) {
		if(input_variables[i]->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) continue;
		vertex_input_attribute_descriptions[i] = (VkVertexInputAttributeDescription){input_variables[i]->location, vertex_input_binding_description.binding, (VkFormat) input_variables[i]->format, 0};
	}

	for(size_t i = 0; i < input_count; ++i) {
		size_t j = 0;
		for(; vertex_input_attribute_descriptions[j].location != i; ++j)
			;
		vertex_input_attribute_descriptions[j].offset = vertex_input_binding_description.stride;
		vertex_input_binding_description.stride += gen_internal_spv_format_size(input_variables[j]->format);
	}
	out_pipeline->vertex_size = vertex_input_binding_description.stride;

	const VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, NULL, 0, 1, &vertex_input_binding_description, input_count, vertex_input_attribute_descriptions};

	error = gfree(input_variables);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	const VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, NULL, 0, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE};
	const VkPipelineTessellationStateCreateInfo pipeline_tessellation_state_create_info = {VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO, NULL, 0, 0};
	const VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, NULL, 0, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f};
	const VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, NULL, 0, VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, NULL, VK_FALSE, VK_FALSE};
	const VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_state_create_info = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO, NULL, 0, VK_FALSE, VK_FALSE, VK_COMPARE_OP_NEVER, VK_FALSE, VK_FALSE, {VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_NEVER, 0, 0, 0}, {VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_NEVER, 0, 0, 0}, 0.0f, 1.0f};
	const VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_state = {VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
	const VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, NULL, 0, VK_FALSE, VK_LOGIC_OP_MAX_ENUM, 1, &pipeline_color_blend_attachment_state, {0.0f, 0.0f, 0.0f, 0.0f}};
	const VkDynamicState pipeline_dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	const VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, NULL, 0, sizeof(pipeline_dynamic_states) / sizeof(pipeline_dynamic_states[0]), pipeline_dynamic_states};
	const VkPipelineLayoutCreateInfo pipeline_layout_create_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, NULL, 0, 0, NULL, 0, NULL};
	VkResult result = vkCreatePipelineLayout(context->internal_device, &pipeline_layout_create_info, context->internal_allocator, &out_pipeline->internal_layout);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreatePipelineLayout);

	const VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, NULL, 0, 1, &targetable->internal_viewport, 1, &targetable->internal_scissor};

	const VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, NULL, 0, (uint32_t) shaders_length, pipeline_shader_stage_create_infos, &pipeline_vertex_input_state_create_info, &pipeline_input_assembly_state_create_info, &pipeline_tessellation_state_create_info, &pipeline_viewport_state_create_info, &pipeline_rasterization_state_create_info, &pipeline_multisample_state_create_info, &pipeline_depth_stencil_state_create_info, &pipeline_color_blend_state_create_info, &pipeline_dynamic_state_create_info, out_pipeline->internal_layout, targetable->internal_render_pass, 0, VK_NULL_HANDLE, -1};
	result = vkCreateGraphicsPipelines(context->internal_device, VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, context->internal_allocator, &out_pipeline->internal_pipeline);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateGraphicsPipelines);

	error = gfree(pipeline_shader_stage_create_infos);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	error = gfree(vertex_input_attribute_descriptions);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	for(size_t i = 0; i < GEN_GFX_FRAMES_IN_FLIGHT; ++i) {
		const VkSemaphoreCreateInfo image_available_semaphore_create_info = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, NULL, 0};
		result = vkCreateSemaphore(context->internal_device, &image_available_semaphore_create_info, context->internal_allocator, &out_pipeline->internal_image_available_semaphores[i]);
		GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateSemaphore);

		const VkSemaphoreCreateInfo render_finished_semaphore_create_info = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, NULL, 0};
		result = vkCreateSemaphore(context->internal_device, &render_finished_semaphore_create_info, context->internal_allocator, &out_pipeline->internal_render_finished_semaphores[i]);
		GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateSemaphore);

		const VkFenceCreateInfo in_flight_fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, NULL, VK_FENCE_CREATE_SIGNALED_BIT};
		result = vkCreateFence(context->internal_device, &in_flight_fence_create_info, context->internal_allocator, &out_pipeline->internal_in_flight_fences[i]);
		GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateFence);
	}

	GEN_ALL_OK;
}

gen_error_t gen_gfx_pipeline_frame_begin(gen_gfx_context_t* const restrict context, gen_gfx_targetable_t* const restrict targetable, gen_gfx_pipeline_t* const restrict pipeline, const gfloat4 clear_color, gen_gfx_frame_t* const restrict out_frame) {
	GEN_FRAME_BEGIN(gen_gfx_pipeline_frame_begin);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(targetable);
	GEN_NULL_CHECK(pipeline);
	GEN_NULL_CHECK(out_frame);

	VkResult result = vkWaitForFences(context->internal_device, 1, &pipeline->internal_in_flight_fences[pipeline->internal_current_frame], VK_TRUE, UINT64_MAX);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkWaitForFences);

	result = vkResetFences(context->internal_device, 1, &pipeline->internal_in_flight_fences[pipeline->internal_current_frame]);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkResetFences);

	result = vkAcquireNextImageKHR(context->internal_device, targetable->internal_swapchain, UINT64_MAX, pipeline->internal_image_available_semaphores[pipeline->internal_current_frame], VK_NULL_HANDLE, &out_frame->internal_image_index);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkAcquireNextImageKHR);

	result = vkResetCommandBuffer(targetable->internal_command_buffers[pipeline->internal_current_frame], 0);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkResetCommandBuffer);

	const VkCommandBufferBeginInfo command_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, NULL, 0, NULL};
	result = vkBeginCommandBuffer(targetable->internal_command_buffers[pipeline->internal_current_frame], &command_buffer_begin_info);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkBeginCommandBuffer);

	vkCmdSetViewport(targetable->internal_command_buffers[pipeline->internal_current_frame], 0, 1, &targetable->internal_viewport);
	vkCmdSetScissor(targetable->internal_command_buffers[pipeline->internal_current_frame], 0, 1, &targetable->internal_scissor);

	const VkClearValue clear_value = {.color = {.float32 = {clear_color.x, clear_color.y, clear_color.z, clear_color.w}}};
	const VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, NULL, targetable->internal_render_pass, targetable->internal_framebuffers[out_frame->internal_image_index], {{0, 0}, {(uint32_t) targetable->internal_swapchain_image_extent.x, (uint32_t) targetable->internal_swapchain_image_extent.y}}, 1, &clear_value};
	vkCmdBeginRenderPass(targetable->internal_command_buffers[pipeline->internal_current_frame], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(targetable->internal_command_buffers[pipeline->internal_current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->internal_pipeline);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_pipeline_frame_draw_vertex_buffer(gen_gfx_context_t* const restrict context, gen_gfx_targetable_t* const restrict targetable, gen_gfx_pipeline_t* const restrict pipeline, const gen_gfx_draw_buffer_t* const restrict vertex_buffer, gen_gfx_frame_t* const restrict frame) {
	GEN_FRAME_BEGIN(gen_gfx_pipeline_frame_draw_vertex_buffer);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(targetable);
	GEN_NULL_CHECK(pipeline);
	GEN_NULL_CHECK(vertex_buffer);
	GEN_NULL_CHECK(frame);

	const VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(targetable->internal_command_buffers[pipeline->internal_current_frame], 0, 1, &vertex_buffer->internal_buffer, &offset);
	vkCmdDraw(targetable->internal_command_buffers[pipeline->internal_current_frame], (uint32_t) (vertex_buffer->buffer.size / pipeline->vertex_size), 1, 0, 0);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_pipeline_frame_end(gen_gfx_context_t* const restrict context, gen_gfx_targetable_t* const restrict targetable, gen_gfx_pipeline_t* const restrict pipeline, const gen_gfx_frame_t* const restrict frame) {
	GEN_FRAME_BEGIN(gen_gfx_pipeline_frame_end);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(targetable);
	GEN_NULL_CHECK(pipeline);
	GEN_NULL_CHECK(frame);

	vkCmdEndRenderPass(targetable->internal_command_buffers[pipeline->internal_current_frame]);

	VkResult result = vkEndCommandBuffer(targetable->internal_command_buffers[pipeline->internal_current_frame]);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkEndCommandBuffer);

	const VkPipelineStageFlags flags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	const VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO, NULL, 1, &pipeline->internal_image_available_semaphores[pipeline->internal_current_frame], flags, 1, &targetable->internal_command_buffers[pipeline->internal_current_frame], 1, &pipeline->internal_render_finished_semaphores[pipeline->internal_current_frame]};

	result = vkQueueSubmit(context->internal_graphics_queue, 1, &submit_info, pipeline->internal_in_flight_fences[pipeline->internal_current_frame]);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkQueueSubmit);

	const VkPresentInfoKHR present_info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, NULL, 1, &pipeline->internal_render_finished_semaphores[pipeline->internal_current_frame], 1, &targetable->internal_swapchain, &frame->internal_image_index, NULL};
	result = vkQueuePresentKHR(context->internal_graphics_queue, &present_info);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkQueuePresentKHR);

	pipeline->internal_current_frame = (pipeline->internal_current_frame + 1) % GEN_GFX_FRAMES_IN_FLIGHT;

	GEN_ALL_OK;
}

gen_error_t gen_gfx_pipeline_destroy(gen_gfx_context_t* const restrict context, gen_gfx_targetable_t* const restrict targetable, gen_gfx_pipeline_t* const restrict pipeline) {
	GEN_FRAME_BEGIN(gen_gfx_pipeline_destroy);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(pipeline);
	GEN_NULL_CHECK(targetable);

	VkResult result = vkQueueWaitIdle(context->internal_graphics_queue);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkQueueWaitIdle);

	for(size_t i = 0; i < GEN_GFX_FRAMES_IN_FLIGHT; ++i) {
		vkDestroySemaphore(context->internal_device, pipeline->internal_image_available_semaphores[i], context->internal_allocator);
		vkDestroySemaphore(context->internal_device, pipeline->internal_render_finished_semaphores[i], context->internal_allocator);
		vkDestroyFence(context->internal_device, pipeline->internal_in_flight_fences[i], context->internal_allocator);
	}

	vkDestroyPipelineLayout(context->internal_device, pipeline->internal_layout, context->internal_allocator);
	vkDestroyPipeline(context->internal_device, pipeline->internal_pipeline, context->internal_allocator);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_shader_create(gen_gfx_context_t* const restrict context, gen_gfx_shader_t* const restrict out_shader, const uint8_t* const restrict shader_data, const size_t shader_data_size) {
	GEN_FRAME_BEGIN(gen_gfx_shader_create);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(out_shader);
	GEN_NULL_CHECK(shader_data);

	gen_error_t error = gzalloc((void**) &out_shader->data, shader_data_size, sizeof(uint8_t));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
	error = gen_memory_copy(shader_data, shader_data_size, out_shader->data, shader_data_size, shader_data_size);
	GEN_ERROR_OUT_IF(error, "`gen_memory_copy` failed");
	out_shader->data_size = shader_data_size;

	const VkShaderModuleCreateInfo shader_create_info = {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, NULL, 0, shader_data_size, (uint32_t*) shader_data};
	VkResult result = vkCreateShaderModule(context->internal_device, &shader_create_info, context->internal_allocator, &out_shader->internal_shader_module);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateShaderModule);

	SpvReflectResult spv_result = spvReflectCreateShaderModule(shader_data_size, shader_data, &out_shader->internal_reflected_module);
	GEN_INTERNAL_ERROR_OUT_IF_SPVRESULT(spv_result, spvReflectCreateShaderModule);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_shader_destroy(gen_gfx_context_t* const restrict context, gen_gfx_shader_t* const restrict shader) {
	GEN_FRAME_BEGIN(gen_gfx_shader_destroy);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(shader);

	gen_error_t error = gfree(shader->data);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	vkDestroyShaderModule(context->internal_device, shader->internal_shader_module, context->internal_allocator);

	spvReflectDestroyShaderModule(&shader->internal_reflected_module);

	GEN_ALL_OK;
}

static size_t gen_internal_vk_find_memory_type(const gen_gfx_context_t* const restrict context, const uint32_t type_filter, const VkMemoryPropertyFlags properties) {
	GEN_FRAME_BEGIN(gen_internal_vk_find_memory_type);

	VkPhysicalDeviceMemoryProperties physical_device_memory_properties = {0};
	vkGetPhysicalDeviceMemoryProperties(context->internal_physical_device, &physical_device_memory_properties);

	for(size_t i = 0; i < physical_device_memory_properties.memoryTypeCount; ++i) {
		if(type_filter & (1 << i) && (physical_device_memory_properties.memoryTypes[i].propertyFlags & properties) == properties) return i;
	}

	return SIZE_MAX;
}

gen_error_t gen_gfx_vertex_buffer_create(gen_gfx_context_t* const restrict context, const uint8_t* const restrict data, const size_t size, gen_gfx_draw_buffer_t* const restrict out_buffer) {
	GEN_FRAME_BEGIN(gen_gfx_vertex_buffer_create);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(data);
	GEN_NULL_CHECK(out_buffer);

	gen_error_t error = gzalloc((void**) &out_buffer->buffer.data, size, sizeof(uint8_t));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");

	error = gen_memory_copy(data, size, out_buffer->buffer.data, size, size);
	GEN_ERROR_OUT_IF(error, "`gen_memory_copy` failed");

	out_buffer->buffer.size = size;

	const VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, NULL, 0, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, 0, NULL};
	VkResult result = vkCreateBuffer(context->internal_device, &buffer_create_info, context->internal_allocator, &out_buffer->internal_buffer);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkCreateBuffer);

	VkMemoryRequirements memory_requirements = {0};
	vkGetBufferMemoryRequirements(context->internal_device, out_buffer->internal_buffer, &memory_requirements);

	const VkMemoryAllocateInfo memory_allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, NULL, memory_requirements.size, (uint32_t) gen_internal_vk_find_memory_type(context, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)};
	result = vkAllocateMemory(context->internal_device, &memory_allocate_info, context->internal_allocator, &out_buffer->internal_memory);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkAllocateMemory);

	result = vkBindBufferMemory(context->internal_device, out_buffer->internal_buffer, out_buffer->internal_memory, 0);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkBindBufferMemory);

	void* mapped = NULL;
	result = vkMapMemory(context->internal_device, out_buffer->internal_memory, 0, size, 0, &mapped);
	GEN_INTERNAL_ERROR_OUT_IF_VKRESULT(result, vkMapMemory);
	error = gen_memory_copy(data, size, mapped, size, size);
	GEN_ERROR_OUT_IF(error, "`gen_memory_copy` failed");
	vkUnmapMemory(context->internal_device, out_buffer->internal_memory);

	GEN_ALL_OK;
}

gen_error_t gen_gfx_vertex_buffer_destroy(gen_gfx_context_t* const restrict context, gen_gfx_draw_buffer_t* const restrict buffer) {
	GEN_FRAME_BEGIN(gen_gfx_vertex_buffer_destroy);

	GEN_NULL_CHECK(context);
	GEN_NULL_CHECK(buffer);

	gen_error_t error = gfree(buffer->buffer.data);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	vkDestroyBuffer(context->internal_device, buffer->internal_buffer, context->internal_allocator);
	vkFreeMemory(context->internal_device, buffer->internal_memory, context->internal_allocator);

	GEN_ALL_OK;
}
