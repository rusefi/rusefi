#
# see also ../../misc/git_cheat_sheet.txt
# both 'rusefi/ChibiOS/original' and 'rusefi/ChibiOS/rusefi' should have upstream branch changes merged at the same time
#
# comparing against rusEfi version of the unmodified in order to compare remote branches on the same timestamp
#

git clone -b stable_16.1.x https://github.com/rusefi/ChibiOS Chibios.16_original
git -C Chibios.16_original pull
git clone -b stable_rusefi https://github.com/rusefi/ChibiOS Chibios.16_rusefi
git -C Chibios.16_rusefi pull
diff -uwr Chibios.16_rusefi Chibios.16_original > rusefi_chibios_16.patch
diff -uwr Chibios.16_original Chibios.16_rusefi > chibios_rusefi_16.patch

 
git clone -b stable_17.6.x https://github.com/rusefi/ChibiOS Chibios.17_original
git -C Chibios.17_original pull
git clone -b stable_17.6.rusefi https://github.com/rusefi/ChibiOS Chibios.17_rusefi
git -C Chibios.17_rusefi pull


diff -uwr Chibios.17_original Chibios.17_rusefi > chibios_rusefi_17.patch
