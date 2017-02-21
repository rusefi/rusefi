

echo This script should be executed from the root of rusEfi master GITHUB local copy
pwd 

echo Updating from SVN
call git pull

set RUSEFI_SVN_PATH=../../rusefi.svn/



ls -l %RUSEFI_SVN_PATH%
rd /s /q %RUSEFI_GIT_PATH%\firmware
rem rd /s /q %RUSEFI_GIT_PATH%\hardware
rem rd /s /q %RUSEFI_GIT_PATH%\java_console
rem rd /s /q %RUSEFI_GIT_PATH%\unit_tests
rem rd /s /q %RUSEFI_GIT_PATH%\win32_functional_tests

cp -r firmware %RUSEFI_SVN_PATH%
cp -r hardware %RUSEFI_SVN_PATH%
rem cp -r java_console %RUSEFI_GIT_PATH%
rem cp -r unit_tests %RUSEFI_GIT_PATH%
rem cp -r win32_functional_tests %RUSEFI_GIT_PATH%
rem rm -f %RUSEFI_GIT_PATH%/readme.*
rem rm -f %RUSEFI_GIT_PATH%/README.*
rem cp -r README.* %RUSEFI_GIT_PATH%
rem cp -r readme.* %RUSEFI_GIT_PATH%


rem echo Going to git copy location


rem cd %RUSEFI_GIT_PATH%

rem git config --global user.email "russianefi@gmail.com"
rem git config --global user.name "rusEfi"

rem git pull
rem git add *
rem git commit -a -m "auto-sync"
rem git push --repo https://rusefi:PASSWORD@github.com/rusefi/rusefi
