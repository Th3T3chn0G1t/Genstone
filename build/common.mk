CONFIG ?= $(GENSTONE_DIR)/build/config.mk
include $(CONFIG)

PLATFORM_MK ?= $(GENSTONE_DIR)/build/platform.mk
include $(PLATFORM_MK)

CLINKER = $(CLANG) -fuse-ld=lld

GLOBAL_CFLAGS += -std=gnu2x
GLOBAL_CFLAGS += -flto
GLOBAL_CFLAGS += -DGEN_DISABLED=0 -DGEN_ENABLED=1
GLOBAL_CFLAGS += -DGEN_DEBUG=0 -DGEN_RELEASE=1 -DGEN_BUILD_MODE=GEN_$(MODE)
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
	GLOBAL_CFLAGS += -fsanitize=undefined,address,cfi -fvisibility=default

	GLOBAL_LFLAGS += -fsanitize=undefined,address,cfi
endif

ifeq ($(MODE), RELEASE)
	GLOBAL_CFLAGS += -Ofast
	GLOBAL_CFLAGS += -ffast-math
endif

ACTION_PREFIX = \\033[1;30m
ACTION_SUFFIX = \\033[0m

%$(OBJECT_SUFFIX): %.c # TODO: Disabling SA as an option
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) -c $(GLOBAL_CFLAGS) $(CFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(CLANG) -c $(GLOBAL_CFLAGS) $(CFLAGS) -o $@ $<

	@$(ECHO) "$(ACTION_PREFIX)$(CLANG) $(GLOBAL_CFLAGS) $(CFLAGS) --analyze $(SAFLAGS) $<$(ACTION_SUFFIX)"
	@$(CLANG) $(GLOBAL_CFLAGS) $(CFLAGS) --analyze $(SAFLAGS) $<

	@$(ECHO) "$(ACTION_PREFIX)$(CLANG_FORMAT) -i $<$(ACTION_SUFFIX)"
	@$(CD) $(GENSTONE_DIR)/genstone $(AND) $(CLANG_FORMAT) -i $(realpath $<)

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

MODULES = $(wildcard $(GENSTONE_DIR)/genstone/*.mk)
MODULE_NAMES = $(subst $(GENSTONE_DIR)/genstone/,,$(subst .mk,,$(MODULES)))
CLEAN_TARGETS = $(addprefix clean_,$(MODULE_NAMES)) clean_common
TEST_TARGETS = $(addprefix test_,$(MODULE_NAMES))

include $(MODULES)
