include build/config.mk

# We want make to use cmd.exe when the *host* is Windows
ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
HAVE_FIND = DISABLED
else
HAVE_FIND = ENABLED
endif

SEP = /
CP = cp -r
RM = rm
RMDIR = rm -rf
DIFF = diff
CAT = cat
WHICH = which -s
ifeq ($(SHELL),cmd.exe)
	SEP = \\
	CP = copy /b /y
	RM = del
	RMDIR = rmdir
	DIFF = fc
	CAT = type
	WHICH = where /q
endif

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
ifneq ($(OVERRIDE_GLOBAL_L_FLAGS),)
GLOBAL_L_FLAGS = $(OVERRIDE_GLOBAL_L_FLAGS)
endif
ifneq ($(OVERRIDE_COMPILER),)
COMPILER = $(OVERRIDE_COMPILER)
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
ifneq ($(OVERRIDE_HAVE_STAT_REPORT),)
HAVE_STAT_REPORT = $(OVERRIDE_HAVE_STAT_REPORT)
endif
ifneq ($(OVERRIDE_TOOLING),)
TOOLING = $(OVERRIDE_TOOLING)
endif
ifneq ($(OVERRIDE_STRIP_BINARIES),)
STRIP_BINARIES = $(OVERRIDE_STRIP_BINARIES)
endif
ifneq ($(OVERIDE_STRIP_TOOL),)
STRIP_TOOL = $(OVERIDE_STRIP_TOOL)
endif

ifneq ($(PLATFORM),DEFAULT)
ifneq ($(PLATFORM),WIN)
ifneq ($(PLATFORM),DWN)
ifneq ($(PLATFORM),LNX)
ifneq ($(PLATFORM),BSD)
ifneq ($(PLATFORM),WEB)
ERROR += "$(ERROR_PREFIX) Invalid value for PLATFORM: \"$(PLATFORM)\"\n"
endif
endif
endif
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

EXIT_CODE := $(shell $(WHICH) $(COMPILER); echo $$?)
ifneq ($(EXIT_CODE),0)
EXIT_CODE := $(shell $(WHICH) $(realpath $(COMPILER)); echo $$?)
ifneq ($(EXIT_CODE),0)
ERROR += "$(ERROR_PREFIX) Invalid value for COMPILER: \"$(COMPILER)\" - File not found\n"
endif
endif

ifneq ($(LINKER),DEFAULT)
EXIT_CODE := $(shell $(WHICH) $(LINKER); echo $$?)
ifneq ($(EXIT_CODE),0)
EXIT_CODE := $(shell $(WHICH) $(realpath $(LINKER)); echo $$?)
ifneq ($(EXIT_CODE),0)
ERROR += "$(ERROR_PREFIX) Invalid value for LINKER: \"$(LINKER)\" - File not found\n"
endif
endif
endif

EXIT_CODE := $(shell $(WHICH) $(AR); echo $$?)
ifneq ($(EXIT_CODE),0)
EXIT_CODE := $(shell $(WHICH) $(realpath $(AR)); echo $$?)
ifneq ($(EXIT_CODE),0)
ERROR += "$(ERROR_PREFIX) Invalid value for AR: \"$(AR)\" - File not found\n"
endif
endif

EXIT_CODE := $(shell $(WHICH) $(CLANG_FORMAT); echo $$?)
ifneq ($(EXIT_CODE),0)
EXIT_CODE := $(shell $(WHICH) $(realpath $(CLANG_FORMAT)); echo $$?)
ifneq ($(EXIT_CODE),0)
ERROR += "$(ERROR_PREFIX) Invalid value for CLANG_FORMAT: \"$(CLANG_FORMAT)\" - File not found\n"
endif
endif

EXIT_CODE := $(shell $(WHICH) $(STRIP_TOOL); echo $$?)
ifneq ($(EXIT_CODE),0)
EXIT_CODE := $(shell $(WHICH) $(realpath $(STRIP_TOOL)); echo $$?)
ifneq ($(EXIT_CODE),0)
ERROR += "$(ERROR_PREFIX) Invalid value for STRIP_TOOL: \"$(STRIP_TOOL)\" - File not found\n"
endif
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

