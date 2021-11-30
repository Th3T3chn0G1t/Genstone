# 		THREADS  ANNEXK
# LNX	0		 1
# DWN	1		 1

C11_COMPAT = 0
C11_COMPAT_THREADS = 0
C11_COMPAT_ANNEXK = 0

ifeq ($(PLATFORM),LNX)
	C11_COMPAT = 1
	C11_COMPAT_THREADS = 0
	C11_COMPAT_ANNEXK = 1

	C11_COMPAT_CFLAGS =
	C11_COMPAT_LFLAGS = -pthread
endif
ifeq ($(PLATFORM),DWN)
	C11_COMPAT = 1
	C11_COMPAT_THREADS = 1
	C11_COMPAT_ANNEXK = 1

	C11_COMPAT_CFLAGS =
	C11_COMPAT_LFLAGS = -pthread
endif

build_message_c11compat:
	@echo "$(SECTION_PREFIX) C11 Compat"
	@echo "$(INFO_PREFIX) C11 optional feature compatibility layers"

ifeq ($(C11_COMPAT),1)
ifeq ($(C11_COMPAT_THREADS),1)
C11_COMPAT_CFLAGS += -Igenstone/vendor/c11compat/mesa/include
C11_COMPAT_LFLAGS +=
endif
ifeq ($(C11_COMPAT_ANNEXK),1)
C11_COMPAT_CFLAGS += -Igenstone/vendor/c11compat/safeclib/include
C11_COMPAT_LFLAGS += -lsafec

C11_COMPAT_ANNEXK_LIB = lib/$(LIB_PREFIX)safec$(DYNAMIC_LIB_SUFFIX)
_C11_COMPAT_ANNEXK_LIB_INTERNAL = genstone/vendor/c11compat/safeclib/src/.libs/$(LIB_PREFIX)safec$(DYNAMIC_LIB_SUFFIX)
_C11_COMPAT_ANNEXK_LIB_INTERNAL_PATTERN = $(wildcard genstone/vendor/c11compat/safeclib/src/.libs/$(LIB_PREFIX)safec*$(DYNAMIC_LIB_SUFFIX)*)
_C11_COMPAT_ANNEXK_LIB_INTERNAL_MAKEFILE = genstone/vendor/c11compat/safeclib/Makefile

C11_COMPAT_LIB += $(C11_COMPAT_ANNEXK_LIB)

# We could clean the submodule here for the base clean target, but we *really* want to avoid rebuilding this lib
# (It takes forever)
#C11_COMPAT_CLEAN_TARGETS += clean_c11compat_annexk
clean_c11compat_annexk: | $(_C11_COMPAT_ANNEXK_LIB_INTERNAL_MAKEFILE)
	$(MAKE) -Cgenstone/vendor/c11compat/safeclib clean
	$(MAKE) -Cgenstone/vendor/c11compat/safeclib distclean
	-rm $(wildcard lib/$(LIB_PREFIX)safec*$(DYNAMIC_LIB_SUFFIX)*)

$(_C11_COMPAT_ANNEXK_LIB_INTERNAL_MAKEFILE):
	cd genstone/vendor/c11compat/safeclib && ./build-aux/autogen.sh
	cd genstone/vendor/c11compat/safeclib && ./configure --prefix=/usr --enable-unsafe

$(_C11_COMPAT_ANNEXK_LIB_INTERNAL): $(_C11_COMPAT_ANNEXK_LIB_INTERNAL_MAKEFILE)
	$(MAKE) -Cgenstone/vendor/c11compat/safeclib

$(C11_COMPAT_ANNEXK_LIB): $(_C11_COMPAT_ANNEXK_LIB_INTERNAL) | lib
	cp -r $(_C11_COMPAT_ANNEXK_LIB_INTERNAL_PATTERN) lib
ifeq ($(PLATFORM),DWN)
	install_name_tool -id @rpath/$(notdir $(wildcard genstone/vendor/c11compat/safeclib/src/.libs/$(LIB_PREFIX)safec.*$(DYNAMIC_LIB_SUFFIX))) $(C11_COMPAT_ANNEXK_LIB)
endif
endif
endif

c11compat: build_message_c11compat $(C11_COMPAT_LIB) 
clean_c11compat: $(C11_COMPAT_CLEAN_TARGETS)
