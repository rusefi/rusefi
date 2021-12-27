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

CAN_BMW_E90_TORQUE_1 = 0x0A8
CAN_BMW_E90_TORQUE_2 = 0x0A9
CAN_BMW_E90_RPM_THROTTLE = 0x0AA
CAN_BMW_E90_TORQUE_DEMAND = 0x0B6
CAN_BMW_E90_IGNITION_KEY = 0x130
CAN_BMW_E65_GEAR_SELECTOR = 0x192
CAN_BMW_E90_COOLANT = 0x1D0
CAN_BMW_E90_DASH_ON = 0x332

CAN_BMW_GEAR_TORQUE_DEMAND2 = 0x0B5
CAN_BMW_GEAR_TRANSMISSION_DATA = 0x0BA
CAN_BMW_GEAR_GEARBOX_DATA_2 = 0x1A2
CAN_BMW_GEAR_TRANSMISSION_DISP = 0x1D2
CAN_BMW_GEAR_GANG_STATUS = 0x304
CAN_BMW_GEAR_NETWORK = 0x498
CAN_BMW_GEAR_SERVICE = 0x598

ECU_BUS = 1
GEAR_BUS = 2

canRxAdd(CAN_BMW_E90_RPM_THROTTLE)
canRxAdd(CAN_BMW_E90_TORQUE_DEMAND)
canRxAdd(CAN_BMW_E90_IGNITION_KEY)
canRxAdd(CAN_BMW_E65_GEAR_SELECTOR)
canRxAdd(CAN_BMW_E90_COOLANT)
canRxAdd(CAN_BMW_E90_DASH_ON)

canRxAdd(CAN_BMW_GEAR_TORQUE_DEMAND2)
canRxAdd(CAN_BMW_GEAR_TRANSMISSION_DATA)
canRxAdd(CAN_BMW_GEAR_GEARBOX_DATA_2)
canRxAdd(CAN_BMW_GEAR_TRANSMISSION_DISP)
canRxAdd(CAN_BMW_GEAR_GANG_STATUS)
canRxAdd(CAN_BMW_GEAR_NETWORK)
canRxAdd(CAN_BMW_GEAR_SERVICE)


txPayload = { }

function onCanRx(bus, id, dlc, data)
	id = id % 2048
	-- local output = string.format("%x", id)

	if id == CAN_BMW_E90_IGNITION_KEY then
		print('!!!!!!!!!!!!! CAN_BMW_E90_IGNITION_KEY')
		txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
    elseif id == CAN_BMW_E90_TORQUE_1 then
		print('CAN_BMW_E90_TORQUE_1')
		txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
    elseif id == CAN_BMW_E90_TORQUE_2 then
		print('CAN_BMW_E90_TORQUE_2')
		txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
    elseif id == CAN_BMW_E90_TORQUE_DEMAND then
		print('CAN_BMW_E90_TORQUE_DEMAND') 
		txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
    elseif id == CAN_BMW_E90_DASH_ON then
		print('CAN_BMW_E90_DASH_ON') 
		txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
	elseif id == CAN_BMW_E65_GEAR_SELECTOR then
		print('CAN_BMW_E65_GEAR_SELECTOR')
		txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
	elseif id == CAN_BMW_E65_GEAR_SELECTOR then
		print('CAN_BMW_E65_GEAR_SELECTOR')
		txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
	elseif id == CAN_BMW_E90_RPM_THROTTLE then
		rpm = twoBytes(data, 4, 0.25)
		print('CAN_BMW_E90_RPM_THROTTLE ' .. rpm)
		txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
	elseif id == CAN_BMW_E90_COOLANT then
		print('CAN_BMW_E90_COOLANT')
		txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
	elseif id == CAN_BMW_GEAR_TORQUE_DEMAND2 then
		print('*******CAN_BMW_GEAR_TORQUE_DEMAND2')
		txCan(ECU_BUS, id, 0, data) -- relay TCU message to non-TCU
	elseif id == CAN_BMW_GEAR_TRANSMISSION_DATA then
		print('*******CAN_BMW_GEAR_TRANSMISSION_DATA')
		txCan(ECU_BUS, id, 0, data) -- relay TCU message to non-TCU
	elseif id == CAN_BMW_GEAR_TRANSMISSION_DISP then
		print('*******CAN_BMW_GEAR_TRANSMISSION_DISP')
		txCan(ECU_BUS, id, 0, data) -- relay TCU message to non-TCU
	elseif id == CAN_BMW_GEAR_GANG_STATUS then
		print('*******CAN_BMW_GEAR_GANG_STATUS')
		txCan(ECU_BUS, id, 0, data) -- relay TCU message to non-TCU
	elseif id == CAN_BMW_GEAR_GEARBOX_DATA_2 then
		print('CAN_BMW_GEAR_GEARBOX_DATA_2')
		txCan(ECU_BUS, id, 0, data) -- relay TCU message to non-TCU
	elseif id == CAN_BMW_GEAR_NETWORK then
		print('CAN_BMW_GEAR_NETWORK')
		txCan(ECU_BUS, id, 0, data) -- relay TCU message to non-TCU
	elseif id == CAN_BMW_GEAR_SERVICE then
		print('CAN_BMW_GEAR_SERVICE')
		txCan(ECU_BUS, id, 0, data) -- relay TCU message to non-TCU
	else
	end

	print('got CAN id=' ..id ..' dlc=' ..dlc)
end

function onTick()
	-- 	txCan(1, 0x611, 1, txPayload)
	-- 	txCan(2, 0x612, 1, txPayload)
end

)", efi::size(config->luaScript));


}
