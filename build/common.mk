CONFIG ?= $(GENSTONE_DIR)/build/config.mk
include $(CONFIG)

include $(GENSTONE_DIR)/build/platform.mk

CLINKER = $(CLANG) -fuse-ld=lld
ANALYZER = $(CLANG) --analyze

GENSTONE_DIAGNOSTIC_CFLAGS = -Werror -Weverything -pedantic -pedantic-errors
DISABLED_DIAGNOSTICS = padded deprecated-non-prototype strict-prototypes \
						poison-system-directories unknown-warning-option \
						gnu-zero-variadic-macro-arguments \
						declaration-after-statement language-extension-token \
						c++98-compat missing-prototypes \
						missing-variable-declarations used-but-marked-unused \
						unsafe-buffer-usage
GENSTONE_DIAGNOSTIC_CFLAGS += $(addprefix -Wno-,$(DISABLED_DIAGNOSTICS))

GLOBAL_CFLAGS += -std=gnu17 -flto $(EXTRA_CFLAGS)
GLOBAL_LFLAGS += -flto $(EXTRA_LFLAGS)

ifneq ($(SANITIZERS),)
	GLOBAL_CFLAGS += -fsanitize=$(SANITIZERS)
	GLOBAL_LFLAGS += -fsanitize=$(SANITIZERS)
endif

SAFLAGS = -Xanalyzer -analyzer-output=text
CHECKERS = core deadcode optin security unix osx valist
CHECKERS += alpha.deadcode alpha.unix alpha.osx alpha.clone
CHECKERS += alpha.security alpha.security.cert alpha.security.cert.pos
CHECKERS += alpha.security.cert.env alpha.security.taint
CHECKERS += alpha.core
SAFLAGS += $(addprefix -Xanalyzer -analyzer-checker=,$(CHECKERS))
SAFLAGS += -Xanalyzer -analyzer-disable-checker=alpha.core.CastToStruct

ifeq ($(MODE), DEBUG)
	GLOBAL_CFLAGS += -glldb -O0
	GLOBAL_CFLAGS += -fstandalone-debug -fno-eliminate-unused-debug-types
	GLOBAL_CFLAGS += -fdebug-macro -fno-omit-frame-pointer
endif

ifeq ($(MODE), RELEASE)
	GLOBAL_CFLAGS += -Ofast -ffast-math
endif

%$(OBJECT_SUFFIX): %.c
	$(CLANG) -c $(GLOBAL_CFLAGS) $(CFLAGS) -o $@ $<
ifeq ($(STATIC_ANALYSIS),ENABLED)
	$(ANALYZER) $(GLOBAL_CFLAGS) $(CFLAGS) $(SAFLAGS) $<
endif

%$(STATIC_LIB_SUFFIX):
	$(STATIC_LIB_TOOL)

%$(DYNAMIC_LIB_SUFFIX):
	$(DYNAMIC_LIB_TOOL)

%$(EXECUTABLE_SUFFIX):
	$(EXECUTABLE_TOOL)

.PHONY: clean_common
clean_common:
	-$(RMDIR) $(GENSTONE_DIR)/lib

$(GENSTONE_DIR)/lib:
	-$(MKDIR) $@
