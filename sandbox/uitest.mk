UI_TEST_SOURCES = $(wildcard sandbox/uitest/*.c)
UI_TEST_OBJECTS = $(UI_TEST_SOURCES:.c=$(OBJECT_SUFFIX))

UI_TEST_EXEC = uitest$(EXECUTABLE_SUFFIX)

build_message_uitest:
	@$(ECHO) "$(SECTION_PREFIX) UI Test Project"
	@$(ECHO) "$(INFO_PREFIX) Genstone UI test sandbox project"

uitest: build_message_uitest $(UI_TEST_EXEC) ### @User Builds a UI test sandbox project

$(UI_TEST_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_UI_CFLAGS) $(shell $(PKGCONFIG) --cflags sdl2) $(shell $(PKGCONFIG) --cflags SDL2_image) $(shell $(PKGCONFIG) --cflags SDL2_ttf)
$(UI_TEST_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) $(GEN_UI_LFLAGS) $(shell $(PKGCONFIG) --libs sdl2) $(shell $(PKGCONFIG) --libs SDL2_image) $(shell $(PKGCONFIG) --libs SDL2_ttf)
$(UI_TEST_EXEC): $(UI_TEST_OBJECTS)

$(UI_TEST_OBJECTS): $(GEN_UI_LIB) $(GEN_CORE_LIB)

clean_uitest:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(UI_TEST_OBJECTS)
	-rm $(UI_TEST_EXEC)
	@$(ECHO) "$(ACTION_SUFFIX)"
