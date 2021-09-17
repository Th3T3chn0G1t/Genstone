# Test that the build system can create both static and dynamic libraries and link them correctly against an executable

BUILD_TEST_EXEC_SOURCES = test/build/main.c
BUILD_TEST_EXEC_OBJECTS = $(BUILD_TEST_EXEC_SOURCES:.c=$(OBJECT_SUFFIX))

BUILD_TEST_EXEC_DYNAMIC = test/build/testexec_dynamic$(EXECUTABLE_SUFFIX)
BUILD_TEST_EXEC_STATIC = test/build/testexec_static$(EXECUTABLE_SUFFIX)

BUILD_TEST_LIB_SOURCES = $(wildcard test/build/testlib_*.c)
BUILD_TEST_LIB_OBJECTS = $(BUILD_TEST_LIB_SOURCES:.c=$(OBJECT_SUFFIX))

BUILD_TEST_LIB_DYNAMIC = lib/$(LIB_PREFIX)test_dynamic$(DYNAMIC_LIB_SUFFIX)
BUILD_TEST_LIB_STATIC = lib/$(LIB_PREFIX)test_static$(STATIC_LIB_SUFFIX)

build_message_build_test:
	@echo "$(SECTION_PREFIX) Build Test"
	@echo "$(INFO_PREFIX) Testing build configuration"

build_test: build_message_build_test $(BUILD_TEST_EXEC_DYNAMIC) $(BUILD_TEST_EXEC_STATIC) lib ### @Test Builds and runs build-system tests
	@echo "$(ACTION_PREFIX)$(BUILD_TEST_EXEC_STATIC)$(ACTION_SUFFIX)"
	@echo "$(ACTION_PREFIX)$(BUILD_TEST_EXEC_DYNAMIC)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),WIN)
	@cd $(subst /,$(SEP),lib && ../$(BUILD_TEST_EXEC_STATIC))
	@cd $(subst /,$(SEP),lib && ../$(BUILD_TEST_EXEC_DYNAMIC))
else
	@LD_LIBRARY_PATH=lib $(BUILD_TEST_EXEC_DYNAMIC)
	@LD_LIBRARY_PATH=lib $(BUILD_TEST_EXEC_STATIC)
endif

$(BUILD_TEST_EXEC_DYNAMIC): LFLAGS = -Llib -ltest_dynamic
$(BUILD_TEST_EXEC_DYNAMIC): $(BUILD_TEST_EXEC_OBJECTS) $(BUILD_TEST_LIB_DYNAMIC)
$(BUILD_TEST_EXEC_STATIC): LFLAGS = -Llib -ltest_static
$(BUILD_TEST_EXEC_STATIC): $(BUILD_TEST_EXEC_OBJECTS) $(BUILD_TEST_LIB_STATIC)

$(BUILD_TEST_LIB_DYNAMIC): LFLAGS =
$(BUILD_TEST_LIB_DYNAMIC): $(BUILD_TEST_LIB_OBJECTS)
$(BUILD_TEST_LIB_STATIC): LFLAGS =
$(BUILD_TEST_LIB_STATIC): $(BUILD_TEST_LIB_OBJECTS)

$(BUILD_TEST_LIB_OBJECTS): Makefile build/common.mk build/config.mk

clean_build_test:
	-$(RM) $(subst /,$(SEP),$(BUILD_TEST_LIB_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(BUILD_TEST_LIB_DYNAMIC))
	-$(RM) $(subst /,$(SEP),$(BUILD_TEST_LIB_STATIC))

	-$(RM) $(subst /,$(SEP),$(BUILD_TEST_EXEC_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(BUILD_TEST_EXEC_DYNAMIC))
	-$(RM) $(subst /,$(SEP),$(BUILD_TEST_EXEC_STATIC))
