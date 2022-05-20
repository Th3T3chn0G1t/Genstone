SAMPLE_SOURCES = $(wildcard sandbox/sample/*.c)
SAMPLE_OBJECTS = $(SAMPLE_SOURCES:.c=$(OBJECT_SUFFIX))

SAMPLE_SHADER_MODULES = sandbox/sample/vertex.spv sandbox/sample/fragment.spv

SAMPLE_EXEC = sample$(EXECUTABLE_SUFFIX)

build_message_sample:
	@$(ECHO) "$(SECTION_PREFIX) Sample Project"
	@$(ECHO) "$(INFO_PREFIX) Genstone sandbox sample project!"

sample: build_message_sample $(SAMPLE_EXEC) $(SAMPLE_SHADER_MODULES) ### @User Builds a sample sandbox project

$(SAMPLE_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_FX_CFLAGS)
$(SAMPLE_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) $(GEN_FX_LFLAGS)
$(SAMPLE_EXEC): $(SAMPLE_OBJECTS)

$(SAMPLE_OBJECTS): $(GEN_CORE_LIB) $(GEN_FX_LIB)

clean_sample:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(SAMPLE_OBJECTS)
	-rm $(SAMPLE_EXEC)
	@$(ECHO) "$(ACTION_SUFFIX)"
