/**
 * @file enum_strings.cpp
 *
 * Maybe we will figure out a way to auto-generate this file?
 *
 * @date Sep 4, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "enum_strings.h"
#include "error_handling.h"

/**
 * These should be not very long because these are displayed on the LCD as is
 */
const char* getConfigurationName(engine_type_e engineType) {
	switch (engineType) {
        case CUSTOM_ENGINE:
          return "CUSTOM";
#if EFI_SUPPORT_DODGE_NEON
	case DODGE_NEON_1995:
		return "Neon95";
	case DODGE_NEON_2003:
		return "Neon03";
#endif /* EFI_SUPPORT_DODGE_NEON */
#if EFI_SUPPORT_FORD_ASPIRE
	case FORD_ASPIRE_1996:
		return "Aspire";
#endif /* EFI_SUPPORT_FORD_ASPIRE */
#if EFI_SUPPORT_FORD_FIESTA
	case FORD_FIESTA:
		return "Fiesta";
#endif /* EFI_SUPPORT_FORD_FIESTA */
#if EFI_SUPPORT_NISSAN_PRIMERA
	case NISSAN_PRIMERA:
		return "Primera";
#endif /* EFI_SUPPORT_NISSAN_PRIMERA */
	case HONDA_ACCORD_CD:
		return "Accord3";
	case HONDA_ACCORD_CD_TWO_WIRES:
		return "Accord2";
	case HONDA_ACCORD_CD_DIP:
		return "HondaD";
	case FORD_INLINE_6_1995:
		return "Fordi6";
	case GY6_139QMB:
		return "Gy6139";
	case MAZDA_MIATA_NB:
		return "MiataNB";
	case MAZDA_323:
		return "M323";
	case SATURN_ION_2004:
		return "Saturn Ion";
	case MINI_COOPER_R50:
		return "CoopR50";
	case FORD_ESCORT_GT:
		return "EscrtGT";
	case CITROEN_TU3JP:
		return "TU3JP";
	case ROVER_V8:
		return "Rvrv8";
	case MITSU_4G93:
		return "Mi4G93";
	case MIATA_1990:
		return "MX590";
	case MIATA_1994_DEVIATOR:
		return "MX594d";
	case MIATA_1994_SPAGS:
		return "MX594s";
	case MIATA_1996:
		return "MX596";
	case BMW_E34:
		return "BMWe34";
        case TEST_ENGINE:
                return "Test";
	default:
		firmwareError("Unexpected: engineType %d", engineType);
		return NULL;
	}
}

const char * pinModeToString(pin_output_mode_e mode) {
	switch (mode) {
	case OM_DEFAULT:
		return "default";
	case OM_INVERTED:
		return "inverted";
	case OM_OPENDRAIN:
		return "open drain";
	case OM_OPENDRAIN_INVERTED:
		return "open drain inverted";
	default:
		return "unexpected";
	}
}
