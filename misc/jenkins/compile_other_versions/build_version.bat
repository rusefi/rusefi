set script_name=build_version.bat
echo Entering %script_name% with %bundle_name%


rm -rf temp
rem This depends on Cygwin date copied under 'datecyg' name to avoid conflict with Windows date
rem By the way, '%%' is the way to escape % in batch files
rem this is copy-pasted at build_current_bundle.bat
for /f %%i in ('datecyg +%%Y%%m%%d_%%H%%M%%S') do set TIMESTAMP=%%i

set folder=snapshot_%TIMESTAMP%_%bundle_name%_rusefi
set folder=temp\%folder%

echo Packaging temp\rusefi_bundle.zip file
call misc\jenkins\build_working_folder.bat

rem TODO: extract FTP duplication with 407 build

cd temp
set bundle_file=rusefi_bundle_%bundle_name%.zip
mv rusefi_bundle.zip %bundle_file%

echo Uploading %bundle_file%
ncftpput -u %RUSEFI_BUILD_FTP_USER% -p %RUSEFI_BUILD_FTP_PASS% %RUSEFI_FTP_SERVER% . %bundle_file%

cd ..

pwd
echo "exiting %script_name%"
