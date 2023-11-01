# Warnings-as-errors...
RUSEFI_OPT = -Werror -Werror=shadow

# ...except these few
RUSEFI_OPT += -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter
