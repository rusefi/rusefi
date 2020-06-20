@echo off
sh.exe compile_and_upload.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe compile_and_upload.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe compile_and_upload.sh )
  )
)
