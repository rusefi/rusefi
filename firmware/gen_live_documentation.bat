@echo off
sh.exe gen_live_documentation.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe gen_live_documentation.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe gen_live_documentation.sh )
  )
)
