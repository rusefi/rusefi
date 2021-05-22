# Kinetis OS

Standard NXP headers 
  firmware/config/boards/kinetis/OS/os/common/ext/CMSIS/KINETIS


To address Kinetis #e10536 errata we need to sleep 100 cycles to make watchdog happy.
This is taken care by  
  firmware/config/boards/kinetis/OS/os/common/startup/ARMCMx/compilers/GCC/startup_ke1xf.S
which is executed prior to the default firmware/ChibiOS/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S

Linker script for 512K ROM
  firmware/config/boards/kinetis/OS/os/common/startup/ARMCMx/compilers/GCC/ld/MKE1xF512.ld
  
flash1a & flash1b is some hardware configuration magic
  _bca is flash1a via .bcaconfig
  _cfm is flash1b via .cfmconfig


firmware/config/boards/kinetis/OS/os/hal/ports/KINETIS/KE1xF/fsl contains Kinetis SDK files with modifications:
 1) include hal.h in the beginning
 2) firmware/config/boards/kinetis/OS/os/hal/ports/KINETIS/KE1xF/fsl/fsl_lpuart.c has UART patch (see AndreiKA tag)
