java -jar ../../rusefi_console.jar reboot_dfu
echo Now sleeping before DFU
sleep 5

STM32_Programmer_CLI.exe -c port=usb1 -rdu