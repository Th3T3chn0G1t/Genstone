# Set build target platform
# Possible values are:
# `DEFAULT`: Detect host OS. Sets to the respective value
# `DWN`: macOS
# `LNX`: Linux
PLATFORM = DEFAULT

# The C compiler to use
# This is unlikely to work with compilers other than clang
COMPILER = clang

# The C++ compiler to use
# This is unlikely to work with compilers other than clang++
COMPILERXX = clang++

# The glslc binary to use
GLSLC = glslc

# The python3 command line to use
PYTHON3 = python3

# The linker to use
# Will be specified to the compiler via. `-fuse-ld` for compatibility reasons
# Set to `DEFAULT` to allow the compiler to select a linker
# Set to `LLD` to select an appropriate `lld` backend for the platform
LINKER = LLD

# The `ar` tool to use for creating static libraries
AR = ar

# The clang-format command line to use
CLANG_FORMAT = clang-format
