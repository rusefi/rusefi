# Warnings-as-errors...
RUSEFI_OPT = -Werror

ifneq ($(ALLOW_SHADOW),yes)
     RUSEFI_OPT += -Werror=shadow
endif

# ...except these few
RUSEFI_OPT += -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter
