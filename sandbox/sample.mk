SAMPLE_SOURCES = $(wildcard sandbox/sample/*.c)
SAMPLE_OBJECTS = $(SAMPLE_SOURCES:.c=.o)

SAMPLE_EXEC = sample$(EXECUTABLE_SUFFIX)

build_message_sample:
	@echo "$(SECTION_PREFIX) Sample Project"
	@echo "$(INFO_PREFIX) Genstone sandbox sample project!"

sample: build_message_sample $(SAMPLE_EXEC) ### @User Builds a sample sandbox project

$(SAMPLE_EXEC): CFLAGS = $(GEN_CORE_CFLAGS)
$(SAMPLE_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(SAMPLE_EXEC): $(SAMPLE_OBJECTS)

$(SAMPLE_OBJECTS): $(GEN_CORE_LIB)

clean_sample:
	-rm $(SAMPLE_OBJECTS)
	-rm $(SAMPLE_EXEC)
