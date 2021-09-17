GEN_DL_TEST_EXEC_SOURCES = test/unit/gendl/main.c
GEN_DL_TEST_EXEC_OBJECTS = $(GEN_DL_TEST_EXEC_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_DL_TEST_LIB_SOURCES = test/unit/gendl/testlib.c
GEN_DL_TEST_LIB_OBJECTS = $(GEN_DL_TEST_LIB_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_DL_TEST_LIB = lib/$(LIB_PREFIX)gendl_test$(DYNAMIC_LIB_SUFFIX)
GEN_DL_TEST_EXEC = test/unit/gendl_test$(EXECUTABLE_SUFFIX)

build_message_gendl_test:
	@echo "$(SECTION_PREFIX) Gendl Test"
	@echo "$(INFO_PREFIX) Testing Genesis dynamic library management"

gendl_test: gencore build_message_gendl_test $(GEN_DL_TEST_EXEC) $(GEN_DL_TEST_LIB) ### @Test Builds and runs Genesis dynamic library utility tests
	@echo "$(ACTION_PREFIX)$(GEN_DL_TEST_EXEC)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),WIN)
	@cd $(subst /,$(SEP),lib && ../$(GEN_DL_TEST_EXEC))
else
	@LD_LIBRARY_PATH=lib $(GEN_DL_TEST_EXEC)
endif

$(GEN_DL_TEST_EXEC): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_DL_TEST_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_DL_TEST_EXEC): $(GEN_DL_TEST_EXEC_OBJECTS)

$(GEN_DL_TEST_LIB): CFLAGS =
$(GEN_DL_TEST_LIB): LFLAGS =
$(GEN_DL_TEST_LIB): $(GEN_DL_TEST_LIB_OBJECTS)

clean_gendl_test:
	-$(RM) $(subst /,$(SEP),$(GEN_DL_TEST_EXEC_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(GEN_DL_TEST_EXEC))

	-$(RM) $(subst /,$(SEP),$(GEN_DL_TEST_LIB_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(GEN_DL_TEST_LIB))
