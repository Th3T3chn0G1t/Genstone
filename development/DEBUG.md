# Debug

In debug builds, some functionality is changed to provide extra validation or execution environment information

`MODE=DEBUG` will set these automatically, but they can also be controlled individually which will override the enabled/disabled state

|Name|Values|Description|Notes|
|---|---|---|---|
|`GEN_FS_PATH_VALIDATION`|`ENABLED` `DISABLED`|Whether to validate paths passed to genfs functions with `gen_path_validate`|Does not affect the presence of `gen_path_validate`|
