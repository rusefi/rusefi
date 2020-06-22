@echo off
sh.exe update_version.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe update_version.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe update_version.sh )
  )
)
