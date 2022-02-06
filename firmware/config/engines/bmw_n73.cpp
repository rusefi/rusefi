/*
 * bmw_n73.cpp
 *
 * @date Oct 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "bmw_n73.h"

void setEngineProteusBMW_N73_GDI() {

}

void setEngineProteusGearboxManInTheMiddle() {
	strncpy(config->luaScript, R"(

function twoBytes(data, offset, factor)
		return (data[offset + 2] * 256 + data[offset + 1]) * factor
end

--
-- crazy copy-pasta, at some point move to sets/bags

tcuMessages = {}
-- 'ecu' means not-TCU
ecuMessages = {}

CAN_BMW_E90_TORQUE_1 = 0x0A8
CAN_BMW_E90_TORQUE_2 = 0x0A9
CAN_BMW_E90_RPM_THROTTLE = 0x0AA
CAN_BMW_E90_DSC_TORQUE_DEMAND = 0x0B6
CAN_BMW_E90_WHEEL_SPEED = 0x0CE
CAN_BMW_E90_IGNITION_KEY = 0x130
CAN_BMW_E65_GEAR_SELECTOR = 0x192
CAN_BMW_E90_DSC_STATUS = 0x19E
CAN_BMW_E90_DSC_SPEED = 0x1A0
CAN_BMW_E90_COOLANT = 0x1D0
-- 1E1 klima?
CAN_BMW_E90_LOCKING = 0x2FC
-- 308 Status MSA ECU
-- 310 outside temperature Kombi
-- 330 fuel, range
CAN_BMW_E90_DASH_ON = 0x332
-- 34F
-- 0x3EF	OBD Daten Motor ECU
-- CAN_BMW_E90_ECU_NETWORK = 0x492
-- CAN_BMW_E90_ECU2_NETWORK = 0x493

CAN_BMW_GEAR_TORQUE_DEMAND2 = 0x0B5
CAN_BMW_GEAR_TRANSMISSION_DATA = 0x0BA
CAN_BMW_GEAR_GEARBOX_DATA_2 = 0x1A2
CAN_BMW_GEAR_TRANSMISSION_DISP = 0x1D2
CAN_BMW_GEAR_GANG_STATUS = 0x304
CAN_BMW_GEAR_NETWORK = 0x498
CAN_BMW_GEAR_SERVICE = 0x598

ECU_BUS = 1
GEAR_BUS = 2

canRxAdd(CAN_BMW_E90_TORQUE_1)
canRxAdd(CAN_BMW_E90_TORQUE_2)
canRxAdd(CAN_BMW_E90_RPM_THROTTLE)
canRxAdd(CAN_BMW_E90_DSC_TORQUE_DEMAND)
canRxAdd(CAN_BMW_E90_WHEEL_SPEED)
canRxAdd(CAN_BMW_E90_IGNITION_KEY)
canRxAdd(CAN_BMW_E65_GEAR_SELECTOR)
canRxAdd(CAN_BMW_E90_DSC_STATUS)
canRxAdd(CAN_BMW_E90_DSC_SPEED)
canRxAdd(CAN_BMW_E90_COOLANT)
canRxAdd(CAN_BMW_E90_LOCKING)
canRxAdd(CAN_BMW_E90_DASH_ON)

canRxAdd(CAN_BMW_GEAR_TORQUE_DEMAND2)
canRxAdd(CAN_BMW_GEAR_TRANSMISSION_DATA)
canRxAdd(CAN_BMW_GEAR_GEARBOX_DATA_2)
canRxAdd(CAN_BMW_GEAR_TRANSMISSION_DISP)
canRxAdd(CAN_BMW_GEAR_GANG_STATUS)
canRxAdd(CAN_BMW_GEAR_NETWORK)
canRxAdd(CAN_BMW_GEAR_SERVICE)


function relayToTcu(id, data)
	txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
end

function relayToEcu(id, data)
	txCan(ECU_BUS, id, 0, data) -- relay non-ECU message to ECU
end

function printDebug(msg)
    print(msg)
end

function onCanRx(bus, id, dlc, data)
	id = id % 2048
	-- local output = string.format("%x", id)

	if id == CAN_BMW_E90_TORQUE_1 then
		TORQ_AVL = 0.5 * (twoBytes(data, 1, 1) >> 4)
		TORQ_AVL_DMEE = 0.5 * (twoBytes(data, 3, 1) >> 4)
		print('CAN_BMW_E90_TORQUE_1 TORQ_AVL=' .. TORQ_AVL .. ' TORQ_AVL_DMEE=' .. TORQ_AVL_DMEE)
		relayToTcu(id, data)
    elseif id == CAN_BMW_E90_TORQUE_2 then
		printDebug('CAN_BMW_E90_TORQUE_2')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_RPM_THROTTLE then
		rpm = twoBytes(data, 4, 0.25)
		print('CAN_BMW_E90_RPM_THROTTLE rpm=' .. rpm)
		relayToTcu(id, data)
    elseif id == CAN_BMW_E90_DSC_TORQUE_DEMAND then
		printDebug('CAN_BMW_E90_DSC_TORQUE_DEMAND')
		relayToTcu(id, data)
    elseif id == CAN_BMW_E90_WHEEL_SPEED then
		printDebug('CAN_BMW_E90_WHEEL_SPEED')
		relayToTcu(id, data)
    elseif id == CAN_BMW_E90_IGNITION_KEY then
    	printDebug('!!!!!!!!!!!!! CAN_BMW_E90_IGNITION_KEY')
    	relayToTcu(id, data)
	elseif id == CAN_BMW_E65_GEAR_SELECTOR then
		printDebug('CAN_BMW_E65_GEAR_SELECTOR')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_DSC_STATUS then
		printDebug('CAN_BMW_E90_DSC_STATUS')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_DSC_SPEED then
		printDebug('CAN_BMW_E90_DSC_SPEED')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_COOLANT then
		printDebug('CAN_BMW_E90_COOLANT')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_LOCKING then
		printDebug('CAN_BMW_E90_LOCKING')
		relayToTcu(id, data)
    elseif id == CAN_BMW_E90_DASH_ON then
		printDebug('CAN_BMW_E90_DASH_ON')
		relayToTcu(id, data)
	elseif id == CAN_BMW_GEAR_TORQUE_DEMAND2 then
		printDebug('*******CAN_BMW_GEAR_TORQUE_DEMAND2')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_TRANSMISSION_DATA then
		printDebug('*******CAN_BMW_GEAR_TRANSMISSION_DATA')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_GEARBOX_DATA_2 then
		printDebug('CAN_BMW_GEAR_GEARBOX_DATA_2')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_TRANSMISSION_DISP then
		printDebug('*******CAN_BMW_GEAR_TRANSMISSION_DISP')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_GANG_STATUS then
		printDebug('*******CAN_BMW_GEAR_GANG_STATUS')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_NETWORK then
		printDebug('CAN_BMW_GEAR_NETWORK')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_SERVICE then
		printDebug('CAN_BMW_GEAR_SERVICE')
		relayToEcu(id, data)
	else
        print('No handler for ' .. id)
	end

	printDebug('got CAN id=' ..id ..' dlc=' ..dlc)
end

function onTick()
end

)", efi::size(config->luaScript));


}
