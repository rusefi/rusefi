@echo off

:: BatchGotAdmin
:-------------------------------------
REM  --> Check for permissions
    IF "%PROCESSOR_ARCHITECTURE%" EQU "amd64" (
>nul 2>&1 "%SYSTEMROOT%\SysWOW64\cacls.exe" "%SYSTEMROOT%\SysWOW64\config\system"
) ELSE (
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
)

REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    set params= %*
    echo UAC.ShellExecute "cmd.exe", "/c ""%~s0"" %params:"=""%", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    pushd "%CD%"
    CD /D "%~dp0"
:--------------------------------------    

cd "Virtual comport driver"
cd Win7

echo Let's install VCP driver silently
if "%PROCESSOR_ARCHITEW6432%" == "AMD64" goto VCP_X64
if "%PROCESSOR_ARCHITECTURE%" == "AMD64" goto VCP_X64
start "" dpinst_x86.exe /s
goto VCP_END
:VCP_X64
start "" dpinst_amd64.exe /s
:VCP_END

cd ../..
echo Done installing VCP silently
pwd

echo Let's install ST-Link driver silently
cd ST-LINK_USB_V2_1_Driver

@echo off
if "%PROCESSOR_ARCHITEW6432%" == "AMD64" goto ST_X64
if "%PROCESSOR_ARCHITECTURE%" == "AMD64" goto ST_X64
start "" dpinst_x86.exe /sw
goto ST_END
:ST_X64
start "" dpinst_amd64.exe /sw
:ST_END

cd ..
echo Done installing ST-Link silently
pwd

cd DFU_Driver
STM32Bootloader.bat
cd ..

exit