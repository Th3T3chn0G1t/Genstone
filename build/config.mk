# `linux`: Linux
# `darwin`: macOS and co.

# Set build host platform
HOST ?=

# Set build target platform
PLATFORM ?=

# Set build mode
# `DEBUG`: Includes debug symbols and disables optimizations
# `RELEASE`: Excludes debug symbols and enables optimizations
MODE ?= DEBUG

# Set enabled sanitizers
SANITIZERS ?= address,undefined

# Set whether to enable static analysis
STATIC_ANALYSIS ?= ENABLED

# The clang command line to use
CLANG ?= clang

# The AR archiver to use
AR ?= ar
