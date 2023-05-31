--
-- Tested on 2006 Passat
--

-- 640
MOTOR_1 = 0x280
-- 648
MOTOR_2 = 0x288
-- 896
MOTOR_3 = 0x380
-- 1152
MOTOR_5 = 0x480

-- this controls onCanRx rate as well!
setTickRate(300)

timeout = 3000

cltSensor = Sensor.new("clt")
cltSensor : setTimeout(timeout)

iatSensor = Sensor.new("iat")
iatSensor : setTimeout(timeout)

rpmSensor = Sensor.new("rpm")
rpmSensor : setTimeout(timeout)

ppsSensor = Sensor.new("AcceleratorPedal")
ppsSensor : setTimeout(timeout)

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

function onMotor1(bus, id, dlc, data)
 rpm = getBitRange(data, 16, 16) * 0.25
 rpmSensor : set(rpm)
 print("RPM " .. rpm)
end

function onMotor2(bus, id, dlc, data)
 clt = getBitRange(data, 8, 8) * 0.75 - 48
 cltSensor : set(clt)
end

function onMotor3(bus, id, dlc, data)
 iat = getBitRange(data, 8, 8) * 0.75 - 48
 pps = getBitRange(data, 16, 8) * 0.40

 print("IAT " .. iat .. " PPS " .. pps)
 iatSensor : set(iat)
 ppsSensor : set(pps)
end

function onMotor5(bus, id, dlc, data)
 fuelConsumption = getBitRange(data, 16, 15)
  print("fuelConsumption " .. fuelConsumption)

end

canRxAdd(1, MOTOR_1, onMotor1)
canRxAdd(1, MOTOR_2, onMotor2)
canRxAdd(1, MOTOR_3, onMotor3)
canRxAdd(1, MOTOR_5, onMotor5)

function onTick()
end