ifeq ($(CYPRESS_CONTRIB),)
  CYPRESS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_PAL TRUE,$(HALCONF)),)
PLATFORMSRC_CONTRIB += ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/GPIOv2/hal_pal_lld.c
endif
else
PLATFORMSRC_CONTRIB += ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/GPIOv2/hal_pal_lld.c
endif

PLATFORMINC_CONTRIB += ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/GPIOv2
