SAMPLE_SOURCES = $(wildcard sandbox/sample/*.c)
SAMPLE_OBJECTS = $(SAMPLE_SOURCES:.c=.o)

SAMPLE_EXEC = sample$(EXECUTABLE_SUFFIX)

sample: $(SAMPLE_EXEC)

$(SAMPLE_EXEC): CFLAGS = $(C11COMPAT_CFLAGS)
$(SAMPLE_EXEC): CFLAGS += -Igenesis/gen-rt/include -Igenesis/gencore/include -Igenesis/genfx/gfx/include -Igenesis/genfx/sfx/include
$(SAMPLE_EXEC): LFLAGS = -Lgenesis/vendor/lib $(C11COMPAT_LFLAGS)
$(SAMPLE_EXEC): $(SAMPLE_OBJECTS)

clean_sample:
	-rm $(SAMPLE_OBJECTS)
	-rm $(SAMPLE_EXEC)
