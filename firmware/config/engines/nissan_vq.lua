	// by definition this should be a copy-paste of https://github.com/rusefi/rusefi/blob/master/firmware/controllers/lua/examples/350z-ac.txt
	setLuaScript( R"(

IN_284 = 0x284 -- 644
IN_285 = 0x285 -- 645
IN_285 = 0x285
IN_35D = 0x35d

ENGINE_1_1F9_505 = 505
ENGINE_2_561 = 561
ENGINE_7_233_563 = 563
ENGINE_3_23D_573 = 573
ENGINE_4_23E_574 = 574
ENGINE_5_551_1361 = 1361
ENGINE_6_580_1408 = 1408

speedSensor = Sensor.new("VehicleSpeed")
speedSensor : setTimeout(3000)

setTickRate(100)
startUpTimer = Timer.new()
startUpTimer : reset()
rpm = 0
pps = 0

globalAcOut = 0
cltGauge = 0

function getTwoBytesMSB(data, offset, factor)
	return (data[offset + 1] * 256 + data[offset + 2]) * factor
end

-- Little-endian System, "Intel"
function setTwoBytesLsb(data, offset, value)
	value = math.floor(value)
	data[offset + 2] = value >> 8
	data[offset + 1] = value & 0xff
end

payloadENGINE_3_573 = { 0x00, 0, 0x0C, 0x01, 0x0A, 0x87, 0xFF, 0xFF }
function sendENGINE_3_573()
  rpmWithSweep = startUpTimer : getElapsedSeconds() < 2 and 8000 or rpm
  setTwoBytesLsb(payloadENGINE_3_573, 3, rpmWithSweep / 3.125)

	payloadENGINE_3_573[2] = pps / 0.392
	payloadENGINE_3_573[3] = pps / 0.392 -- Throttle_position_capped
	payloadENGINE_3_573[8] = cltGauge
	txCan(1, ENGINE_3_23D_573, 0, payloadENGINE_3_573)
end

payloadENGINE_7_563 = {0x79, 0xa2, 0x00, 0x18, 0x0e, 0x00, 0x00, 0x01}
function sendENGINE_7_233_563()
	local RPMread = rpm / 3.125
	local RPMhi = RPMread / 256
	local RPMlo = RPMread

	payloadENGINE_7_563[1] = cltGauge
	payloadENGINE_7_563[4] = state_233
	payloadENGINE_7_563[5] = RPMlo
	payloadENGINE_7_563[8] = RPMhi

   state_233 = rpm > 250 and 0x10 or 0x18

	txCan(1, ENGINE_7_233_563, 0, payloadENGINE_7_563) -- transmit CLT and RPM for who knows whom
end

function sendENGINE_1_505()
  	if rpm > 250 then
  		if cltValue <= 80 then
  			txCan(1, ENGINE_1_1F9_505, 0, fanPayloadOff)
  		elseif cltValue < 85 then
  		    -- send nothing
  		elseif cltValue < 90 then
  			txCan(1, ENGINE_1_1F9_505, 0, fanPayloadLo)
  		else
  			txCan(1, ENGINE_1_1F9_505, 0, fanPayloadHi)
  		end
  	else
  		txCan(1, ENGINE_1_1F9_505, 0, fanPayloadOff)
  	end
  	-- print('CLT temp' ..cltValue)
end

payloadENGINE_2_561 = {0xe0, 0x80, 0x09, 0xe0, 0xd4, 0xc3, 0x4c, 0x9e}
function sendENGINE_2_231_561()
	payloadENGINE_2_561[3] = pps / 0.5 -- tps or pps
	txCan(1, ENGINE_2_561, 0, payloadENGINE_2_561)
end

payloadENGINE_4_574 = {0x00, 0x40, 0xff, 0x45, 0x00, 0xd6, 0x00, 0xa2}
function sendENGINE_4_23E_574()
	payloadENGINE_4_574[3] = (100 - pps) / 0.392 -- data[3] -- affects desired torque converter pressure Throttle_position_inverted
	payloadENGINE_4_574[7] = pps / 0.392 -- data[7] -- TPS
	txCan(1, ENGINE_4_23E_574, 0, payloadENGINE_4_574)
end

CAN_721_2d1 = 721
payload721_2d1 = {0x00, 0x84, 0x00, 0x00, 0x31, 0xf8, 0x01}
function sendCan721_2d1()
	txCan(1, CAN_721_2d1, 0, payload721_2d1)
end

CAN_734_2de = 734
payload734_2de = {0x0f, 0x08, 0x02, 0x00, 0x19, 0x65, 0x07, 0xa8}
function sendCan734_2de()
	txCan(1, CAN_734_2de, 0, payload734_2de)
end

payloadENGINE_5 = {0x7d, 0xdb, 0x00, 0xa0, 0x00, 0x02, 0x80, 0xff}
function sendENGINE_5_551_1361()
	txCan(1, ENGINE_5_551_1361, 0, payloadENGINE_5)
end

payloadENGINE_6 = {0x00, 0x82, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00}
function sendEngine6_580_1408()
	txCan(1, ENGINE_6_580_1408, 0, payloadENGINE_6)
end

_10msPeriodTimer = Timer.new()
_15msPeriodTimer = Timer.new()

function onTick()
    pps = getSensor("AcceleratorPedal") or 0
    pps = (pps == nil and 0 or pps)
    rpm = math.floor(getSensor("RPM") + 0.5)


	cltValue = getSensor("CLT")
	cltValue = (cltValue == nil and 0 or cltValue)

	--print('ac out = ' ..globalAcOut)
	if globalAcOut == 1 and rpm > 250 then
		fanPayloadOff = { 0x88, 0x00 }
		fanPayloadLo = { 0x88, 0x00 }
		fanPayloadHi = { 0x88, 0x00 }
	else
		--print('ac off payload')
		fanPayloadOff = { 0x00, 0x00 }
		fanPayloadLo = { 0x40, 0x00 }
		fanPayloadHi = { 0x80, 0x00 }
	end

	-- clt gauge stuff
	if cltValue < 115 then
		cltGauge = math.floor(cltValue * 1.5 + 0.5)
	else
		cltGauge = 0xF0
	end


	if _10msPeriodTimer : getElapsedSeconds() > 0.01 then
    _10msPeriodTimer : reset()
    sendENGINE_1_505()
    sendENGINE_2_231_561()
    sendENGINE_7_233_563()
    sendENGINE_3_573()
    sendENGINE_4_23E_574()
    sendCan721_2d1()
    sendCan734_2de()
	end


	if _15msPeriodTimer : getElapsedSeconds() > 0.015 then
    _15msPeriodTimer : reset()
    sendENGINE_5_551_1361()
    sendEngine6_580_1408()
	end


end

function onCanRxAbs1(bus, id, dlc, data)
    kph = getTwoBytesMSB(data, 0, 0.005)
    speedSensor : set(kph)
end

function onCanRxAc(bus, id, dlc, data)
	--print('got CAN id=' ..id ..' dlc=' ..dlc)
	--print('ac value is= '..data[1])
	if data[1] == 193 then
		setAcRequestState(true)
		globalAcOut = 1
		--print('ac is on')
	else
		setAcRequestState(false)
		globalAcOut = 0
		--print('ac is off')
	end

end

function onCanRxWheelSpeed1(bus, id, dlc, data)
end

function onCanRxWheelSpeed2(bus, id, dlc, data)
end

canRxAdd(IN_284, onCanRxWheelSpeed1)
canRxAdd(IN_285, onCanRxWheelSpeed2)
canRxAdd(IN_35D, onCanRxAbs1)
canRxAdd(IN_35D, onCanRxAc)

)");
