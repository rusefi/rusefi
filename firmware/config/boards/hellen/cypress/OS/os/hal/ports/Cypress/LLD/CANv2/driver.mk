ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_CAN TRUE,$(HALCONF)),)
PLATFORMSRC += ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/CANv2/hal_can_lld.c
endif
else
PLATFORMSRC += ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/CANv2/hal_can_lld.c
endif

PLATFORMINC += ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/CANv2
