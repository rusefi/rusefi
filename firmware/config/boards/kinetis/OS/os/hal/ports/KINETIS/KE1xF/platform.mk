ifeq ($(KINETIS_CONTRIB),)
  KINETIS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

PLATFORMSRC_CONTRIB := ${CHIBIOS}/os/hal/ports/common/ARMCMx/nvic.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/hal_lld.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/kinetis_irq.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/clock_config.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_acmp.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_adc12.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_clock.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_dmamux.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_edma.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_lpspi.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_lpspi_edma.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_lpuart.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_lpuart_edma.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl/fsl_smc.c \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/PITv2/hal_st_lld.c
                       
PLATFORMINC_CONTRIB := ${CHIBIOS}/os/hal/ports/common/ARMCMx \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF \
                       ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/KE1xF/fsl

include ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/DMAv2/driver.mk
include ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/GPIOv2/driver.mk
include ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/UARTv2/driver.mk
include ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/SPIv2/driver.mk
include ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/FTMv2/driver.mk
include ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/ADCv2/driver.mk
include ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/COMPv2/driver.mk
include ${KINETIS_CONTRIB}/os/hal/ports/KINETIS/LLD/PITv2/driver.mk

# Shared variables
ALLCSRC += $(PLATFORMSRC_CONTRIB)
ALLINC  += $(PLATFORMINC_CONTRIB)
