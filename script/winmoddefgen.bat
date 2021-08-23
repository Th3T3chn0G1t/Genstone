@echo off

echo "LIBRARY %2" > %1
echo "EXPORTS" >> %1

set headers=
shift
shift
:arg_loop
if "%1"=="" goto break
set headers=%headers% %1
shift
goto arg_loop

:break

set externs=
for /f %%i in (headers) do (
    set line=
    for /f "tokens=*" %%j in ('findstr extern %%i') do set line=%%j
    if "%line%" != "" set externs=%externs% %line%
)

for /f %%i in (externs) do (
    set symbol=
    for /f "tokens=*" %%j in ("%%i") do set symbol=%%j
    for /f "tokens=1 delims=(" %%j in ("%symbol%") do set symbol=%%j
    echo "  %symbol%" >> %1
)
