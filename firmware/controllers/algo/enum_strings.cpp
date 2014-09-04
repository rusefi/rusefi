/**
 * @file enum_strings.cpp
 *
 * Maybe we will figure out a way to auto-generate this file?
 *
 * @date Sep 4, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "enum_strings.h"
#include "error_handling.h"


const char* getConfigurationName(engine_type_e engineType) {
	switch (engineType) {
#if EFI_SUPPORT_DODGE_NEON
	case DODGE_NEON_1995:
		return "Dodge Neon";
#endif /* EFI_SUPPORT_DODGE_NEON */
#if EFI_SUPPORT_FORD_ASPIRE
	case FORD_ASPIRE_1996:
		return "Ford Aspire";
#endif /* EFI_SUPPORT_FORD_ASPIRE */
#if EFI_SUPPORT_FORD_FIESTA
	case FORD_FIESTA:
		return "Ford Fiesta";
#endif /* EFI_SUPPORT_FORD_FIESTA */
#if EFI_SUPPORT_NISSAN_PRIMERA
	case NISSAN_PRIMERA:
		return "Nissan Primera";
#endif /* EFI_SUPPORT_NISSAN_PRIMERA */
	case HONDA_ACCORD_CD:
		return "Honda Accord 3w";
	case HONDA_ACCORD_CD_TWO_WIRES:
		return "Honda Accord 2w";
	case HONDA_ACCORD_CD_DIP:
		return "Honda Dip";
	case FORD_INLINE_6_1995:
		return "Ford 1995 inline 6";
	case GY6_139QMB:
		return "Gy6 139qmb";
	case MAZDA_MIATA_NB:
		return "Mazda Miata NB";
	case MAZDA_323:
		return "Mazda 323";
	case SATURN_ION_2004:
		return "Saturn Ion";
	case MINI_COOPER_R50:
		return "Mini Cooper R50";
	case FORD_ESCORT_GT:
		return "Ford Escort GT";
	case CITROEN_TU3JP:
		return "Citroen TU3JP";
	case ROVER_V8:
		return "Rover v8";
	case MITSU_4G93:
		return "Mitsu 4G93";
	case MIATA_1990:
		return "Miata 1990";
	case MIATA_1994:
		return "Miata 1994";
	case MIATA_1996:
		return "Miata 1996";
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

const char * algorithmToString(engine_load_mode_e mode) {
	switch(mode) {
	case LM_TPS:
		return "Alpha-N";
	case LM_MAF:
		return "Maf";
	case LM_MAP:
		return "pMap";
	case LM_SPEED_DENSITY:
		return "SD";
	default:
		return "unexpected";
	}
}
