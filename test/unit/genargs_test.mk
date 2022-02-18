GEN_ARGS_TEST_SOURCES = $(wildcard test/unit/genargs/*.c)
GEN_ARGS_TEST_OBJECTS = $(GEN_ARGS_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_ARGS_TEST = test/unit/genargs_test$(EXECUTABLE_SUFFIX)

build_message_genargs_test:
	@$(ECHO) "$(SECTION_PREFIX) Genargs Test"
	@$(ECHO) "$(INFO_PREFIX) Testing Genstone argument parser"

genargs_test: build_message_genargs_test $(GEN_ARGS_TEST) ### @Test Builds and runs Genstone argument parser tests
	@$(ECHO) "$(ACTION_PREFIX)$(GEN_ARGS_TEST)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),DWN)
	@DYLD_LIBRARY_PATH=lib $(GEN_ARGS_TEST)
else
	@LD_LIBRARY_PATH=lib $(GEN_ARGS_TEST)
endif

$(GEN_ARGS_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_ARGS_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_ARGS_TEST): $(GEN_ARGS_TEST_OBJECTS)

$(GEN_ARGS_TEST_OBJECTS): $(GEN_CORE_LIB)

clean_genargs_test:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(GEN_ARGS_TEST_OBJECTS)
	-rm $(GEN_ARGS_TEST)
	@$(ECHO) "$(ACTION_SUFFIX)"
