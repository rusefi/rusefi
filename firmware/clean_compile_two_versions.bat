@echo off
sh.exe clean_compile_two_versions.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe clean_compile_two_versions.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe clean_compile_two_versions.sh )
  )
)
