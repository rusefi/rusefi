rem Depends on ST Link and ST VCP being installed manually

rm -rf silent_st_drivers/ST-LINK_USB_V2_1_Driver
rm -rf silent_st_drivers/"Virtual comport driver"

cp -r "C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK_USB_V2_1_Driver" silent_st_drivers
rem https://github.com/rusefi/rusefi_external_utils/blob/master/stsw-stm32102_1_4_0.zip
cp -r "C:\Program Files (x86)\STMicroelectronics\Software\Virtual comport driver" silent_st_drivers

rem compress 'silent_st_drivers' folder
"C:\Program Files\7-Zip\7z.exe" a silent_st_drivers.exe -mmt -mx5 -sfx silent_st_drivers