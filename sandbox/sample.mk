SAMPLE_SOURCES = $(wildcard sandbox/sample/*.c)
SAMPLE_OBJECTS = $(SAMPLE_SOURCES:.c=.o)

SAMPLE_EXEC = sample$(EXECUTABLE_SUFFIX)

sample: $(SAMPLE_EXEC)

$(SAMPLE_EXEC): $(SAMPLE_OBJECTS)

clean_sample:
	-rm $(SAMPLE_OBJECTS)
	-rm $(SAMPLE_EXEC)
