GEN_PROC_TEST_SOURCES = test/unit/genproc/main.c
GEN_PROC_TEST_OBJECTS = $(GEN_PROC_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_PROC_TEST = test/unit/genproc_test$(EXECUTABLE_SUFFIX)

build_message_genproc_test:
	@echo "$(SECTION_PREFIX) Genproc Test"
	@echo "$(INFO_PREFIX) Testing Genstone subprocess management"

genproc_test: gencore build_message_genproc_test $(GEN_PROC_TEST) ### @Test Builds and runs Genstone subprocess management utility tests
	@echo "$(ACTION_PREFIX)$(GEN_PROC_TEST)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),WIN)
	@cd lib && $(subst /,$(SEP),../$(GEN_PROC_TEST))
else
	@LD_LIBRARY_PATH=lib $(GEN_PROC_TEST)
endif

$(GEN_PROC_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_PROC_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_PROC_TEST): $(GEN_PROC_TEST_OBJECTS)

$(GEN_PROC_TEST_LIB): CFLAGS =
$(GEN_PROC_TEST_LIB): LFLAGS =
$(GEN_PROC_TEST_LIB): $(GEN_PROC_TEST_LIB_OBJECTS)

clean_genproc_test:
	-$(RM) $(subst /,$(SEP),$(GEN_PROC_TEST_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(GEN_PROC_TEST))

