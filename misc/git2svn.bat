rem See also git2svn_init.bat


echo This script should be executed from the root of rusEfi master GITHUB local copy
pwd 

echo Updating from GIT
call git fetch --all
call git reset --hard origin/master
IF NOT ERRORLEVEL echo ERROR git pull-ing
IF NOT ERRORLEVEL 0 EXIT /B 1


set RUSEFI_SVN_PATH=../../svn/
set RUSEFI_SVN_W_PATH=..\..\svn
set RUSEFI_GIT_PATH=../git/rusefi


mkdir -p %RUSEFI_SVN_PATH%"
echo "Going to %RUSEFI_SVN_PATH%" 
cd %RUSEFI_SVN_PATH%
echo Updating from SVN
svn up
cd %RUSEFI_GIT_PATH%


ls -l %RUSEFI_SVN_PATH%
rd /s /q %RUSEFI_SVN_W_PATH%\firmware
rd /s /q %RUSEFI_SVN_W_PATH%\hardware
rd /s /q %RUSEFI_SVN_W_PATH%\java_console
rd /s /q %RUSEFI_SVN_W_PATH%\unit_tests
rd /s /q %RUSEFI_SVN_W_PATH%\misc
rd /s /q %RUSEFI_SVN_W_PATH%\simulator

cp -r firmware      %RUSEFI_SVN_PATH%
cp -r hardware      %RUSEFI_SVN_PATH%
cp -r java_console  %RUSEFI_SVN_PATH%
cp -r unit_tests    %RUSEFI_SVN_PATH%
cp -r misc          %RUSEFI_SVN_PATH%
cp -r simulator     %RUSEFI_SVN_PATH%
rem rm -f %RUSEFI_GIT_PATH%/readme.*
rem rm -f %RUSEFI_GIT_PATH%/README.*
cp -r README.*               %RUSEFI_SVN_PATH%
cp -r readme.*               %RUSEFI_SVN_PATH%


cd %RUSEFI_SVN_PATH%
pwd

rem http://stackoverflow.com/questions/2120844/how-do-i-add-all-new-files-to-svn
svn add --force .

svn ci  --no-auth-cache --non-interactive --username rusefi --password %RUSEFI_SVN_PASSWORD% -m "auto-merge from github"


