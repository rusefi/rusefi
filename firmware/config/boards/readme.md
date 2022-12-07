# Boards Directory

rusEFI supports quite a wide array of hardware - we support stm32f4 and we support stm32h7, we have [kinetis](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/k-series-cortex-m4:KINETIS_K_SERIES) and we support cypress MCUs. We support on-board a number of smart GPIO chips, overall we support quite a wide array of hardware. With all those options our goal is to provide nice _user_ experience which means upfront investment by _board designer_.

For best user experience we have more than a dozen of different _binaries_ which are all compiled from same _firmware_ files with different configuration. This folder is all about that process.


See [misc/jenkins/compile_other_versions/run.bat](misc/jenkins/compile_other_versions/run.bat) which is executed by build server.


See https://rusefi.com/build_server/

By definition, BOARD_NAME is a folder in firmware\config\boards

One BOARD_NAME could be producing a number of artifacts via compile_$BUNDLE_NAME.sh scripts

Work in progress: SHORT_BOARDNAME becomes BUNDLE_NAME

New board procedure once you have new folder in this *boards* directory

1) update [gen_config.sh](https://github.com/rusefi/rusefi/blob/master/firmware/gen_config.sh): add "board board" pair into 'for BOARD' iteration - this would produce new signature*.h file and new rusefi*.ini file

2) update [build-firmware.yaml](https://github.com/rusefi/rusefi/blob/master/.github/workflows/build-firmware.yaml) to get new firmware bundle on https://rusefi.com/build_server/

3) add connector pinout mapping [yaml](https://en.wikipedia.org/wiki/YAML) file see examples of yaml files in 'connectors' subfolders here.


See also https://github.com/rusefi/rusefi/wiki/Hardware


## FAQ

# Q: how do I change Primary UART pins via TS?

A: Primary UART, critical error LED pin and maybe a couple more of the most fundamental pins could NOT be chagned via TS. You would need another binary altogether - either another official binary or a custom binary.

# Q: I need more info!

A: please remember about https://github.com/rusefi/rusefi/wiki/Hardware#q-this-is-all-very-cool-but-you-guys-do-not-have-a-plugplay-for-my-trabant-i-think-i-will-go-and-make-a-new-rusefi-board-just-for-my-trabant

