include build/config.mk

# We want make to use cmd.exe when the *host* is Windows
ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
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

SEP = /
CP = cp
RM = rm
RMDIR = rm -rf
ifeq ($(SHELL),cmd.exe)
	SEP = \\
	CP = copy /b /y
	RM = del
	RMDIR = rmdir
endif

GLOBAL_C_FLAGS += -DWIN=1 -DDWN=2 -DLNX=3 -DBSD=4 -DPLATFORM=$(PLATFORM)
GLOBAL_CMAKE_MODULE_FLAGS = -G "Unix Makefiles"

ifeq ($(PLATFORM),WIN)
	LIB_PREFIX =
	DYNAMIC_LIB_SUFFIX = .dll
	STATIC_LIB_SUFFIX = .lib
	EXECUTABLE_SUFFIX = .exe
	OBJECT_SUFFIX = .obj

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
	GLOBAL_C_FLAGS += -O3 -DMODE=RELEASE -msse2 -mavx2
	GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_BUILD_TYPE=Release

	ifeq ($(PLATFORM),WIN)
		GLOBAL_C_FLAGS += -D_MT
		GLOBAL_L_FLAGS += -llibcmt.lib
		GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
	endif
else
	GLOBAL_C_FLAGS += -g -O0 -DMODE=DEBUG
	GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_BUILD_TYPE=Debug

	ifeq ($(PLATFORM),WIN)
		GLOBAL_C_FLAGS += -D_MT -D_DEBUG
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

%$(OBJECT_SUFFIX): %.c
	$(COMPILER) -c $(GLOBAL_C_FLAGS) $(CFLAGS) -o $@ $<

%$(STATIC_LIB_SUFFIX):
	$(STATIC_LIB_TOOL)

%$(DYNAMIC_LIB_SUFFIX):
	$(DYNAMIC_LIB_TOOL) $(GLOBAL_L_FLAGS) $(LFLAGS)

%$(EXECUTABLE_SUFFIX):
	$(LINKER) -o $@ $(filter %$(OBJECT_SUFFIX),$^) $(GLOBAL_L_FLAGS) -fPIE $(LFLAGS)
