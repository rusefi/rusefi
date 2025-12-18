# rusefi_rules.mk

# Warnings-as-errors...
RUSEFI_OPT = -Werror
# some compilers seem to have this off by default?
RUSEFI_OPT += -Werror=stringop-truncation

ifneq ($(ALLOW_SHADOW),yes)
     RUSEFI_OPT += -Werror=shadow
endif

# ...except these few
RUSEFI_OPT += -Wno-error=sign-compare
RUSEFI_OPT += -Wno-error=overloaded-virtual
RUSEFI_OPT += -Wno-error=unused-parameter

# todo 'delete-non-abstract-non-virtual-dtor issue or not #8476'
# reduce clutter while compiling unit tests
RUSEFI_OPT += -Wno-delete-non-abstract-non-virtual-dtor

# Build .srec for flash_can.sh and flash_usb.sh helpers
SREC = true
