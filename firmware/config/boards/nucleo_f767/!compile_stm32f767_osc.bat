@echo off
sh.exe compile_stm32f767_osc.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe compile_stm32f767_osc.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe compile_stm32f767_osc.sh )
  )
)
