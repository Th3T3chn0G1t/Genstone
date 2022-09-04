# Set build target platform
# `DEFAULT`: Detect host OS
# `LINUX`: Linux
# `OSX`: macOS
# `WINDOWS`: Windows
PLATFORM ?= DEFAULT

# Set build mode
# `DEBUG`: Includes debug symbols and disables optimizations
# `RELEASE`: Excludes debug symbols and enables optimizations
MODE ?= DEBUG

# The C compiler to use
CLANG ?= clang

# The AR archiver to use
AR ?= ar

# The clang-format command to use
CLANG_FORMAT ?= clang-format

# Extra compiler flags to apply to all Genstone sources
EXTRA_CFLAGS ?=

# Extra linker flags to apply to all Genstone binaries
EXTRA_LFLAGS ?=
