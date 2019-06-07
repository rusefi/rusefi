set script_name=build_version.bat
echo Entering %script_name% with %bundle_name%


rm -rf temp
set folder=snapshot_%date:~10%%date:~4,2%%date:~7,2%_%time:~0,2%%time:~3,2%_%bundle_name%_rusefi
set folder=temp\%folder%

rem this replaces spaces with 0s - that's needed before 10am
set folder=%folder: =0%

echo Packaging temp\rusefi_bundle.zip file
call misc\jenkins\build_working_folder.bat

rem TODO: extract FTP duplication with 407 build

cd temp
set bundle_file=rusefi_bundle_%bundle_name%.zip
mv rusefi_bundle.zip %bundle_file%

echo Uploading %bundle_file%
ncftpput -u %RUSEFI_BUILD_FTP_USER% -p %RUSEFI_BUILD_FTP_PASS% %FTP_SERVER% . %bundle_file%

cd ..

pwd
echo "exiting %script_name%"
