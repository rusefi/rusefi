FULL_BUNDLE_FILE="$BUNDLE_FULL_NAME.zip"
UPDATE_BUNDLE_FILE="$BUNDLE_FULL_NAME_autoupdate.zip"

echo "Packaging temp/$FULL_BUNDLE_FILE file"

rm -rf temp
mkdir temp

SCRIPT_NAME=build_working_folder.sh
echo "Entering $SCRIPT_NAME"

pwd
# This working folder name starts with 'temp/'
echo "$SCRIPT_NAME: Working folder: $FOLDER"
mkdir $FOLDER
echo $BUNDLE_FULL_NAME > $FOLDER/bundle_name.ini

CONSOLE_FOLDER="$FOLDER/console"
DRIVERS_FOLDER="$FOLDER/drivers"
mkdir $CONSOLE_FOLDER
mkdir $DRIVERS_FOLDER
ls -l $FOLDER

wget https://rusefi.com/build_server/st_files/silent_st_drivers.exe -P $DRIVERS_FOLDER
[ -e $DRIVERS_FOLDER/silent_st_drivers.exe ] || (echo "$SCRIPT_NAME: ERROR DOWNLOADING silent_st_drivers.exe"; exit 1)

if [ "$INI_FILE_OVERRIDE" = "no" ]; then
    INI_FILE_OVERRIDE="rusefi.ini"
    echo "$SCRIPT_NAME: N/a ini_file_override specified"
fi

if [ -z $INI_FILE_OVERRIDE ]; then
    INI_FILE_OVERRIDE="rusefi.ini"
    echo "$SCRIPT_NAME: No ini_file_override specified"
fi
echo "$SCRIPT_NAME: Will use $INI_FILE_OVERRIDE"

if [ -z $RUSEFI_CONSOLE_SETTINGS ]; then
  echo "$SCRIPT_NAME: No rusefi_console_settings"
else
  echo "Using rusefi_console_settings [$RUSEFI_CONSOLE_SETTINGS]"
  cp $RUSEFI_CONSOLE_SETTINGS $CONSOLE_FOLDER
fi

cp java_console/rusefi_autoupdate.jar     $CONSOLE_FOLDER
cp java_console_binary/rusefi_console.jar $CONSOLE_FOLDER
cp java_tools/ts_plugin_launcher/build/jar/rusefi_plugin_launcher.jar $CONSOLE_FOLDER
cp simulator/build/rusefi_simulator.exe   $CONSOLE_FOLDER
cp misc/console_launcher/rusefi_*.exe     $CONSOLE_FOLDER
cp java_console/rusefi.xml                $CONSOLE_FOLDER

cp misc/console_launcher/readme.html $FOLDER

cp firmware/tunerstudio/$INI_FILE_OVERRIDE $FOLDER
# Unsetting since would not be used anywhere else
INI_FILE_OVERRIDE=""
RUSEFI_CONSOLE_SETTINGS=""

# users probably do not really care for this file
# cp firmware/svnversion.h %folder%

cp -r misc/install/openocd $CONSOLE_FOLDER
cp -r misc/install/DfuSe $CONSOLE_FOLDER
# 407 has additional version of firmware
cp firmware/deliver/rusefi_no_asserts.bin $FOLDER
cp firmware/deliver/rusefi_no_asserts.dfu $FOLDER
# 746 builds one version at the moment
# probably not needed cp firmware/build/rusefi.hex %folder%
cp firmware/deliver/rusefi.bin $FOLDER
# probably not needed cp firmware/build/rusefi.elf %folder%
cp firmware/deliver/rusefi.dfu $FOLDER

if [ -n $BUNDLE_NAME ]; then
    mv $FOLDER/rusefi.dfu $FOLDER/rusefi_$BUNDLE_NAME.dfu
fi


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
