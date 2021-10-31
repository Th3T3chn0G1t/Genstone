GEN_UI_CFLAGS = -Igenstone/genuine/include $(GEN_CORE_CFLAGS)
GEN_UI_LFLAGS = -lgenuine $(GEN_CORE_LFLAGS)

GEN_UI_SOURCES = $(wildcard genstone/genuine/*.c)
GEN_UI_OBJECTS = $(GEN_UI_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_UI_LIB = lib/$(LIB_PREFIX)genuine$(DYNAMIC_LIB_SUFFIX)

.ONESHELL:
build_message_genuine:
	@echo "$(SECTION_PREFIX) Genuine"
	@echo "$(INFO_PREFIX) Genstone GUI module"

.ONESHELL:
genuine: gencore build_message_genuine $(GEN_UI_LIB) ### @Genstone Builds Genstone user-interface utilities

$(GEN_UI_LIB): CFLAGS = $(GEN_CORE_CFLAGS)
$(GEN_UI_LIB): LFLAGS = -Llib $(GEN_CORE_LFLAGS)
$(GEN_UI_LIB): $(GEN_UI_OBJECTS) | lib

$(GEN_UI_OBJECTS): $(wildcard genstone/genuine/include/*.h)

.ONESHELL:
clean_genuine:
	-$(RM) $(GEN_UI_OBJECTS)
	-$(RM) $(GEN_UI_LIB)
