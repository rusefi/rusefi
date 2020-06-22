@echo off
sh.exe compile-kinetis.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe compile-kinetis.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe compile-kinetis.sh )
  )
)
