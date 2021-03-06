GEN_FS_TEST_SOURCES = $(wildcard test/unit/genfs/*.c)
GEN_FS_TEST_OBJECTS = $(GEN_FS_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_FS_TEST = test/unit/genfs_test$(EXECUTABLE_SUFFIX)

build_message_genfs_test:
	@$(ECHO) "$(SECTION_PREFIX) Genfs Test"
	@$(ECHO) "$(INFO_PREFIX) Testing Genstone filesystem utilities"

genfs_test: build_message_genfs_test $(GEN_FS_TEST) ### @Test Builds and runs Genstone filesystem utility tests
	@$(ECHO) "$(ACTION_PREFIX)$(GEN_FS_TEST)$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),DWN)
	@DYLD_LIBRARY_PATH=lib $(GEN_FS_TEST)
else
	@LD_LIBRARY_PATH=lib $(GEN_FS_TEST)
endif

$(GEN_FS_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_FS_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_FS_TEST): $(GEN_FS_TEST_OBJECTS)

$(GEN_FS_TEST_OBJECTS): $(GEN_CORE_LIB)

clean_genfs_test:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(GEN_FS_TEST_OBJECTS)
	-rm $(GEN_FS_TEST)
	@$(ECHO) "$(ACTION_SUFFIX)"
