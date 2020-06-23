@echo off
sh.exe compile_frankenso_na6.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe compile_frankenso_na6.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe compile_frankenso_na6.sh )
  )
)
