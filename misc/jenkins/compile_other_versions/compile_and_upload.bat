set COMPILE_FOLDER=%1
set bundle_name=%2
set ini_file_override=%3
set rusefi_console_settings=%4

set script_name=combile_and_upload.bat
echo Entering %script_name% with 1=%COMPILE_FOLDER% 2=%bundle_name% 3=%ini_file_override% 4=%rusefi_console_settings%

if %COMPILE_FOLDER%.==. (
    echo COMPILE_FOLDER parameter expected
    exit -1
)

if %bundle_name%.==. (
    echo bundle_name parameter expected
    exit -1
)

set COMPILE_SCRIPT=!compile-%bundle_name%.bat

cd firmware
call clean.bat
cd ..

cd firmware\config\boards
pwd
cd %COMPILE_FOLDER%
pwd
echo Invoking %COMPILE_SCRIPT%

call %COMPILE_SCRIPT%
if not exist build/rusefi.hex echo Just to confirm - FAILED to compile with %COMPILE_SCRIPT%
if not exist build/rusefi.hex exit -1

cd ..
rem We are back at root rusEfi folder
pwd

call misc\jenkins\compile_other_versions\build_version.bat

echo "exiting %script_name%"
