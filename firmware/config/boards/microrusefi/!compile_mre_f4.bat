@echo off
sh.exe compile_mre_f4.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe compile_mre_f4.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe compile_mre_f4.sh )
  )
)
