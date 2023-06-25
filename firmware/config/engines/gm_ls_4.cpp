/*
 * gm_ls_4.cpp
 *
 */

#include "gm_ls_4.h"
#include "defaults.h"
#include <rusefi/arrays.h>
#include "proteus_meta.h"

void setGmLs4() {
	engineConfiguration->globalTriggerAngleOffset = 86;

	engineConfiguration->fuelReferencePressure = 400; // 400 kPa, 58 psi
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
	engineConfiguration->injector.flow = 440;

	engineConfiguration->cylindersCount = 8;
	setLeftRightBanksNeedBetterName();
	engineConfiguration->firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->displacement = 6.2;

	engineConfiguration->etbIdleThrottleRange = 15;

	engineConfiguration->tChargeAirIncrLimit = 5;
	engineConfiguration->tChargeAirDecrLimit = 15;

    strncpy(config->luaScript, R"(

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

    )", efi::size(config->luaScript));

	setPPSCalibration(0.51, 2.11, 1.01, 4.23);
	setTPS1Calibration(880, 129, 118, 870);
}

void setProteusGmLs4() {
#if HW_PROTEUS
	engineConfiguration->etbFunctions[1] = DC_None;

	engineConfiguration->mainRelayPin = PROTEUS_LS_12;
	setPPSInputs(PROTEUS_IN_ANALOG_VOLT_2, PROTEUS_IN_ANALOG_VOLT_11);
	setTPS1Inputs(PROTEUS_IN_ANALOG_VOLT_4, PROTEUS_IN_ANALOG_VOLT_3);

// todo: tps
#endif //HW_PROTEUS
	setGmLs4();
}
