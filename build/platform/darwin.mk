PLATFORM_SOURCEDIRS += darwin

include $(PLATFORM_DIR)/unix.mk

GLOBAL_CFLAGS += -DGEN_PLATFORM_SYMBOL_PREFIX=\"_\"

DYNAMIC_LIB_SUFFIX = .dylib

BEGIN_FULL_STATIC = -Wl,-force_load,
END_FULL_STATIC =

DYNAMIC_LIB_TOOL += -install_name "@rpath/$(notdir $@)"
