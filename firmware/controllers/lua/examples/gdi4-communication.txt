-- communication with https://github.com/rusefi/rusefi-hardware/tree/main/GDI-4ch/firmware

GDI4_BASE_ADDRESS = 0xF0

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

function getTwoBytesLSB(data, offset, factor)
	return (data[offset + 2] * 256 + data[offset + 1]) * factor
end

function setTwoBytesLsb(data, offset, value)
	value = math.floor(value)
	data[offset + 2] = value >> 8
	data[offset + 1] = value & 0xff
end

function onCanConfiguration(bus, id, dlc, data)
	print('Received configuration ' ..arrayToString(data))
	print ("BoostVoltage " ..getTwoBytesLSB(data, 0, 0.01) )
	print ("BoostCurrent " ..getTwoBytesLSB(data, 2, 0.01) )
	print ("PeakCurrent " ..getTwoBytesLSB(data, 4, 0.01) )
	print ("HoldCurrent " ..getTwoBytesLSB(data, 6, 0.01) )
end

canRxAdd(GDI4_BASE_ADDRESS, printPacket)
canRxAdd(GDI4_BASE_ADDRESS + 1, onCanConfiguration)
canRxAdd(GDI4_BASE_ADDRESS + 2, printPacket)

local data_set_voltage = { 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

FIXED_POINT = 100

function onTick()

	boostVoltage = getCalibration("mc33_hvolt")
	boostCurrent = getCalibration("mc33_i_boost") / 1000.0
	peakCurrent = getCalibration("mc33_i_peak") / 1000.0
	print("boostVoltage " .. boostVoltage .." boostCurrent " ..boostCurrent .." peakCurrent " ..peakCurrent)

	holdCurrent = getCalibration("mc33_i_hold")
	peakDuration = getCalibration("mc33_t_peak_tot") / 1000.0
	holdDuration = getCalibration("mc33_t_hold_tot") / 1000.0
	print("holdCurrent " .. holdCurrent .. " peakDuration " .. peakDuration .." hold duration " ..holdDuration)

	setTwoBytesLsb(data_set_voltage, 1, boostVoltage * FIXED_POINT)
	setTwoBytesLsb(data_set_voltage, 3, boostCurrent * FIXED_POINT)
	setTwoBytesLsb(data_set_voltage, 5, peakCurrent * FIXED_POINT)
	print('Sending ' ..arrayToString(data_set_voltage))



	txCan(1, 0x201, 1, data_set_voltage)

end


