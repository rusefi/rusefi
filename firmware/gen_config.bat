@echo off
bash.exe gen_config.sh

rem This would automatically copy latest file to 'dev' TS project
set ts_path="%HOMEDRIVE%%HOMEPATH%\Documents\TunerStudioProjects"
echo %ts_path%
cp tunerstudio/generated/rusefi.ini %ts_path%\dev\projectCfg\mainController.ini
cp tunerstudio/generated/rusefi_microrusefi.ini %ts_path%\dev_mre\projectCfg\mainController.ini
