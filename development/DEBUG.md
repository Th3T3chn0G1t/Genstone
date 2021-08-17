# Debug

In debug builds, some functionality is changed to provide extra validation or execution environment information

Each key can be set to either `ENABLED` or `DISABLED`, making them truthy or falsy respectively.

`MODE=DEBUG` will set these automatically, but they can also be controlled individually which will override the enabled/disabled state

|Name|Description|Notes|
|---|---|---|
|`GEN_DEBUG_PATH_VALIDATION`|Whether to validate paths passed to genfs functions with `gen_path_validate`|Does not affect the presence of `gen_path_validate`|
|`GEN_DEBUG_FOREACH_REGISTER`|Whether to use register variables for iteration in `GEN_FOREACH` statements|Disabling this can sometimes help with printing iterator values from a debugger|
|`GEN_DEBUG_FOREACH_PRECALC`|Whether to use variables outside the for loop in `GEN_FOREACH` statements to possibly allow the compiler to optimize expressions passed to `len` or `container`|This will cause `GEN_FOREACH` statements to produce some pseudo-mangled variables for the length and container. Disabling this can help with printing length values or container locations|