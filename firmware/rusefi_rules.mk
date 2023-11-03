# Warnings-as-errors...
RUSEFI_OPT = -Werror
RUSEFI_CPPOPT = -Werror=shadow

# ...except these few
RUSEFI_OPT += -Wno-error=unused-function
RUSEFI_OPT += -Wno-error=sign-compare
RUSEFI_OPT += -Wno-error=unused-parameter
RUSEFI_OPT += -Wno-error=undef
RUSEFI_OPT += -Wno-error=stringop-overflow
