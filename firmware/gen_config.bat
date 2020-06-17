@echo off
sh.exe gen_config.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe gen_config.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe gen_config.sh )
  )
)


rem This would automatically copy latest file to 'dev' TS project
set ts_path="%HOMEDRIVE%%HOMEPATH%\Documents\TunerStudioProjects"
echo %ts_path%
cp tunerstudio/rusefi.ini %ts_path%\dev\projectCfg\mainController.ini
cp tunerstudio/rusefi_microrusefi.ini %ts_path%\dev_mre\projectCfg\mainController.ini