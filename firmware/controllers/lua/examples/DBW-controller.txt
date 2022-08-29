-- DBW protocol for legacy standalone systems

BASE_CAN_ID = 256

setTickRate(100)

-- Little-endian System, "Intel"
function setTwoBytesLsb(data, offset, value)
	value = math.floor(value)
	data[offset + 2] = value >> 8
	data[offset + 1] = value & 0xff
end

-- MOTOROLA order, MSB (Most Significant Byte/Big Endian) comes first
function setTwoBytesMsb(data, offset, value)
	value = math.floor(value)
	data[offset + 1] = value >> 8
	data[offset + 2] = value & 0xff
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

function printPacket(bus, id, dlc, data)
    print('Received ' ..arrayToString(data))
end

canRxAdd(1, BASE_CAN_ID + 4, printPacket)
protocolPacket = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

function onTick()
	local ppsValue = getSensor("AcceleratorPedal")
	local TPS = getSensor("TPS1")

	setTwoBytesMsb(protocolPacket, 0, ppsValue / 100 * 1024)
	setTwoBytesMsb(protocolPacket, 2, TPS / 100 * 1024)

	txCan(1, BASE_CAN_ID + 0, 0, protocolPacket)
end
