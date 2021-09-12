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
ifeq ($(SHELL),cmd.exe)
	SEP = \\
	CP = copy /b /y
	RM = del
	RMDIR = rmdir
	DIFF = fc
	CAT = type
endif

ERROR_PREFIX = \\033[0;31m\033[1mError:\\033[0m
INFO_PREFIX = \\033[0;34m\033[1mInfo:\\033[0m
NOTE_PREFIX = \\033[0;36m\033[1mNote:\\033[0m
SECTION_PREFIX = \\033[1;32m\033[1mSection:\\033[0m

ACTION_PREFIX = \\033[1;30m
ACTION_SUFFIX = \\033[0m

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
ifneq ($(OVERRIDE_BUILD_PREREQS),)
BUILD_PREREQS = $(OVERRIDE_BUILD_PREREQS)
endif
ifneq ($(OVERRIDE_BUILD_POST),)
BUILD_POST = $(OVERRIDE_BUILD_POST)
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
ifneq ($(OVERRIDE_FASTBUILD_TOOLS),)
FASTBUILD_TOOLS = $(OVERRIDE_FASTBUILD_TOOLS)
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

ifneq ($(TEST),BUILD)
ifneq ($(TEST),UNITS)
ifneq ($(TEST),ALL)
ifneq ($(TEST),NONE)
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

ifneq ($(FASTBUILD_TOOLS),ENABLED)
ifneq ($(FASTBUILD_TOOLS),DISABLED)
ERROR += "$(ERROR_PREFIX) Invalid value for FASTBUILD_TOOLS: \"$(FASTBUILD_TOOLS)\"\n"
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
	ifeq ($(HAVE_STAT_REPORT),ENABLED)
		GLOBAL_C_FLAGS += -fproc-stat-report
		GLOBAL_L_FLAGS += -fproc-stat-report
	endif
	GLOBAL_C_FLAGS += -v
endif

ifeq ($(PLATFORM),WIN)
	LIB_PREFIX =
	DYNAMIC_LIB_SUFFIX = .dll
	STATIC_LIB_SUFFIX = .lib
	EXECUTABLE_SUFFIX = .exe
	OBJECT_SUFFIX = .obj

	GLOBAL_C_FLAGS += -D_MT
	GLOBAL_L_FLAGS += -lshlwapi.lib

	OBJECT_FORMAT = PE

	DYNAMIC_LIB_TOOL = $(LINKER) -shared -o $@ $(filter %$(OBJECT_SUFFIX),$^) && script/winimplibgen.bat $@
	STATIC_LIB_TOOL = ar -cvq $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),LNX)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC -D_DEFAULT_SOURCE

	OBJECT_FORMAT = ELF

	DYNAMIC_LIB_TOOL = $(LINKER) -shared -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = ar -cvq $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),DWN)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .dylib
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC

	OBJECT_FORMAT = MACHO

	DYNAMIC_LIB_TOOL = $(LINKER) -dynamiclib -install_name "@rpath/$(notdir $@)" -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = libtool -static -o $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),BSD)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC

	OBJECT_FORMAT = ELF

	DYNAMIC_LIB_TOOL = $(LINKER) -shared -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = ar -cvq $@ $(filter %$(OBJECT_SUFFIX),$^)
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

	DYNAMIC_LIB_TOOL = $(LINKER) -shared -Wl,-mwasm64 -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = $(LINKER) -static -Wl,-mwasm64 -o $@ $(filter %$(OBJECT_SUFFIX),$^)
endif

ifeq ($(BUILD_MODE),RELEASE)
	GLOBAL_C_FLAGS += -m64 -fopenmp -Ofast -ffast-math -DNDEBUG -flto
	ifneq ($(wildcard *.profraw),)
		GLOBAL_C_FLAGS += -fprofile-instr-use=$(wildcard *.profraw)
	endif
	GLOBAL_L_FLAGS += -flto
	GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_BUILD_TYPE=Release

	ifeq ($(PLATFORM),WIN)
		GLOBAL_L_FLAGS += -llibcmt.lib
		GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
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
endif

ifeq ($(TOOLING),ENABLED)
	GLOBAL_C_FLAGS += --coverage -fprofile-instr-generate -fsanitize=undefined
	GLOBAL_L_FLAGS += -fprofile-instr-generate -fsanitize=undefined
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

ifeq ($(FASTBUILD_TOOLS),ENABLED)
	ifeq ($(OBJECT_FORMAT),ELF)
		include build/vendor/mold.mk
		BUILD_PREREQS += mold
		COMPILER = $(COMPILER) --ld-path=$(realpath build/vendor/mold/mold)
	endif
endif

clean_tmpfile:
	@echo "$(ACTION_PREFIX)$(RM) $(wildcard tmp/*.tmp)$(ACTION_SUFFIX)"
	-@$(RM) $(wildcard tmp/*.tmp)

clean_clang_tooling_artifacts:
ifeq ($(HAVE_FIND),ENABLED)
	-rm $(shell find . -name "*.gcda")
	-rm $(shell find . -name "*.gcno")
endif

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

%$(STATIC_LIB_SUFFIX):
	@echo "$(ACTION_PREFIX)$(STATIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(STATIC_LIB_TOOL)

%$(DYNAMIC_LIB_SUFFIX):
	@echo "$(ACTION_PREFIX)$(DYNAMIC_LIB_TOOL) $(GLOBAL_L_FLAGS) $(LFLAGS)$(ACTION_SUFFIX)"
	@$(DYNAMIC_LIB_TOOL) $(GLOBAL_L_FLAGS) $(LFLAGS)

%$(EXECUTABLE_SUFFIX):
	@echo "$(ACTION_PREFIX)$(LINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) $(GLOBAL_L_FLAGS) -fPIE $(LFLAGS)$(ACTION_SUFFIX)"
	@$(LINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) $(GLOBAL_L_FLAGS) -fPIE $(LFLAGS)
