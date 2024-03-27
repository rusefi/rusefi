# Warnings-as-errors...
RUSEFI_OPT = -Werror
# some compilers seem to have this off by default?
RUSEFI_OPT += -Werror=stringop-truncation

ifneq ($(ALLOW_SHADOW),yes)
#     RUSEFI_OPT += -Werror=shadow
endif

# ...except these few
RUSEFI_OPT += -Wno-error=sign-compare
RUSEFI_OPT += -Wno-error=overloaded-virtual
RUSEFI_OPT += -Wno-error=unused-parameter
RUSEFI_OPT += -Wno-error=unused-function
