PLATFORM_SOURCEDIRS += darwin

include $(PLATFORM_DIR)/unix.mk

DYNAMIC_LIB_SUFFIX = .dylib

BEGIN_FULL_STATIC = -Wl,-force_load,
END_FULL_STATIC =

DYNAMIC_LIB_TOOL += -install_name "@rpath/$(notdir $@)"
