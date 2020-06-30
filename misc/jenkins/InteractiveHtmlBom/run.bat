@echo off
sh.exe run.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe run.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe run.sh )
  )
)
