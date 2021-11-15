@echo off & setlocal enabledelayedexpansion
set arg1=%~1
set count=0
if not defined arg1 goto end

:loop
if not "!arg1:~%count%,1!" == "" (
    set /a count+=1
    goto loop
)

echo %count%

:end
exit /B 0