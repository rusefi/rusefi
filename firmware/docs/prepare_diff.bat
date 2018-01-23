
git clone -b stable_16.1.x https://github.com/rusefi/ChibiOS Chibios.16_original
git clone -b stable_rusefi https://github.com/rusefi/ChibiOS Chibios.16_rusefi
diff -uwr Chibios.16_rusefi Chibios.16_original > rusefi_chibios_16.patch
diff -uwr Chibios.16_original Chibios.16_rusefi > chibios_rusefi_16.patch