ifneq ($(HAVE_STAT_REPORT),ENABLED)
ifneq ($(HAVE_STAT_REPORT),DISABLED)
ERROR += "$(ERROR_PREFIX) Invalid value for HAVE_STAT_REPORT: \"$(HAVE_STAT_REPORT)\"\n"
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
	ifeq ($(OS),Windows_NT)
		PLATFORM = WIN
	else
		UNAME = $(shell uname -s)
		ifeq ($(UNAME),Linux)
			PLATFORM = LNX
		endif
		ifeq ($(UNAME),Darwin)
			PLATFORM = DWN
		endif
		ifeq ($(UNAME),FreeBSD)
			PLATFORM = BSD
		endif
		ifeq ($(UNAME),OpenBSD)
			PLATFORM = BSD
		endif
		ifeq ($(UNAME),NetBSD)
			PLATFORM = BSD
		endif
    endif
endif

ifeq ($(LINKER),DEFAULT)
CLINKER := $(COMPILER)
else
CLINKER := $(COMPILER) -fuse-ld=$(realpath $(LINKER))
endif

GLOBAL_C_FLAGS += -Wthread-safety -D__STDC_WANT_LIB_EXT1__=1 -std=c2x -DDEBUG=1 -DRELEASE=0 -DMODE=$(BUILD_MODE) -DENABLED=1 -DDISABLED=0 -DWIN=1 -DDWN=2 -DLNX=3 -DBSD=4 -DWEB=5 -DPLATFORM=$(PLATFORM)
GLOBAL_CMAKE_MODULE_FLAGS = -G "Unix Makefiles"

CLANG_STATIC_ANALYZER_FLAGS = -Xanalyzer -analyzer-output=text

CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=core -Xanalyzer -analyzer-checker=deadcode
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=optin -Xanalyzer -analyzer-checker=security
ifneq ($(PLATFORM),WIN)
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=unix
endif
ifeq ($(PLATFORM),DWN)
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=osx
endif

CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=alpha.clone -Xanalyzer -analyzer-checker=alpha.core
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=alpha.deadcode -Xanalyzer -analyzer-checker=alpha.security
ifneq ($(PLATFORM),WIN)
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=alpha.unix
endif
ifeq ($(PLATFORM),DWN)
CLANG_STATIC_ANALYZER_FLAGS += -Xanalyzer -analyzer-checker=alpha.osx
endif

# Need to check clang version for this to work (12.0.1?)
ifeq ($(BUILD_SYS_DEBUG),ENABLED)
	ifeq ($(PLATFORM),DWN)
		GLOBAL_L_FLAGS += -Wl,-why_load,-print_statistics
	endif
	ifeq ($(HAVE_STAT_REPORT),ENABLED)
		GLOBAL_C_FLAGS += -fproc-stat-report
		GLOBAL_L_FLAGS += -fproc-stat-report
	endif
endif

ifeq ($(PLATFORM),WIN)
	LIB_PREFIX =
	DYNAMIC_LIB_SUFFIX = .dll
	STATIC_LIB_SUFFIX = .lib
	EXECUTABLE_SUFFIX = .exe
	OBJECT_SUFFIX = .obj

	# Presume windows targets are x86 for now
	GLOBAL_C_FLAGS += -D_MT -mavx512-fp16
	GLOBAL_L_FLAGS += -lshlwapi.lib

	OBJECT_FORMAT = PE

	DYNAMIC_LIB_TOOL = $(CLINKER) -shared $(GLOBAL_L_FLAGS) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^) && script/winimplibgen.bat $@
	STATIC_LIB_TOOL = $(AR) -rv $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),LNX)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC -D_DEFAULT_SOURCE
	GLOBAL_L_FLAGS += -Wl,-O1

	OBJECT_FORMAT = ELF

	DYNAMIC_LIB_TOOL = $(CLINKER) -shared $(GLOBAL_L_FLAGS) $(LFLAGS)-o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = $(AR) -rv $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),DWN)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .dylib
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC

	OBJECT_FORMAT = MACHO

	DYNAMIC_LIB_TOOL = $(CLINKER) -dynamiclib $(GLOBAL_L_FLAGS) $(LFLAGS) -install_name "@rpath/$(notdir $@)" -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = $(AR) -rv $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),BSD)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC
	GLOBAL_L_FLAGS += -Wl,-O1

	OBJECT_FORMAT = ELF

	DYNAMIC_LIB_TOOL = $(CLINKER) -shared $(GLOBAL_L_FLAGS) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = $(AR) -rv $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),WEB)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC
	GLOBAL_L_FLAGS += -mwasm64 -s SIDE_MODULE=1 -Wl,-mwasm64

	OBJECT_FORMAT = WASM

	DYNAMIC_LIB_TOOL = $(CLINKER) -shared $(GLOBAL_L_FLAGS) $(LFLAGS) -Wl,-mwasm64 -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = $(AR) -rv $@ $(filter %$(OBJECT_SUFFIX),$^)
