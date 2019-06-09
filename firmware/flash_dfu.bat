
rem on linux that would be
rem dfu-util -a 0 -D rusefi_no_asserts.dfu -R

DfuSeCommand.exe -c -d --fn build/rusefi.dfu

rem to read from comtroller use
rem DfuSeCommand.exe -c -u --fn downloaded.dfu

rem Note that DfuSe_Demo_V3.0.6_Setup.exe does not install "stm32 bootloader" device drivers automaticaly
rem You need to point Windows at a relevant folder inside C:\Program Files (x86)\STMicroelectronics\Software\DfuSe v3.0.6\Bin\Driver

rem DfuSeCommand.exe -c -d --fn rusefi.dfu
