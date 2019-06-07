set COMPILE_FOLDER=%1
set COMPILE_SCRIPT=%2

bundle_nameset script_name=combile_and_upload.bat
echo Entering %script_name% with %COMPILE_FOLDER% %COMPILE_SCRIPT%


if %COMPILE_FOLDER%.==. (
    echo COMPILE_FOLDER parameter expected
    exit -1
)

if %COMPILE_SCRIPT%.==. (
    echo COMPILE_SCRIPT parameter expected
    exit -1
)


cd firmware
call clean.bat
cd ..


cd firmware\config
cd %COMPILE_FOLDER%

call %COMPILE_SCRIPT%

pwd
echo "exiting %script_name%"
