GLEAN_SOURCES = $(wildcard sandbox/glean/*.c)
GLEAN_OBJECTS = $(GLEAN_SOURCES:.c=$(OBJECT_SUFFIX))

GLEAN_EXEC = glean$(EXECUTABLE_SUFFIX)

build_message_glean:
	@$(ECHO) "$(SECTION_PREFIX) Glean"
	@$(ECHO) "$(INFO_PREFIX) Port of Glean editor to a Genstone project"

glean: build_message_glean $(GLEAN_EXEC)

$(GLEAN_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(shell $(PKGCONFIG) --cflags gtk+-3.0) $(shell $(PKGCONFIG) --cflags json-glib-1.0) $(shell $(PKGCONFIG) --cflags vte-2.91)
$(GLEAN_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) -lpthread $(shell $(PKGCONFIG) --libs gtk+-3.0) $(shell $(PKGCONFIG) --libs json-glib-1.0) $(shell $(PKGCONFIG) --libs vte-2.91)
$(GLEAN_EXEC): $(GLEAN_OBJECTS)

$(GLEAN_OBJECTS): $(GEN_CORE_LIB)

clean_glean:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(GLEAN_OBJECTS)
	-rm $(GLEAN_EXEC)
	@$(ECHO) "$(ACTION_SUFFIX)"
