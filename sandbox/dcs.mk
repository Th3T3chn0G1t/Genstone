DCS_SOURCES = $(wildcard sandbox/dcs/src/*.c)
DCS_OBJECTS = $(DCS_SOURCES:.c=.o)

DCS_EXEC = dcs$(EXECUTABLE_SUFFIX)

build_message_dcs:
	@echo "$(SECTION_PREFIX) DCS"
	@echo "$(INFO_PREFIX) DCS language development"

dcs: build_message_dcs $(DCS_EXEC) ### @User DCS language development

$(DCS_EXEC): CFLAGS = $(GEN_CORE_CFLAGS)
$(DCS_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(DCS_EXEC): $(DCS_OBJECTS)

$(DCS_OBJECTS): $(GEN_CORE_LIB) $(MIMALLOC_LIB) $(SAFEC_LIB)

clean_dcs:
	@echo "$(ACTION_PREFIX)"
	-rm $(DCS_OBJECTS)
	-rm $(DCS_EXEC)
	@echo "$(ACTION_SUFFIX)"
