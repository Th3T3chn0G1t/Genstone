CONFIG ?= $(GENSTONE_DIR)/build/config.mk
include $(CONFIG)

PLATFORM_MK ?= $(GENSTONE_DIR)/build/platform.mk
include $(PLATFORM_MK)

CLINKER = $(CLANGXX) -fuse-ld=lld

GLOBAL_CXXFLAGS += -std=c++20
GLOBAL_CXXFLAGS += $(EXTRA_CXXFLAGS)

GLOBAL_CEXCL_FLAGS = -std=gnu2x

GENSTONE_DIAGNOSTIC_CFLAGS = -Werror -Weverything
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-poison-system-directories -Wno-declaration-after-statement
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-padded -Wno-c++98-compat -Wno-pointer-arith -Wno-cast-align
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-overlength-strings -Wno-gnu-conditional-omitted-operand
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-gnu-zero-variadic-macro-arguments -Wno-gnu-auto-type
GENSTONE_DIAGNOSTIC_CFLAGS += -Wno-gnu-statement-expression

GLOBAL_CFLAGS += -flto -fvisibility=default
GLOBAL_CFLAGS += -DGEN_DISABLED=0 -DGEN_ENABLED=1
GLOBAL_CFLAGS += -DGEN_DEBUG=0 -DGEN_RELEASE=1 -DGEN_BUILD_MODE=GEN_$(MODE) -DGENSTONE_DIR=\"$(GENSTONE_DIR)\"
GLOBAL_CFLAGS += -DGEN_LINUX=0 -DGEN_OSX=1 -DGEN_WINDOWS=2 -DGEN_PLATFORM=GEN_$(PLATFORM)
GLOBAL_CFLAGS += $(EXTRA_CFLAGS)

GLOBAL_LFLAGS += -flto
GLOBAL_LFLAGS += $(EXTRA_LFLAGS)

SAFLAGS = -D__ANALYZER -Xanalyzer -analyzer-output=text
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
endif

ifeq ($(MODE), RELEASE)
	GLOBAL_CFLAGS += -Ofast
	GLOBAL_CFLAGS += -ffast-math
endif

ACTION_PREFIX = \\033[1;30m
ACTION_SUFFIX = \\033[0m

%$(OBJECT_SUFFIX): %.cpp
ifeq ($(SANITIZE),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(CLANGXX) -c $(GLOBAL_CFLAGS) -fsanitize=$(SANITIZERS) $(CFLAGS) $(GLOBAL_CXXFLAGS) $(CXXFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(CLANGXX) -c $(GLOBAL_CFLAGS) -fsanitize=$(SANITIZERS) $(CFLAGS) $(GLOBAL_CXXFLAGS) $(CXXFLAGS) -o $@ $<
else
	@$(ECHO) "$(ACTION_PREFIX)$(CLANGXX) -c $(GLOBAL_CFLAGS) $(CFLAGS) $(GLOBAL_CXXFLAGS) $(CXXFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(CLANGXX) -c $(GLOBAL_CFLAGS) $(CFLAGS) $(GLOBAL_CXXFLAGS) $(CXXFLAGS) -o $@ $<
endif

ifeq ($(STATIC_ANALYSIS),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(CLANGXX) $(GLOBAL_CFLAGS) $(CFLAGS) $(GLOBAL_CXXFLAGS) $(CXXFLAGS) --analyze $(SAFLAGS) $<$(ACTION_SUFFIX)"
	@$(CLANGXX) $(GLOBAL_CFLAGS) $(CFLAGS) $(GLOBAL_CXXFLAGS) $(CXXFLAGS) --analyze $(SAFLAGS) $<
endif

%$(OBJECT_SUFFIX): %.c
ifeq ($(SANITIZE),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) -std=c2x -c $(GLOBAL_CFLAGS) -fsanitize=$(SANITIZERS) $(CFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(CLANG) -std=c2x -c $(GLOBAL_CFLAGS) -fsanitize=$(SANITIZERS) $(CFLAGS) -o $@ $<
else
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) -std=c2x -c $(GLOBAL_CFLAGS) $(CFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(CLANG) -std=c2x -c $(GLOBAL_CFLAGS) $(CFLAGS) -o $@ $<
endif

ifeq ($(STATIC_ANALYSIS),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) -std=c2x $(GLOBAL_CFLAGS) $(CFLAGS) --analyze $(SAFLAGS) $<$(ACTION_SUFFIX)"
	@$(CLANG) -std=c2x $(GLOBAL_CFLAGS) $(CFLAGS) --analyze $(SAFLAGS) $<
endif

%$(OBJECT_SUFFIX): %.m
ifeq ($(SANITIZE),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) -c $(GLOBAL_CFLAGS) -fsanitize=$(SANITIZERS) $(CFLAGS) $(OBJCFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(CLANG) -c $(GLOBAL_CFLAGS) $(CFLAGS) -fsanitize=$(SANITIZERS) $(OBJCFLAGS) -o $@ $<
else
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) -c $(GLOBAL_CFLAGS) $(CFLAGS) $(OBJCFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(CLANG) -c $(GLOBAL_CFLAGS) $(CFLAGS) $(OBJCFLAGS) -o $@ $<
endif

ifeq ($(STATIC_ANALYSIS),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) $(GLOBAL_CFLAGS) $(CFLAGS) $(OBJCFLAGS) --analyze $(SAFLAGS) $<$(ACTION_SUFFIX)"
	@$(CLANG) $(GLOBAL_CFLAGS) $(CFLAGS) $(OBJCFLAGS) --analyze $(SAFLAGS) $<
endif

%$(STATIC_LIB_SUFFIX):
	@$(ECHO) "$(ACTION_PREFIX)$(STATIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(STATIC_LIB_TOOL)

%$(DYNAMIC_LIB_SUFFIX):
	@$(ECHO) "$(ACTION_PREFIX)$(DYNAMIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(DYNAMIC_LIB_TOOL)

%$(EXECUTABLE_SUFFIX):
	@$(ECHO) "$(ACTION_PREFIX)$(EXECUTABLE_TOOL)$(ACTION_SUFFIX)"
	@$(EXECUTABLE_TOOL)

.PHONY: clean_common
clean_common:
	@$(ECHO) "$(ACTION_PREFIX)"
	-$(RMDIR) $(GENSTONE_DIR)/lib
	@$(ECHO) "$(ACTION_SUFFIX)"

$(GENSTONE_DIR)/lib:
	@$(ECHO) "$(ACTION_PREFIX)$(MKDIR) $@$(ACTION_SUFFIX)"
	-@$(MKDIR) $@
