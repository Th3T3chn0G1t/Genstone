MIMALLOC_DEFINES = -DMI_SECURE=4 -DMI_SHARED_LIB -DMI_SHARED_LIB_EXPORT
ifeq ($(BUILD_MODE),DEBUG)
MIMALLOC_DEFINES +=  -DMI_DEBUG_FULL=3 -DMI_SHOW_ERRORS=1
endif

MIMALLOC_CFLAGS = -Igenstone/vendor/mimalloc/include $(MIMALLOC_DEFINES)
MIMALLOC_LFLAGS = -lmimalloc -pthread

MIMALLOC_DISABLED_SOURCES = alloc-override.c alloc-override-osx.c page-queue.c static.c
MIMALLOC_SOURCES = $(filter-out $(addprefix genstone/vendor/mimalloc/src/,$(MIMALLOC_DISABLED_SOURCES)),$(wildcard genstone/vendor/mimalloc/src/*.c))
MIMALLOC_OBJECTS = $(MIMALLOC_SOURCES:.c=$(OBJECT_SUFFIX))

MIMALLOC_LIB = lib/$(LIB_PREFIX)mimalloc$(DYNAMIC_LIB_SUFFIX)

_MIMALLOC_CFLAGS = -include stdbool.h -ftls-model=local-dynamic -Igenstone/vendor/mimalloc/include $(MIMALLOC_DEFINES)

build_message_mimalloc:
	@$(ECHO) "$(SECTION_PREFIX) Mimalloc"
	@$(ECHO) "$(INFO_PREFIX) The gods of Microsoft hath bestowed their memory magic on us"

mimalloc: build_message_mimalloc $(MIMALLOC_LIB) ### @Vendor builds Microsoft `mimalloc` as a Genstone module

$(MIMALLOC_LIB): CFLAGS = $(_MIMALLOC_CFLAGS)
$(MIMALLOC_LIB): LFLAGS = $(_MIMALLOC_LFLAGS)
$(MIMALLOC_LIB): CLANG_FORMAT = DISABLED
$(MIMALLOC_LIB): $(MIMALLOC_OBJECTS) | lib

clean_mimalloc:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(MIMALLOC_OBJECTS)
	-rm $(MIMALLOC_LIB)
	@$(ECHO) "$(ACTION_SUFFIX)"
