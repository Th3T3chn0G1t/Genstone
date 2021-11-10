GLEAN_SOURCES = $(wildcard sandbox/glean/*.c)
GLEAN_OBJECTS = $(GLEAN_SOURCES:.c=.o)

GLEAN_EXEC = glean$(EXECUTABLE_SUFFIX)

build_message_glean:
	@echo "$(SECTION_PREFIX) Glean"
	@echo "$(INFO_PREFIX) Port of Glean editor to a Genstone project"

glean: gencore $(GLEAN_EXEC) ### @User Builds Glean

$(GLEAN_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(sort $(shell pkg-config --cflags gtk+-3.0) $(shell pkg-config --cflags json-glib-1.0) $(shell pkg-config --cflags vte-2.91))
$(GLEAN_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) -lpthread $(sort $(shell pkg-config --libs gtk+-3.0) $(shell pkg-config --libs json-glib-1.0) $(shell pkg-config --libs vte-2.91))
$(GLEAN_EXEC): $(GLEAN_OBJECTS)

clean_glean:
	-$(RM) $(subst /,$(SEP),$(GLEAN_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(GLEAN_EXEC))
