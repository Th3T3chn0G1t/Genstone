_VULKAN_COMMON_DEFINES = -DAPI_NAME="Vulkan" -DVK_ENABLE_BETA_EXTENSIONS -DVULKAN_NON_CMAKE_BUILD -DSYSCONFDIR=\"/etc\" -DFALLBACK_CONFIG_DIRS=\"/etc/xdg\" -DFALLBACK_DATA_DIRS=\"/usr/local/share:/usr/share\"
_VULKAN_COMMON_CFLAGS = $(_VULKAN_COMMON_DEFINES) -Igenstone/vendor/Vulkan-Headers/include -Igenstone/vendor/Vulkan-Loader/loader -Igenstone/vendor/Vulkan-Loader/loader/generated

_VULKAN_CFLAGS = $(_VULKAN_COMMON_CFLAGS) -Wno-implicit-fallthrough -Wno-unused-parameter -Wno-missing-field-initializers -Wno-typedef-redefinition -Wno-availability -Wno-nullability-completeness -Wno-expansion-to-defined -DTARGET_OS_IPHONE=0 -fno-strict-aliasing -fno-builtin-memcmp
_VULKAN_LFLAGS =
ifeq ($(PLATFORM),DWN)
_VULKAN_CFLAGS += -DVK_USE_PLATFORM_MACOS_MVK -DVK_USE_PLATFORM_METAL_EXT
_VULKAN_LFLAGS += -framework CoreFoundation
endif
ifeq ($(PLATFORM),LNX)
_VULKAN_CFLAGS += -DVK_USE_PLATFORM_XCB_KHR -pthread $(shell $(PKGCONFIG) --cflags xcb)
_VULKAN_LFLAGS += -ldl -pthread -lm $(shell $(PKGCONFIG) --libs xcb)
endif
ifeq ($(BUILD_MODE),DEBUG)
_VULKAN_CFLAGS += -DDEBUG
endif

VULKAN_CFLAGS = $(_VULKAN_COMMON_CFLAGS)
VULKAN_LFLAGS = -lvulkan

VULKAN_DISABLED_SOURCES = dirent_on_windows.c loader_windows.c asm_offset.c generated/vk_loader_extensions.c
VULKAN_SOURCES = $(filter-out $(addprefix genstone/vendor/Vulkan-Loader/loader/,$(VULKAN_DISABLED_SOURCES)),$(wildcard genstone/vendor/Vulkan-Loader/loader/*.c) $(wildcard genstone/vendor/Vulkan-Loader/loader/generated/*.c))
VULKAN_OBJECTS = $(VULKAN_SOURCES:.c=$(OBJECT_SUFFIX))

VULKAN_LIB = lib/$(LIB_PREFIX)vulkan$(DYNAMIC_LIB_SUFFIX)

build_message_vulkan:
	@$(ECHO) "$(SECTION_PREFIX) Vulkan"
	@$(ECHO) "$(INFO_PREFIX) Khronos helps us make graphics"

vulkan: build_message_vulkan $(VULKAN_LIB) ### @Vendor Builds Vulkan as a Genstone module

$(VULKAN_LIB): CFLAGS = $(_VULKAN_CFLAGS)
$(VULKAN_LIB): LFLAGS = -Llib $(_VULKAN_LFLAGS)
$(VULKAN_LIB): CLANG_FORMAT = DISABLED
$(VULKAN_LIB): CFISAN_FLAGS = -fno-lto
$(VULKAN_LIB): $(VULKAN_OBJECTS) | lib

clean_vulkan:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(VULKAN_OBJECTS)
	-rm $(VULKAN_LIB)
	@$(ECHO) "$(ACTION_SUFFIX)"
