@echo off
sh.exe build_working_folder.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe build_working_folder.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe build_working_folder.sh )
  )
)
