@echo off
sh.exe clean.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe clean.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe clean.sh )
  )
)
