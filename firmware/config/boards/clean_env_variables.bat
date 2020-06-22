@echo off
sh.exe clean_env_variables.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe clean_env_variables.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe clean_env_variables.sh )
  )
)
