echo I am simulator_test.bat
pwd

cd java_console

echo Running simulator functional test
which java
which javac
ant simlator_test

IF NOT ERRORLEVEL echo ERROR DETECTED
IF NOT ERRORLEVEL 0 EXIT /B 1

echo "TIMESTAMP %date% %time%"
echo simulator_test.bat: done
