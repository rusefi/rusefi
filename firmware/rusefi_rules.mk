# rusefi_rules.mk - this one is shared by all four configurations (bootloader, embedded, simulator and unit_tests)


# Warnings-as-errors...
RUSEFI_OPT = -Werror
# some compilers seem to have this off by default? (GCC-specific, not supported by Clang)
ifneq ($(USE_CLANG),yes)
  RUSEFI_OPT += -Werror=stringop-truncation
endif

ifneq ($(ALLOW_SHADOW),yes)
     RUSEFI_OPT += -Werror=shadow
endif

# ...except these few
RUSEFI_OPT += -Wno-error=sign-compare
# Clang-specific: ChibiOS uses null pointer subtraction for offsetof-like macros
ifeq ($(USE_CLANG),yes)
  RUSEFI_OPT += -Wno-error=null-pointer-subtraction
  # Clang warns about treating .h PCH input as c++-header in C++ mode
  RUSEFI_OPT += -Wno-error=deprecated
  # Missing 'override' keyword on virtual method overrides
  RUSEFI_OPT += -Wno-error=inconsistent-missing-override
  # Clang fires this on unique_ptr with non-virtual dtor
  RUSEFI_OPT += -Wno-error=delete-non-abstract-non-virtual-dtor
  # Implicit float-to-int constant conversions
  RUSEFI_OPT += -Wno-error=implicit-const-int-float-conversion
  # Unused const variables in headers
  RUSEFI_OPT += -Wno-error=unused-const-variable
  # Unused private fields
  RUSEFI_OPT += -Wno-error=unused-private-field
  # Variables that may be uninitialized on some code paths
  RUSEFI_OPT += -Wno-error=sometimes-uninitialized
endif
RUSEFI_OPT += -Wno-error=overloaded-virtual
RUSEFI_OPT += -Wno-error=unused-parameter

# todo 'delete-non-abstract-non-virtual-dtor issue or not #8476'
# reduce clutter while compiling unit tests
RUSEFI_OPT += -Wno-delete-non-abstract-non-virtual-dtor

# Build .srec for flash_can.sh and flash_usb.sh helpers
SREC = true

# Board-specific generated header overrides - shared by all four configurations.
# SHORT_BOARD_NAME is resolved at build time (deferred expansion).
DDEFS += -DMETA_GENERATED_H_OVERRIDE=\"rusefi_generated_$(SHORT_BOARD_NAME).h\"
DDEFS += -DMETA_ENGINE_GENERATED_H_OVERRIDE=\"engine_configuration_generated_structures_$(SHORT_BOARD_NAME).h\"
DDEFS += -DSIGNATURE_H_OVERRIDE=\"signature_$(SHORT_BOARD_NAME).h\"
