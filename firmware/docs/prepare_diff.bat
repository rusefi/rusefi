rem #
rem # see also ../../misc/git_cheat_sheet.txt
rem # both 'rusefi/ChibiOS/original' and 'rusefi/ChibiOS/rusefi' should have upstream branch changes merged at the same time
rem #
rem # comparing against rusEfi version of the unmodified in order to compare remote branches on the same timestamp
rem #

rem git clone -b stable_16.1.x https://github.com/rusefi/ChibiOS Chibios.16_original
rem git -C Chibios.16_original pull
rem git clone -b stable_rusefi https://github.com/rusefi/ChibiOS Chibios.16_rusefi
rem git -C Chibios.16_rusefi pull
rem diff -uwr Chibios.16_rusefi Chibios.16_original > rusefi_chibios_16.patch
rem diff -uwr Chibios.16_original Chibios.16_rusefi > chibios_rusefi_16.patch

git clone -b stable_20.3.x        https://github.com/rusefi/ChibiOS Chibios.20_original
git -C Chibios.20_original pull
git clone -b stable_20.3.x.rusefi https://github.com/rusefi/ChibiOS Chibios.20_rusefi
git -C Chibios.20_rusefi pull

diff -uwr Chibios.20_original Chibios.20_rusefi > chibios_rusefi_20.patch
