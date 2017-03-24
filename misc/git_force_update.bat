
echo going to Root of SVN project
cd ..

set RUSEFI_GIT_PATH=..\rusefi.git

cd %RUSEFI_GIT_PATH%

git fetch --all
git reset --hard origin/master