endif

ifeq ($(BUILD_MODE),RELEASE)
	GLOBAL_C_FLAGS += -m64 -Ofast -ffast-math -DNDEBUG -flto
	GLOBAL_L_FLAGS += -flto
	GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_BUILD_TYPE=Release

	ifneq ($(wildcard *.profraw),)
		GLOBAL_C_FLAGS += -fprofile-instr-use=$(wildcard *.profraw)
	endif

	ifeq ($(PLATFORM),WIN)
		GLOBAL_L_FLAGS += -llibcmt.lib
		GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
	endif

	ifeq ($(PLATFORM),DWN)
		GLOBAL_L_FLAGS += -Wl,-dead_strip,-no_implicit_dylibs,-warn_unused_dylibs,-dead_strip_dylibs,-interposable,-warn_stabs,-warn_commons,-debug_variant,-unaligned_pointers,warning
	endif
else
	GLOBAL_C_FLAGS += -m64 -glldb -O0 -DDEBUG -fstandalone-debug -fno-eliminate-unused-debug-types -fdebug-macro -fno-lto
	GLOBAL_L_FLAGS += -fno-lto
	GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_BUILD_TYPE=Debug

	ifeq ($(PLATFORM),WIN)
		GLOBAL_C_FLAGS += -D_DEBUG
		GLOBAL_L_FLAGS += -Wl,-nodefaultlib:libcmt.lib -llibcmtd.lib
		GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug
	endif

	ifeq ($(PLATFORM),DWN)
		GLOBAL_L_FLAGS += -Wl,-random_uuid,-warn_stabs,-warn_commons
	endif
endif

