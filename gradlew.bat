@echo off
setlocal
call "%~dp0java_tools\gradlew.bat" -p "%~dp0." %*
exit /b %errorlevel%
