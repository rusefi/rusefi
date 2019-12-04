rem 
rem STM32F469 version of the firmware for https://rusefi.com/forum/viewtopic.php?f=4&t=1215
rem

cd ../../..
set PROJECT_BOARD=prometheus/f469
set USE_BOOTLOADER=yes

call config/boards/common_make.bat

