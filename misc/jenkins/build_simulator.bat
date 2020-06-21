@echo off
sh.exe misc\jenkins\build_simulator.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe misc\jenkins\build_simulator.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe misc\jenkins\build_simulator.sh )
  )
)
