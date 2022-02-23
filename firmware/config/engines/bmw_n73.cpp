/*
 * bmw_n73.cpp
 *
 * @date Oct 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "bmw_n73.h"

void setEngineProteusBMW_N73_GDI() {

}

/**
 * set engine_type 9
 */
void setEngineProteusGearboxManInTheMiddle() {
	strncpy(config->luaScript, R"(

function getTwoBytes(data, offset, factor)
	return (data[offset + 2] * 256 + data[offset + 1]) * factor
end

function setTwoBytes(data, offset, value)
	data[offset + 1] = value % 255
	data[offset + 2] = (value >> 8) % 255
end

function getBitRange(data, bitIndex, bitWidth)
	byteIndex = bitIndex >> 3
	shift = bitIndex - byteIndex * 8
	value = data[1 + byteIndex]
	if (shift + bitIndex > 8) then
		value = value + data[2 + byteIndex] * 256
	end
	mask = (1 << bitWidth) - 1
	return (value >> shift) & mask
end

	function bmwChecksum(canID, data, offset, length)
		checksum = canID
		for i = offset, offset + length - 1,1
		do
	   		checksum = checksum + data[i]
		end
		checksum = (checksum >> 8) + (checksum & 0xff)
		return checksum
	end

-- this controls onCanRx rate as well!
setTickRate(300)
--
-- crazy copy-pasta, at some point move to sets/bags

ignitionKeyByte0 = 256

tcuMessages = { }
-- 'ecu' means not-TCU
ecuMessages = { }

CAN_BMW_E90_TORQUE_1 = 0x0A8
CAN_BMW_E90_TORQUE_2 = 0x0A9
counterCAN_BMW_E90_RPM_THROTTLE = 0
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
CAN_BMW_E90_MSA = 0x308
-- 310 outside temperature Kombi
-- 330 fuel, range
CAN_BMW_E90_DASH_ON = 0x332
-- 34F
-- 0x3EF	OBD Daten Motor ECU
CAN_BMW_E90_ECU_NETWORK = 0x492
-- CAN_BMW_E90_ECU2_NETWORK = 0x493
CAN_BMW_E90_INPA_TCU = 0x618

CAN_BMW_GEAR_TORQUE_DEMAND2 = 0x0B5
CAN_BMW_GEAR_TRANSMISSION_DATA = 0x0BA
CAN_BMW_GEAR_GEARBOX_DATA_2 = 0x1A2
CAN_BMW_GEAR_TRANSMISSION_DISP = 0x1D2
CAN_BMW_GEAR_GANG_STATUS = 0x304
CAN_BMW_GEAR_NETWORK = 0x498
CAN_BMW_GEAR_SERVICE = 0x598
CAN_BMW_GEAR_INPA_RESPONSE = 0x6f1

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
canRxAdd(CAN_BMW_E90_MSA)
canRxAdd(CAN_BMW_E90_DASH_ON)
canRxAdd(CAN_BMW_E90_ECU_NETWORK)
canRxAdd(CAN_BMW_E90_INPA_TCU)

canRxAdd(CAN_BMW_GEAR_TORQUE_DEMAND2)
canRxAdd(CAN_BMW_GEAR_TRANSMISSION_DATA)
canRxAdd(CAN_BMW_GEAR_GEARBOX_DATA_2)
canRxAdd(CAN_BMW_GEAR_TRANSMISSION_DISP)
canRxAdd(CAN_BMW_GEAR_GANG_STATUS)
canRxAdd(CAN_BMW_GEAR_NETWORK)
canRxAdd(CAN_BMW_GEAR_SERVICE)
canRxAdd(CAN_BMW_GEAR_INPA_RESPONSE)


function relayToTcu(id, data)
	txCan(GEAR_BUS, id, 0, data) -- relay non-TCU message to TCU
end

function relayToEcu(id, data)
	txCan(ECU_BUS, id, 0, data) -- relay non-ECU message to ECU
end

function printDebug(msg)
	print(msg)
end

hexstr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "A", "B", "C", "D", "E", "F" }

function decimalToHex(num)
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

function print_array(arr)
	local str = ""
	local index = 1
	while arr[index] ~= nil do
		str = str.." "..decimalToHex(arr[index])
		index = index + 1
	end
	return str
end

function onCanRx(bus, id, dlc, data)
	id = id % 2048
	-- local output = string.format("%x", id)

	if id == CAN_BMW_E90_TORQUE_1 then
		TORQ_AVL = 0.5 * (getTwoBytes(data, 1, 1) >> 4)
		TORQ_AVL_DMEE = 0.5 * (getTwoBytes(data, 3, 1) >> 4)
		print('CAN_BMW_E90_TORQUE_1 TORQ_AVL=' ..TORQ_AVL ..' TORQ_AVL_DMEE=' ..TORQ_AVL_DMEE)
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_TORQUE_2 then
		printDebug('CAN_BMW_E90_TORQUE_2')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_RPM_THROTTLE then
		rpm = getTwoBytes(data, 4, 0.25)
		pedal = data [1 + 3] * 100 / 256.0
		print('CAN_BMW_E90_RPM_THROTTLE rpm=' .. rpm .. ' pedal=' .. pedal)

		output = {0, 0, 0, data [1 + 3], data [1 + 4], data [1 + 5], 0, 0}
        counterCAN_BMW_E90_RPM_THROTTLE = (counterCAN_BMW_E90_RPM_THROTTLE + 1) % 15
        output[2] = counterCAN_BMW_E90_RPM_THROTTLE
        output[1] = bmwChecksum(CAN_BMW_E90_RPM_THROTTLE, output, 2, 7)

		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_DSC_TORQUE_DEMAND then
		printDebug('CAN_BMW_E90_DSC_TORQUE_DEMAND')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_WHEEL_SPEED then
		printDebug('CAN_BMW_E90_WHEEL_SPEED')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_IGNITION_KEY then
		if ignitionKeyByte0 ~= data[1] then
		    ignitionKeyByte0 = data[1]
		    if ignitionKeyByte0 == 0x55 then
		        printDebug('!!!!!!!!!!!!! CAN_BMW_E90_IGNITION_KEY CRANKING')
  		    else
  		        printDebug('!!!!!!!!!!!!! CAN_BMW_E90_IGNITION_KEY ' .. ignitionKeyByte0)
            end
  		end
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
	    cltByte = data[1 + 0]
        mapByte = data[1 + 3]
        fuelWord = getTwoBytes(data, 4, 1)
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_LOCKING then
		printDebug('CAN_BMW_E90_LOCKING')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_MSA then
		printDebug('CAN_BMW_E90_MSA')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_DASH_ON then
		printDebug('CAN_BMW_E90_DASH_ON')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_ECU_NETWORK then
		printDebug('CAN_BMW_E90_ECU_NETWORK')
		relayToTcu(id, data)
	elseif id == CAN_BMW_E90_INPA_TCU then
		print('CAN_BMW_E90_INPA_TCU')
		relayToTcu(id, data)
	elseif id == CAN_BMW_GEAR_TORQUE_DEMAND2 then
	    TORQ_TAR_EGS = getBitRange(data, 12, 12)
	    ST_TORQ_TAR_EGS = getBitRange(data, 36, 2)
		printDebug('*******CAN_BMW_GEAR_TORQUE_DEMAND2 ' .. TORQ_TAR_EGS .. ' ' .. ST_TORQ_TAR_EGS)
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_TRANSMISSION_DATA then
		printDebug('*******CAN_BMW_GEAR_TRANSMISSION_DATA')
		gearBits = data[1] & 0xF
		if (gearBits >= 5 and gearBits <= 0xA) then
		    printDebug('*******CAN_BMW_GEAR_TRANSMISSION_DATA gear ' .. (gearBits - 4))
		end
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_GEARBOX_DATA_2 then
		printDebug('*******CAN_BMW_GEAR_GEARBOX_DATA_2')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_TRANSMISSION_DISP then
		printDebug('*******CAN_BMW_GEAR_TRANSMISSION_DISP')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_GANG_STATUS then
		printDebug('*******CAN_BMW_GEAR_GANG_STATUS')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_NETWORK then
		printDebug('*******CAN_BMW_GEAR_NETWORK')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_SERVICE then
		printDebug('*******CAN_BMW_GEAR_SERVICE')
		relayToEcu(id, data)
	elseif id == CAN_BMW_GEAR_INPA_RESPONSE then
		print('*******CAN_BMW_GEAR_INPA_RESPONSE')
		relayToEcu(id, data)
	else
		print('No handler for ' ..id)
	end

--	printDebug('got CAN id=' ..id ..' dlc=' ..dlc)
end

function onTick()
end

)", efi::size(config->luaScript));


}
