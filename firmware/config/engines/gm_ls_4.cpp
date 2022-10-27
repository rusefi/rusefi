/*
 * gm_ls_4.cpp
 *
 */

#include "gm_ls_4.h"
#include "defaults.h"

void setGmLs4() {
	engineConfiguration->globalTriggerAngleOffset = 86;

    // would not hurt just in case no cam
	engineConfiguration->twoWireBatchInjection = true;


	engineConfiguration->fuelReferencePressure = 400; // 400 kPa, 58 psi
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
	engineConfiguration->injector.flow = 440;

	engineConfiguration->specs.cylindersCount = 8;
	setLeftRightBanksNeedBetterName();
	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->specs.displacement = 6.2;

	engineConfiguration->useETBforIdleControl = true;
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
    print('crankingBits ' .. crankingBits .. ', isCranking ' .. isCranking)
end

canRxAdd(IGN_STATUS, canIgnStatus)

-- todo: take VIN from configuration? encode VIN?
canVin1    = { 0x47, 0x4E, 0x4C, 0x43, 0x32, 0x45, 0x30, 0x34 }
canVin2    = { 0x42, 0x52, 0x32, 0x31, 0x36, 0x33, 0x36, 0x36 }
dataECMEngineStatus = { 0x84, 0x09, 0x99, 0x0A, 0x00, 0x40, 0x08, 0x00 }

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

}

void setProteusGmLs4() {
	setGmLs4();

}