ifeq ($(TOOLING),ENABLED)
	GLOBAL_C_FLAGS += --coverage -fprofile-instr-generate -fsanitize=undefined
	GLOBAL_L_FLAGS += --coverage -fprofile-instr-generate -fsanitize=undefined
	ifneq ($(PLATFORM),WEB)
		GLOBAL_C_FLAGS += -fsanitize=address
		GLOBAL_L_FLAGS += -fsanitize=address
	endif
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
	@echo "$(ACTION_PREFIX)$(RM) $(wildcard tmp/*.tmp)$(ACTION_SUFFIX)"
	-@$(RM) $(wildcard tmp/*.tmp)

clean_clang_tooling_artifacts:
ifeq ($(HAVE_FIND),ENABLED)
	-rm $(shell find . -name "*.gcda")
	-rm $(shell find . -name "*.gcno")
endif
	-rm $(wildcard "*.profraw")

tmp:
	-mkdir $@

%$(OBJECT_SUFFIX): %.c build/config.mk | tmp
	@echo "$(ACTION_PREFIX)$(COMPILER) -c $(GLOBAL_C_FLAGS) $(CFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(COMPILER) -c $(GLOBAL_C_FLAGS) $(CFLAGS) -o $@ $<

ifeq ($(STATIC_ANALYSIS),ENABLED)
	@echo "$(ACTION_PREFIX)$(COMPILER) $(GLOBAL_C_FLAGS) $(CFLAGS) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<$(ACTION_SUFFIX)"
	@$(COMPILER) $(GLOBAL_C_FLAGS) $(CFLAGS) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<
endif

	@echo "$(ACTION_PREFIX)($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && ($(DIFF) $< tmp/$(notdir $<)-format.tmp)$(ACTION_SUFFIX)"
	-@($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && ($(DIFF) $< tmp/$(notdir $<)-format.tmp)

ifeq ($(AUTO_APPLY_FORMAT),ENABLED)
	@echo "$(ACTION_PREFIX)$(CLANG_FORMAT) -i $<$(ACTION_SUFFIX)"
	-@$(CLANG_FORMAT) -i $<
else
	@echo "$(ACTION_PREFIX)$(CLANG_FORMAT) --dry-run -Werror $<$(ACTION_SUFFIX)"
	-@$(CLANG_FORMAT) --dry-run -Werror $<
endif

ifeq ($(STRIP_BINARIES),ENABLED)
ifeq ($(STRIP_TOOL),LINKER)
	@echo "$(ACTION_PREFIX)$(CLINKER) -Wl,-s -o $@ $@$(ACTION_SUFFIX)"
	@$(CLINKER) -Wl,-s -o $@ $@
else
	@echo "$(ACTION_PREFIX)$(STRIP_TOOL) -x -D -o $@ $@$(ACTION_SUFFIX)"
	@$(STRIP_TOOL) -x -D -o $@ $@
endif
endif
ifeq ($(STRIP_BINARIES),DEBUG)
ifeq ($(STRIP_TOOL),LINKER)
	@echo "$(ACTION_PREFIX)$(CLINKER) -Wl,-S -o $@ $@$(ACTION_SUFFIX)"
	@$(CLINKER) -Wl,-S -o $@ $@
else
	@echo "$(ACTION_PREFIX)$(STRIP_TOOL) -S -D -o $@ $@$(ACTION_SUFFIX)"
	@$(STRIP_TOOL) -S -D -o $@ $@
endif
endif

%$(STATIC_LIB_SUFFIX):
	@echo "$(ACTION_PREFIX)$(STATIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(STATIC_LIB_TOOL)

ifeq ($(STRIP_BINARIES),ENABLED)
ifeq ($(STRIP_TOOL),LINKER)
	@echo "$(ACTION_PREFIX)$(CLINKER) -Wl,-s -o $@ $@$(ACTION_SUFFIX)"
	@$(CLINKER) -Wl,-s -o $@ $@
else
	@echo "$(ACTION_PREFIX)$(STRIP_TOOL) -x -D -o $@ $@$(ACTION_SUFFIX)"
	@$(STRIP_TOOL) -x -D -o $@ $@
endif
endif
ifeq ($(STRIP_BINARIES),DEBUG)
ifeq ($(STRIP_TOOL),LINKER)
	@echo "$(ACTION_PREFIX)$(CLINKER) -Wl,-S -o $@ $@$(ACTION_SUFFIX)"
	@$(CLINKER) -Wl,-S -o $@ $@
else
	@echo "$(ACTION_PREFIX)$(STRIP_TOOL) -S -D -o $@ $@$(ACTION_SUFFIX)"
	@$(STRIP_TOOL) -S -D -o $@ $@
endif
endif


%$(DYNAMIC_LIB_SUFFIX):
	@echo "$(ACTION_PREFIX)$(DYNAMIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(DYNAMIC_LIB_TOOL)

ifeq ($(STRIP_BINARIES),ENABLED)
ifeq ($(STRIP_TOOL),LINKER)
	@echo "$(ACTION_PREFIX)$(CLINKER) -Wl,-s -o $@ $@$(ACTION_SUFFIX)"
	@$(CLINKER) -Wl,-s -o $@ $@
else
	@echo "$(ACTION_PREFIX)$(STRIP_TOOL) -x -D -o $@ $@$(ACTION_SUFFIX)"
	@$(STRIP_TOOL) -x -D -o $@ $@
endif
endif
ifeq ($(STRIP_BINARIES),DEBUG)
ifeq ($(STRIP_TOOL),LINKER)
	@echo "$(ACTION_PREFIX)$(CLINKER) -Wl,-S -o $@ $@$(ACTION_SUFFIX)"
	@$(CLINKER) -Wl,-S -o $@ $@
else
	@echo "$(ACTION_PREFIX)$(STRIP_TOOL) -S -D -o $@ $@$(ACTION_SUFFIX)"
	@$(STRIP_TOOL) -S -D -o $@ $@
endif
endif

%$(EXECUTABLE_SUFFIX):
	@echo "$(ACTION_PREFIX)$(CLINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) $(GLOBAL_L_FLAGS) -fPIE $(LFLAGS)$(ACTION_SUFFIX)"
	@$(CLINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) $(GLOBAL_L_FLAGS) -fPIE $(LFLAGS)

ifeq ($(STRIP_BINARIES),ENABLED)
ifeq ($(STRIP_TOOL),LINKER)
	@echo "$(ACTION_PREFIX)$(CLINKER) -Wl,-s -o $@ $@$(ACTION_SUFFIX)"
	@$(CLINKER) -Wl,-s -o $@ $@
else
	@echo "$(ACTION_PREFIX)$(STRIP_TOOL) -x -D -o $@ $@$(ACTION_SUFFIX)"
	@$(STRIP_TOOL) -x -D -o $@ $@
endif
endif
ifeq ($(STRIP_BINARIES),DEBUG)
ifeq ($(STRIP_TOOL),LINKER)
	@echo "$(ACTION_PREFIX)$(CLINKER) -Wl,-S -o $@ $@$(ACTION_SUFFIX)"
	@$(CLINKER) -Wl,-S -o $@ $@
else
	@echo "$(ACTION_PREFIX)$(STRIP_TOOL) -S -D -o $@ $@$(ACTION_SUFFIX)"
	@$(STRIP_TOOL) -S -D -o $@ $@
endif
endif
