@echo off
sh.exe misc\jenkins\compile_other_versions\compile_and_upload.sh %1 %2 %3 %4 || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe misc\jenkins\compile_other_versions\compile_and_upload.sh %1 %2 %3 %4
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe misc\jenkins\compile_other_versions\compile_and_upload.sh %1 %2 %3 %4 )
  )
)
