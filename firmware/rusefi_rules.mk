# Warnings-as-errors...
RUSEFI_OPT = -Werror

ifneq ($(ALLOW_SHADOW),yes)
#     RUSEFI_OPT += -Werror=shadow
endif

# ...except these few
RUSEFI_OPT += -Wno-error=unused-function
RUSEFI_OPT += -Wno-error=sign-compare
RUSEFI_OPT += -Wno-error=unused-parameter
