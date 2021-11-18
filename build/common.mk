ifneq ($(OVERRIDE_CONFIG),)
include $(OVERRIDE_CONFIG)
else
include build/config.mk
endif

ifneq ($(OVERRIDE_TOOLCHAIN),)
TOOLCHAIN = $(OVERRIDE_TOOLCHAIN)
endif

include $(TOOLCHAIN)

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
MV = mv
ifeq ($(SHELL),cmd.exe)
	SEP = \\
	CP = copy /b /y
	RM = del
	RMDIR = rmdir
	DIFF = fc
	CAT = type
	MV = move
endif

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
ifneq ($(OVERIDE_STRIP_TOOL),)
STRIP_TOOL = $(OVERIDE_STRIP_TOOL)
endif
ifneq ($(OVERRIDE_DISABLED_MODULES),)
DISABLED_MODULES = $(OVERRIDE_DISABLED_MODULES)
endif
ifneq ($(OVERRIDE_CMAKE),)
CMAKE = $(OVERRIDE_CMAKE)
endif

ifneq ($(PLATFORM),DEFAULT)
ifneq ($(PLATFORM),WIN)
ifneq ($(PLATFORM),DWN)
ifneq ($(PLATFORM),LNX)
ifneq ($(PLATFORM),BSD)
ERROR += "$(ERROR_PREFIX) Invalid value for PLATFORM: \"$(PLATFORM)\"\n"
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

CXX_UNSUPPORTED_CFLAGS += -std=c20
GLOBAL_CXX_FLAGS += -std=c++20 -Wno-c++98-compat-pedantic -Wno-old-style-cast
GLOBAL_C_FLAGS += -fmacro-backtrace-limit=0 -Wthread-safety -D__STDC_WANT_LIB_EXT1__=1 -std=c2x -DDEBUG=1 -DRELEASE=0 -DMODE=$(BUILD_MODE) -DENABLED=1 -DDISABLED=0 -DWIN=1 -DDWN=2 -DLNX=3 -DBSD=4 -DPLATFORM=$(PLATFORM)
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

ifeq ($(PLATFORM),WIN)
	LIB_PREFIX =
	DYNAMIC_LIB_SUFFIX = .dll
	STATIC_LIB_SUFFIX = .lib
	EXECUTABLE_SUFFIX = .exe
	OBJECT_SUFFIX = .obj

	# Presume windows targets are x86 for now
	GLOBAL_C_FLAGS += -D_MT
	GLOBAL_L_FLAGS += -lshlwapi.lib

	OBJECT_FORMAT = PE

	DYNAMIC_LIB_TOOL = dlltool --export-all-symbols -z $(subst $(DYNAMIC_LIB_SUFFIX),.def,$@) -D $(notdir $@) $(filter %$(OBJECT_SUFFIX),$^) && $(CLINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) -shared $(GLOBAL_L_FLAGS) $(LFLAGS) -Wl,-def:$(subst $(DYNAMIC_LIB_SUFFIX),.def,$@),-implib:$(subst $(DYNAMIC_LIB_SUFFIX),$(STATIC_LIB_SUFFIX),$@)
	STATIC_LIB_TOOL = $(AR) -r $@ $(filter %$(OBJECT_SUFFIX),$^)
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
	STATIC_LIB_TOOL = $(AR) -r $@ $(filter %$(OBJECT_SUFFIX),$^)
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
	STATIC_LIB_TOOL = $(AR) -r $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),BSD)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC

	OBJECT_FORMAT = ELF

	DYNAMIC_LIB_TOOL = $(CLINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) -shared $(GLOBAL_L_FLAGS) $(LFLAGS)
	STATIC_LIB_TOOL = $(AR) -r $@ $(filter %$(OBJECT_SUFFIX),$^)
endif

