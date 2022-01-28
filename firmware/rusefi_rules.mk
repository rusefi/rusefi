# Warnings-as-errors...
RUSEFI_OPT = -Werror

# ...except these few
RUSEFI_OPT += -Wno-error=unused-function -Wno-error=sign-compare
