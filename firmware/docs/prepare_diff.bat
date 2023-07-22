rem #
rem # see also ../../misc/git_cheat_sheet.txt
rem # both 'rusefi/ChibiOS/original' and 'rusefi/ChibiOS/rusefi' should have upstream branch changes merged at the same time
rem #
rem # comparing against rusEfi version of the unmodified in order to compare remote branches on the same timestamp
rem #

git clone -b stable_18.2.x        https://github.com/rusefi/ChibiOS Chibios.18_original
git -C Chibios.18_original pull
git clone -b stable_18.2.rusefi   https://github.com/rusefi/ChibiOS Chibios.18_rusefi
git -C Chibios.18_rusefi pull

git clone -b stable_20.3.x        https://github.com/rusefi/ChibiOS Chibios.20_original
git -C Chibios.20_original pull
git clone -b stable_20.3.x.rusefi https://github.com/rusefi/ChibiOS Chibios.20_rusefi
git -C Chibios.20_rusefi pull

diff -uwr Chibios.18_original Chibios.18_rusefi > chibios_rusefi_18.patch
diff -uwr Chibios.20_original Chibios.20_rusefi > chibios_rusefi_20.patch

