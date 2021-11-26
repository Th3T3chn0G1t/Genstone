![Genstone Logo](brand/banner.png)

[![CI](https://github.com/Th3T3chn0G1t/Genstone/actions/workflows/CI.yml/badge.svg?branch=trunk)](https://github.com/Th3T3chn0G1t/Genstone/actions/workflows/CI.yml)
[![Documentation](https://github.com/Th3T3chn0G1t/Genstone/actions/workflows/docs.yml/badge.svg)](https://github.com/Th3T3chn0G1t/Genstone/actions/workflows/docs.yml)
[![Discord](https://img.shields.io/discord/906624261203058688?color=4e5d94&label=discord&logo=discord&logoColor=4e5d94)](https://discord.gg/7rG4ueJgX6)

### A C framework which creates a secure application layer

## Building

#### Note
Genstone uses Javadoc-style docstrings to document functions, variables outside of functions, macros, structures and structure members. You can view these directly in the headers, generate local docs (Use `make docs`), or view the latest documentation build on Github pages at https://th3t3chn0g1t.github.io/Genstone/ (Recommended).

Genstone uses a configurable system of `Makefile` (Have only tested with GNU Make flavour) modules to build, this also applies to integrating submodule building.

The base `Makefile` in the project root tries to build the sandbox project be default.

Documentation of most user-oriented targets is available through `make help`. This is provided through `###` docstrings in Makefiles. These can be specified in user modules in the following format:
```
target: PREREQUISITES (Will be removed from output) ### @HighlightedThing The rest of the docstring
```

`make list` will list the targets due to be built for `all` (the default target) based on configuration. 

### Prerequisites

The codebase will only build with `clang`. You will also need `make` installed in some form.

### Building

#### Note
On some versions of macOS, the provided `clang` version does not support some of the features used in `gencalc`. To disable `gencalc` avoid this issue, add `gencalc` (and `gencalc_test` if neccesary) to `DISABLED_MODULES`. Alternatively, if you need `gencalc`, consider installing the Homebrew version of `clang` with the features enabled using `brew install llvm` and adding a link to `/usr/local/Cellar/llvm/{VERSION}/bin/clang` in your path (with `ln -sf /usr/local/Cellar/llvm/{VERSION}/bin/clang /usr/local/bin/clang-12` or the like). Homebrew can be gotten from [brew.sh](https://brew.sh).

The default configuration should build a sandbox project, so after a fresh clone just run `make`. (Configuration may be required for adding additional projects). For cleaning the project, `make clean` should remove most artifacts. YOU MAY NEED TO RUN TWICE due to the way make processes `$(wildcard)` and `$(shell)` functions.

The Annex K compat lib (safeclib) takes a *very* long time to build, so a separate clean command is provided `clean_c11compat_annexk`.

### Special Targets

Additional build targets exist for building IDE configuration files (`ideconf`) and documentation (`docs`). These targets' settings are also controlled by values in `build/config.mk`. `ideconf` will overwrite existing configuration and does not behave differently on subsequent invocations (i.e. calling once and then modifying is recommended).

Documentation requires `Doxygen` to be installed and available on the `PATH`.

If you don't plan to use the source of Genstone during development, you can generate a more lightweight development environment using `make devenv`.

This is *not* viable if you plan to embed Genstone into your own project, as it generates the project for your platform and configuration.

`make devenv` will output to `out/` with generated `include`, `lib` and `docs` folders.

It is recommended to use this in conjunction with `make ideconf` to generate your development environment configuration.

### Options

Variables to influence the way an application or module is built with Genstone.

### Makefile Options

#### Note
These options can be overriden with a variable `OVERRIDE_keyname` in the executing environment. e.g. `OVERRIDE_LINKER=lld.ld make`.

Set options for the build process itself (e.g. for cross-compilation) in `build/config.mk` amd `build/toolchain.mk`, the file from which these options are sourced can be changed with `OVERRIDE_CONFIG`. All variables are commented to explain their purpose and possible values. The more commonly used options are toward the top of the files.

### Compilation Options

Setting in-code options can be done via. `-D` flags set via. the config Makefile.

|Name|Values|Default|Description|Notes|
|---|---|---|---|---|
|`PLATFORM`|`DWN` `LNX` `BSD`|Determined by Makefile|The target platform for the compilation|Do not mix platforms in a binary|
|`MODE`|`DEBUG` `RELEASE`|Determined by Makefile|The target output optimization mode for compilation|It is usually prefereable to set via. the `BUILD_MODE` key in `config.mk` to avoid missing mode-specific build operations|
|`GEN_TOOLING_DEPTH`|Any valid array size|64|The maximum depth of a tooled call stack|Is used to initialize a stateful buffer|
|`GEN_FREQ_PROFILE_MAX`|Any valid array size|64|The maximum number of frequency profilers|Is used to initialize a stateful buffer|
|`GEN_GLOG_STREAM_COUNT`|Any valid array size|8|The maximum number of output streams `glog` and `glogf` can output to|Is used to initialize a stateful buffer|
|`GEN_GLOGGIFY_EH`|`ENABLED` `DISABLED`|`ENABLED`|Whether error sites in Genstone modules are allowed to output string information via. `glog`|Depending on the error, this may contain more information than centralized error functions|
|`GEN_CENTRALIZE_EH`|`ENABLED` `DISABLED`|`DISABLED`|Whether to call `gen_error_handler` at error sites in Genstone modules|Creates global state if enabled. Set `gen_error_handler_passthrough` to get a passthrough pointer in `gen_error_handler`|
|`GEN_DEBUG_PATH_VALIDATION`|`ENABLED` `DISABLED`|`BUILD_MODE=DEBUG`: `ENABLED` `BUILD_MODE=RELEASE`: `DISABLED`|Whether to validate paths passed to genfs functions with `gen_path_validate`|Does not affect the presence of `gen_path_validate`|
|`GEN_DEBUG_FOREACH_REGISTER`|`ENABLED` `DISABLED`|`BUILD_MODE=DEBUG`: `DISABLED` `BUILD_MODE=RELEASE`: `ENABLED`|Whether to use register variables for iteration in `GEN_FOREACH` statements|Disabling this can sometimes help with printing iterator values from a debugger|
|`GEN_PROC_READ_BUFFER_CHUNK`|Any valid array size greater than 0|128|The size of chunk to read at once while storing output in `gen_proc_get_output`|Increasing this if you have large amounts of output in subprocesses may improve performance|
|`GEN_PRESUMED_SYMBOL_MAX_LEN`|Any valid array size|1024|Presumed maximum length of a symbol to be imported from a dynamic library|Default is taken from Annex B minimum symbol name limitation reccommendations|
|`GEN_FATAL_ANNEXK_CONSTRAINTS`|`ENABLED` `DISABLED`|`ENABLED`|Whether the Genstone-installed Annex K constraint handler should trigger a fatal error and abort the program|You may want to disable this if you are getting false Annex K constraint hits or are using unsafe string manipulation code|
|`GEN_USE_MIMALLOC`|`ENABLED` `DISABLED`|`ENABLED`|Whether to use mimalloc as the allocator|Disabling mimalloc may be detrimental to performance and is not as well tested nor as secure|
|`GEN_CALC_COMMON_TYPES`|`ENABLED` `DISABLED`|`ENABLED`|Enables the default definition of common vector and matrix types in gencalc||

