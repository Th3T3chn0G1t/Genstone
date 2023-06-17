PLATFORM_SOURCEDIRS += linux

include $(PLATFORM_DIR)/unix.mk

BEGIN_FULL_STATIC = -Wl,--whole-archive $(eval)
END_FULL_STATIC = $(eval) -Wl,--no-whole-archive
