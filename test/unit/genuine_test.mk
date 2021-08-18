GEN_UI_TEST_SOURCES = $(wildcard test/unit/genuine/*.c)
GEN_UI_TEST_OBJECTS = $(GEN_UI_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_UI_TEST = test/unit/genuine_test$(EXECUTABLE_SUFFIX)

build_message_genuine_test:
	@echo "$(SECTION_PREFIX) Genuine Test"
	@echo "$(INFO_PREFIX) Testing Genesis GUI module"

genuine_test: build_message_genuine_test $(GEN_UI_TEST)
	@echo "$(ACTION_PREFIX)$(GEN_UI_TEST)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),WIN)
	@cd $(subst /,$(SEP),lib && ../$(GEN_UI_TEST))
else
	@LD_LIBRARY_PATH=lib $(GEN_UI_TEST)
endif

$(GEN_UI_TEST): CFLAGS = $(GEN_UI_CFLAGS)
$(GEN_UI_TEST): LFLAGS = -Llib $(GEN_UI_LFLAGS)
$(GEN_UI_TEST): $(GEN_UI_TEST_OBJECTS) $(GEN_UI_LIB)

clean_genuine_test:
	-$(RM) $(subst /,$(SEP),$(GEN_UI_TEST_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(GEN_UI_TEST))
