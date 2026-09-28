@echo off
setlocal EnableExtensions DisableDelayedExpansion

set "firmware="
for %%F in ("%~dp0..\..\..\rusefi*.bin") do (
    if exist "%%~fF" if not exist "%%~fF\" (
        if defined firmware (
            echo Error: More than one rusefi*.bin file found in "%~dp0..\..\..".
            exit /b 1
        )
        set "firmware=%%~fF"
    )
)

if not defined firmware (
    echo Error: No rusefi*.bin file found in "%~dp0..\..\..".
    exit /b 1
)

"%~dp0STM32_Programmer_CLI.exe" -c port=usb1 -w "%firmware%" 0x08000000 --verify --start 0x08000000
exit /b %errorlevel%
