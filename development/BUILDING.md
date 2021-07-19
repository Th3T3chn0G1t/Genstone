# Building

Genesis uses a configurable system of Make (Have only tested with GNU Make flavour) modules to build, this also applies to integrating submodule building

The base `Makefile` in the project root tries to build the sandbox project be default, generating it if not present

## Options

Variables to influence the way an application or module is built with Genesis

### Makefile Options

Set options for the build process itself (e.g. for cross-compilation) in `build/config.mk`. All variables are commented to explain their purpose and possible values

#### Note
There is no error checking performed on entered values, invalid values will result in unwanted behaviour

### Compilation Options

Setting in-code options is done via. `-D` flags set via. the Makefile config
|Name|Values|Default|Description|Notes|
|---|---|---|---|---|
|`PLATFORM`|`WIN` `DWN` `LNX` `BSD`|Determined by Makefile|The target platform for the compilation|Do not mix platforms in a binary|