ifeq ($(BUILD_MODE),RELEASE)
	GLOBAL_C_FLAGS += -m64 -Ofast -ffast-math -DNDEBUG -flto
	GLOBAL_L_FLAGS += -flto
	GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_BUILD_TYPE=Release

	ifeq ($(PLATFORM),WIN)
		GLOBAL_L_FLAGS += -llibcmt.lib
		GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
	endif

	ifeq ($(PLATFORM),LNX)
		GLOBAL_L_FLAGS += -Wl,-O1
	endif

	ifeq ($(PLATFORM),BSD)
		GLOBAL_L_FLAGS += -Wl,-O1
	endif

	ifeq ($(PLATFORM),DWN)
		GLOBAL_L_FLAGS += -Wl,-dead_strip,-no_implicit_dylibs,-warn_unused_dylibs,-dead_strip_dylibs,-interposable,-warn_stabs,-warn_commons,-debug_variant,-unaligned_pointers,warning
	endif
else
	GLOBAL_C_FLAGS += -m64 -glldb -O0 -fstandalone-debug -fno-eliminate-unused-debug-types -fdebug-macro -fno-lto
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
	ifneq ($(PLATFORM),DWN) # macOS libc dies when you `fork` with sanitizers enabled
		GLOBAL_C_FLAGS += -fsanitize=undefined
		GLOBAL_L_FLAGS += -fsanitize=undefined
		ifneq ($(PLATFORM),WIN) # Windows debug CRT conflicts with `clang` ASAN
			GLOBAL_C_FLAGS += -fsanitize=address
			GLOBAL_L_FLAGS += -fsanitize=address
		endif
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

ifneq ($(PLATFORM),WIN) # The windows CRT does this for us ;p
	@echo "$(ACTION_PREFIX)genstone/vendor/c11compat/safeclib/scripts/check_for_unsafe_apis $<$(ACTION_SUFFIX)"
	@genstone/vendor/c11compat/safeclib/scripts/check_for_unsafe_apis $<
endif

ifeq ($(STATIC_ANALYSIS),ENABLED)
	@echo "$(ACTION_PREFIX)$(COMPILER) $(GLOBAL_C_FLAGS) $(CFLAGS) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<$(ACTION_SUFFIX)"
	@$(COMPILER) $(GLOBAL_C_FLAGS) $(CFLAGS) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<
endif

ifeq ($(PLATFORM),WIN)
	@echo "$(CLANG_FORMAT) --style=file $<$(ACTION_SUFFIX)"
	-$(CLANG_FORMAT) --style=file $<
else
	@echo "$(ACTION_PREFIX)($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && ($(DIFF) $< tmp/$(notdir $<)-format.tmp)$(ACTION_SUFFIX)"
	-@($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && ($(DIFF) $< tmp/$(notdir $<)-format.tmp)
endif

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

%$(OBJECT_SUFFIX): %.cpp build/config.mk | tmp
	@echo "$(ACTION_PREFIX)$(COMPILERXX) -c $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(CFLAGS) $(CXXFLAGS))  -o $@ $<$(ACTION_SUFFIX)"
	@$(COMPILERXX) -c $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(CFLAGS) $(CXXFLAGS)) -o $@ $<

ifneq ($(PLATFORM),WIN) # The windows CRT does this for us ;p
	@echo "$(ACTION_PREFIX)genstone/vendor/c11compat/safeclib/scripts/check_for_unsafe_apis $<$(ACTION_SUFFIX)"
	@genstone/vendor/c11compat/safeclib/scripts/check_for_unsafe_apis $<
endif

ifeq ($(STATIC_ANALYSIS),ENABLED)
	@echo "$(ACTION_PREFIX)$(COMPILERXX) $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(CFLAGS) $(CXXFLAGS)) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<$(ACTION_SUFFIX)"
	@$(COMPILERXX) $(filter-out $(CXX_UNSUPPORTED_CFLAGS),$(GLOBAL_C_FLAGS) $(GLOBAL_CXX_FLAGS) $(CFLAGS) $(CXXFLAGS)) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<
endif

ifeq ($(PLATFORM),WIN)
	@echo "$(CLANG_FORMAT) --style=file $<$(ACTION_SUFFIX)"
	-$(CLANG_FORMAT) --style=file $<
else
	@echo "$(ACTION_PREFIX)($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && ($(DIFF) $< tmp/$(notdir $<)-format.tmp)$(ACTION_SUFFIX)"
	-@($(CLANG_FORMAT) --style=file $< > tmp/$(notdir $<)-format.tmp) && ($(DIFF) $< tmp/$(notdir $<)-format.tmp)
endif

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
