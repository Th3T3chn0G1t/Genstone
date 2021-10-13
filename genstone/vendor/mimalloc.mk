MIMALLOC_CFLAGS += -Igenstone/vendor/mimalloc/include
MIMALLOC_LFLAGS += -lmimalloc

MIMALLOC_LIB = lib/$(LIB_PREFIX)mimalloc$(DYNAMIC_LIB_SUFFIX)
_MIMALLOC_LIB_INTERNAL = genstone/vendor/tmp/mimalloc/$(LIB_PREFIX)safec$(DYNAMIC_LIB_SUFFIX)
_MIMALLOC_LIB_INTERNAL_MAKEFILE = genstone/vendor/tmp/mimalloc/Makefile

vendor/tmp:
	-mkdir $@

vendor/tmp/mimalloc: | vendor/tmp
	-mkdir $@

clean_mimalloc: | $(_MIMALLOC_LIB_INTERNAL_MAKEFILE)
	$(MAKE) -Cgenstone/vendor/mimalloc/safeclib clean

$(_MIMALLOC_LIB_INTERNAL_MAKEFILE): | vendor/tmp/mimalloc
	$(CMAKE) $(GLOBAL_CMAKE_MODULE_FLAGS) -Bgenstone/vendor/tmp/mimalloc genstone/vendor/mimalloc

$(_MIMALLOC_LIB_INTERNAL): $(_MIMALLOC_LIB_INTERNAL_MAKEFILE)
	$(MAKE) -Cgenstone/vendor/tmp/mimalloc

$(MIMALLOC_LIB): $(_MIMALLOC_LIB_INTERNAL) | lib
	$(CP) $(subst /,$(SEP),$(_MIMALLOC_LIB_INTERNAL_PATTERN) lib)
ifeq ($(PLATFORM),DWN)
	install_name_tool -id @rpath/$(notdir $(wildcard genstone/vendor/mimalloc/safeclib/src/.libs/$(LIB_PREFIX)safec.*$(DYNAMIC_LIB_SUFFIX))) $(MIMALLOC_LIB)
endif

build_message_mimalloc:
	@echo "$(SECTION_PREFIX) Mimalloc"
	@echo "$(INFO_PREFIX) The gods of Microsoft hath bestowed their memory magic on us"

mimalloc: build_message_mimalloc $(MIMALLOC_LIB) 
clean_mimalloc: $(MIMALLOC_CLEAN_TARGETS)
