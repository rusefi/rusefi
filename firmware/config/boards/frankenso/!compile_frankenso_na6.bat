@echo off
sh.exe compile-frankenso-na6.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe compile-frankenso-na6.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe compile-frankenso-na6.sh )
  )
)
