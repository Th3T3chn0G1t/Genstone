# Set build host platform
# `DEFAULT`: Detect OS
# `LINUX`: Linux
# `OSX`: macOS
# `WINDOWS`: Windows
HOST ?= DEFAULT

# Set build target platform
# `DEFAULT`: Use `HOST`
# `LINUX`: Linux
# `OSX`: macOS
# `WINDOWS`: Windows
PLATFORM ?= DEFAULT

# Set build mode
# `DEBUG`: Includes debug symbols and disables optimizations
# `RELEASE`: Excludes debug symbols and enables optimizations
MODE ?= DEBUG

# Set whether to enable sanitizers
# `ENABLED`: Enables sanitizers
# `DISABLED`: Disables sanitizers
SANITIZERS ?= ENABLED

# Set whether to enable static analysis
# `ENABLED`: Enables static analysis
# `DISABLED`: Disables static analysis
STATIC_ANALYSIS ?= DISABLED

# The C compiler to use
CLANG ?= clang

# The C+ compiler to use
CLANGXX ?= clang++

# The AR archiver to use
AR ?= ar

# The clang-format command to use
CLANG_FORMAT ?= clang-format

# Extra compiler flags to apply to all Genstone sources
EXTRA_CFLAGS ?=

# Extra linker flags to apply to all Genstone binaries
EXTRA_LFLAGS ?=
