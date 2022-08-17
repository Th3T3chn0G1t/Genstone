_GEN_FX_COMMON_CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_CALC_CFLAGS) $(VULKAN_CFLAGS) $(SPIRV_CFLAGS) $(VULKAN_MEMORY_ALLOCATOR_CFLAGS) $(shell $(PKGCONFIG) --cflags xcb) $(shell $(PKGCONFIG) --cflags xcb-xkb) $(shell $(PKGCONFIG) --cflags xcb-icccm)
_GEN_FX_COMMON_LFLAGS = $(GEN_CORE_LFLAGS) $(GEN_CALC_LFLAGS) $(VULKAN_LFLAGS) $(SPIRV_LFLAGS) $(VULKAN_MEMORY_ALLOCATOR_LFLAGS) $(shell $(PKGCONFIG) --libs xcb) $(shell $(PKGCONFIG) --libs xcb-xkb) $(shell $(PKGCONFIG) --libs xcb-icccm)

_GEN_FX_COMMON_CFLAGS += -DXCB=0
ifeq ($(PLATFORM),LNX)
_GEN_FX_COMMON_CFLAGS += -DWINSYS=XCB
else
_GEN_FX_COMMON_CFLAGS += -DWINSYS
endif
ifeq ($(BUILD_MODE),DEBUG)
_GEN_FX_COMMON_LFLAGS += $(VULKAN_VALIDATION_LFLAGS)
endif

_GEN_FX_CFLAGS = $(_GEN_FX_COMMON_CFLAGS)
_GEN_FX_LFLAGS = $(_GEN_FX_COMMON_LFLAGS)

GEN_FX_CFLAGS = -Igenstone/genfx/include $(_GEN_FX_COMMON_CFLAGS)
GEN_FX_LFLAGS = -lgenfx $(_GEN_FX_COMMON_LFLAGS)

GEN_FX_SOURCES = $(wildcard genstone/genfx/*.c)
GEN_FX_OBJECTS = $(GEN_FX_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_FX_LIB = lib/$(LIB_PREFIX)genfx$(DYNAMIC_LIB_SUFFIX)

build_message_genfx:
	@$(ECHO) "$(SECTION_PREFIX) Genfx"
	@$(ECHO) "$(INFO_PREFIX) Genstone audiovisual effects module"

genfx: gencore build_message_genfx $(GEN_FX_LIB) ### @Genstone Builds Genstone audiovisual effects module

$(GEN_FX_LIB): CFLAGS = $(_GEN_FX_CFLAGS)
$(GEN_FX_LIB): LFLAGS = -Llib $(_GEN_FX_LFLAGS)
$(GEN_FX_LIB): $(GEN_FX_OBJECTS) $(GEN_CORE_LIB) $(VULKAN_LIB) $(SPIRV_LIB) $(VULKAN_MEMORY_ALLOCATOR_LIB) | lib
ifeq ($(BUILD_MODE),DEBUG)
$(GEN_FX_LIB): $(VULKAN_VALIDATION_REDIRECT)
endif

$(GEN_FX_OBJECTS): $(wildcard genstone/genfx/include/*.h)
$(GEN_FX_OBJECTS): $(GEN_CORE_LIB) $(VULKAN_LIB) $(SPIRV_LIB)

clean_genfx:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(GEN_FX_OBJECTS)
	-rm $(GEN_FX_LIB)
	@$(ECHO) "$(ACTION_SUFFIX)"
