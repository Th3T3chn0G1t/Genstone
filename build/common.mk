include build/config.mk

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

GLOBAL_C_FLAGS += -DPLATFORM=$(PLATFORM)
GLOBAL_CMAKE_MODULE_FLAGS = -G "Unix Makefiles"

ifeq ($(PLATFORM),WIN)
	LIB_PREFIX =
	DYNAMIC_LIB_SUFFIX = .dll
	STATIC_LIB_SUFFIX = .lib
	EXECUTABLE_SUFFIX = .exe

	DYNAMIC_LIB_C_FLAGS =
	DYNAMIC_LIB_L_FLAGS =

	DYNAMIC_LIB_TOOL = dlltool --export-all-symbols -l $(subst $(DYNAMIC_LIB_SUFFIX),$(STATIC_LIB_SUFFIX),$@) $(filter %.o,$^); $(LINKER) -shared -Wl,-implib:$(addsuffix $(STATIC_LIB_SUFFIX), $(basename $@)) -o $@ $(filter %.o,$^)
	STATIC_LIB_TOOL = ar -cvq $@ $(filter %.o,$^)
endif
ifeq ($(PLATFORM),LNX)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out

	DYNAMIC_LIB_C_FLAGS = -fPIC

	DYNAMIC_LIB_TOOL = $(LINKER) -shared -o $@ $(filter %.o,$^)
	STATIC_LIB_TOOL = ar -cvq $@ $(filter %.o,$^)
endif
ifeq ($(PLATFORM),DWN)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .dylib
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out

	DYNAMIC_LIB_C_FLAGS = -fPIC

	DYNAMIC_LIB_TOOL = $(LINKER) -dynamiclib -install_name "@rpath/$(notdir $@)" -o $@ $(filter %.o,$^)
	STATIC_LIB_TOOL = libtool -static -o $@ $(filter %.o,$^)
endif
ifeq ($(PLATFORM),BSD)
	LIB_PREFIX = lib
	DYNAMIC_LIB_SUFFIX = .so
	STATIC_LIB_SUFFIX = .a
	EXECUTABLE_SUFFIX = .out

	DYNAMIC_LIB_C_FLAGS = -fPIC

	DYNAMIC_LIB_TOOL = $(LINKER) -shared -o $@ $(filter %.o,$^)
	STATIC_LIB_TOOL = ar -cvq $@ $(filter %.o,$^)
endif

ifeq ($(BUILD_MODE),RELEASE)
	GLOBAL_C_FLAGS += -O3 -DBUILD_RELEASE -msse2 -mavx2
	GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_BUILD_TYPE=Release

	ifeq ($(PLATFORM),WIN)
		GLOBAL_C_FLAGS += -D_MT
		GLOBAL_L_FLAGS += -llibcmt.lib
		GLOBAL_CMAKE_MODULE_FLAGS += -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
	endif
else
	GLOBAL_C_FLAGS += -g -O0 -DBUILD_DEBUG
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

%.o: %.c
	$(COMPILER) -c $(GLOBAL_C_FLAGS) $(CFLAGS) -o $@ $<

%$(STATIC_LIB_SUFFIX):
	$(STATIC_LIB_TOOL)

%$(DYNAMIC_LIB_SUFFIX):
	$(DYNAMIC_LIB_TOOL) $(LFLAGS)

%$(EXECUTABLE_SUFFIX):
	$(LINKER) $(GLOBAL_L_FLAGS) $(LFLAGS) -o $@ $(filter %.o,$^)
