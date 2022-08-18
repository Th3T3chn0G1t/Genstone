CONFIG ?= build/config.mk
include $(CONFIG)

include build/platform.mk

CLINKER = $(CLANGXX) -fuse-ld=lld

GLOBAL_CFLAGS += -std=gnu2x
GLOBAL_CFLAGS += -flto
GLOBAL_CFLAGS += -DGEN_DISABLED=0 -DGEN_ENABLED=1
GLOBAL_CFLAGS += -DGEN_DEBUG=0 -DGEN_RELEASE=1 -DGEN_BUILD_MODE=GEN_$(MODE)
GLOBAL_CFLAGS += -DGEN_LINUX=0 -DGEN_OSX=1 -DGEN_WINDOWS=2 -DGEN_PLATFORM=GEN_$(PLATFORM)
GLOBAL_CFLAGS += $(EXTRA_CFLAGS)

GLOBAL_LFLAGS += -flto
GLOBAL_LFLAGS += $(EXTRA_LFLAGS)

SAFLAGS = -Xanalyzer -analyzer-output=text
SAFLAGS += -Xanalyzer -analyzer-checker=core -Xanalyzer -analyzer-checker=deadcode
SAFLAGS += -Xanalyzer -analyzer-checker=optin
SAFLAGS += -Xanalyzer -analyzer-checker=unix
SAFLAGS += -Xanalyzer -analyzer-checker=alpha.core
SAFLAGS += -Xanalyzer -analyzer-checker=alpha.deadcode
SAFLAGS += -Xanalyzer -analyzer-checker=alpha.unix

ifeq ($(PLATFORM),DWN)
	SAFLAGS += -Xanalyzer -analyzer-checker=osx
	SAFLAGS += -Xanalyzer -analyzer-checker=alpha.osx
endif

ifeq ($(MODE), DEBUG)
	GLOBAL_CFLAGS += -glldb -O0
	GLOBAL_CFLAGS += -fstandalone-debug -fno-eliminate-unused-debug-types -fdebug-macro -fno-omit-frame-pointer
	GLOBAL_CFLAGS += -fsanitize=undefined,address,cfi -fvisibility=default

	GLOBAL_LFLAGS += -fsanitize=undefined,address,cfi
endif

ifeq ($(MODE), RELEASE)
	GLOBAL_CFLAGS += -Ofast
	GLOBAL_CFLAGS += -ffast-math
endif

ACTION_PREFIX = \\033[1;30m
ACTION_SUFFIX = \\033[0m

%$(OBJECT_SUFFIX): %.c
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) -c $(GLOBAL_CFLAGS) $(CFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(CLANG) -c $(GLOBAL_CFLAGS) $(CFLAGS) -o $@ $<

	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) --analyze $(GLOBAL_CFLAGS) $(CFLAGS) $(SAFLAGS) $<$(ACTION_SUFFIX)"
	@$(CLANG) --analyze $(GLOBAL_CFLAGS) $(CFLAGS) $(SAFLAGS) $<

	@$(CD) genstone $(AND) $(CLANG_FORMAT) -i $<

%$(STATIC_LIB_SUFFIX):
	@$(ECHO) "$(ACTION_PREFIX)$(STATIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(STATIC_LIB_TOOL)

%$(DYNAMIC_LIB_SUFFIX):
	@$(ECHO) "$(ACTION_PREFIX)$(DYNAMIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(DYNAMIC_LIB_TOOL)

%$(EXECUTABLE_SUFFIX):
	@$(ECHO) "$(ACTION_PREFIX)$(EXECUTABLE_TOOL)$(ACTION_SUFFIX)"
	@$(EXECUTABLE_TOOL)
