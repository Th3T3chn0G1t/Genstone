SAFEC_CFLAGS = -Igenstone/vendor/safec_overrides -Igenstone/vendor/safeclib -Igenstone/vendor/safeclib/include -Igenstone/vendor/safeclib/src
SAFEC_LFLAGS = -lsafec

SAFEC_LIB = lib/$(LIB_PREFIX)safec$(DYNAMIC_LIB_SUFFIX)

SAFEC_DISABLED_SOURCES = extwchar/wcsstr.c
SAFEC_SOURCES = $(filter-out $(addprefix genstone/vendor/safeclib/src/,$(SAFEC_DISABLED_SOURCES)),$(wildcard genstone/vendor/safeclib/src/*.c) $(wildcard genstone/vendor/safeclib/src/extmem/*.c) $(wildcard genstone/vendor/safeclib/src/extstr/*.c) $(wildcard genstone/vendor/safeclib/src/extwchar/*.c) $(wildcard genstone/vendor/safeclib/src/io/*.c) $(wildcard genstone/vendor/safeclib/src/str/*.c) $(wildcard genstone/vendor/safeclib/src/mem/*.c) $(wildcard genstone/vendor/safeclib/src/misc/*.c) $(wildcard genstone/vendor/safeclib/src/os/*.c) $(wildcard genstone/vendor/safeclib/src/wchar/*.c))
SAFEC_OBJECTS = $(SAFEC_SOURCES:.c=$(OBJECT_SUFFIX))

_SAFEC_CFLAGS = -include string.h -Igenstone/vendor/safec_overrides -Igenstone/vendor/safeclib -Igenstone/vendor/safeclib/include -Igenstone/vendor/safeclib/src
_SAFEC_LFLAGS =

build_message_safeclib:
	@echo "$(SECTION_PREFIX) Safeclib"
	@echo "$(INFO_PREFIX) For one night and one night only, Rurban brings us Annex K"

safeclib: build_message_safeclib $(SAFEC_LIB) 

$(SAFEC_LIB): CFLAGS = $(_SAFEC_CFLAGS)
$(SAFEC_LIB): LFLAGS = $(_SAFEC_LFLAGS)
$(SAFEC_LIB): CLANG_FORMAT = DISABLED
$(SAFEC_LIB): $(SAFEC_OBJECTS) | lib

genstone/vendor/safeclib/src/extmem/timingsafe_memcmp.o: CFLAGS += -include limits.h

clean_safeclib:
	@echo "$(ACTION_PREFIX)"
	-rm $(SAFEC_OBJECTS)
	-rm $(SAFEC_LIB)
	@echo "$(ACTION_SUFFIX)"
