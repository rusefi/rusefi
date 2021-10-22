@echo off
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


exit