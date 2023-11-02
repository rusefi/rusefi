# Warnings-as-errors...
RUSEFI_OPT = -Werror
RUSEFI_CPPOPT = -Werror=shadow

# ...except these few
RUSEFI_OPT += -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=undef -Wno-error=stringop-overflow
