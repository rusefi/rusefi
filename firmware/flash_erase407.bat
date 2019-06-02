rem st-link_cli -c SWD ur -ME
rem 0x100000 would erase both code and configuration
rem use 0x080000 if you want to erase only configuratio
set script_name=flash_erase407.bat
echo Entering %script_name%


rem weird, it used to be much nicer with openocd 0.8.0, file location was not broken?
rem maybe https://sourceforge.net/p/openocd/tickets/105/ ?

pwd
cd ../misc/install

if not exist openocd/openocd.exe echo openocd/openocd.exe NOT FOUND
if not exist openocd/openocd.exe exit -1


rem newer discovery boards
echo Invoking openocd...
openocd\openocd.exe -f openocd/stm32f4discovery.cfg -c init -c targets -c "halt" -c "flash erase_address 0x08000000 0x0100000" -c shutdown
IF NOT ERRORLEVEL 0 echo ERROR in %script_name%
IF NOT ERRORLEVEL 0 EXIT /B 1

echo Just invoked openocd to erase chip!


rem older disocovery boards or cheap eBay ST-Link
rem openocd\openocd.exe -f openocd/stm32f4discovery.cfg -c init -c targets -c "halt" -c "flash erase_address 0x08000000 0x0100000" -c shutdown

echo "exiting %script_name%"
