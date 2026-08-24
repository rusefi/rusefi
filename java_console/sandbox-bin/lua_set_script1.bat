@echo off
rem lua_set_script1.bat
rem Builds the rusEFI java console and applies sandbox_script1.lua to the connected ECU.

setlocal
set SCRIPT_DIR=%~dp0
pushd "%SCRIPT_DIR%..\.."
call gradlew.bat :ui:shadowJar || goto :error
popd

java -jar console\rusefi_console.jar set_lua "%SCRIPT_DIR%sandbox_script1.lua"
goto :eof

:error
echo Build failed
exit /b 1
