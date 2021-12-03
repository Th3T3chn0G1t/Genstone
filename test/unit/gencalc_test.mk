GEN_CALC_TEST_SOURCES = $(wildcard test/unit/gencalc/*.c)
GEN_CALC_TEST_OBJECTS = $(GEN_CALC_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_CALC_TEST = test/unit/gencalc_test$(EXECUTABLE_SUFFIX)

build_message_gencalc_test:
	@echo "$(SECTION_PREFIX) Gencalc Test"
	@echo "$(INFO_PREFIX) Testing Genstone mathematics utilities"

gencalc_test: build_message_gencalc_test $(GEN_CALC_TEST) ### @Test Builds and runs Genstone mathematics utility tests
	@echo "$(ACTION_PREFIX)$(GEN_CALC_TEST)$(ACTION_SUFFIX)"
	@LD_LIBRARY_PATH=lib $(GEN_CALC_TEST)

$(GEN_CALC_TEST): CFLAGS = $(GEN_CALC_CFLAGS)
$(GEN_CALC_TEST): LFLAGS = -Llib $(GEN_CALC_LFLAGS)
$(GEN_CALC_TEST): $(GEN_CALC_TEST_OBJECTS)

$(GEN_CALC_TEST_OBJECTS): $(GEN_CALC_LIB) $(GEN_CORE_LIB) $(MIMALLOC_LIB) $(SAFEC_LIB)

clean_gencalc_test:
	-rm $(GEN_CALC_TEST_OBJECTS)
	-rm $(GEN_CALC_TEST)
