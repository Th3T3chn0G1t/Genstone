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
# `DEBUG`: Include debug information, no optimization
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

# Runs tests as part of an appropriate build stage
# Possible values are:
# `BUILD`: Tests the build system
# `UNITS`: Runs unit tests
# `ALL`: Runs all available tests
TEST = ALL

# Flags to be provided to the compiler at every call
# Does not get applied to 3rd party compilation
GLOBAL_C_FLAGS = -std=c17
# Flags to be provided to the linker at every call
# Does not get applied to 3rd party linkage
GLOBAL_L_FLAGS =

# The C compiler to use
# It is not recommended to change this
COMPILER = clang
# The linker to use
# For the purposes of portability and ease of invocation -
# This should be the same as the compiler
# It is not recommended to change this
LINKER = clang
