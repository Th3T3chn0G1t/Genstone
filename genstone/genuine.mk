GEN_UI_CFLAGS = -Igenstone/genuine/include $(GEN_CORE_CFLAGS)  $(GEN_CALC_CFLAGS)
GEN_UI_LFLAGS = -lgenuine $(GEN_CORE_LFLAGS) $(GEN_CALC_LFLAGS)

GEN_UI_SOURCES = $(wildcard genstone/genuine/*.c)
GEN_UI_OBJECTS = $(GEN_UI_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_UI_LIB = lib/$(LIB_PREFIX)genuine$(DYNAMIC_LIB_SUFFIX)

build_message_genuine:
	@$(ECHO) "$(SECTION_PREFIX) Genuine"
	@$(ECHO) "$(INFO_PREFIX) Genstone GUI module"

genuine: gencore build_message_genuine $(GEN_UI_LIB) ### @Genstone Builds Genstone user-interface utilities

$(GEN_UI_LIB): CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_CALC_CFLAGS)
$(GEN_UI_LIB): LFLAGS = -Llib $(GEN_CORE_LFLAGS) $(GEN_CALC_LFLAGS)
$(GEN_UI_LIB): $(GEN_UI_OBJECTS) $(GEN_CORE_LIB) | lib

$(GEN_UI_OBJECTS): $(wildcard genstone/genuine/include/*.h)
$(GEN_UI_OBJECTS): $(GEN_CORE_LIB)

clean_genuine:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(GEN_UI_OBJECTS)
	-rm $(GEN_UI_LIB)
	@$(ECHO) "$(ACTION_SUFFIX)"
