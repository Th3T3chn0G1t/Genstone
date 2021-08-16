# TODO

#### Note
This document is purposely being kept off Github issues to create distinction between actual in progress features and "plans for the future" which is effectively what this. It also makes it easier to throw plans onto this doc during development as new requirements are found

If the project ever garners multiple contributors this can be migrated to Github issues

- Building
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
    - Embedded toolchain
        - Embedded `dlltool` binary or equivalent script for Windows builds
    - Visual build tool for options
- Documentation
    - Custom static documentation generator
    - Formal writeup for Genesis
    - Automatic Github action for documentation generator
    - Github pages for generated docpages
- Contribution
    - Comprehensive contribution guidelines
    - Contribution ettiquette
    - Clang-tidy
    - Clang-format
- Development
    - Logger
    - CI
        - Static analysis
        - Prebuilt binaries
    - Argument parser
    - Execution environment utilities
        - rpath
        - loaded dylibs
        - executable path
        - environment variables
        - working directory
        - current filesystem/volume info
            - Check what file meta the OS/filesystem supports
        - binary-type (ABI) info
    - genfs
        - Windows 10 1607 long paths support https://docs.microsoft.com/en-us/windows/win32/fileio maximum-file-path-limitation
        - Increase the tolerances on gen_path_relative
    - gencommon
        - streq to replace strcmp
    - **Profiling**
    - generator (editor)
    - genesis
        - run tree func
    - perform validation in debug mode
    - Windows GetLastError conversion table (?)
- Examples
    - Write examples
        - Build system module examples
    - Perform CI on examples
- Community
