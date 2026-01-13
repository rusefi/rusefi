/*
 * gm_ls_4.cpp
 *
 */

#include "gm_ls_4.h"
#include "defaults.h"
#include <rusefi/arrays.h>
#include "proteus_meta.h"
#include "settings.h"

#ifdef HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

void setGmLs4() {
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_GM);
	strcpy(engineConfiguration->engineCode, "gen4");
	engineConfiguration->globalTriggerAngleOffset = 86;
	engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;

  engineConfiguration->etbIdleThrottleRange = 30;

    engineConfiguration->cranking.rpm = 400;
    engineConfiguration->rpmHardLimit = 6000;

      engineConfiguration->etb.pFactor = 7.320831;
      engineConfiguration->etb.iFactor = 116.5986;
      engineConfiguration->etb.dFactor = 0.0765;
      engineConfiguration->etb.minValue = -90;
      engineConfiguration->etb.maxValue = 90;


#if HW_PROTEUS
	engineConfiguration->etbFunctions[1] = DC_None;

	setPPSInputs(PROTEUS_IN_ANALOG_VOLT_2, PROTEUS_IN_ANALOG_VOLT_11);
	setTPS1Inputs(PROTEUS_IN_ANALOG_VOLT_4, PROTEUS_IN_ANALOG_VOLT_3);

// todo: tps
#endif //HW_PROTEUS

#if defined(HW_HELLEN_8CHAN)
	engineConfiguration->injectionPins[4] = Gpio::MM176_INJ5;
	engineConfiguration->injectionPins[5] = Gpio::MM176_INJ6;
	engineConfiguration->injectionPins[6] = Gpio::MM176_INJ7;
	engineConfiguration->injectionPins[7] = Gpio::MM176_INJ8;
	engineConfiguration->ignitionPins[4] = Gpio::MM176_IGN5;
	engineConfiguration->ignitionPins[5] = Gpio::MM176_IGN6;
	engineConfiguration->ignitionPins[6] = Gpio::MM176_IGN7;
	engineConfiguration->ignitionPins[7] = Gpio::MM176_IGN8;

  engineConfiguration->vvtPins[0] = Gpio::MM176_OUT_PWM1; // 8D - VVT 1
  engineConfiguration->map.sensor.hwChannel = MM176_IN_CRANK_ANALOG;
  engineConfiguration->triggerInputPins[0] = Gpio::MM176_IN_D4; // 9A
  engineConfiguration->camInputs[1] = Gpio::Unassigned;
  engineConfiguration->camInputs[2] = Gpio::Unassigned;
  engineConfiguration->camInputs[3] = Gpio::Unassigned;
#endif

#ifdef HW_HELLEN_UAEFI121
  engineConfiguration->camInputs[1] = Gpio::Unassigned;
	// cylinders 1 and 6
	// cylinders 2 and 3
  engineConfiguration->ignitionPins[2] = Gpio::Unassigned;
  engineConfiguration->ignitionPins[3] = Gpio::Unassigned;
  engineConfiguration->ignitionPins[4] = Gpio::Unassigned;
  engineConfiguration->ignitionPins[5] = Gpio::Unassigned;
	// cylinders 7 and 4
	engineConfiguration->ignitionPins[6] = Gpio::MM100_IGN4;
	// cylinders 8 and 5
	engineConfiguration->ignitionPins[7] = Gpio::MM100_IGN3;
#endif

#ifdef HW_HELLEN_SUPER_UAEFI
	engineConfiguration->triggerInputPins[0] = Gpio::MM100_IN_D2; // HALL2

	engineConfiguration->camInputs[1] = Gpio::Unassigned;
#endif

#ifdef HW_HELLEN_UAEFI
	engineConfiguration->injectionPins[6] = Gpio::MM100_OUT_PWM1;
	engineConfiguration->injectionPins[7] = Gpio::MM100_INJ8;

    engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	// cylinders 1 and 6
	engineConfiguration->ignitionPins[0] = Gpio::MM100_IGN1;
	// cylinders 2 and 3
	engineConfiguration->ignitionPins[1] = Gpio::MM100_IGN2;
	engineConfiguration->ignitionPins[2] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[4] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[5] = Gpio::Unassigned;
	// cylinders 7 and 4
	engineConfiguration->ignitionPins[6] = Gpio::MM100_IGN4;
	// cylinders 8 and 5
	engineConfiguration->ignitionPins[7] = Gpio::MM100_IGN3;


	engineConfiguration->triggerInputPins[0] = Gpio::MM100_IN_D1; // HALL1
	engineConfiguration->invertPrimaryTriggerSignal = true;

	engineConfiguration->camInputs[0] = Gpio::MM100_IN_D2; // HALL2
	engineConfiguration->camInputs[1] = Gpio::Unassigned;

