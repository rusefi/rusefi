@echo off
sh.exe compile_mre_f4_hardware_QC_special_build.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe compile_mre_f4_hardware_QC_special_build.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe compile_mre_f4_hardware_QC_special_build.sh )
  )
)
