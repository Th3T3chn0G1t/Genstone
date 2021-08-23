GEN_TOOLING_TEST_SOURCES = $(wildcard test/unit/gentooling/*.c)
GEN_TOOLING_TEST_OBJECTS = $(GEN_TOOLING_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_TOOLING_TEST = test/unit/gentooling_test$(EXECUTABLE_SUFFIX)

build_message_gentooling_test:
	@echo "$(SECTION_PREFIX) Gentooling Test"
	@echo "$(INFO_PREFIX) Testing Genesis tooling utilities"

gentooling_test: build_message_gentooling_test $(GEN_TOOLING_TEST)
	@echo "$(ACTION_PREFIX)$(GEN_TOOLING_TEST)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),WIN)
	@cd $(subst /,$(SEP),lib && ../$(GEN_TOOLING_TEST))
else
	@LD_LIBRARY_PATH=lib $(GEN_TOOLING_TEST)
endif

$(GEN_TOOLING_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_TOOLING_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_TOOLING_TEST): $(GEN_TOOLING_TEST_OBJECTS) $(GEN_CORE_LIB)

clean_gentooling_test:
	-$(RM) $(subst /,$(SEP),$(GEN_TOOLING_TEST_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(GEN_TOOLING_TEST))
