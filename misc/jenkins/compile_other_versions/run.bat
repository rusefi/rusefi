
set FTP_SERVER=home451478433.1and1-data.host

pwd

call misc\jenkins\build_java_console.bat
if not exist java_console_binary/rusefi_console.jar exit -1



cd firmware
call clean.bat
cd ..

echo Compiling F767
cd firmware\config\boards\NUCLEO_F746
call !compile-nucleo_f746.bat
cd ..
pwd


rm -rf temp
set stm_arch=stm32f467
set folder=snapshot_%date:~10%%date:~4,2%%date:~7,2%_%time:~0,2%%time:~3,2%_%stm_arch%_rusefi
set folder=temp\%folder%

rem this replaces spaces with 0s - that's needed before 10am
set folder=%folder: =0%

echo Building file
call misc\jenkins\build_working_folder.bat

rem TODO: extract FTP duplication with 407 build

echo open ftp://u71977750-build:%RUSEFI_BUILD_FTP_PASS%@%FTP_SERVER%/ > ftp_commands.txt
echo binary >> ftp_commands.txt
echo put rusefi_bundle.zip rusefi_bundle_F746.zip >> ftp_commands.txt

echo exit >> ftp_commands.txt

cd temp
call winscp.com /script=../ftp_commands.txt
IF NOT ERRORLEVEL 0 echo winscp error DETECTED
IF NOT ERRORLEVEL 0 EXIT /B 1


echo OK for now

