include build/config.mk

# We want make to use cmd.exe when the *host* is Windows
ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
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
ifneq ($(BUILD_SYS_DEBUG),ENABLED)
ERROR += "$(ERROR_PREFIX) Invalid value for BUILD_SYS_DEBUG: \"$(BUILD_SYS_DEBUG)\"\n"
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

GLOBAL_C_FLAGS += -D__STDC_WANT_LIB_EXT1__=1 -std=c17 -DDEBUG=1 -DRELEASE=0 -DMODE=$(BUILD_MODE) -DENABLED=1 -DDISABLED=0 -DWIN=1 -DDWN=2 -DLNX=3 -DBSD=4 -DPLATFORM=$(PLATFORM)
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

ifeq ($(BUILD_SYS_DEBUG),ENABLED)
# Need to check clang version for this to work (12.0.1?)
# 	GLOBAL_C_FLAGS += -fproc-stat-report
# 	GLOBAL_L_FLAGS += -fproc-stat-report
endif

ifeq ($(PLATFORM),WIN)
	LIB_PREFIX =
	DYNAMIC_LIB_SUFFIX = .dll
	STATIC_LIB_SUFFIX = .lib
	EXECUTABLE_SUFFIX = .exe
	OBJECT_SUFFIX = .obj

	GLOBAL_C_FLAGS += -D_MT
	GLOBAL_L_FLAGS += -lshlwapi.lib

	DYNAMIC_LIB_TOOL = dlltool --export-all-symbols -z $(subst $(DYNAMIC_LIB_SUFFIX),.def,$@) -D $(notdir $@) $(filter %$(OBJECT_SUFFIX),$^) && $(LINKER) -shared -o $@ $(filter %$(OBJECT_SUFFIX),$^) -Wl,-def:$(subst $(DYNAMIC_LIB_SUFFIX),.def,$@),-implib:$(subst $(DYNAMIC_LIB_SUFFIX),$(STATIC_LIB_SUFFIX),$@)
	STATIC_LIB_TOOL = ar -cvq $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
ifeq ($(PLATFORM),LNX)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_C_FLAGS += -fPIC -D_DEFAULT_SOURCE

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

	DYNAMIC_LIB_TOOL = $(LINKER) -shared -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = ar -cvq $@ $(filter %$(OBJECT_SUFFIX),$^)
endif

ifeq ($(BUILD_MODE),RELEASE)
# We don't use NDEBUG in the codebase so this is provided for convenience
	GLOBAL_C_FLAGS += -Ofast -ffast-math -msse2 -mavx2 -DNDEBUG -flto
	GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_BUILD_TYPE=Release

	ifeq ($(PLATFORM),WIN)
		GLOBAL_L_FLAGS += -llibcmt.lib
		GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
	endif
else
	GLOBAL_C_FLAGS += -glldb -O0 -fsanitize=address,undefined -fstandalone-debug -fno-eliminate-unused-debug-types -fdebug-macro
	GLOBAL_L_FLAGS += -fsanitize=address,undefined

	GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_BUILD_TYPE=Debug

	ifeq ($(PLATFORM),WIN)
		GLOBAL_C_FLAGS += -D_DEBUG
		GLOBAL_L_FLAGS += -Wl,-nodefaultlib:libcmt.lib -llibcmtd.lib
		GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug
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
	-$(RM) $(wildcard *.tmp)

%$(OBJECT_SUFFIX): %.c build/config.mk
	@echo "$(ACTION_PREFIX)$(COMPILER) -c $(GLOBAL_C_FLAGS) $(CFLAGS) -o $@ $<$(ACTION_SUFFIX)"
	@$(COMPILER) -c $(GLOBAL_C_FLAGS) $(CFLAGS) -o $@ $<
	@echo "$(ACTION_PREFIX)$(COMPILER) $(GLOBAL_C_FLAGS) $(CFLAGS) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<$(ACTION_SUFFIX)"
	@$(COMPILER) $(GLOBAL_C_FLAGS) $(CFLAGS) --analyze $(CLANG_STATIC_ANALYZER_FLAGS) $<
# 	 -($(CLANG_FORMAT) --style=file $< > $(notdir $<)-format.tmp) && ($(DIFF) $< $(notdir $<)-format.tmp > /dev/stderr)
# 	 -$(CLANG_FORMAT) --dry-run -Werror $<

%$(STATIC_LIB_SUFFIX):
	@echo "$(ACTION_PREFIX)$(STATIC_LIB_TOOL)$(ACTION_SUFFIX)"
	@$(STATIC_LIB_TOOL)

%$(DYNAMIC_LIB_SUFFIX):
	@echo "$(ACTION_PREFIX)$(DYNAMIC_LIB_TOOL) $(GLOBAL_L_FLAGS) $(LFLAGS)$(ACTION_SUFFIX)"
	@$(DYNAMIC_LIB_TOOL) $(GLOBAL_L_FLAGS) $(LFLAGS)

%$(EXECUTABLE_SUFFIX):
	@echo "$(ACTION_PREFIX)$(LINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) $(GLOBAL_L_FLAGS) -fPIE $(LFLAGS)$(ACTION_SUFFIX)"
	@$(LINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) $(GLOBAL_L_FLAGS) -fPIE $(LFLAGS)
