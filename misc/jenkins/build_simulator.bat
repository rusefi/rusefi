@echo off
sh.exe build_simulator.sh || (
  if exist C:\cygwin64 (
    C:\cygwin64\bin\sh.exe build_simulator.sh
  ) else (
    if exist C:\cygwin ( C:\cygwin\bin\sh.exe build_simulator.sh )
  )
)
