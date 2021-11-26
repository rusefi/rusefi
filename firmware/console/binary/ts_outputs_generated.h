// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Fri Nov 26 18:48:47 EST 2021
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of ts_outputs_s
struct ts_outputs_s {
	/**
	offset 0 bit 0 */
	bool sd_present : 1 {};
	/**
	offset 0 bit 1 */
	bool isIgnitionEnabledIndicator : 1 {};
	/**
	offset 0 bit 2 */
	bool isInjectionEnabledIndicator : 1 {};
	/**
	offset 0 bit 3 */
	bool sd_logging_internal : 1 {};
	/**
	offset 0 bit 4 */
	bool isCylinderCleanupActivated : 1 {};
	/**
	offset 0 bit 5 */
	bool isFuelPumpOn : 1 {};
	/**
	offset 0 bit 6 */
	bool isFanOn : 1 {};
	/**
	offset 0 bit 7 */
	bool isO2HeaterOn : 1 {};
	/**
	offset 0 bit 8 */
	bool checkEngine : 1 {};
	/**
	offset 0 bit 9 */
	bool needBurn : 1 {};
	/**
	offset 0 bit 10 */
	bool sd_msd : 1 {};
	/**
	offset 0 bit 11 */
	bool clutchUpState : 1 {};
	/**
	offset 0 bit 12 */
	bool clutchDownState : 1 {};
	/**
	offset 0 bit 13 */
	bool isFan2On : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedb14 : 1 {};
	/**
	offset 0 bit 15 */
	bool brakePedalState : 1 {};
	/**
	offset 0 bit 16 */
	bool toothLogReady : 1 {};
	/**
	offset 0 bit 17 */
	bool acSwitchState : 1 {};
	/**
	offset 0 bit 18 */
	bool isTpsError : 1 {};
	/**
	offset 0 bit 19 */
	bool isCltError : 1 {};
	/**
	offset 0 bit 20 */
	bool isMapError : 1 {};
	/**
	offset 0 bit 21 */
	bool isIatError : 1 {};
	/**
	offset 0 bit 22 */
	bool acState : 1 {};
	/**
	offset 0 bit 23 */
	bool isTriggerError : 1 {};
	/**
	offset 0 bit 24 */
	bool hasCriticalError : 1 {};
	/**
	offset 0 bit 25 */
	bool isWarnNow : 1 {};
	/**
	offset 0 bit 26 */
	bool isPedalError : 1 {};
	/**
	offset 0 bit 27 */
	bool isKnockChipOk : 1 {};
	/**
	offset 0 bit 28 */
	bool launchTriggered : 1 {};
	/**
	offset 0 bit 29 */
	bool isTps2Error : 1 {};
	/**
	offset 0 bit 30 */
	bool isIdleClosedLoop : 1 {};
	/**
	offset 0 bit 31 */
	bool isIdleCoasting : 1 {};
	/**
	 * offset 4
	 */
	scaled_channel<uint16_t, 1, 1> rpm = (uint16_t)0;
	/**
	 * offset 6
	 */
	uint16_t rpmAcceleration = (uint16_t)0;
	/**
	 * offset 8
	 */
	scaled_channel<uint16_t, 100, 1> speedToRpmRatio = (uint16_t)0;
	/**
	 * offset 10
	 */
	scaled_channel<uint8_t, 1, 1> vehicleSpeedKph = (uint8_t)0;
	/**
	 * offset 11
	 */
	scaled_channel<uint8_t, 1, 1> internalMcuTemperature = (uint8_t)0;
	/**
	 * offset 12
	 */
	scaled_channel<uint16_t, 100, 1> coolantTemperature = (uint16_t)0;
	/**
	 * offset 14
	 */
	scaled_channel<uint16_t, 100, 1> intakeAirTemperature = (uint16_t)0;
	/**
	 * offset 16
	 */
	scaled_channel<uint16_t, 100, 1> auxTemp1 = (uint16_t)0;
	/**
	 * offset 18
	 */
	scaled_channel<uint16_t, 100, 1> auxTemp2 = (uint16_t)0;
	/**
	 * offset 20
	 */
	scaled_channel<uint16_t, 100, 1> throttlePosition = (uint16_t)0;
	/**
	 * offset 22
	 */
	scaled_channel<uint16_t, 100, 1> pedalPosition = (uint16_t)0;
	/**
	 * offset 24
	 */
	uint16_t tpsADC = (uint16_t)0;
	/**
	 * offset 26
	 */
	scaled_channel<uint16_t, 1000, 1> massAirFlowVoltage = (uint16_t)0;
	/**
	 * offset 28
	 */
	scaled_channel<uint16_t, 10, 1> massAirFlow = (uint16_t)0;
	/**
	 * offset 30
	 */
	scaled_channel<uint16_t, 30, 1> manifoldAirPressure = (uint16_t)0;
	/**
	 * offset 32
	 */
	scaled_channel<uint16_t, 30, 1> baroPressure = (uint16_t)0;
	/**
	 * offset 34
	 */
	scaled_channel<uint16_t, 10000, 1> lambda = (uint16_t)0;
	/**
	 * offset 36
	 */
	scaled_channel<uint8_t, 10, 1> knockRetard = (uint8_t)0;
	/**
	 * offset 37
	 */
	uint8_t unused37 = (uint8_t)0;
	/**
	 * offset 38
	 */
	scaled_channel<uint16_t, 1000, 1> vBatt = (uint16_t)0;
	/**
	 * offset 40
	 */
	scaled_channel<uint16_t, 30, 1> oilPressure = (uint16_t)0;
	/**
	 * offset 42
	 */
	scaled_channel<uint16_t, 50, 1> vvtPositionB1I = (uint16_t)0;
	/**
	 * offset 44
	 */
	scaled_channel<uint16_t, 1000, 1> chargeAirMass = (uint16_t)0;
	/**
	 * airmass in mg, 0-65 grams
	 * offset 46
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelMass = (uint16_t)0;
	/**
	 * offset 48
	 */
	scaled_channel<uint16_t, 1000, 1> currentTargetAfr = (uint16_t)0;
	/**
	 * This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	 * offset 50
	 */
	scaled_channel<uint16_t, 100, 1> fuelBase = (uint16_t)0;
	/**
	 * Total fuel with CLT IAT and TPS acceleration without injector lag corrections per cycle, as pulse per cycle
	 * offset 52
	 */
	scaled_channel<uint16_t, 100, 1> fuelRunning = (uint16_t)0;
	/**
	 * Actual last injection time - including all compensation and injection mode
	 * offset 54
	 */
	scaled_channel<uint16_t, 300, 1> actualLastInjection = (uint16_t)0;
	/**
	 * offset 56
	 */
	scaled_channel<uint8_t, 2, 1> injectorDutyCycle = (uint8_t)0;
	/**
	 * offset 57
	 */
	scaled_channel<uint8_t, 2, 1> veValue = (uint8_t)0;
	/**
	 * offset 58
	 */
	scaled_channel<uint16_t, 50, 1> injectionOffset = (uint16_t)0;
	/**
	 * offset 60
	 */
	scaled_channel<uint16_t, 100, 1> tCharge = (uint16_t)0;
	/**
	 * offset 62
	 */
	scaled_channel<uint16_t, 300, 1> injectorLagMs = (uint16_t)0;
	/**
	 * offset 64
	 */
	scaled_channel<uint16_t, 100, 1> iatCorrection = (uint16_t)0;
	/**
	 * offset 66
	 */
	scaled_channel<uint16_t, 100, 1> cltCorrection = (uint16_t)0;
	/**
	 * offset 68
	 */
	scaled_channel<uint16_t, 100, 1> baroCorrection = (uint16_t)0;
	/**
	 * offset 70
	 */
	scaled_channel<uint16_t, 1, 1> currentEnginePhase = (uint16_t)0;
	/**
	 * offset 72
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelAmount = (uint16_t)0;
	/**
	 * offset 74
	 */
	scaled_channel<uint16_t, 100, 1> wallFuelCorrection = (uint16_t)0;
	/**
	 * offset 76
	 */
	scaled_channel<uint16_t, 1, 1> unused76 = (uint16_t)0;
	/**
	 * TPS acceleration enrichment
	 * offset 78
	 */
	scaled_channel<uint16_t, 100, 1> deltaTps = (uint16_t)0;
	/**
	 * offset 80
	 */
	scaled_channel<uint16_t, 1, 1> unused80 = (uint16_t)0;
	/**
	 * offset 82
	 */
	scaled_channel<uint16_t, 300, 1> tpsAccelFuel = (uint16_t)0;
	/**
	 * offset 84
	 */
	scaled_channel<uint16_t, 50, 1> ignitionAdvance = (uint16_t)0;
	/**
	 * offset 86
	 */
	scaled_channel<uint16_t, 300, 1> sparkDwell = (uint16_t)0;
	/**
	 * offset 88
	 */
	scaled_channel<uint16_t, 100, 1> coilDutyCycle = (uint16_t)0;
	/**
	 * offset 90
	 */
	scaled_channel<uint16_t, 100, 1> idlePosition = (uint16_t)0;
	/**
	 * offset 92
	 */
	scaled_channel<uint16_t, 100, 1> etbTarget = (uint16_t)0;
	/**
	 * offset 94
	 */
	scaled_channel<uint16_t, 100, 1> etb1DutyCycle = (uint16_t)0;
	/**
	 * offset 96
	 */
	scaled_channel<uint16_t, 100, 1> etb1Error = (uint16_t)0;
	/**
	 * offset 98
	 */
	scaled_channel<uint16_t, 100, 1> fuelTankLevel = (uint16_t)0;
	/**
	 * offset 100
	 */
	scaled_channel<uint16_t, 1, 1> totalFuelConsumption = (uint16_t)0;
	/**
	 * offset 102
	 */
	scaled_channel<uint16_t, 200, 1> fuelFlowRate = (uint16_t)0;
	/** total size 104*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) console/binary/output_channels.txt Fri Nov 26 18:48:47 EST 2021
