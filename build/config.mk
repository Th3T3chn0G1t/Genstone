# Set build platform
# Possible values are:
# `DEFAULT`: Detect host OS. Sets to the respective value
# `WIN`: Windows
# `DWN`: macOS
# `LNX`: Linux
# `BSD`: BSD
PLATFORM = DEFAULT

# Set build optimization mode
# Possible values are
# `DEBUG`: Include debug information, reduced/minimal optimization
# `RELEASE`: Don't include debug information, highest optimization
BUILD_MODE = DEBUG

# Path to the sandbox project module Makefile
SANDBOX_PROJECT_MODULE = sandbox/sample.mk

# Paths to extra module Makefiles
ADDITIONAL_BUILD_MODULES =
# Prerequisite rule names for the build
BUILD_PREREQS =
# Post-build rule names
BUILD_POST =

# A list of module names to exclude from the build
DISABLED_MODULES =

# Runs tests as part of an appropriate build stage
# Possible values are:
# `BUILD`: Tests the build system
# `UNITS`: Runs unit tests
# `ALL`: Runs all available tests
TEST = ALL

# Flags to be provided to the compiler at every call
# Does not get applied to 3rd party compilation
GLOBAL_C_FLAGS =
# Flags to be provided to the linker at every call
# Does not get applied to 3rd party linkage
GLOBAL_L_FLAGS =

# The C compiler to use
# It is not recommended to change this
COMPILER = clang
# The linker to use
# For the purposes of portability and ease of invocation - this should be the same as the compiler
# It is not recommended to change this
LINKER = clang

# The clang-format command line to use
CLANG_FORMAT = clang-format

# The IDE to generate IDE configuration for with `ideconf` target
IDE =

# Debugging for the build system
# You probably don't need to enable this
# Possible values are:
# `ENABLED`: Enables build system debugging
# `DISABLED`: Disables build system debugging
BUILD_SYS_DEBUG = ENABLED
