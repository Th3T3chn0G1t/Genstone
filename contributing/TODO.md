# TODO

#### Note
This document is purposely being kept off Github issues to create distinction between actual in progress features and "plans for the future" which is effectively what this. It also makes it easier to throw plans onto this doc during development as new requirements are found

If the project ever garners multiple contributors this can be migrated to Github issues

- Building
    - Allow overriding of config options via. environment
    - Make deps. on headers
    - Generators
        - Default project
        - IDE-friendly development project (just binaries and headers)
            - Generate more IDE configs
        - Module Makefile
            - CMake submodule Makefile
        - Visualisation
            - Dependency graph
            - Loc
            - Development time
    - Separate build commons into a standalone project
    - Embedded toolchain
    - Visual build tool for options
    - Modules
        - Enabling/Disabling
        - Output
            - Pretty names
            - Diagnostic output
        - Debugging
    - `clang-format`
        - Pretty diffs between source and formatted source
            - Auto-replace vs. warning/error on diff
- Documentation
    - Formal writeup for Genesis
    - Automatic Github action for documentation generator
    - Github pages for generated docpages
- Development
    - Contribution
        - Comprehensive contribution guidelines
        - Contribution ettiquette
    - Clang-tidy
    - Clang-format
    - CI
        - Static analysis
        - Prebuilt binaries
    - Argument parser
    - Move Mesa thread compat function implementations to source file for inclusion in c11compat lib
    - Get tgmath.h working under windows
- Examples
    - Write examples
        - Build system module examples
    - Perform CI on examples
- Community
    - Discord server
