CPPTEST_SOURCES = $(wildcard sandbox/cpptest/*.cpp)
CPPTEST_OBJECTS = $(CPPTEST_SOURCES:.cpp=.o)

CPPTEST_EXEC = cpptest$(EXECUTABLE_SUFFIX)

build_message_cpptest:
	@echo "$(SECTION_PREFIX) C++ Sample Project"
	@echo "$(INFO_PREFIX) Genstone sandbox cpptest project!"

cpptest: gencore gepport $(CPPTEST_EXEC) ### @User Builds a c++ test sandbox project

$(CPPTEST_EXEC): CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_PORT_CFLAGS)
$(CPPTEST_EXEC): LFLAGS = -Llib $(GEN_CORE_LFLAGS) $(GEN_PORT_LFLAGS)
$(CPPTEST_EXEC): $(CPPTEST_OBJECTS)

clean_cpptest:
	-$(RM) $(subst /,$(SEP),$(CPPTEST_OBJECTS))
	-$(RM) $(subst /,$(SEP),$(CPPTEST_EXEC))
