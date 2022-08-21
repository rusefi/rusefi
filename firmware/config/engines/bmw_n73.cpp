/*
 * bmw_n73.cpp
 *
 * @date Oct 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

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
	if (shift + bitWidth > 8) then
		value = value + data[2 + byteIndex] * 256
	end
	mask = (1 << bitWidth) - 1
	return (value >> shift) & mask
end

function bmwChecksum(canID, data, offset, length)
	checksum = canID
	for i = offset, offset + length - 1, 1
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

E90_TORQUE_1 = 0x0A8
E90_TORQUE_2 = 0x0A9
counterE90_RPM_THROTTLE = 0
E90_RPM_THROTTLE = 0x0AA
E90_DSC_TORQUE_DEMAND = 0x0B6
E90_WHEEL_SPEED = 0x0CE
E90_IGNITION_KEY = 0x130
E90_GEAR_SELECTOR = 0x192
E90_DSC_STATUS = 0x19E
E90_DSC_SPEED = 0x1A0
E90_COOLANT = 0x1D0
E90_GEAR_SELECTOR = 0x198
E90_LOCKING = 0x2FC
E90_MSA = 0x308
E90_DASH_ON = 0x332
E90_ECU_NETWORK = 0x492
E90_INPA_TCU = 0x618

TCU_TORQUE_DEMAND2 = 0x0B5
TCU_TRANSMISSION_DATA = 0x0BA
TCU_GEARBOX_DATA_2 = 0x1A2
TCU_TRANSMISSION_DISP = 0x1D2
TCU_GANG_STATUS = 0x304
TCU_NETWORK = 0x498
TCU_SERVICE = 0x598
TCU_INPA_RESPONSE = 0x6f1

ECU_BUS = 1
TCU_BUS = 2

canRxAdd(E90_TORQUE_1)
canRxAdd(E90_TORQUE_2)
canRxAdd(E90_RPM_THROTTLE)
canRxAdd(E90_DSC_TORQUE_DEMAND)
canRxAdd(E90_WHEEL_SPEED)
canRxAdd(E90_IGNITION_KEY)
canRxAdd(E90_GEAR_SELECTOR)
canRxAdd(E90_DSC_STATUS)
canRxAdd(E90_DSC_SPEED)
canRxAdd(E90_COOLANT)
--canRxAdd(E90_LOCKING)
--canRxAdd(E90_MSA)
--canRxAdd(E90_DASH_ON)
canRxAdd(E90_ECU_NETWORK)
canRxAdd(E90_INPA_TCU)

canRxAdd(TCU_TORQUE_DEMAND2)
canRxAdd(TCU_TRANSMISSION_DATA)
canRxAdd(TCU_GEARBOX_DATA_2)
canRxAdd(TCU_TRANSMISSION_DISP)
canRxAdd(TCU_GANG_STATUS)
canRxAdd(TCU_NETWORK)
canRxAdd(TCU_SERVICE)
canRxAdd(TCU_INPA_RESPONSE)


function relayToTcu(id, data)
	txCan(TCU_BUS, id, 0, data) -- relay non-TCU message to TCU
end

function relayToEcu(id, data)
	txCan(ECU_BUS, id, 0, data) -- relay non-ECU message to ECU
end

function printDebug(msg)
	print(msg)
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

function onCanRx(bus, id, dlc, data)
	id = id % 2048

	if id == E90_TORQUE_1 then
		TORQ_AVL = 0.5 * (getTwoBytes(data, 1, 1) >> 4)
		TORQ_AVL_DMEE = 0.5 * (getTwoBytes(data, 3, 1) >> 4)
		printDebug('E90_TORQUE_1 TORQ_AVL=' ..TORQ_AVL ..' TORQ_AVL_DMEE=' ..TORQ_AVL_DMEE)
		relayToTcu(id, data)
	elseif id == E90_TORQUE_2 then
		printDebug('E90_TORQUE_2')
		relayToTcu(id, data)
	elseif id == E90_RPM_THROTTLE then
		rpm = getTwoBytes(data, 4, 0.25)
		pedal = data [1 + 3] * 100 / 256.0
		printDebug('E90_RPM_THROTTLE rpm=' .. rpm .. ' pedal=' .. pedal)

		output = {0, 0, 0, data [1 + 3], data [1 + 4], data [1 + 5], 0, 0}
        counterE90_RPM_THROTTLE = (counterE90_RPM_THROTTLE + 1) % 15
        output[2] = counterE90_RPM_THROTTLE
        output[1] = bmwChecksum(E90_RPM_THROTTLE, output, 2, 7)

--		print('original ' ..arrayToString(data))
--		print('repacked ' ..arrayToString(output))

		relayToTcu(id, output)
	elseif id == E90_DSC_TORQUE_DEMAND then
		printDebug('E90_DSC_TORQUE_DEMAND')
		relayToTcu(id, data)
	elseif id == E90_WHEEL_SPEED then
		printDebug('E90_WHEEL_SPEED')
		relayToTcu(id, data)
	elseif id == E90_IGNITION_KEY then
		if ignitionKeyByte0 ~= data[1] then
		    ignitionKeyByte0 = data[1]
		    if ignitionKeyByte0 == 0x55 then
		        printDebug('!!!!!!!!!!!!! E90_IGNITION_KEY CRANKING')
  		    else
  		        printDebug('!!!!!!!!!!!!! E90_IGNITION_KEY ' .. ignitionKeyByte0)
            end
  		end
		relayToTcu(id, data)
	elseif id == E90_GEAR_SELECTOR then
		printDebug('E90_GEAR_SELECTOR')
		relayToTcu(id, data)
	elseif id == E90_DSC_STATUS then
		printDebug('E90_DSC_STATUS')
		relayToTcu(id, data)
	elseif id == E90_DSC_SPEED then
		printDebug('E90_DSC_SPEED')
		relayToTcu(id, data)
	elseif id == E90_COOLANT then
		printDebug('E90_COOLANT')
	    cltByte = data[1 + 0]
        mapByte = data[1 + 3]
        fuelWord = getTwoBytes(data, 4, 1)
		relayToTcu(id, data)
	elseif id == E90_LOCKING then
		printDebug('E90_LOCKING')
		relayToTcu(id, data)
	elseif id == E90_MSA then
		printDebug('E90_MSA')
		relayToTcu(id, data)
	elseif id == E90_DASH_ON then
		printDebug('E90_DASH_ON')
		relayToTcu(id, data)
	elseif id == E90_ECU_NETWORK then
		printDebug('E90_ECU_NETWORK')
		relayToTcu(id, data)
	elseif id == E90_INPA_TCU then
		printDebug('E90_INPA_TCU')
		relayToTcu(id, data)
	elseif id == TCU_TORQUE_DEMAND2 then
	    TORQ_TAR_EGS = getBitRange(data, 12, 12)
	    ST_TORQ_TAR_EGS = getBitRange(data, 36, 2)
		printDebug('*******TCU_TORQUE_DEMAND2 ' .. TORQ_TAR_EGS .. ' ' .. ST_TORQ_TAR_EGS)
		relayToEcu(id, data)
	elseif id == TCU_TRANSMISSION_DATA then
		printDebug('*******TCU_TRANSMISSION_DATA')
		gearBits = data[1] & 0xF
		if (gearBits >= 5 and gearBits <= 0xA) then
			print('*******TCU_TRANSMISSION_DATA gear ' ..(gearBits - 4))
		elseif gearBits == 1 or gearBits == 2 or gearBits == 3 then
		    printDebug('*******TCU_TRANSMISSION_DATA gear R/N/P')
		else
		    printDebug('*******TCU_TRANSMISSION_DATA gear ' .. gearBits)
		end
		relayToEcu(id, data)
	elseif id == TCU_GEARBOX_DATA_2 then
		printDebug('*******TCU_GEARBOX_DATA_2')
		relayToEcu(id, data)
	elseif id == TCU_TRANSMISSION_DISP then
		printDebug('*******TCU_TRANSMISSION_DISP')
		relayToEcu(id, data)
	elseif id == TCU_GANG_STATUS then
		printDebug('*******TCU_GANG_STATUS')
		relayToEcu(id, data)
	elseif id == TCU_NETWORK then
		printDebug('*******TCU_NETWORK')
		relayToEcu(id, data)
	elseif id == TCU_SERVICE then
		printDebug('*******TCU_SERVICE')
		relayToEcu(id, data)
	elseif id == TCU_INPA_RESPONSE then
		printDebug('*******TCU_INPA_RESPONSE')
		relayToEcu(id, data)
	else
		print('No handler for ' ..id)
	end

--	printDebug('got CAN id=' ..id ..' dlc=' ..dlc)
end

function onTick()
 -- empty 'onTick' until we make 'onTick' method optional
end

)", efi::size(config->luaScript));


}
