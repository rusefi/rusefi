@echo off
sh.exe prepare_bundle.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe prepare_bundle.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe prepare_bundle.sh )
  )
)
