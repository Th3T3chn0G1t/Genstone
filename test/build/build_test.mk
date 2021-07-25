# Test that the build system can create both static and dynamic libraries and link them correctly against an executable

BUILD_TEST_EXEC_SOURCES = test/build/main.c
BUILD_TEST_EXEC_OBJECTS = $(BUILD_TEST_EXEC_SOURCES:.c=.o)

BUILD_TEST_EXEC_DYNAMIC = test/build/testexec_dynamic$(EXECUTABLE_SUFFIX)
BUILD_TEST_EXEC_STATIC = test/build/testexec_static$(EXECUTABLE_SUFFIX)

BUILD_TEST_LIB_SOURCES = $(wildcard test/build/testlib_*.c)
BUILD_TEST_LIB_OBJECTS = $(BUILD_TEST_LIB_SOURCES:.c=.o)

BUILD_TEST_LIB_DYNAMIC = test/build/$(LIB_PREFIX)test_dynamic$(DYNAMIC_LIB_SUFFIX)
BUILD_TEST_LIB_STATIC = test/build/$(LIB_PREFIX)test_static$(STATIC_LIB_SUFFIX)

build_test: $(BUILD_TEST_EXEC_DYNAMIC) $(BUILD_TEST_EXEC_STATIC)
ifeq ($(PLATFORM),WIN)
	type test/build/test_dynamic.def
	cd test/build; ./testexec_dynamic$(EXECUTABLE_SUFFIX)
	cd test/build; ./testexec_static$(EXECUTABLE_SUFFIX)
else
	LD_LIBRARY_PATH=test/build test/build/testexec_dynamic$(EXECUTABLE_SUFFIX)
	LD_LIBRARY_PATH=test/build test/build/testexec_static$(EXECUTABLE_SUFFIX)
endif

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
	-rm $(BUILD_TEST_LIB_OBJECTS)
	-rm $(BUILD_TEST_LIB_DYNAMIC)
	-rm $(BUILD_TEST_LIB_STATIC)

	-rm $(BUILD_TEST_EXEC_OBJECTS)
	-rm $(BUILD_TEST_EXEC_DYNAMIC)
	-rm $(BUILD_TEST_EXEC_STATIC)
