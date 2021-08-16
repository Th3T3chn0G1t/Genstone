GEN_CORE_TEST_SOURCES = $(wildcard test/unit/gencore/*.c)
GEN_CORE_TEST_OBJECTS = $(GEN_CORE_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_CORE_TEST = test/unit/gencore_test$(EXECUTABLE_SUFFIX)

build_message_gencore_test:
	@echo "$(SECTION_PREFIX) Gencore Test"
	@echo "$(INFO_PREFIX) Testing core Genesis utilities"

gencore_test: build_message_gencore_test $(GEN_CORE_TEST)
	@echo "$(ACTION_PREFIX)$(GEN_CORE_TEST)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),WIN)
	@cd $(subst /,$(SEP),lib && ../$(GEN_CORE_TEST))
else
	@LD_LIBRARY_PATH=lib $(GEN_CORE_TEST)
endif

$(GEN_CORE_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_CORE_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_CORE_TEST): $(GEN_CORE_TEST_OBJECTS) $(GEN_CORE_LIB)

clean_gencore_test:
	-$(RM) $(subst /,$(SEP),$(GEN_CORE_TEST_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(GEN_CORE_TEST))