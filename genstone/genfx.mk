_GEN_FX_CFLAGS = $(shell pkg-config --cflags xcb)
_GEN_FX_LFLAGS = $(shell pkg-config --libs xcb)

GEN_FX_CFLAGS = -Igenstone/genfx/include $(GEN_CORE_CFLAGS) $(GEN_CALC_CFLAGS) $(_GEN_FX_CFLAGS)
GEN_FX_LFLAGS = -lgenfx $(GEN_CORE_LFLAGS) $(GEN_CALC_LFLAGS) $(_GEN_FX_LFLAGS)

GEN_FX_SOURCES = $(wildcard genstone/genfx/*.c)
GEN_FX_OBJECTS = $(GEN_FX_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_FX_LIB = lib/$(LIB_PREFIX)genfx$(DYNAMIC_LIB_SUFFIX)

build_message_genfx:
	@$(ECHO) "$(SECTION_PREFIX) Genfx"
	@$(ECHO) "$(INFO_PREFIX) Genstone audiovisual effects module"

genfx: gencore build_message_genfx $(GEN_FX_LIB) ### @Genstone Builds Genstone audiovisual effects module

$(GEN_FX_LIB): CFLAGS = $(GEN_CORE_CFLAGS) $(GEN_CALC_CFLAGS) $(_GEN_FX_CFLAGS)
$(GEN_FX_LIB): LFLAGS = -Llib $(GEN_CORE_LFLAGS) $(GEN_CALC_LFLAGS) $(_GEN_FX_LFLAGS)
$(GEN_FX_LIB): $(GEN_FX_OBJECTS) $(GEN_CORE_LIB) | lib

$(GEN_FX_OBJECTS): $(wildcard genstone/genfx/include/*.h)
$(GEN_FX_OBJECTS): $(GEN_CORE_LIB)

clean_genfx:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(GEN_FX_OBJECTS)
	-rm $(GEN_FX_LIB)
	@$(ECHO) "$(ACTION_SUFFIX)"
