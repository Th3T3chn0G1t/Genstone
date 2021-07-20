BUILD_TEST_SOURCES = $(wildcard test/build/*.c)
BUILD_TEST_OBJECTS = $(BUILD_TEST_SOURCES:.c=.o)

BUILD_TEST_LIB = test/build/$(LIB_PREFIX)test$(DYNAMIC_LIB_SUFFIX)

build_test: $(BUILD_TEST_LIB)

$(BUILD_TEST_LIB): $(BUILD_TEST_OBJECTS)

clean_build_test:
	-rm $(BUILD_TEST_OBJECTS)
	-rm $(BUILD_TEST_LIB)
