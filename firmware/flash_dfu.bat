

rem on linux that would be
rem dfu-util -a 0 -D rusefi_no_asserts.dfu -R

"../misc/install/STM32_Programmer_CLI/bin/STM32_Programmer_CLI.exe" -c port=usb1 -w deliver/rusefi.hex --verify --start 0x08000000

