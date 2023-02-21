@echo off

call compile.bat

rem svn info > ../firmware_binary/version.txt
rem cp config/features.h ../firmware_binary
rem cp build/fome.hex ../firmware_binary
rem cp build/fome.elf ../firmware_binary

rem cp tunerstudio/fome.ini ../firmware_binary

rem cd ../firmware_binary
rem del firmaware_binary.zip
rem 7z a firmaware_binary.zip fome.hex fome.ini features.h flash.bat
rem cd ../firmware


echo Looking at size
arm-none-eabi-size  --format=berkeley "build\fome.elf"
echo Compiler gcc version
arm-none-eabi-gcc -v

echo Have the file, let's program the board right away
call flash_openocd

echo Done flashing                       
