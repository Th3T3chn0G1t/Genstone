# The C compiler to use
# This is unlikely to work with compilers other than clang
COMPILER = clang

# The linker to use
# Will be specified to the compiler via. `-fuse-ld` for compatibility reasons
# Set to `DEFAULT` to allow the compiler to select a linker
# Set to `LLD` to select an appropriate `lld` backend for the platform
LINKER = LLD

# The `ar` tool to use for creating static libraries
AR = ar

# The clang-format command line to use
CLANG_FORMAT = clang-format

# The tool to use for stripping binaries
# Expected to have a command line interface similar to `llvm-strip`
# If the target platform's linker should supports stripping, please set this to `LINKER` (the literal word *not* the value of the `LINKER` variable)
# Stripping is more effective if the linker is responsible for stripping
STRIP_TOOL = strip

# CMake exec to use for building CMake submodules
CMAKE = cmake

# ifeq ($(PLATFORM),WIN)
# CHECKED_CLANG_CURL_TARGET_PATTERN = https://github.com/microsoft/checkedc/releases/download/*win64.exe
# else
# CHECKED_CLANG_CURL_TARGET_PATTERN = https://github.com/microsoft/checkedc/releases/download/*.tar.gz
# endif

# checkedc_clang:
# 	echo 
# ifeq ($(PLATFORM),WIN)
# else
# endif
