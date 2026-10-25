#include "pch.h"

#include "defaults.h"
#include "mazda_miata_nc.h"

void setMazdaMiataNC() {
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MAZDA);
	strcpy(engineConfiguration->engineCode, "LF-VE");
	strcpy(engineConfiguration->vehicleName, "Miata NC");

	setInline4();
	engineConfiguration->displacement = 2.0;
	setCrankOperationMode();
	engineConfiguration->trigger.type = trigger_type_e::TT_36_2_2_2;
	engineConfiguration->vvtMode[0] = VVT_MAZDA_L;
	// Calibrate globalTriggerAngleOffset and vvtOffsets[0] on the engine.
	// No VVT output is assigned for this initial start-up preset.

	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->ignitionDwellForCrankingMs = 4;

	// Stock returnless fuel system: regulator in the tank, referenced to atmosphere.
	// 2008 Mazda LF manual: 204-216 cc/min at 350-410 kPa. Use the midpoints.
	// https://www.ncmiata-servicemanual.com/src/g5w01/html/id011445800700.html
	// https://www.ncmiata-servicemanual.com/src/g5w01/html/id011445800500.html
	engineConfiguration->injector.flow = 210;
	engineConfiguration->fuelReferencePressure = 380;
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

	// Retain the shared base maps, injector lag and board wiring defaults.
	// TPS/PPS, MAP and temperature sensor calibrations require vehicle verification.
}
