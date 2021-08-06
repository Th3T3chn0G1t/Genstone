# Test that the build system can create both static and dynamic libraries and link them correctly against an executable

BUILD_TEST_EXEC_SOURCES = test/build/main.c
BUILD_TEST_EXEC_OBJECTS = $(BUILD_TEST_EXEC_SOURCES:.c=.o)

BUILD_TEST_EXEC_DYNAMIC = test/build/testexec_dynamic$(EXECUTABLE_SUFFIX)
BUILD_TEST_EXEC_STATIC = test/build/testexec_static$(EXECUTABLE_SUFFIX)

BUILD_TEST_LIB_SOURCES = $(wildcard test/build/testlib_*.c)
BUILD_TEST_LIB_OBJECTS = $(BUILD_TEST_LIB_SOURCES:.c=.o)

BUILD_TEST_LIB_DYNAMIC = test/build/$(LIB_PREFIX)test_dynamic$(DYNAMIC_LIB_SUFFIX)
BUILD_TEST_LIB_STATIC = test/build/$(LIB_PREFIX)test_static$(STATIC_LIB_SUFFIX)

build_test: $(BUILD_TEST_EXEC_DYNAMIC) $(BUILD_TEST_EXEC_STATIC) lib
	$(CP) $(subst /,$(SEP),$(BUILD_TEST_LIB_DYNAMIC) lib/$(notdir $(BUILD_TEST_LIB_DYNAMIC)))
ifeq ($(PLATFORM),WIN)
	cd $(subst /,$(SEP),lib && ../$(BUILD_TEST_EXEC_DYNAMIC))
	cd $(subst /,$(SEP),lib && ../$(BUILD_TEST_EXEC_STATIC))
else
	LD_LIBRARY_PATH=lib $(BUILD_TEST_EXEC_DYNAMIC)
	LD_LIBRARY_PATH=lib $(BUILD_TEST_EXEC_STATIC)
endif
	$(RM) $(subst /,$(SEP),lib/$(notdir $(BUILD_TEST_LIB_DYNAMIC)))

$(BUILD_TEST_EXEC_DYNAMIC): LFLAGS = -Ltest/build -ltest_dynamic
$(BUILD_TEST_EXEC_DYNAMIC): $(BUILD_TEST_EXEC_OBJECTS) $(BUILD_TEST_LIB_DYNAMIC)
$(BUILD_TEST_EXEC_STATIC): LFLAGS = -Ltest/build -ltest_static
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
