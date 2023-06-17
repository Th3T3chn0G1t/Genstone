CONFIG ?= $(GENSTONE_DIR)/build/config.mk
include $(CONFIG)

include $(GENSTONE_DIR)/build/platform.mk

CLINKER = $(CLANG) -fuse-ld=lld
ANALYZER = $(CLANG) --analyze

GENSTONE_DIAGNOSTIC_CFLAGS = -Werror -Weverything -pedantic -pedantic-errors
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-padded -Wno-deprecated-non-prototype
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-strict-prototypes
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-gnu-zero-variadic-macro-arguments
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-declaration-after-statement
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-language-extension-token -Wno-c++98-compat
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-missing-prototypes
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-used-but-marked-unused

GLOBAL_CFLAGS = -std=gnu17 -flto $(EXTRA_CFLAGS)
GLOBAL_LFLAGS = -flto $(EXTRA_LFLAGS)

ifneq ($(SANITIZERS),)
	GLOBAL_CFLAGS += -fsanitize=$(SANITIZERS)
	GLOBAL_LFLAGS += -fsanitize=$(SANITIZERS)
endif

SAFLAGS = -Xanalyzer -analyzer-output=text
CHECKERS = core deadcode optin security unix osx
CHECKERS += alpha.core alpha.deadcode alpha.unix alpha.osx alpha.clone
CHECKERS += alpha.security alpha.security.cert alpha.security.cert.pos
CHECKERS += alpha.security.cert.env alpha.security.taint
SAFLAGS += $(addprefix -Xanalyzer -analyzer-checker=,$(CHECKERS))

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
