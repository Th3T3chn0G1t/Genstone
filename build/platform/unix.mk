PLATFORM_SOURCEDIRS += unix libc

LIB_PREFIX = lib
STATIC_LIB_SUFFIX = .a
DYNAMIC_LIB_SUFFIX = .so
EXECUTABLE_SUFFIX = .out
OBJECT_SUFFIX = .o

STATIC_LIB_TOOL = $(AR) rcs $@ $(filter %$(OBJECT_SUFFIX),$^)
DYNAMIC_LIB_TOOL = $(CLINKER) -shared $(GLOBAL_LFLAGS) \
					$(addprefix -L,$(LIBDIRS)) $(LFLAGS) \
					-o $@ $(filter %$(OBJECT_SUFFIX),$^)

# Needed to avoid `,` from causing issues in a function call
INTERNAL_TOOL_RPATH := -Wl,-rpath,
EXECUTABLE_TOOL = $(CLINKER) $(GLOBAL_LFLAGS) \
					$(addprefix -L,$(LIBDIRS)) \
					$(addprefix $(INTERNAL_TOOL_RPATH),$(LIBDIRS)) \
					$(LFLAGS) -o $@ $(filter %$(OBJECT_SUFFIX),$^)
