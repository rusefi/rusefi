@echo off
sh.exe compile_stm32f746_nucleo.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe compile_stm32f746_nucleo.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe compile_stm32f746_nucleo.sh )
  )
)
