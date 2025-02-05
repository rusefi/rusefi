// This file asserts that the compiler is appropriate for rusEFI use.

// non-MCU builds are significantly more tolerant
#if EFI_PROD_CODE

#define GCC_VERSION ((__GNUC__ * 100) + (__GNUC_MINOR__ * 10) + ( __GNUC_PATCHLEVEL__ ))

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#pragma message("GCC is " STR(__GNUC__)"."STR(__GNUC_MINOR__)"."STR(__GNUC_PATCHLEVEL__))

// Firmware builds require at least GCC 11.3.1
#if (GCC_VERSION < 1131)
	#error "GCC compiler >= 11.3.1 required"
#endif

#if (GCC_VERSION > 1400)
	#error "Looks like we support only 11 and 12?"
#endif


#endif
