ifeq ($(HOST), DEFAULT)
	ifeq ($(OS), Windows_NT)
		HOST = WINDOWS
	else
		ifeq ($(shell uname -s), Linux)
			HOST = LINUX
		endif
		ifeq ($(shell uname -s), Darwin)
			HOST = OSX
		endif
	endif
endif

ifeq ($(PLATFORM), DEFAULT)
	PLATFORM = $(HOST)
endif

ifeq ($(HOST), LINUX)
	RM = rm
	RMDIR = rm -rf
	ECHO = echo -e
	CD = cd
	AND = &&
	MKDIR = mkdir
	CAT = cat
endif
ifeq ($(PLATFORM), LINUX)
	LIB_PREFIX = lib
	STATIC_LIB_SUFFIX = .a
	DYNAMIC_LIB_SUFFIX = .so
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_CFLAGS += -fPIC -D_SVID_SOURCE -D_GNU_SOURCE -D_DEFAULT_SOURCE -D__USE_GNU

	DYNAMIC_LIB_TOOL = $(CLINKER) -shared $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = $(AR) -r -c $@ $(filter %$(OBJECT_SUFFIX),$^)
	INTERNAL_EXECUTABLE_TOOL_LFLAG := -Wl,-rpath,
	EXECUTABLE_TOOL = $(CLINKER) -fPIE $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(addprefix $(INTERNAL_EXECUTABLE_TOOL_LFLAG),$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)

        ifeq ($(shell uname -o),Android)
                GLOBAL_CFLAGS += -DGEN_LINUX_ANDROID
		DYNAMIC_LIB_TOOL += $(addprefix $(INTERNAL_EXECUTABLE_TOOL_LFLAG),$(LIBDIRS))
        endif
endif

ifeq ($(HOST),OSX)
	RM = rm
	RMDIR = rm -rf
	ECHO = echo
	CD = cd
	AND = &&
	MKDIR = mkdir
	CAT = cat
endif
ifeq ($(PLATFORM), OSX)
	LIB_PREFIX = lib
	STATIC_LIB_SUFFIX = .a
	DYNAMIC_LIB_SUFFIX = .dylib
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_CFLAGS += -fPIC

	DYNAMIC_LIB_TOOL = $(CLINKER) -dynamiclib $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -install_name "@rpath/$(notdir $@)" -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = $(AR) -r -c $@ $(filter %$(OBJECT_SUFFIX),$^)
	INTERNAL_EXECUTABLE_TOOL_LFLAG := -Wl,-rpath,
	EXECUTABLE_TOOL = $(CLINKER) -fPIE $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(addprefix $(INTERNAL_EXECUTABLE_TOOL_LFLAG),$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
endif

ifeq ($(HOST), WINDOWS)
	RM = del
	RMDIR = rmdir
	ECHO = echo
	CD = cd
	AND = &&
	MKDIR = mkdir
	CAT = type
endif
ifeq ($(PLATFORM), WINDOWS)
	LIB_PREFIX =
	STATIC_LIB_SUFFIX = .lib
	DYNAMIC_LIB_SUFFIX = .dll
	EXECUTABLE_SUFFIX = .exe
	OBJECT_SUFFIX = .obj

	DYNAMIC_LIB_TOOL = $(CLINKER) -shared $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = $(AR) -r -c $@ $(filter %$(OBJECT_SUFFIX),$^)
	EXECUTABLE_TOOL = $(CLINKER) -fPIE $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
