GLEAN_SOURCES = $(wildcard sandbox/glean/*.c)
GLEAN_OBJECTS = $(GLEAN_SOURCES:.c=.o)

GLEAN_EXEC = glean$(EXECUTABLE_SUFFIX)

build_message_glean:
	@echo "$(SECTION_PREFIX) Glean"
	@echo "$(INFO_PREFIX) Port of Glean editor to a Genstone project"

glean: build_message_glean $(GLEAN_EXEC) ### @User Builds Glean

$(GLEAN_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(shell pkg-config --cflags gtk+-3.0) $(shell pkg-config --cflags json-glib-1.0) $(shell pkg-config --cflags vte-2.91)
$(GLEAN_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) -lpthread $(shell pkg-config --libs gtk+-3.0) $(shell pkg-config --libs json-glib-1.0) $(shell pkg-config --libs vte-2.91)
$(GLEAN_EXEC): $(GLEAN_OBJECTS)

$(GLEAN_OBJECTS): $(GEN_CORE_LIB) $(MIMALLOC_LIB) $(SAFEC_LIB)

clean_glean:
	@echo "$(ACTION_PREFIX)"
	-rm $(GLEAN_OBJECTS)
	-rm $(GLEAN_EXEC)
	@echo "$(ACTION_SUFFIX)"
