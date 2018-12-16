

echo This script should be executed from the root of rusEfi trunk SVN local copy
pwd 

echo Updating from SVN
call svn update

set RUSEFI_GIT_PATH=../rusefi.github/rusefi



ls -l %RUSEFI_GIT_PATH%
rd /s /q %RUSEFI_GIT_PATH%\firmware
rd /s /q %RUSEFI_GIT_PATH%\hardware
rd /s /q %RUSEFI_GIT_PATH%\java_console
rd /s /q %RUSEFI_GIT_PATH%\unit_tests
rd /s /q %RUSEFI_GIT_PATH%\simulator

cp -r firmware %RUSEFI_GIT_PATH%
cp -r hardware %RUSEFI_GIT_PATH%
cp -r java_console %RUSEFI_GIT_PATH%
cp -r unit_tests %RUSEFI_GIT_PATH%
cp -r simulator %RUSEFI_GIT_PATH%
rm -f %RUSEFI_GIT_PATH%/readme.*
rm -f %RUSEFI_GIT_PATH%/README.*
cp -r README.* %RUSEFI_GIT_PATH%
cp -r readme.* %RUSEFI_GIT_PATH%


echo Going to git copy location


cd %RUSEFI_GIT_PATH%

git config --global user.email "russianefi@gmail.com"
git config --global user.name "rusEfi"

git pull
git add *
git commit -a -m "auto-sync"
git push --repo https://rusefi:PASSWORD@github.com/rusefi/rusefi
