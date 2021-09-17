SAMPLE_SOURCES = $(wildcard sandbox/sample/*.c)
SAMPLE_OBJECTS = $(SAMPLE_SOURCES:.c=.o)

SAMPLE_EXEC = sample$(EXECUTABLE_SUFFIX)

build_message_sample:
	@echo "$(SECTION_PREFIX) Sample Project"
	@echo "$(INFO_PREFIX) Genesis sandbox sample project!"

sample: gencore $(SAMPLE_EXEC) ### @User Builds a sample sandbox project

$(SAMPLE_EXEC): CFLAGS = $(GEN_CORE_CFLAGS)
$(SAMPLE_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(SAMPLE_EXEC): $(SAMPLE_OBJECTS)

clean_sample:
	-$(RM) $(subst /,$(SEP),$(SAMPLE_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(SAMPLE_EXEC))
