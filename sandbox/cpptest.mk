CPPTEST_SOURCES = $(wildcard sandbox/cpptest/*.cpp)
CPPTEST_OBJECTS = $(CPPTEST_SOURCES:.cpp=$(OBJECT_SUFFIX))

CPPTEST_EXEC = cpptest$(EXECUTABLE_SUFFIX)

build_message_cpptest:
	@$(ECHO) "$(SECTION_PREFIX) C++ Sample Project"
	@$(ECHO) "$(INFO_PREFIX) Genstone sandbox cpptest project!"

cpptest: $(CPPTEST_EXEC) ### @User Builds a c++ test sandbox project

$(CPPTEST_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_PORT_CFLAGS)
$(CPPTEST_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) $(GEN_PORT_LFLAGS)
$(CPPTEST_EXEC): $(CPPTEST_OBJECTS)

$(CPPTEST_OBJECTS): $(GEN_PORT_LIB) $(GEN_CORE_LIB)

clean_cpptest:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(CPPTEST_OBJECTS)
	-rm $(CPPTEST_EXEC)
	@$(ECHO) "$(ACTION_SUFFIX)"
