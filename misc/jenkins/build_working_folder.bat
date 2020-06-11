set full_bundle_file=%bundle_full_name%.zip
set update_bundle_file=%bundle_full_name%_autoupdate.zip

echo Packaging temp\%full_bundle_file% file

rm -rf temp
mkdir temp

set script_name=build_working_folder.bat
echo Entering %script_name%

pwd
rem This working folder name starts with 'temp/'
echo %script_name%: Working folder: %folder%
mkdir %folder%
echo %bundle_full_name% > %folder%\bundle_name.ini

set console_folder=%folder%\console
set drivers_folder=%folder%\drivers
mkdir %console_folder%
mkdir %drivers_folder%
ls -l %folder%

wget https://rusefi.com/build_server/st_files/silent_st_drivers.exe -P %drivers_folder%
if not exist %drivers_folder%/silent_st_drivers.exe (
 echo %script_name%: ERROR DOWNLOADING silent_st_drivers.exe
 EXIT /B 1
)

if %ini_file_override%x==nox (
    set ini_file_override=rusefi.ini
    echo %script_name%: N/a ini_file_override specified
)
if %ini_file_override%.==. (
    set ini_file_override=rusefi.ini
    echo %script_name%: No ini_file_override specified
)
echo %script_name%: Will use %ini_file_override%

if %rusefi_console_settings%.==. (
  echo %script_name%: No rusefi_console_settings
) else (
  echo Using rusefi_console_settings [%rusefi_console_settings%]
  cp %rusefi_console_settings% %console_folder%
)

cp java_console/rusefi_autoupdate.jar     %console_folder%
cp java_console_binary/rusefi_console.jar %console_folder%
cp simulator/build/rusefi_simulator.exe   %console_folder%
cp misc/console_launcher/rusefi_*.exe     %console_folder%
cp java_console/rusefi.xml                %console_folder%

cp misc/console_launcher/readme.html %folder%

cp firmware/tunerstudio/%ini_file_override% %folder%
rem Unsetting since would not be used anywhere else
set ini_file_override=
set rusefi_console_settings=

rem users probably do not really care for this file
rem cp firmware/svnversion.h %folder%

cp -r misc/install/openocd %console_folder%
cp -r misc/install/DfuSe %console_folder%
rem 407 has additional version of firmware
cp firmware/deliver/rusefi_no_asserts.bin %folder%
cp firmware/deliver/rusefi_no_asserts.dfu %folder%
rem 746 builds one version at the moment
rem probably not needed cp firmware/build/rusefi.hex %folder%
cp firmware/deliver/rusefi.bin %folder%
rem probably not needed cp firmware/build/rusefi.elf %folder%
cp firmware/deliver/rusefi.dfu %folder%

if [%bundle_name%] neq [] (
    mv %folder%/rusefi.dfu %folder%/rusefi_%bundle_name%.dfu
)


if not exist firmware/deliver/rusefi.bin echo %script_name%: rusefi.bin not found
if not exist firmware/deliver/rusefi.bin exit -1

cd temp


echo "Building bundle"
pwd
zip -r %full_bundle_file% *
IF NOT ERRORLEVEL 0 echo %script_name%: ERROR INVOKING zip
IF NOT ERRORLEVEL 0 EXIT /B 1

echo %script_name%: Bundle %full_bundle_file% ready
ls -l %full_bundle_file%

if not exist %full_bundle_file% echo %script_name%: ERROR not found %full_bundle_file%
if not exist %full_bundle_file% EXIT /B 1

echo "%script_name%: Uploading full bundle"
ncftpput -u %RUSEFI_BUILD_FTP_USER% -p %RUSEFI_BUILD_FTP_PASS% %RUSEFI_FTP_SERVER% . %full_bundle_file%

cd ..

mkdir artifacts
mv temp/%full_bundle_file% artifacts

echo Removing more static content
rm -rf %console_folder%/openocd
rm -rf %console_folder%/DfuSe
rm -rf %console_folder%/rusefi_simulator.exe

rm -rf %drivers_folder%

rem for autoupdate we do not want the unique folder name with timestamp
cd %folder%
zip -r ../%update_bundle_file% *
cd ..
ls -l %update_bundle_file%
ncftpput -u %RUSEFI_BUILD_FTP_USER% -p %RUSEFI_BUILD_FTP_PASS% %RUSEFI_FTP_SERVER% autoupdate %update_bundle_file%
cd ..

echo "%script_name%: We are back in root directory"

pwd
echo Exiting %script_name%