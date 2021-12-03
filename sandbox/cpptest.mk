CPPTEST_SOURCES = $(wildcard sandbox/cpptest/*.cpp)
CPPTEST_OBJECTS = $(CPPTEST_SOURCES:.cpp=.o)

CPPTEST_EXEC = cpptest$(EXECUTABLE_SUFFIX)

build_message_cpptest:
	@echo "$(SECTION_PREFIX) C++ Sample Project"
	@echo "$(INFO_PREFIX) Genstone sandbox cpptest project!"

cpptest: $(CPPTEST_EXEC) ### @User Builds a c++ test sandbox project

$(CPPTEST_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_PORT_CFLAGS)
$(CPPTEST_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) $(GEN_PORT_LFLAGS)
$(CPPTEST_EXEC): $(CPPTEST_OBJECTS)

$(CPPTEST_OBJECTS): $(GEN_CORE_LIB) $(GEN_PORT_LIB)

clean_cpptest:
	-rm $(CPPTEST_OBJECTS)
	-rm $(CPPTEST_EXEC)
