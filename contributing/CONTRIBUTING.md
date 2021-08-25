# Contributing

## Issues

If you find a **bug** or have a **feature request** you should open an issue

Issues are not to be used for issues with development outside of unexpected behaviour or for any general development help. There are better places to ask

## Code Style

#### Note
If a file or module has special or extra rules for formatting or general style then it will be outlined at the start of the file or in a `module.md` file

Please pay heed to formatting issues raised by `clang-format` during compilation. If you find an inconsistency not covered by `clang-format`, feel free to open an issue on the matter

Modules which are self-referential with includes should use their local copies of required resources. i.e. `#include "include/foo.h"`. For inter-module operation using `#include <foo.h>` is fine

Opt for allowing the caller to allocate nontrivial types and take a pointer to storage. i.e. `void foo(bar_t* storage);` instead of `bar_t foo(void);`

Opt for using explicit sizes instead of `NULL`-termination for buffers.

### Documentation

Please try to fix documentation warnings as reported, as non-impactful errors occlude important errors down the line

Document all elements present in headers - no magic public APIs - and make an effort to document local helper functions and macros

#### Note
Doxygen operates the preprocessor as `PLATFORM == LNX`, this means that platform-defined macros and types should be documented under the linux branch. The same applies as `MODE == DEBUG`

### Tests

Every function in the codebase should be tested, `test/unit/` contains existing tests which you can base on

`gencommon.h` provides assertion macros through `GEN_REQUIRE_*` for expected values as well as logging functions to outline test progress in the form `glog(INFO, "Testing foo_bar()...");`

The `GEN_REQUIRE_*` and `glog/f` macros are untested because they are ubiquitously used across the codebase and as such breakage should be evident

### Examples

## Ettiquette

Common sense applies here: don't be rude, don't be a code-snob, don't harrass people

Additionally please keep swearing out of documentation and comments
