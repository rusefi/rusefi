rem Depends on ST Link and ST VCP being installed manually

call clean_staging_folder.bat


rem https://github.com/rusefi/rusefi_external_utils/blob/master/stsw-stm32102_1_4_0.zip
cp -r "C:\Program Files (x86)\STMicroelectronics\Software\Virtual comport driver" silent_st_drivers




rem compress 'silent_st_drivers' folder
"C:\Program Files\7-Zip\7z.exe" a silent_vcp_driver.exe -mmt -mx5 -sfx silent_st_drivers
