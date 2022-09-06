ifeq ($(PLATFORM), DEFAULT)
	ifeq ($(OS), Windows_NT)
		PLATFORM = WINDOWS
	else
		ifeq ($(shell uname -s), Linux)
			PLATFORM = LINUX
		endif
		ifeq ($(shell uname -s), Darwin)
			PLATFORM = OSX
		endif
	endif
endif

ifeq ($(PLATFORM), LINUX)
	LIB_PREFIX = lib
	STATIC_LIB_SUFFIX = .a
	DYNAMIC_LIB_SUFFIX = .so
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_CFLAGS += -fPIC

	DYNAMIC_LIB_TOOL = $(CLINKER) -shared $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	STATIC_LIB_TOOL = $(AR) -r -c $@ $(filter %$(OBJECT_SUFFIX),$^)
	INTERNAL_EXECUTABLE_TOOL_LFLAG := -Wl,-rpath,
	EXECUTABLE_TOOL = $(CLINKER) -fPIE $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(addprefix $(INTERNAL_EXECUTABLE_TOOL_LFLAG),$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)

	RM = rm
	RMDIR = rm -rf
	ECHO = echo -e
	CD = cd
	AND = &&
	MKDIR = mkdir
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

	RM = rm
	RMDIR = rm -rf
	ECHO = echo
	CD = cd
	AND = &&
	MKDIR = mkdir
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

	RM = del
	RMDIR = rmdir
	ECHO = echo
	CD = cd
	AND = &&
	MKDIR = mkdir
endif
