include ${CHIBIOS}/os/hal/ports/STM32/STM32F7xx/platform.mk

PLATFORMSRC += ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/LLD/FSMCv1/hal_fsmc.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/LLD/FSMCv1/hal_nand_lld.c \
               ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/LLD/FSMCv1/hal_fsmc_sram.c \
               ${CHIBIOS_CONTRIB}/os/hal/src/hal_fsmc_sdram.c

PLATFORMINC += ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/LLD/FSMCv1 \
               ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/LLD
