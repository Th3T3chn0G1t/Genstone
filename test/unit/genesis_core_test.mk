GEN_CORE_TEST_SOURCES = $(wildcard test/unit/genesis_core/*.c)
GEN_CORE_TEST_OBJECTS = $(GEN_CORE_TEST_SOURCES:.c=.o)

GEN_CORE_TEST = test/unit/gen_core_test$(EXECUTABLE_SUFFIX)

genesis_core_test: $(GEN_CORE_TEST)
ifeq ($(PLATFORM),WIN)
	cd lib; ./$(notdir $(GEN_CORE_TEST))
else
	LD_LIBRARY_PATH=lib $(GEN_CORE_TEST)
endif

$(GEN_CORE_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_CORE_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_CORE_TEST): $(GEN_CORE_TEST_OBJECTS) gencore

clean_genesis_core_test:
	-rm $(GEN_CORE_TEST_OBJECTS)
	-rm $(GEN_CORE_TEST)
