set COMPILE_FOLDER=%1
set BOARD_NAME=%2


set script_name=combile_and_upload.bat
echo Entering %script_name% with %COMPILE_FOLDER% %BOARD_NAME%

if %COMPILE_FOLDER%.==. (
    echo COMPILE_FOLDER parameter expected
    exit -1
)

if %BOARD_NAME%.==. (
    echo BOARD_NAME parameter expected
    exit -1
)

set COMPILE_SCRIPT=!compile-%BOARD_NAME%.bat

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
echo "exiting %script_name%"
