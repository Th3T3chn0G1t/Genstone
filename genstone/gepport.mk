GEN_PORT_CFLAGS = -Igenstone/gepport/include $(GEN_CORE_CFLAGS)
GEN_PORT_LFLAGS = -lgepport $(GEN_CORE_LFLAGS)

GEN_PORT_SOURCES = $(wildcard genstone/gepport/*.cpp)
GEN_PORT_OBJECTS = $(GEN_PORT_SOURCES:.cpp=$(OBJECT_SUFFIX))

GEN_PORT_LIB = lib/$(LIB_PREFIX)gepport$(STATIC_LIB_SUFFIX)

build_message_gepport:
	@echo "$(SECTION_PREFIX) Gepport"
	@echo "$(INFO_PREFIX) Genstone C++ backporting module"

gepport: gencore build_message_gepport $(GEN_PORT_LIB) ### @Genstone Builds Genstone user-interface utilities

$(GEN_PORT_LIB): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_PORT_LIB): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_PORT_LIB): $(GEN_PORT_OBJECTS) | lib

$(GEN_PORT_OBJECTS): $(wildcard genstone/gepport/include/*.hpp)

clean_gepport:
	-$(RM) $(GEN_PORT_OBJECTS)
	-$(RM) $(GEN_PORT_LIB)