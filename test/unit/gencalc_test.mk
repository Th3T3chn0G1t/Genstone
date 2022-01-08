GEN_CALC_TEST_SOURCES = $(wildcard test/unit/gencalc/*.c)
GEN_CALC_TEST_OBJECTS = $(GEN_CALC_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_CALC_TEST = test/unit/gencalc_test$(EXECUTABLE_SUFFIX)

build_message_gencalc_test:
	@$(ECHO) "$(SECTION_PREFIX) Gencalc Test"
	@$(ECHO) "$(INFO_PREFIX) Testing Genstone mathematics utilities"

gencalc_test: build_message_gencalc_test $(GEN_CALC_TEST) ### @Test Builds and runs Genstone mathematics utility tests
	@$(ECHO) "$(ACTION_PREFIX)$(GEN_CALC_TEST)$(ACTION_SUFFIX)"
	@LD_LIBRARY_PATH=lib $(GEN_CALC_TEST)

$(GEN_CALC_TEST): CFLAGS = $(GEN_CALC_CFLAGS)
$(GEN_CALC_TEST): LFLAGS = -Llib $(GEN_CALC_LFLAGS)
$(GEN_CALC_TEST): $(GEN_CALC_TEST_OBJECTS)

$(GEN_CALC_TEST_OBJECTS): $(GEN_CALC_LIB) $(GEN_CORE_LIB)

clean_gencalc_test:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(GEN_CALC_TEST_OBJECTS)
	-rm $(GEN_CALC_TEST)
	@$(ECHO) "$(ACTION_SUFFIX)"
