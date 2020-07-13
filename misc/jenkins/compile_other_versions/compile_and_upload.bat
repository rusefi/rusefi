@echo off
bash.exe misc\jenkins\compile_other_versions\compile.sh %1 %2 %3 %4
IF %ERRORLEVEL% NEQ 0 EXIT /B 1
pwd

bash.exe misc\jenkins\compile_other_versions\prepare_bundle.sh %1 %2 %3 %4
IF %ERRORLEVEL% NEQ 0 EXIT /B 1
pwd
