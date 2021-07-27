GEN_ARGS_TEST_SOURCES = $(wildcard test/unit/genargs/*.c)
GEN_ARGS_TEST_OBJECTS = $(GEN_ARGS_TEST_SOURCES:.c=.o)

GEN_ARGS_TEST = test/unit/genargs_test$(EXECUTABLE_SUFFIX)

genargs_test: $(GEN_ARGS_TEST)
ifeq ($(PLATFORM),WIN)
	cd lib; ../$(GEN_ARGS_TEST)
else
	LD_LIBRARY_PATH=lib $(GEN_ARGS_TEST)
endif

$(GEN_ARGS_TEST): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_ARGS_TEST): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_ARGS_TEST): $(GEN_ARGS_TEST_OBJECTS) $(GEN_CORE_LIB)

clean_genargs_test:
	-rm $(GEN_ARGS_TEST_OBJECTS)
	-rm $(GEN_ARGS_TEST)
