# Contributing

## Issues

If you find a **bug** or have a **feature request** you should open an issue

Issues are not to be used for issues with development outside of unexpected behaviour or for any general development help. There are better places to ask

## Code Style

The codebase follows a style similar to Chromium (at least according to `clang-format`). That being said there are a few changes so please do pay heed to formatting issues raised by `clang-format` during compilation

If you find an inconsistency not covered by `clang-format`, feel free to open an issue on the matter

Modules which are self-referential with includes and linkage should use their local copies of required resources. i.e. `#include "include/foo.h"` and explicit `-Lmymodule/lib -lbar`

Opt for allowing the caller to allocate nontrivial types and take a pointer to storage. i.e. `void foo(bar_t* storage);` instead of `bar_t foo(void);`

## Ettiquette

Common sense applies here: don't be rude, don't be a code-snob, don't harrass people

Additionally please keep swearing out of documentation and comments
