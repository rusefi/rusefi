// This file asserts that the compiler is appropriate for rusEFI use.

// non-MCU builds are significantly more tolerant
#if EFI_PROD_CODE

#define GCC_VERSION ((__GNUC__ * 100) + (__GNUC_MINOR__ * 10) + ( __GNUC_PATCHLEVEL__ ))

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// Firmware builds require at least GCC 9.3.1
#if (GCC_VERSION < 931)
	#pragma message("GCC is " STR(__GNUC__)"."STR(__GNUC_MINOR__)"."STR(__GNUC_PATCHLEVEL__))
	#error "GCC compiler >= 9.3.1 required"
#endif

// Firmware builds but does not actually run on GCC10 :(
// todo: one day someone should fix it, do YOU want to help?
#if (GCC_VERSION > 1000)
	#pragma message("GCC is " STR(__GNUC__)"."STR(__GNUC_MINOR__)"."STR(__GNUC_PATCHLEVEL__))
	#error "GCC10 not supported see https://github.com/rusefi/rusefi/labels/Gcc10"
#endif

#endif
