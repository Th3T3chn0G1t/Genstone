ifneq ($(OVERRIDE_CONFIG),)
include $(OVERRIDE_CONFIG)
else
include build/config.mk
endif

ifneq ($(OVERRIDE_TOOLCHAIN),)
TOOLCHAIN = $(OVERRIDE_TOOLCHAIN)
endif

include $(TOOLCHAIN)

ifneq ($(OVERRIDE_BORING_OUTPUT),)
BORING_OUTPUT = $(OVERRIDE_BORING_OUTPUT)
endif

ifeq ($(BORING_OUTPUT),ENABLED)
ERROR_PREFIX = Error:
INFO_PREFIX = Info:
NOTE_PREFIX = Note:
SECTION_PREFIX = Section:

ACTION_PREFIX =
ACTION_SUFFIX =

TARGET_PREFIX =
TARGET_SUFFIX =

NOTABLE_PREFIX =
NOTABLE_SUFFIX =
else
ERROR_PREFIX = \\033[0;31m\\033[1mError:\\033[0m
INFO_PREFIX = \\033[0;34m\\033[1mInfo:\\033[0m
NOTE_PREFIX = \\033[0;36m\\033[1mNote:\\033[0m
SECTION_PREFIX = \\033[1;32m\\033[1mSection:\\033[0m

ACTION_PREFIX = \\033[1;30m
ACTION_SUFFIX = \\033[0m

TARGET_PREFIX = \\033[1;32m\\033[1m
TARGET_SUFFIX = \\033[0m

NOTABLE_PREFIX = \\033[0;33m\\033[1m
NOTABLE_SUFFIX = \\033[0m
endif

ifneq ($(OVERRIDE_PLATFORM),)
PLATFORM = $(OVERRIDE_PLATFORM)
endif
ifneq ($(OVERRIDE_BUILD_MODE),)
BUILD_MODE = $(OVERRIDE_BUILD_MODE)
endif
ifneq ($(OVERRIDE_SANDBOX_PROJECT_MODULE),)
SANDBOX_PROJECT_MODULE = $(OVERRIDE_SANDBOX_PROJECT_MODULE)
endif
ifneq ($(OVERRIDE_ADDITIONAL_BUILD_MODULES),)
ADDITIONAL_BUILD_MODULES = $(OVERRIDE_ADDITIONAL_BUILD_MODULES)
endif
ifneq ($(OVERRIDE_TEST),)
TEST = $(OVERRIDE_TEST)
endif
ifneq ($(OVERRIDE_GLOBAL_C_FLAGS),)
GLOBAL_C_FLAGS = $(OVERRIDE_GLOBAL_C_FLAGS)
endif
ifneq ($(OVERRIDE_GLOBAL_CXX_FLAGS),)
GLOBAL_C_FLAGS = $(OVERRIDE_GLOBAL_CXX_FLAGS)
endif
ifneq ($(OVERRIDE_GLOBAL_L_FLAGS),)
GLOBAL_L_FLAGS = $(OVERRIDE_GLOBAL_L_FLAGS)
endif
ifneq ($(OVERRIDE_COMPILER),)
COMPILER = $(OVERRIDE_COMPILER)
endif
ifneq ($(OVERRIDE_COMPILERXX),)
COMPILERXX = $(OVERRIDE_COMPILERXX)
endif
ifneq ($(OVERRIDE_LINKER),)
LINKER = $(OVERRIDE_LINKER)
endif
ifneq ($(OVERRIDE_AR),)
AR = $(OVERRIDE_AR)
endif
ifneq ($(OVERRIDE_CLANG_FORMAT),)
CLANG_FORMAT = $(OVERRIDE_CLANG_FORMAT)
endif
ifneq ($(OVERRIDE_IDE),)
IDE = $(OVERRIDE_IDE)
endif
ifneq ($(OVERRIDE_BUILD_SYS_DEBUG),)
BUILD_SYS_DEBUG = $(OVERRIDE_BUILD_SYS_DEBUG)
endif
ifneq ($(OVERRIDE_AUTO_APPLY_FORMAT),)
AUTO_APPLY_FORMAT = $(OVERRIDE_AUTO_APPLY_FORMAT)
endif
ifneq ($(OVERRIDE_STATIC_ANALYSIS),)
STATIC_ANALYSIS = $(OVERRIDE_STATIC_ANALYSIS)
endif
ifneq ($(OVERRIDE_TOOLING),)
TOOLING = $(OVERRIDE_TOOLING)
endif
ifneq ($(OVERRIDE_STRIP_BINARIES),)
STRIP_BINARIES = $(OVERRIDE_STRIP_BINARIES)
endif
ifneq ($(OVERRIDE_DISABLED_MODULES),)
DISABLED_MODULES = $(OVERRIDE_DISABLED_MODULES)
endif

ifneq ($(PLATFORM),DEFAULT)
ifneq ($(PLATFORM),DWN)
ifneq ($(PLATFORM),LNX)
ERROR += "$(ERROR_PREFIX) Invalid value for PLATFORM: \"$(PLATFORM)\"\n"
endif
endif
endif

ifneq ($(BUILD_MODE),DEBUG)
ifneq ($(BUILD_MODE),RELEASE)
ERROR += "$(ERROR_PREFIX) Invalid value for BUILD_MODE: \"$(BUILD_MODE)\"\n"
endif
endif

ifeq ($(wildcard $(SANDBOX_PROJECT_MODULE)),)
ERROR += "$(ERROR_PREFIX) Invalid value for SANDBOX_PROJECT_MODULE: \"$(SANDBOX_PROJECT_MODULE)\" - File not found\n"
endif

ifneq ($(ADDITIONAL_BUILD_MODULES),)
ifeq ($(wildcard $(ADDITIONAL_BUILD_MODULES)),)
ERROR += "$(ERROR_PREFIX) Invalid value for ADDITIONAL_BUILD_MODULES: \"$(ADDITIONAL_BUILD_MODULES)\" - File not found\n"
endif
endif

ifneq ($(TEST),BUILD)
ifneq ($(TEST),UNITS)
ifneq ($(TEST),ALL)
ifneq ($(TEST),DISABLED)
ERROR += "$(ERROR_PREFIX) Invalid value for TEST: \"$(TEST)\"\n"
endif
endif
endif
endif

ifneq ($(BUILD_SYS_DEBUG),ENABLED)
ifneq ($(BUILD_SYS_DEBUG),DISABLED)
ERROR += "$(ERROR_PREFIX) Invalid value for BUILD_SYS_DEBUG: \"$(BUILD_SYS_DEBUG)\"\n"
endif
endif

ifneq ($(AUTO_APPLY_FORMAT),ENABLED)
ifneq ($(AUTO_APPLY_FORMAT),DISABLED)
ERROR += "$(ERROR_PREFIX) Invalid value for AUTO_APPLY_FORMAT: \"$(AUTO_APPLY_FORMAT)\"\n"
endif
endif

ifneq ($(STATIC_ANALYSIS),ENABLED)
ifneq ($(STATIC_ANALYSIS),DISABLED)
ERROR += "$(ERROR_PREFIX) Invalid value for STATIC_ANALYSIS: \"$(STATIC_ANALYSIS)\"\n"
endif
endif

ifneq ($(TOOLING),ENABLED)
ifneq ($(TOOLING),DISABLED)
ERROR += "$(ERROR_PREFIX) Invalid value for TOOLING: \"$(TOOLING)\"\n"
endif
endif


ifneq ($(STRIP_BINARIES),ENABLED)
ifneq ($(STRIP_BINARIES),DISABLED)
ifneq ($(STRIP_BINARIES),DEBUG)
ERROR += "$(ERROR_PREFIX) Invalid value for STRIP_BINARIES: \"$(STRIP_BINARIES)\"\n"
endif
endif
endif


ifeq ($(PLATFORM),DEFAULT)
	UNAME = $(shell uname -s)
	ifeq ($(UNAME),Linux)
		PLATFORM = LNX
	endif
	ifeq ($(UNAME),Darwin)
		PLATFORM = DWN
	endif
endif

ifeq ($(LINKER),DEFAULT)
	CLINKER := $(COMPILERXX)
else
	ifeq ($(LINKER),LLD)
		ifneq ($(PLATFORM),DWN)
			CLINKER := $(COMPILERXX) -fuse-ld=lld
		else
			CLINKER := $(COMPILERXX)
		endif
	else
		CLINKER := $(COMPILERXX) -fuse-ld=$(realpath $(LINKER))
	endif
endif

ECHO = echo -e

CXX_UNSUPPORTED_CFLAGS += -std=gnu2x
GLOBAL_CXX_FLAGS += -std=gnu++17
GLOBAL_C_FLAGS += -fcomment-block-commands=example -fmacro-backtrace-limit=0 -Wthread-safety -D__STDC_WANT_LIB_EXT1__=1 -std=gnu2x -DDEBUG=1 -DRELEASE=0 -DMODE=$(BUILD_MODE) -DENABLED=1 -DDISABLED=0 -DDWN=2 -DLNX=3 -DPLATFORM=$(PLATFORM)

ifeq ($(STRIP_BINARIES),ENABLED)
GLOBAL_L_FLAGS += -Wl,-s
endif
ifeq ($(STRIP_BINARIES),DEBUG)
GLOBAL_L_FLAGS += -Wl,-S
endif

DEPENDENCY_GEN_FLAGS = -MM -MF$(subst .o,.depfile,$@) 

CLANG_STATIC_ANALYZER_FLAGS = -Xanalyzer -analyzer-output=text

CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=core -Xanalyzer -analyzer-checker=deadcode
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=optin -Xanalyzer -analyzer-checker=security
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=unix
ifeq ($(PLATFORM),DWN)
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=osx
endif

CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=alpha.core # This one kinda sucks -Xanalyzer -analyzer-checker=alpha.clone
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=alpha.deadcode -Xanalyzer -analyzer-checker=alpha.security
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=alpha.unix

ifeq ($(PLATFORM),DWN)
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=osx
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=alpha.osx
endif

ifeq ($(BUILD_SYS_DEBUG),ENABLED)
	ifeq ($(PLATFORM),DWN)
		GLOBAL_L_FLAGS += -Wl,-why_load,-print_statistics
	endif
	GLOBAL_C_FLAGS += -fproc-stat-report
	GLOBAL_L_FLAGS += -fproc-stat-report
endif

ifeq ($(PLATFORM),LNX)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC -D_DEFAULT_SOURCE

	OBJECT_FORMAT = ELF

	DYNAMIC_LIB_TOOL = $(CLINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) -shared $(GLOBAL_L_FLAGS) $(LFLAGS)
	STATIC_LIB_TOOL = $(AR) -r -c $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),DWN)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .dylib
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC

	OBJECT_FORMAT = MACHO

	DYNAMIC_LIB_TOOL = $(CLINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) -dynamiclib $(GLOBAL_L_FLAGS) $(LFLAGS) -install_name "@rpath/$(notdir $@)"
	STATIC_LIB_TOOL = $(AR) -r -c $@ $(filter %$(OBJECT_SUFFIX),$^)
endif

ifeq ($(BUILD_MODE),RELEASE)
	GLOBAL_C_FLAGS += -m64 -Ofast -ffast-math -DNDEBUG -flto -fopenmp -mllvm -polly -mllvm -polly-parallel -mllvm -polly-omp-backend=LLVM
	GLOBAL_L_FLAGS += -flto -fopenmp

	ifeq ($(PLATFORM),LNX)
		GLOBAL_L_FLAGS += -Wl,-O1
	endif

	ifeq ($(PLATFORM),DWN)
		GLOBAL_L_FLAGS += -Wl,-dead_strip,-no_implicit_dylibs,-warn_unused_dylibs,-dead_strip_dylibs,-interposable,-warn_stabs,-warn_commons,-debug_variant,-unaligned_pointers,warning
	endif
else
	GLOBAL_C_FLAGS += -m64 -glldb -O0 -fstandalone-debug -fno-eliminate-unused-debug-types -fdebug-macro -fno-lto
	GLOBAL_L_FLAGS += -fno-lto

	ifeq ($(PLATFORM),DWN)
		GLOBAL_L_FLAGS += -Wl,-random_uuid,-warn_stabs,-warn_commons
	endif
endif

ifeq ($(TOOLING),ENABLED)
	GLOBAL_C_FLAGS += -fsanitize=undefined,address
	GLOBAL_L_FLAGS += -fsanitize=undefined,address
endif

ifeq ($(TEST),ALL)
	TEST_UNITS = 1
	TEST_BUILD = 1
endif
ifeq ($(TEST),UNITS)
	TEST_UNITS = 1
endif
ifeq ($(TEST),BUILD)
	TEST_BUILD = 1
endif

clean_tmpfile:
	@$(ECHO) "$(ACTION_PREFIX)"
	-@rm $(wildcard tmp/*.tmp)
	@$(ECHO) "$(ACTION_SUFFIX)"

clean_clang_tooling_artifacts:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(shell find . -name "*.gcda")
	-rm $(shell find . -name "*.gcno")
	-rm $(wildcard "*.profraw")
	@$(ECHO) "$(ACTION_SUFFIX)"

tmp:
	-mkdir $@

%$(OBJECT_SUFFIX): %.c build/config.mk | tmp
	@$(ECHO) "$(ACTION_PREFIX)$(COMPILER) -c $(GLOBAL_C_FLAGS) $(CFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(COMPILER) -c $(GLOBAL_C_FLAGS) $(CFLAGS) -o $@ $<

	@$(ECHO) "$(ACTION_PREFIX)$(COMPILER) -c $(GLOBAL_C_FLAGS) $(DEPENDENCY_GEN_FLAGS) $(CFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(COMPILER) -c $(GLOBAL_C_FLAGS) $(DEPENDENCY_GEN_FLAGS) $(CFLAGS) -o $@ $<

ifeq ($(STATIC_ANALYSIS),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(COMPILER) $(GLOBAL_C_FLAGS) $(CFLAGS) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<$(ACTION_SUFFIX)"
	@$(COMPILER) $(GLOBAL_C_FLAGS) $(CFLAGS) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<
endif

	@$(ECHO) "$(ACTION_PREFIX)($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && (diff $< tmp/$(notdir $<)-format.tmp)$(ACTION_SUFFIX)"
	-@($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && (diff $< tmp/$(notdir $<)-format.tmp)

ifeq ($(AUTO_APPLY_FORMAT),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG_FORMAT) -i $<$(ACTION_SUFFIX)"
	-@$(CLANG_FORMAT) -i $<
else
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG_FORMAT) --dry-run -Werror $<$(ACTION_SUFFIX)"
	-@$(CLANG_FORMAT) --dry-run -Werror $<
endif

%$(OBJECT_SUFFIX): %.cpp build/config.mk | tmp
	@$(ECHO) "$(ACTION_PREFIX)$(COMPILERXX) -c $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(CFLAGS) $(CXXFLAGS))  -o $@ $<$(ACTION_SUFFIX)"
	@$(COMPILERXX) -c $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(CFLAGS) $(CXXFLAGS)) -o $@ $<

	@$(ECHO) "$(ACTION_PREFIX)$(COMPILERXX) -c $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(DEPENDENCY_GEN_FLAGS) $(CFLAGS) $(CXXFLAGS))  -o $@ $<$(ACTION_SUFFIX)"
	@$(COMPILERXX) -c $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(DEPENDENCY_GEN_FLAGS) $(CFLAGS) $(CXXFLAGS)) -o $@ $<

ifeq ($(STATIC_ANALYSIS),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(COMPILERXX) $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(CFLAGS) $(CXXFLAGS)) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<$(ACTION_SUFFIX)"
	@$(COMPILERXX) $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(CFLAGS) $(CXXFLAGS)) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<
endif

	@$(ECHO) "$(ACTION_PREFIX)($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && (diff $< tmp/$(notdir $<)-format.tmp)$(ACTION_SUFFIX)"
	-@($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && (diff $< tmp/$(notdir $<)-format.tmp)

ifeq ($(AUTO_APPLY_FORMAT),ENABLED)
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG_FORMAT) -i $<$(ACTION_SUFFIX)"
	-@$(CLANG_FORMAT) -i $<
else
	@$(ECHO) "$(ACTION_PREFIX)$(CLANG_FORMAT) --dry-run -Werror $<$(ACTION_SUFFIX)"
	-@$(CLANG_FORMAT) --dry-run -Werror $<
endif

%$(STATIC_LIB_SUFFIX):
	@$(ECHO) "$(ACTION_PREFIX)$(STATIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(STATIC_LIB_TOOL)

%$(DYNAMIC_LIB_SUFFIX):
	@$(ECHO) "$(ACTION_PREFIX)$(DYNAMIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(DYNAMIC_LIB_TOOL)

%$(EXECUTABLE_SUFFIX):
	@$(ECHO) "$(ACTION_PREFIX)$(CLINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) $(GLOBAL_L_FLAGS) -fPIE $(LFLAGS)$(ACTION_SUFFIX)"
	@$(CLINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) $(GLOBAL_L_FLAGS) -fPIE $(LFLAGS)
