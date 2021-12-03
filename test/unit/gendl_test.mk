GEN_DL_TEST_EXEC_SOURCES = test/unit/gendl/main.c
GEN_DL_TEST_EXEC_OBJECTS = $(GEN_DL_TEST_EXEC_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_DL_TEST_LIB_SOURCES = test/unit/gendl/testlib.c
GEN_DL_TEST_LIB_OBJECTS = $(GEN_DL_TEST_LIB_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_DL_TEST_LIB = lib/$(LIB_PREFIX)gendl_test$(DYNAMIC_LIB_SUFFIX)
GEN_DL_TEST_EXEC = test/unit/gendl_test$(EXECUTABLE_SUFFIX)

build_message_gendl_test:
	@echo "$(SECTION_PREFIX) Gendl Test"
	@echo "$(INFO_PREFIX) Testing Genstone dynamic library management"

gendl_test: build_message_gendl_test $(GEN_DL_TEST_EXEC) $(GEN_DL_TEST_LIB) ### @Test Builds and runs Genstone dynamic library utility tests
	@echo "$(ACTION_PREFIX)$(GEN_DL_TEST_EXEC)$(ACTION_SUFFIX)"
	@LD_LIBRARY_PATH=lib $(GEN_DL_TEST_EXEC)
	-rm $(GEN_DL_TEST_LIB)

$(GEN_DL_TEST_EXEC): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_DL_TEST_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_DL_TEST_EXEC): $(GEN_DL_TEST_EXEC_OBJECTS)

$(GEN_DL_TEST_LIB): CFLAGS =
$(GEN_DL_TEST_LIB): LFLAGS =
$(GEN_DL_TEST_LIB): $(GEN_DL_TEST_LIB_OBJECTS)

$(GEN_DL_TEST_EXEC_OBJECTS): $(GEN_CORE_LIB) $(MIMALLOC_LIB) $(SAFEC_LIB)

clean_gendl_test:
	-rm $(GEN_DL_TEST_EXEC_OBJECTS)
	-rm $(GEN_DL_TEST_EXEC)

	-rm $(GEN_DL_TEST_LIB_OBJECTS)
	-rm $(GEN_DL_TEST_LIB)
