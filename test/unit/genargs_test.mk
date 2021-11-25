GEN_ARGS_TEST_SOURCES = $(wildcard test/unit/genargs/*.c)
GEN_ARGS_TEST_OBJECTS = $(GEN_ARGS_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_ARGS_TEST = test/unit/genargs_test$(EXECUTABLE_SUFFIX)

build_message_genargs_test:
	@echo "$(SECTION_PREFIX) Genargs Test"
	@echo "$(INFO_PREFIX) Testing Genstone argument parser"

genargs_test: gencore build_message_genargs_test $(GEN_ARGS_TEST) ### @Test Builds and runs Genstone argument parser tests
	@echo "$(ACTION_PREFIX)$(GEN_ARGS_TEST)$(ACTION_SUFFIX)"
	@LD_LIBRARY_PATH=lib $(GEN_ARGS_TEST)

$(GEN_ARGS_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_ARGS_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_ARGS_TEST): $(GEN_ARGS_TEST_OBJECTS)

clean_genargs_test:
	-$(RM) $(subst /,$(SEP),$(GEN_ARGS_TEST_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(GEN_ARGS_TEST))
