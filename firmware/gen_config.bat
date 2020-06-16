@echo off
if exist C:\cygwin64 (
 C:\cygwin64\bin\bash.exe gen_config.sh
) else (
 if exist C:\cygwin ( C:\cygwin\bin\bash.exe gen_config.sh )
)
