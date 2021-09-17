GEN_UI_CFLAGS = -Igenesis/genuine/include $(GEN_CORE_CFLAGS)
GEN_UI_LFLAGS = -lgenuine $(GEN_CORE_LFLAGS)

GEN_UI_SOURCES = $(wildcard genesis/genuine/*.c)
GEN_UI_OBJECTS = $(GEN_UI_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_UI_LIB = lib/$(LIB_PREFIX)genuine$(DYNAMIC_LIB_SUFFIX)

build_message_genuine:
	@echo "$(SECTION_PREFIX) Genuine"
	@echo "$(INFO_PREFIX) Genesis GUI module"

genuine: gencore build_message_genuine $(GEN_UI_LIB) ### @Genesis Builds Genesis user-interface utilities

$(GEN_UI_LIB): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_UI_LIB): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_UI_LIB): $(GEN_UI_OBJECTS) | lib

$(GEN_UI_OBJECTS): $(wildcard genesis/genuine/include/*.h)

clean_genuine:
	-$(RM) $(GEN_UI_OBJECTS)
	-$(RM) $(GEN_UI_LIB)
