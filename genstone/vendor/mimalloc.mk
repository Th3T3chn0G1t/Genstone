MIMALLOC_CFLAGS += -Igenstone/vendor/mimalloc/include
MIMALLOC_LFLAGS += -lmimalloc

MIMALLOC_LIB = lib/$(LIB_PREFIX)mimalloc$(DYNAMIC_LIB_SUFFIX)
ifeq ($(BUILD_MODE),DEBUG)
_MIMALLOC_LIB_TAGGED_OUTPUT = lib/$(LIB_PREFIX)mimalloc-secure-debug$(DYNAMIC_LIB_SUFFIX)
else
_MIMALLOC_LIB_TAGGED_OUTPUT = lib/$(LIB_PREFIX)mimalloc-secure$(DYNAMIC_LIB_SUFFIX)
endif
_MIMALLOC_LIB_INTERNAL = genstone/vendor/tmp/mimalloc/$(LIB_PREFIX)mimalloc$(DYNAMIC_LIB_SUFFIX)
_MIMALLOC_LIB_INTERNAL_PATTERN = $(wildcard genstone/vendor/tmp/mimalloc/$(LIB_PREFIX)mimalloc*$(DYNAMIC_LIB_SUFFIX)*)
_MIMALLOC_LIB_INTERNAL_MAKEFILE = genstone/vendor/tmp/mimalloc/Makefile

_MIMALLOC_CMAKE_FLAGS = -DMI_SECURE=ON -DMI_OVERRIDE=OFF
ifeq ($(BUILD_MODE),DEBUG)
_MIMALLOC_CMAKE_FLAGS += -DMI_DEBUG_FULL=ON
endif

genstone/vendor/tmp:
	-mkdir $@

genstone/vendor/tmp/mimalloc: | genstone/vendor/tmp
	-mkdir $@

$(_MIMALLOC_LIB_INTERNAL_MAKEFILE): | genstone/vendor/tmp/mimalloc
	$(CMAKE) $(GLOBAL_CMAKE_MODULE_FLAGS) $(_MIMALLOC_CMAKE_FLAGS) -DCOMPILE_DEFINITIONS=_CRT_SECURE_NO_WARNINGS  -Bgenstone/vendor/tmp/mimalloc genstone/vendor/mimalloc

$(_MIMALLOC_LIB_INTERNAL): $(_MIMALLOC_LIB_INTERNAL_MAKEFILE)
	$(MAKE) -Cgenstone/vendor/tmp/mimalloc

$(MIMALLOC_LIB): $(_MIMALLOC_LIB_INTERNAL) | lib
	$(CP) $(subst /,$(SEP),$(_MIMALLOC_LIB_INTERNAL_PATTERN)) lib
# We want a basename mimalloc lib for linkage
	$(CP) $(subst /,$(SEP),$(_MIMALLOC_LIB_TAGGED_OUTPUT) $(MIMALLOC_LIB))

build_message_mimalloc:
	@echo "$(SECTION_PREFIX) Mimalloc"
	@echo "$(INFO_PREFIX) The gods of Microsoft hath bestowed their memory magic on us"

mimalloc: build_message_mimalloc $(MIMALLOC_LIB) ### @Vendor builds Microsoft `mimalloc` as a Genstone module

clean_mimalloc:
	-$(RMDIR) genstone/vendor/tmp/mimalloc
	-$(RM) $(wildcard lib/*mimalloc*)
