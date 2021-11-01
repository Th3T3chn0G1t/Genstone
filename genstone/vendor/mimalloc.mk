MIMALLOC_CFLAGS += -Igenstone/vendor/mimalloc/include
MIMALLOC_LFLAGS += -lmimalloc

MIMALLOC_LIB = lib/$(LIB_PREFIX)mimalloc$(DYNAMIC_LIB_SUFFIX)
ifeq ($(BUILD_MODE),DEBUG)
_MIMALLOC_LIB_TAGGED_OUTPUT = lib/$(LIB_PREFIX)mimalloc-secure-debug$(DYNAMIC_LIB_SUFFIX)
else
_MIMALLOC_LIB_TAGGED_OUTPUT = lib/$(LIB_PREFIX)mimalloc-secure$(DYNAMIC_LIB_SUFFIX)
endif
ifeq ($(PLATFORM),WIN)
ifeq ($(BUILD_MODE),DEBUG)
_MIMALLOC_LIB_TAGGED_OUTPUT_WINDOWS_STATIC_LINK_LIB = lib/$(LIB_PREFIX)mimalloc-secure-debug$(STATIC_LIB_SUFFIX)
else
_MIMALLOC_LIB_TAGGED_OUTPUT_WINDOWS_STATIC_LINK_LIB = lib/$(LIB_PREFIX)mimalloc-secure$(STATIC_LIB_SUFFIX)
endif
endif
_MIMALLOC_LIB_INTERNAL = genstone/vendor/tmp/mimalloc/$(LIB_PREFIX)mimalloc$(DYNAMIC_LIB_SUFFIX)
_MIMALLOC_LIB_INTERNAL_PATTERN = $(wildcard genstone/vendor/tmp/mimalloc/$(LIB_PREFIX)mimalloc*$(DYNAMIC_LIB_SUFFIX)*)
ifeq ($(PLATFORM),WIN)
_MIMALLOC_LIB_INTERNAL_PATTERN_WINDOWS_STATIC_LINK_LIB = $(wildcard genstone/vendor/tmp/mimalloc/$(LIB_PREFIX)mimalloc*$(STATIC_LIB_SUFFIX)*)
endif
_MIMALLOC_LIB_INTERNAL_MAKEFILE = genstone/vendor/tmp/mimalloc/Makefile
ifeq ($(PLATFORM),WIN)
_MIMALLOC_LIB_WINDOWS_STATIC_LINK_LIB = lib/$(LIB_PREFIX)mimalloc$(STATIC_LIB_SUFFIX)
endif

genstone/vendor/tmp:
	-mkdir $@

genstone/vendor/tmp/mimalloc: | genstone/vendor/tmp
	-mkdir $@

$(_MIMALLOC_LIB_INTERNAL_MAKEFILE): | genstone/vendor/tmp/mimalloc
	$(CMAKE) $(GLOBAL_CMAKE_MODULE_FLAGS) -DCOMPILE_DEFINITIONS=_CRT_SECURE_NO_WARNINGS -DMI_SECURE=ON -Bgenstone/vendor/tmp/mimalloc genstone/vendor/mimalloc

$(_MIMALLOC_LIB_INTERNAL): $(_MIMALLOC_LIB_INTERNAL_MAKEFILE)
	$(MAKE) -Cgenstone/vendor/tmp/mimalloc

$(MIMALLOC_LIB): $(_MIMALLOC_LIB_INTERNAL) | lib
# Windows multiple copy requires + to combine the list
ifeq ($(PLATFORM),WIN)
	for %%I in ($(subst /,$(SEP),$(_MIMALLOC_LIB_INTERNAL_PATTERN))) do copy %%I lib
	for %%I in ($(subst /,$(SEP),$(_MIMALLOC_LIB_INTERNAL_PATTERN_WINDOWS_STATIC_LINK_LIB))) do copy %%I lib
else
	$(CP) $(subst /,$(SEP),$(_MIMALLOC_LIB_INTERNAL_PATTERN)) lib
endif
# We want a basename mimalloc lib for linkage
# On Unix platforms this copies the alias link, not the main lib
	$(CP) $(subst /,$(SEP),$(_MIMALLOC_LIB_TAGGED_OUTPUT) $(MIMALLOC_LIB))
ifeq ($(PLATFORM),WIN)
	$(CP) $(subst /,$(SEP),$(_MIMALLOC_LIB_TAGGED_OUTPUT_WINDOWS_STATIC_LINK_LIB) $(_MIMALLOC_LIB_WINDOWS_STATIC_LINK_LIB))
endif

build_message_mimalloc:
	@echo "$(SECTION_PREFIX) Mimalloc"
	@echo "$(INFO_PREFIX) The gods of Microsoft hath bestowed their memory magic on us"

mimalloc: build_message_mimalloc $(MIMALLOC_LIB) ### @Vendor builds Microsoft `mimalloc` as a Genstone module

clean_mimalloc:
	-$(RMDIR) genstone/vendor/tmp/mimalloc
	-$(RM) $(wildcard lib/*mimalloc*)
