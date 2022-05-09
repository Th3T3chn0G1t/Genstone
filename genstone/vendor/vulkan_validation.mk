_VULKAN_VALIDATION_DEFINES = -DAPI_NAME=vulkan
_VULKAN_VALIDATION_INCLUDES = -Igenstone/vendor/Vulkan-ValidationLayers/layers -Igenstone/vendor/Vulkan-ValidationLayers -Igenstone/vendor/Vulkan-ValidationLayers/layers/generated
_VULKAN_VALIDATION_CXXFLAGS = $(_VULKAN_VALIDATION_INCLUDES) $(_VULKAN_VALIDATION_DEFINES) $(VULKAN_CFLAGS) $(_VULKAN_CFLAGS) $(SPIRV_CFLAGS) -fno-strict-aliasing -fno-builtin-memcmp -fvisibility=hidden -Wno-macro-redefined
_VULKAN_VALIDATION_COMMON_LFLAGS = $(VULKAN_LFLAGS) $(SPIRV_LFLAGS)
_VULKAN_VALIDATION_LFLAGS = $(_VULKAN_VALIDATION_COMMON_LFLAGS) $(_VULKAN_LFLAGS)

VULKAN_VALIDATION_LFLAGS = $(_VULKAN_VALIDATION_COMMON_LFLAGS)

VULKAN_VALIDATION_C_SOURCES = $(wildcard genstone/vendor/Vulkan-ValidationLayers/layers/*.c)
VULKAN_VALIDATION_CXX_SOURCES = $(wildcard genstone/vendor/Vulkan-ValidationLayers/layers/*.cpp) $(wildcard genstone/vendor/Vulkan-ValidationLayers/layers/generated/*.cpp)
VULKAN_VALIDATION_OBJECTS = $(VULKAN_VALIDATION_CXX_SOURCES:.cpp=$(OBJECT_SUFFIX)) $(VULKAN_VALIDATION_C_SOURCES:.c=$(OBJECT_SUFFIX))

VULKAN_VALIDATION_LIB = lib/$(LIB_PREFIX)vulkan-validation$(DYNAMIC_LIB_SUFFIX)
VULKAN_VALIDATION_REDIRECT = lib/$(LIB_PREFIX)VkLayer_khronos_validation$(DYNAMIC_LIB_SUFFIX)

build_message_vulkan_validation:
	@$(ECHO) "$(SECTION_PREFIX) Vulkan Validation"
	@$(ECHO) "$(INFO_PREFIX) Khronos helps us not be stupid"

vulkan_validation: build_message_vulkan_validation $(VULKAN_VALIDATION_REDIRECT) $(VULKAN_VALIDATION_LIB) ### @Vendor Builds Vulkan Validation Layers as a Genstone module

$(VULKAN_VALIDATION_REDIRECT): $(VULKAN_VALIDATION_LIB)
	ln -sf $< $@

$(VULKAN_VALIDATION_LIB): CXXFLAGS = $(_VULKAN_VALIDATION_CXXFLAGS)
$(VULKAN_VALIDATION_LIB): CFLAGS = $(_VULKAN_VALIDATION_CXXFLAGS)
$(VULKAN_VALIDATION_LIB): LFLAGS = -Llib $(_VULKAN_VALIDATION_LFLAGS)
$(VULKAN_VALIDATION_LIB): CLANG_FORMAT = DISABLED
$(VULKAN_VALIDATION_LIB): $(VULKAN_VALIDATION_OBJECTS) $(SPIRV_LIB) $(VULKAN_LIB) | lib
