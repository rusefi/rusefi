
rem on linux that would be
rem dfu-util -a 0 -D rusefi_release.dfu -R

rem ..\misc\hex2dfu\HEX2DFU.exe build/rusefi.hex   -out build/rusefi.dfu
rem DfuSeCommand.exe -c -d --fn build/rusefi.dfu

rem to read from comtroller use
rem DfuSeCommand.exe -c -u --fn downloaded.dfu

rem Note that DfuSe_Demo_V3.0.6_Setup.exe does not install "stm32 bootloader" device drivers automaticaly
rem You need to point Windows at a relevant folder inside C:\Program Files (x86)\STMicroelectronics\Software\DfuSe v3.0.6\Bin\Driver

DfuSeCommand.exe -c -d --fn rusefi.dfu
