GIDLE_SOURCES = $(wildcard sandbox/gidle/*.c)
GIDLE_OBJECTS = $(GIDLE_SOURCES:.c=$(OBJECT_SUFFIX))

GIDLE_EXEC = gidle$(EXECUTABLE_SUFFIX)

build_message_gidle:
	@$(ECHO) "$(SECTION_PREFIX) Gidle"
	@$(ECHO) "$(INFO_PREFIX) A game written under Genstone"

gidle: build_message_gidle $(GIDLE_EXEC) ### @User Builds Gidle

$(GIDLE_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_UI_CFLAGS) $(shell $(PKGCONFIG) --cflags sdl2) $(shell $(PKGCONFIG) --cflags SDL2_image) $(shell $(PKGCONFIG) --cflags SDL2_ttf)
$(GIDLE_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) $(GEN_UI_LFLAGS) $(shell $(PKGCONFIG) --libs sdl2) $(shell $(PKGCONFIG) --libs SDL2_image) $(shell $(PKGCONFIG) --libs SDL2_ttf)
$(GIDLE_EXEC): $(GIDLE_OBJECTS)

$(GIDLE_OBJECTS): $(GEN_UI_LIB) $(GEN_CORE_LIB)

clean_gidle:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(GIDLE_OBJECTS)
	-rm $(GIDLE_EXEC)
	@$(ECHO) "$(ACTION_SUFFIX)"
