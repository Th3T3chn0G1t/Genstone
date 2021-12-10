SAFEC_CFLAGS = -Igenstone/vendor/safeclib/include
SAFEC_LFLAGS = -lsafec

SAFEC_LIB = lib/$(LIB_PREFIX)safec$(DYNAMIC_LIB_SUFFIX)
_SAFEC_LIB_INTERNAL = genstone/vendor/safeclib/src/.libs/$(LIB_PREFIX)safec$(DYNAMIC_LIB_SUFFIX)
_SAFEC_LIB_INTERNAL_PATTERN = $(wildcard genstone/vendor/safeclib/src/.libs/$(LIB_PREFIX)safec*$(DYNAMIC_LIB_SUFFIX)*)
_SAFEC_LIB_INTERNAL_MAKEFILE = genstone/vendor/safeclib/Makefile
SAFEC_LIB_CONFIGURE_FLAGS = --enable-unsafe --disable-doc --enable-warn-dmax --enable-silent-rules --enable-tests=no
ifeq ($(BUILD_MODE),DEBUG)
SAFEC_LIB_CONFIGURE_FLAGS += --enable-debug-build --enable-debug
endif

build_message_safeclib:
	@echo "$(SECTION_PREFIX) Safeclib"
	@echo "$(INFO_PREFIX) For one night and one night only, Rurban brings us Annex K"

$(_SAFEC_LIB_INTERNAL_MAKEFILE):
	@echo "$(ACTION_PREFIX)autoreconf -Wall --install$(ACTION_SUFFIX)"
	@echo "$(ACTION_PREFIX)"
	@cd genstone/vendor/safeclib && autoreconf -Wall --install
	@echo "$(ACTION_SUFFIX)"

	@echo "$(ACTION_PREFIX)./configure $(GLOBAL_CONFIGURE_FLAGS) CFLAGS="$(GLOBAL_CONFIGURE_CFLAGS)" LFLAGS="$(GLOBAL_CONFIGURE_LFLAGS)" $(SAFEC_LIB_CONFIGURE_FLAGS)$(ACTION_SUFFIX)"
	@echo "$(ACTION_PREFIX)"
	@cd genstone/vendor/safeclib && ./configure $(GLOBAL_CONFIGURE_FLAGS) CFLAGS="$(GLOBAL_CONFIGURE_CFLAGS)" LFLAGS="$(GLOBAL_CONFIGURE_LFLAGS)" $(SAFEC_LIB_CONFIGURE_FLAGS)
	@echo "$(ACTION_SUFFIX)"

$(_SAFEC_LIB_INTERNAL): $(_SAFEC_LIB_INTERNAL_MAKEFILE)
	@echo "$(ACTION_PREFIX)$(MAKE) -Cgenstone/vendor/safeclib$(ACTION_SUFFIX)"
	@echo "$(ACTION_PREFIX)"
	@$(MAKE) -Cgenstone/vendor/safeclib
	@echo "$(ACTION_SUFFIX)"

$(SAFEC_LIB): $(_SAFEC_LIB_INTERNAL) | lib
	@echo "$(ACTION_PREFIX)cp -r $(_SAFEC_LIB_INTERNAL_PATTERN) lib$(ACTION_SUFFIX)"
	@echo "$(ACTION_PREFIX)"
	@cp -r $(_SAFEC_LIB_INTERNAL_PATTERN) lib
	@echo "$(ACTION_SUFFIX)"
ifeq ($(PLATFORM),DWN)
	@echo "$(ACTION_PREFIX)install_name_tool -id @rpath/$(notdir $(wildcard genstone/vendor/safeclib/src/.libs/$(LIB_PREFIX)safec.*$(DYNAMIC_LIB_SUFFIX))) $(SAFEC_LIB)$(ACTION_SUFFIX)"
	@echo "$(ACTION_PREFIX)"
	@install_name_tool -id @rpath/$(notdir $(wildcard genstone/vendor/safeclib/src/.libs/$(LIB_PREFIX)safec.*$(DYNAMIC_LIB_SUFFIX))) $(SAFEC_LIB)
	@echo "$(ACTION_SUFFIX)"
endif

safeclib: build_message_safeclib $(SAFEC_LIB) 

clean_safeclib:
# We could clean the submodule here for the base clean target, but we *really* want to avoid rebuilding this lib
# (It takes forever)
clean_safeclib_real:
	@echo "$(ACTION_PREFIX)"
	-$(MAKE) -Cgenstone/vendor/safeclib clean
	-rm $(_SAFEC_LIB_INTERNAL_MAKEFILE)
	-rm $(wildcard lib/$(LIB_PREFIX)safec*$(DYNAMIC_LIB_SUFFIX)*)
	@echo "$(ACTION_SUFFIX)"
