	strncpy(config->luaScript, R"(
AIRBAG = 0x050
GRA = 0x388
-- 1088
TCU_1 = 0x440
-- 1344
TCU_2 = 0x540
-- 1440
BRAKE_2 = 0x5A0


MOTOR_1 = 0x280
MOTOR_3 = 0x380
MOTOR_INFO = 0x580
MOTOR_5 = 0x480
MOTOR_6 = 0x488
MOTOR_7 = 0x588

function onTcu1(bus, id, dlc, data)
    print("onTcu1")
end

function onTcu2(bus, id, dlc, data)
    print("onTcu2")
end

canRxAdd(AIRBAG)
canRxAdd(TCU_1, onTcu1)
canRxAdd(TCU_2, onTcu2)
-- canRxAdd(BRAKE_2)

fuelCounter = 0

function setBitRange(data, totalBitIndex, bitWidth, value)
	local byteIndex = totalBitIndex >> 3
	local bitInByteIndex = totalBitIndex - byteIndex * 8
	if (bitInByteIndex + bitWidth > 8) then
		bitsToHandleNow = 8 - bitInByteIndex
		setBitRange(data, totalBitIndex + bitsToHandleNow, bitWidth - bitsToHandleNow, value >> bitsToHandleNow)
		bitWidth = bitsToHandleNow
	end
	mask = (1 << bitWidth) - 1
	data[1 + byteIndex] = data[1 + byteIndex] & (~(mask << bitInByteIndex))
	maskedValue = value & mask
	shiftedValue = maskedValue << bitInByteIndex
	data[1 + byteIndex] = data[1 + byteIndex] | shiftedValue
end


function setTwoBytes(data, offset, value)
	data[offset + 1] = value % 255
	data[offset + 2] = (value >> 8) % 255
end

shallSleep = Timer.new()

-- we want to turn on with hardware switch while ignition key is off
hadIgnitionEvent = false

function onCanRx(bus, id, dlc, data)
	if id == AIRBAG then
		-- looks like we have ignition key do not sleep!
		shallSleep : reset()
		hadIgnitionEvent = true
	else
		print('got CAN id=' ..id ..' dlc=' ..dlc)


	end
end

function setTwoBytes(data, offset, value)
	value = math.floor(value)
	data[offset + 2] = value >> 8
	data[offset + 1] = value & 0xff
end

function xorChecksum(data, targetIndex)
	local index = 1
	local result = 0
	while data[index] ~= nil do
		if index ~= targetIndex then
			result = result ~ data[index]
		end
		index = index + 1
	end
	data[targetIndex] = result
	return result
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

canMotor1    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
canMotorInfo = { 0x00, 0x00, 0x00, 0x14, 0x1C, 0x93, 0x48, 0x14 }
canMotor3    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
motor5Data   = { 0x1C, 0x08, 0xF3, 0x55, 0x19, 0x00, 0x00, 0xAD }
motor6Data   = { 0x00, 0x00, 0x00, 0x7E, 0xFE, 0xFF, 0xFF, 0x00 }
canMotor7    = { 0x1A, 0x66, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00 }

setTickRate(100)

everySecondTimer = Timer.new()
canMotorInfoCounter = 0

counter = 0

mafSensor = Sensor.new("maf")
mafCalibrationIndex = findCurveIndex("mafcurve")

function onTick()

    freqValue = getSensor("AuxSpeed1") or 0
	mafValue = curve(mafCalibrationIndex, 5)
--	print(freqValue .. " mafValue=" .. mafValue)
	mafSensor : set(mafValue)

	counter = (counter + 1) % 16

	rpm = getSensor("RPM") or 0
	clt = getSensor("CLT") or 0
	iat = getSensor("IAT") or 0
	tps = getSensor("TPS1") or 0
	vbat = getSensor("BatteryVoltage") or 0

	fakeTorque = interpolate(0, 6, 100, 60, tps)

	engineTorque = fakeTorque * 0.9
	innerTorqWithoutExt = fakeTorque
	torqueLoss = 10
	requestedTorque = fakeTorque

	canMotor1[2] = engineTorque / 0.39
	canMotor1[5] = innerTorqWithoutExt / 0.4
	canMotor1[6] = tps / 0.4
	canMotor1[7] = torqueLoss / 0.39
	canMotor1[8] = requestedTorque / 0.39
	setTwoBytes(canMotor1, 2, 4 * rpm)
	txCan(1, MOTOR_1, 0, canMotor1)

	desired_wheel_torque = fakeTorque
	canMotor3[2] = (iat + 48) / 0.75
	canMotor3[3] = tps / 0.4
	canMotor3[5] = 0x20
	setBitRange(canMotor3, 24, 12, math.floor(desired_wheel_torque / 0.39))
	canMotor3[8] = tps / 0.4
	txCan(1, MOTOR_3, 0, canMotor3)

	setBitRange(motor5Data, 5, 9, fuelCounter)
	xorChecksum(motor5Data, 8)
	txCan(1, MOTOR_5, 0, motor5Data)

    actualTorque = fakeTorque
    feedbackGearbox = 255

    motor6Data[2] = math.floor(engineTorque / 0.39)
    motor6Data[3] = math.floor(actualTorque / 0.39)
    motor6Data[6] = math.floor(feedbackGearbox / 0.39)
    xorChecksum(motor6Data, 1)
   	txCan(1, MOTOR_6, 0, motor6Data)

	txCan(1, MOTOR_7, 0, canMotor7)

    local timeToTurnOff = shallSleep : getElapsedSeconds() > 2
    local connectedToUsb = vbat < 4

	if hadIgnitionEvent and timeToTurnOff then
		-- looks like ignition key was removed
		mcu_standby()
	end

	if everySecondTimer : getElapsedSeconds() > 1 then
		everySecondTimer : reset()

		fuelCounter = fuelCounter + 20

		canMotorInfoCounter = (canMotorInfoCounter + 1) % 8
		canMotorInfo[1] = 0x90 + (canMotorInfoCounter * 2)
		txCan(1, MOTOR_INFO, 0, canMotorInfo)
	end
end

)", efi::size(config->luaScript));
