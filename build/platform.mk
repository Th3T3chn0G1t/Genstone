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
	LS = ls
	FIND = find
	FIND_FNAME = -name
endif
ifeq ($(PLATFORM), LINUX)
	LIB_PREFIX = lib
	STATIC_LIB_SUFFIX = .a
	DYNAMIC_LIB_SUFFIX = .so
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_CFLAGS += -fPIC -D_SVID_SOURCE -D_GNU_SOURCE -D_DEFAULT_SOURCE -D__USE_GNU

	BEGIN_FULL_STATIC = -Wl,--whole-archive
	END_FULL_STATIC = $(eval) -Wl,--no-whole-archive

	STATIC_LIB_TOOL = $(AR) -r -c $@ $(filter %$(OBJECT_SUFFIX),$^)
	INTERNAL_EXECUTABLE_TOOL_LFLAG := -Wl,-rpath,
	ifeq ($(SANITIZE),ENABLED)
		MODULE_FLAGS = -fsanitize=$(SANITIZERS)
	endif
	DYNAMIC_LIB_TOOL = $(CLINKER) $(MODULE_FLAGS) -shared $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	EXECUTABLE_TOOL = $(CLINKER) $(MODULE_FLAGS) -fPIE $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(addprefix $(INTERNAL_EXECUTABLE_TOOL_LFLAG),$(subst ./,,$(LIBDIRS))) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)

	ifeq ($(shell uname -o),Android)
# TODO: Add API version to definition here and select generic Unix backend if version <30
		GLOBAL_CFLAGS += -DGEN_LINUX_ANDROID
		DYNAMIC_LIB_TOOL += $(addprefix $(INTERNAL_EXECUTABLE_TOOL_LFLAG),$(subst ./,,$(LIBDIRS)))
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
	LS = ls
	FIND = find
	FIND_FNAME = -name
endif
ifeq ($(PLATFORM), OSX)
	LIB_PREFIX = lib
	STATIC_LIB_SUFFIX = .a
	DYNAMIC_LIB_SUFFIX = .dylib
	EXECUTABLE_SUFFIX = .out
	OBJECT_SUFFIX = .o

	GLOBAL_CFLAGS += -fPIC

	BEGIN_FULL_STATIC = -Wl,-force_load,
	END_FULL_STATIC =

	STATIC_LIB_TOOL = $(AR) -r -c $@ $(filter %$(OBJECT_SUFFIX),$^)
	INTERNAL_EXECUTABLE_TOOL_LFLAG := -Wl,-rpath,
	ifeq ($(SANITIZE),ENABLED)
		MODULE_FLAGS = -fsanitize=$(SANITIZERS)
	endif
	DYNAMIC_LIB_TOOL = $(CLINKER) -dynamiclib $(MODULE_FLAGS) $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -install_name "@rpath/$(notdir $@)" -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	EXECUTABLE_TOOL = $(CLINKER) -fPIE $(MODULE_FLAGS) $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(addprefix $(INTERNAL_EXECUTABLE_TOOL_LFLAG),$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
endif

ifeq ($(HOST), WINDOWS)
	RM = del
	RMDIR = rmdir
	ECHO = echo
	CD = cd
	AND = &&
	MKDIR = mkdir
	CAT = type
	LS = dir
	FIND = tree
	FIND_FNAME =
endif
ifeq ($(PLATFORM), WINDOWS)
	LIB_PREFIX =
	STATIC_LIB_SUFFIX = .lib
	DYNAMIC_LIB_SUFFIX = .dll
	EXECUTABLE_SUFFIX = .exe
	OBJECT_SUFFIX = .obj

	STATIC_LIB_TOOL = $(AR) -r -c $@ $(filter %$(OBJECT_SUFFIX),$^)
ifeq ($(SANITIZE),ENABLED)
	DYNAMIC_LIB_TOOL = $(CLINKER) -fsanitize=$(SANITIZERS) -shared $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	EXECUTABLE_TOOL = $(CLINKER) -fsanitize=$(SANITIZERS) -fPIE $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
else
	DYNAMIC_LIB_TOOL = $(CLINKER) -shared $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
	EXECUTABLE_TOOL = $(CLINKER) -fPIE $(GLOBAL_LFLAGS) $(addprefix -L,$(LIBDIRS)) $(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
endif
endif
