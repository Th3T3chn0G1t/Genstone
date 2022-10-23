GEN_TESTS_CFLAGS = $(GEN_CORE_CFLAGS) -I$(GENSTONE_DIR)/genstone/gentests/include
GEN_TESTS_LFLAGS = $(GEN_CORE_LFLAGS) -lgentests
GEN_TESTS_LIBDIRS = $(GEN_CORE_LIBDIRS) $(GENSTONE_DIR)/lib

GEN_TESTS_SOURCES = $(wildcard $(GENSTONE_DIR)/genstone/gentests/*.c)
GEN_TESTS_OBJECTS = $(GEN_TESTS_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_TESTS_LIB = $(GENSTONE_DIR)/lib/$(LIB_PREFIX)gentests$(STATIC_LIB_SUFFIX)

$(GEN_TESTS_LIB): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_TESTS_LIB): LFLAGS = $(GEN_CORE_LFLAGS)
$(GEN_TESTS_LIB): LIBDIRS = $(GEN_CORE_LIBDIRS)
$(GEN_TESTS_LIB): $(GEN_TESTS_OBJECTS) | $(GENSTONE_DIR)/lib

.PHONY: gentests
gentests: $(GEN_TESTS_LIB)

.PHONY: gentests
test_gentests:

.PHONY: clean_gentests
clean_gentests:
	@$(ECHO) "$(ACTION_PREFIX)"
	-$(RM) $(GEN_TESTS_OBJECTS)
	-$(RM) $(GEN_TESTS_LIB)
	@$(ECHO) "$(ACTION_SUFFIX)"
