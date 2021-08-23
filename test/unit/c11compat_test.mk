C11_COMPAT_TEST_SOURCES = $(wildcard test/unit/c11compat/*.c)
C11_COMPAT_TEST_OBJECTS = $(C11_COMPAT_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

C11_COMPAT_TEST = test/unit/c11compat_test$(EXECUTABLE_SUFFIX)

build_message_c11compat_test:
	@echo "$(SECTION_PREFIX) C11 Compat Test"
	@echo "$(INFO_PREFIX) Testing C11 optional feature compatibility layers"

c11compat_test: gencore build_message_c11compat_test $(C11_COMPAT_TEST)
	@echo "$(ACTION_PREFIX)$(C11_COMPAT_TEST)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),WIN)
	@cd $(subst /,$(SEP),lib && ../$(C11_COMPAT_TEST))
else
	@LD_LIBRARY_PATH=lib $(C11_COMPAT_TEST)
endif

$(C11_COMPAT_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(C11_COMPAT_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(C11_COMPAT_TEST): $(C11_COMPAT_TEST_OBJECTS)

clean_c11compat_test:
	-$(RM) $(subst /,$(SEP),$(C11_COMPAT_TEST_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(C11_COMPAT_TEST))
