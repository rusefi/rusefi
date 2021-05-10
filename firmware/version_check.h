// This file asserts that the compiler is appropriate for rusEFI use.

#pragma once

// non-MCU builds are significantly more tolerant
#if EFI_PROD_CODE

static constexpr auto gccVersion = (__GNUC__ * 100) + (__GNUC_MINOR__ * 10) + ( __GNUC_PATCHLEVEL__ );

// Firmware builds require at least GCC 9.3.1
static_assert(gccVersion >= 931, "GCC compiler >= 9.3.1 required");

#endif
