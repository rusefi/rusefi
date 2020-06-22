@echo off
sh.exe common_make.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe common_make.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe common_make.sh )
  )
)
