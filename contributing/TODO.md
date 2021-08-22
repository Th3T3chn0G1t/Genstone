# TODO

#### Note
This document is purposely being kept off Github issues to create distinction between actual in progress features and "plans for the future" which is effectively what this. It also makes it easier to throw plans onto this doc during development as new requirements are found

If the project ever garners multiple contributors this can be migrated to Github issues

- Building
    - Generators
        - Generate more IDE configs
        - Visualisation
            - Dependency graph
            - Loc
            - Development time
    - Embedded toolchain
        - Embedded `dlltool` binary or equivalent script for Windows builds
- Documentation
    - Formal writeup for Genesis
    - Automatic Github action for documentation generator
- Contribution
    - Comprehensive contribution guidelines
    - Contribution ettiquette
    - Clang-tidy
    - Clang-format
- Development
    - Prebuilt binaries
    - genfs
        - Increase the tolerances on gen_path_relative
    - gencommon
        - streq to replace strcmp
    - Profiling
        - Frequency profiling (how often something happens)
        - Framed profiling (how much of something is spent where)
    - Callback attribute to allow optimizations https://clang.llvm.org/docs/AttributeReference.html#callback
    - generator (editor)
        - Visual build tool
    - genesis
        - run tree func
    - Windows GetLastError conversion table (?)
    - gendl
        - Dylib introspection
- Examples
    - Write examples
        - Build system module examples
    - Perform CI on examples
- Community