#endif

	engineConfiguration->fuelReferencePressure = 400; // 400 kPa, 58 psi
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
	engineConfiguration->injector.flow = 400;

  engineConfiguration->tpsAccelLookback = 0.3;
  engineConfiguration->tpsAccelEnrichmentThreshold = 5;
  engineConfiguration->tpsDecelEnleanmentThreshold = 7;
  engineConfiguration->tpsAccelFractionPeriod = 3;
  engineConfiguration->tpsAccelFractionDivisor = 1.5;
  engineConfiguration->wwaeTau = 0;
  engineConfiguration->wwaeBeta = 0;

	engineConfiguration->cylindersCount = 8;
	setLeftRightBanksNeedBetterName();
	engineConfiguration->firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->displacement = 6.2;
	engineConfiguration->map.sensor.type = MT_GM_1_BAR;

	engineConfiguration->tChargeAirIncrLimit = 5;
	engineConfiguration->tChargeAirDecrLimit = 15;

// see https://github.com/rusefi/rusefi_documentation/tree/master/OEM-Docs/GM/Tahoe-2011
    setLuaScript( R"(

function getBitRange(data, bitIndex, bitWidth)
	byteIndex = bitIndex >> 3
	shift = bitIndex - byteIndex * 8
	value = data[1 + byteIndex]
	if (shift + bitWidth > 8) then
		value = value + data[2 + byteIndex] * 256
	end
	mask = (1 << bitWidth) - 1
	return (value >> shift) & mask
end

hexstr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "A", "B", "C", "D", "E", "F" }

function toHexString(num)
	if num == 0 then
		return '0'
	end

	local result = ""
	while num > 0 do
		local n = num % 16
		result = hexstr[n + 1] ..result
		num = math.floor(num / 16)
	end
	return result
end

function arrayToString(arr)
	local str = ""
	local index = 1
	while arr[index] ~= nil do
		str = str.." "..toHexString(arr[index])
		index = index + 1
	end
	return str
end

STARTER_OUTPUT_INDEX = 0
startPwm(STARTER_OUTPUT_INDEX, 100, 0)

-- 201
ECMEngineStatus = 0xC9
IGN_STATUS = 0x1f1
-- 0x514
VIN_Part1 = 1300
-- 04E1
VIN_Part2 = 1249

setTickRate(100)

function canIgnStatus(bus, id, dlc, data)
    crankingBits = getBitRange(data, 2, 2)
    isCranking = (crankingBits == 2)
-- need special considerations to append boolean   print('crankingBits ' .. crankingBits .. ', isCranking ' .. isCranking)
    print('crankingBits ' .. crankingBits)
end

function printAny(bus, id, dlc, data)
    print('packet ' .. id)
end

canRxAdd(IGN_STATUS, canIgnStatus)
-- canRxAddMask(0, 0xFFFFFFF, printAny)

-- todo: take VIN from configuration? encode VIN?
canVin1    = { 0x47, 0x4E, 0x4C, 0x43, 0x32, 0x45, 0x30, 0x34 }
canVin2    = { 0x42, 0x52, 0x32, 0x31, 0x36, 0x33, 0x36, 0x36 }
dataECMEngineStatus = { 0x84, 0x09, 0x99, 0x0A, 0x00, 0x40, 0x08, 0x00 }

-- todo: smarter loop code :)
canVin1[1] = vin(1)
canVin1[2] = vin(2)
canVin1[3] = vin(3)
canVin1[4] = vin(4)
canVin1[5] = vin(5)
canVin1[6] = vin(6)
canVin1[7] = vin(7)
canVin1[8] = vin(8)

canVin2[1] = vin(9)
canVin2[2] = vin(10)
canVin2[3] = vin(11)
canVin2[4] = vin(12)
canVin2[5] = vin(13)
canVin2[6] = vin(14)
canVin2[7] = vin(15)
canVin2[8] = vin(16)

function onTick()
    txCan(1, VIN_Part1, 0, canVin1)
    txCan(1, VIN_Part2, 0, canVin2)

    -- good enough for fuel module!
    txCan(1, ECMEngineStatus, 0, dataECMEngineStatus)

    if isCranking then
        setPwmDuty(STARTER_OUTPUT_INDEX, 1)
    else
        setPwmDuty(STARTER_OUTPUT_INDEX, 0)
    end
end

    )");

	setPPSCalibration(0.51, 2.11, 1.01, 4.23);
	setTPS1Calibration(880, 129, 118, 870);
}
