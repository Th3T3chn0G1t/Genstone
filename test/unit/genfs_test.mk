GEN_FS_TEST_SOURCES = $(wildcard test/unit/genfs/*.c)
GEN_FS_TEST_OBJECTS = $(GEN_FS_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_FS_TEST = test/unit/genfs_test$(EXECUTABLE_SUFFIX)

build_message_genfs_test:
	@echo "$(SECTION_PREFIX) Genfs Test"
	@echo "$(INFO_PREFIX) Testing Genstone filesystem utilities"

genfs_test: gencore build_message_genfs_test $(GEN_FS_TEST) ### @Test Builds and runs Genstone filesystem utility tests
	@echo "$(ACTION_PREFIX)$(GEN_FS_TEST)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),WIN)
	@cd lib && $(subst /,$(SEP),../$(GEN_FS_TEST))
else
	@LD_LIBRARY_PATH=lib $(GEN_FS_TEST)
endif

$(GEN_FS_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_FS_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_FS_TEST): $(GEN_FS_TEST_OBJECTS)

clean_genfs_test:
	-$(RM) $(subst /,$(SEP),$(GEN_FS_TEST_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(GEN_FS_TEST))
