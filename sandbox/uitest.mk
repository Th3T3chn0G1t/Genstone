UI_TEST_SOURCES = $(wildcard sandbox/uitest/*.c)
UI_TEST_OBJECTS = $(UI_TEST_SOURCES:.c=.o)

UI_TEST_EXEC = uitest$(EXECUTABLE_SUFFIX)

build_message_uitest:
	@echo "$(SECTION_PREFIX) UI Test Project"
	@echo "$(INFO_PREFIX) Genstone UI test sandbox project"

uitest: build_message_uitest $(UI_TEST_EXEC) ### @User Builds a UI test sandbox project

$(UI_TEST_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_UI_CFLAGS) $(shell pkg-config --cflags sdl2) $(shell pkg-config --cflags SDL2_image) $(shell pkg-config --cflags SDL2_ttf)
$(UI_TEST_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) $(GEN_UI_LFLAGS) $(shell pkg-config --libs sdl2) $(shell pkg-config --libs SDL2_image) $(shell pkg-config --libs SDL2_ttf)
$(UI_TEST_EXEC): $(UI_TEST_OBJECTS)

$(UI_TEST_OBJECTS): $(GEN_CORE_LIB) $(GEN_UI_LIB)

clean_uitest:
	-rm $(UI_TEST_OBJECTS)
	-rm $(UI_TEST_EXEC)
