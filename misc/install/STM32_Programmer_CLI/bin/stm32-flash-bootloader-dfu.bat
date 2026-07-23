for %%F in (../../../bin/device/openblt_*.bin) do STM32_Programmer_CLI.exe -c port=usb1 -w "%%F" 0x08000000 --verify --start 0x08000000
