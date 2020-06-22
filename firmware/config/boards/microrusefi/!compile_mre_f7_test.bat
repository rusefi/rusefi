@echo off
sh.exe compile-mre-f7-test.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe compile-mre-f7-test.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe compile-mre-f7-test.sh )
  )
)
