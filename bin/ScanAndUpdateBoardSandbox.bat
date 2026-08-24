@echo off
setlocal
rem Compiles the java_console :ui test source set and launches the manual
rem ScanAndUpdateBoardSandbox harness (auto-detect ECU on serial, download its bundle,
rem reboot to OpenBLT and flash the fresh .srec). Any extra args are forwarded to main().

set "ROOT=%~dp0.."
set "GRADLEW=%ROOT%\gradlew.bat"
set "ARGS_FILE=%ROOT%\java_console\ui\build\scanAndUpdate.args"
pushd "%ROOT%"

echo Compiling :ui test classes and resolving classpath...
rem --no-configuration-cache: a cached task graph can serialize a stale doLast, so force a fresh
rem configuration each run to guarantee a current classpath (the task is trivial to reconfigure)
call "%GRADLEW%" --no-configuration-cache :ui:writeScanAndUpdateArgs
if errorlevel 1 (
    echo Gradle build failed
    popd
    exit /b 1
)
if not exist "%ARGS_FILE%" (
    echo Classpath argument file not found: %ARGS_FILE%
    popd
    exit /b 1
)

rem The classpath is ~9 KB (over the cmd variable / command-line limit), so it is passed via a
rem JVM @argument-file rather than -cp on the command line.
echo Launching ScanAndUpdateBoardSandbox...
java @"%ARGS_FILE%" com.rusefi.autodetect.ScanAndUpdateBoardSandbox %*
set "RC=%errorlevel%"

popd
exit /b %RC%
