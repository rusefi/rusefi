echo Statding test_bundle.bat

cd java_console
rem call ant autotest
rem IF NOT ERRORLEVEL 0 echo auto test ERROR DETECTED
rem IF NOT ERRORLEVEL 0 EXIT /B 1
cd ..

pwd
echo Cleaning
cd firmware
call clean.bat
cd ..

cd misc
call hw_test.bat
IF NOT ERRORLEVEL 0 echo real hardware test ERROR DETECTED
IF NOT ERRORLEVEL 0 EXIT /B 1
cd ..
pwd

echo %date% %time%
echo test_bundle.bat: done