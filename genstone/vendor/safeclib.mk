SAFEC_CFLAGS = -Igenstone/vendor/safeclib/include
SAFEC_LFLAGS = -lsafec

SAFEC_LIB = lib/$(LIB_PREFIX)safec$(DYNAMIC_LIB_SUFFIX)
_SAFEC_LIB_INTERNAL = genstone/vendor/safeclib/src/.libs/$(LIB_PREFIX)safec$(DYNAMIC_LIB_SUFFIX)
_SAFEC_LIB_INTERNAL_PATTERN = $(wildcard genstone/vendor/safeclib/src/.libs/$(LIB_PREFIX)safec*$(DYNAMIC_LIB_SUFFIX)*)
_SAFEC_LIB_INTERNAL_MAKEFILE = genstone/vendor/safeclib/Makefile

build_message_safeclib:
	@echo "$(SECTION_PREFIX) Safeclib"
	@echo "$(INFO_PREFIX) For one night and one night only, Rurban brings us Annex K"

$(_SAFEC_LIB_INTERNAL_MAKEFILE):
	cd genstone/vendor/safeclib && ./build-aux/autogen.sh
	cd genstone/vendor/safeclib && ./configure --prefix=/usr --enable-unsafe

$(_SAFEC_LIB_INTERNAL): $(_SAFEC_LIB_INTERNAL_MAKEFILE)
	$(MAKE) -Cgenstone/vendor/safeclib

$(SAFEC_LIB): $(_SAFEC_LIB_INTERNAL) | lib
	cp -r $(_SAFEC_LIB_INTERNAL_PATTERN) lib
ifeq ($(PLATFORM),DWN)
	install_name_tool -id @rpath/$(notdir $(wildcard genstone/vendor/safeclib/src/.libs/$(LIB_PREFIX)safec.*$(DYNAMIC_LIB_SUFFIX))) $(SAFEC_LIB)
endif

safeclib: build_message_safeclib $(SAFEC_LIB) 

clean_safeclib:
# We could clean the submodule here for the base clean target, but we *really* want to avoid rebuilding this lib
# (It takes forever)
clean_safeclib_real: | $(_SAFEC_LIB_INTERNAL_MAKEFILE)
	$(MAKE) -Cgenstone/vendor/safeclib clean
	$(MAKE) -Cgenstone/vendor/safeclib distclean
	-rm $(wildcard lib/$(LIB_PREFIX)safec*$(DYNAMIC_LIB_SUFFIX)*)
