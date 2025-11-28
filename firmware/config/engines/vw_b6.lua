	setLuaScript( R"(
AIRBAG = 0x050
TCU_1088_440 = 0x440
TCU_1344_540 = 0x540
-- 1440
BRAKE_2 = 0x5A0

VWTP_OUT = 0x200
VWTP_IN = 0x202
VWTP_TESTER = 0x300

-- 640
MOTOR_1 = 0x280
-- 644
MOTOR_BRE = 0x284
-- 648
MOTOR_2 = 0x288
-- 800
Kombi_1 = 0x320
-- 896
MOTOR_3 = 0x380
-- 1152
MOTOR_5 = 0x480
-- 1160
MOTOR_6 = 0x488
-- 1386
ACC_GRA = 0x56A
-- 1408 the one with variable payload
MOTOR_INFO = 0x580
-- 1416
MOTOR_7 = 0x588

TCU_BUS = 1

fakeTorque = 0

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

function onTcu2(bus, id, dlc, data)
--	print("onTcu2")
end

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

function onAirBag(bus, id, dlc, data)
	-- looks like we have ignition key do not sleep!
	shallSleep : reset()
	hadIgnitionEvent = true
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

counter440 = 0
function onTcu1(bus, id, dlc, data)
--	print("onTcu1")
	    isShiftActive = getBitRange(data, 0, 1)
        tcuStatus = getBitRange(data, 1, 1)
        EGSRequirement = getBitRange(data, 7, 1)

            counter440 = counter440 + 1
            if counter440 % 1 == 0 then
                print("TCU " .. isShiftActive .. " " .. tcuStatus .. " " .. EGSRequirement)
            end
end

motor1Data   = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
motorBreData = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
motor2Data   = { 0x8A, 0x8D, 0x10, 0x04, 0x00, 0x4C, 0xDC, 0x87 }
motor2mux = {0x8A, 0xE8, 0x2C, 0x64}
canMotorInfo = { 0x00, 0x00, 0x00, 0x14, 0x1C, 0x93, 0x48, 0x14 }
canMotorInfo1= { 0x99, 0x14, 0x00, 0x7F, 0x00, 0xF0, 0x47, 0x01 }
canMotorInfo3= { 0x9B, 0x14, 0x00, 0x11, 0x1F, 0xE0, 0x0C, 0x46 }
canMotor3    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
motor5mux = {0x1c, 0x54, 0x84, 0xc2}
motor5Data   = { 0x1C, 0x08, 0xF3, 0x55, 0x19, 0x00, 0x00, 0xAD }
motor6Data   = { 0x00, 0x00, 0x00, 0x7E, 0xFE, 0xFF, 0xFF, 0x00 }
motor7Data   = { 0x1A, 0x66, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00 }
accGraData   = { 0x00, 0x00, 0x08, 0x00, 0x1A, 0x00, 0x02, 0x01 }

setTickRate(300)

everySecondTimer = Timer.new()

mafSensor = Sensor.new("maf")
mafCalibrationIndex = findCurveIndex("mafcurve")

canMotorInfoTotalCounter = 0

canRxAdd(AIRBAG, onAirBag)
canRxAdd(TCU_1088_440, onTcu1)
canRxAdd(TCU_1344_540, onTcu2)
--canRxAdd(BRAKE_2)

rpm = 0

function sendMotor1()
	engineTorque = fakeTorque * 0.9
	innerTorqWithoutExt = fakeTorque
	torqueLoss = 20
	requestedTorque = fakeTorque

	motor1Data[2] = engineTorque / 0.39
	setTwoBytes(motor1Data, 2, rpm / 0.25)
	motor1Data[5] = innerTorqWithoutExt / 0.4
	motor1Data[6] = tps / 0.4
	motor1Data[7] = torqueLoss / 0.39
	motor1Data[8] = requestedTorque / 0.39

-- print ('MOTOR_1 fakeTorque ' ..fakeTorque)
-- print ('MOTOR_1 engineTorque ' ..engineTorque ..' RPM ' ..rpm)
-- print ('MOTOR_1 innerTorqWithoutExt ' ..innerTorqWithoutExt ..' tps ' ..tps)

-- print ('MOTOR_1 torqueLoss ' ..torqueLoss ..' requestedTorque ' ..requestedTorque)

	txCan(TCU_BUS, MOTOR_1, 0, motor1Data)
end


function onMotor1(bus, id, dlc, data)

	rpm = math.floor(getSensor("RPM") + 0.5)
	tps = getSensor("TPS1") or 0

	fakeTorque = interpolate(0, 6, 100, 60, tps)

	sendMotor1()
end

function sendMotor3()
	iat = getSensor("IAT") or 0
	tps = getSensor("TPS1") or 0

	desired_wheel_torque = fakeTorque
	canMotor3[2] = (iat + 48) / 0.75
	canMotor3[3] = tps / 0.4
	canMotor3[5] = 0x20
	setBitRange(canMotor3, 24, 12, math.floor(desired_wheel_torque / 0.39))
	canMotor3[8] = tps / 0.4
	txCan(TCU_BUS, MOTOR_3, 0, canMotor3)
end


motorBreCounter = 0
function sendMotorBre()
	motorBreCounter = (motorBreCounter + 1) % 16

	setBitRange(motorBreData, 8, 4, motorBreCounter)
	xorChecksum(motorBreData, 1)

	txCan(TCU_BUS, MOTOR_BRE, 0, motorBreData)
end

motor2counter = 0
function sendMotor2()
	motor2counter = (motor2counter + 1) % 16

	minTorque = fakeTorque / 2
	-- todo: add CLT
	motor2Data[7] = math.floor(minTorque / 0.39)

--print ( "brake " .. getBitRange(data, 16, 2) .. " " .. rpm)

	brakeBit = rpm < 2000 and 1 or 0
	setBitRange(motor2Data, 16, 1, brakeBit)

	index = math.floor(motor2counter / 4)
	motor2Data[1] = motor2mux[1 + index]

	txCan(TCU_BUS, MOTOR_2, 0, motor2Data)
end

motor5counter = 0
motor5FuelCounter = 0
function sendMotor5()
    motor5counter = (motor5counter + 1) % 16
	index = math.floor(motor5counter / 4)
	motor5Data[1] = motor5mux[1 + index]

--	setBitRange(motor5Data, 5, 9, motor5FuelCounter)
	xorChecksum(motor5Data, 8)
	txCan(TCU_BUS, MOTOR_5, 0, motor5Data)
end

motor6counter = 0
function sendMotor6()
	motor6counter = (motor6counter + 1) % 16

	engineTorque = fakeTorque * 0.9
	actualTorque = fakeTorque
	feedbackGearbox = 255

	motor6Data[2] = math.floor(engineTorque / 0.39)
	motor6Data[3] = math.floor(actualTorque / 0.39)
	motor6Data[6] = math.floor(feedbackGearbox / 0.39)
	setBitRange(motor6Data, 60, 4, motor6counter)

	xorChecksum(motor6Data, 1)
	txCan(TCU_BUS, MOTOR_6, 0, motor6Data)
end

accGraCounter = 0
function sendAccGra()
	accGraCounter = (accGraCounter + 1) % 16
	setBitRange(accGraData, 60, 4, accGraCounter)
	xorChecksum(accGraData, 1)

	txCan(TCU_BUS, ACC_GRA, 0, accGraData)
end

canMotorInfoCounter = 0
function sendMotorInfo()
	canMotorInfoTotalCounter = canMotorInfoTotalCounter + 1
	canMotorInfoCounter = (canMotorInfoCounter + 1) % 16

	baseByte = canMotorInfoTotalCounter < 6 and 0x80 or 0x90
	canMotorInfo[1] = baseByte + (canMotorInfoCounter)
	canMotorInfo1[1] = baseByte + (canMotorInfoCounter)
	canMotorInfo3[1] = baseByte + (canMotorInfoCounter)
	mod4 = canMotorInfoCounter % 4

	if (mod4 == 0 or mod4 == 2) then
		txCan(TCU_BUS, MOTOR_INFO, 0, canMotorInfo)
	elseif (mod4 == 1) then
		txCan(TCU_BUS, MOTOR_INFO, 0, canMotorInfo1)
	else
		txCan(TCU_BUS, MOTOR_INFO, 0, canMotorInfo3)
	end
end

function sendMotor7()
	txCan(TCU_BUS, MOTOR_7, 0, motor7Data)
end

local tcuId = 0

function onCanHello(bus, id, dlc, data)
	-- here we handle 201 packets
	print('Got Hello Response ' ..arrayToString(data))
	tcuId = data[6] * 256 + data[5]

	print('From TCU ' ..tcuId)
	txCan(1, tcuId, 0, { 0xA0, 0x0F, 0x8A, 0xFF, 0x32, 0xFF })
end

local sendCounter = 2
local packetCounter = 1
local payLoadIndex = 0

local groups = { 10 }
-- todo: smarter array size calculation?
local groupsSize = 1

local groupIndex = 1

vssSensor = Sensor.new("VehicleSpeed")
vssSensor : setTimeout(2000)

function onKombi(bus, id, dlc, data)
	speed = getBitRange(data, 46, 10) * 0.32
	vssSensor : set(speed)
end

canRxAdd(Kombi_1, onKombi)

-- unused method did we mean to reset codes? todo: probably remove soon
function onCanTester(bus, id, dlc, data)
	-- here we handle 300 packets

	-- 	print('Got from tester ' ..arrayToString(data))

	if data[1] == 0xA3 then
		-- 		print ("Keep-alive")
		txCan(1, tcuId, 0, { 0xA1, 0x0F, 0x8A, 0xFF, 0x4A, 0xFF })

		groupIndex = groupIndex + 1
		if groupIndex > groupsSize then
			groupIndex = 1
		end
		groupId = groups[groupIndex]
		print (groupIndex .." " ..groupId)


		reqFirst = 0x10 + sendCounter
		print("Requesting next group " ..groupId .." with counter " ..sendCounter)
		txCan(1, tcuId, 0, { reqFirst, 0x00, 0x02, 0x21, groupId })

		sendCounter = sendCounter + 1
		if sendCounter == 16 then
			sendCounter = 0
		end
		return
	end


	if data[1] == 0xA1 then
		print ("Happy 300 packet")
		txCan(1, tcuId, 0, { 0x10, 0x00, 0x02, 0x10, 0x89 })
		return
	end

	if data[1] == 0xA8 then
		print ("They said Bye-Bye")
		return
	end


	if data[1] == 0x10 and dlc == 5 then
		ackPacket = 0xB0 + packetCounter
		print ("Sending ACK B1 " ..ackPacket)
		txCan(1, tcuId, 0, { ackPacket })
		-- request first group from array
		txCan(1, tcuId, 0, { 0x11, 0x00, 0x02, 0x21, groups[1] })
		return
	end

	top4 = math.floor(data[1] / 16)

	if top4 == 0xB then
		-- 		print("Got ACK")
		return
	end

	if top4 == 2 or top4 == 1 then
		print ("Looks like payload index " ..payLoadIndex ..": " ..arrayToString(data))

		if groupId == 2 and payLoadIndex == 0 then
			L7 = data[7]
			H9 = data[8]
			V = 256 * H9 + L7
			print("V 0 " ..V)
		end

		if groupId == 2 and payLoadIndex == 1 then
			L3 = data[3]
			H4 = data[4]
			V = 256 * H4 + L3
			print("V 1 " ..V)
		end

		if groupId == 2 and payLoadIndex == 2 then
			L2 = data[2]
			H3 = data[3]
			V = 256 * H3 + L2
			print("V 2 " ..V)
		end

		payLoadIndex = payLoadIndex + 1

		packetCounter = packetCounter + 1
		if packetCounter > 15 then
			packetCounter = 0
		end

		if top4 == 1 then
			ackPacket = 0xB0 + packetCounter
			print ("Sending payload ACK " ..ackPacket)
			txCan(1, tcuId, 0, { ackPacket })
			payLoadIndex = 0
		end

		return
	end

	print('Got unexpected ' ..arrayToString(data))
end

canRxAdd(VWTP_IN, onCanHello)

--txCan(1, VWTP_OUT, 0, { 0x02, 0xC0, 0x00, 0x10, 0x00, 0x03, 0x01 })

function onTick()

	freqValue = getSensor("AuxSpeed1") or 0
	mafValue = curve(mafCalibrationIndex, 5)
	-- 	print(freqValue .. " mafValue=" .. mafValue)
	mafSensor : set(mafValue)

	rpm = getSensor("RPM") or 0
	vbat = getSensor("BatteryVoltage") or 0

	if rpm == 0 then
		canMotorInfoTotalCounter = 0
	end

	onMotor1(0, 0, 0, nil)
	sendMotor3()

	sendMotor2()
	sendMotor5()
	sendMotor6()
	sendMotor7()
	sendMotorBre()
	sendAccGra()

	local timeToTurnOff = shallSleep : getElapsedSeconds() > 2
	local connectedToUsb = vbat < 4

	if hadIgnitionEvent and timeToTurnOff then
		-- looks like ignition key was removed
		-- 		mcu_standby()
	end

	if everySecondTimer : getElapsedSeconds() > 1 then
		everySecondTimer : reset()

		print("CAN OK " .. getOutput("canWriteOk") .. " not OK " .. getOutput("canWriteNotOk"))

        if rpm > 0 then
		    motor5FuelCounter = motor5FuelCounter + 20
        end

		sendMotorInfo()

	end
end

)");
