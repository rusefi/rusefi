	// by definition this should be a copy-paste of https://github.com/rusefi/rusefi/blob/master/firmware/controllers/lua/examples/350z-ac.txt
	strncpy(config->luaScript, R"(

IN_284 = 0x284 -- 644
IN_285 = 0x285 -- 645
IN_285 = 0x285
IN_35D = 0x35d

OUT_1F9 = 0x1F9 -- 505
OUT_233 = 0x233 -- 563
OUT_23D = 0x23D -- 573
OUT_23E = 0x23E -- 574

setTickRate(100)
t = Timer.new()
t : reset()

HEATER_OUTOUT_INDEX = 1
startPwm(HEATER_OUTOUT_INDEX, 1, 0)

globalAcOut = 0

function getTwoBytesMSB(data, offset, factor)
	return (data[offset + 1] * 256 + data[offset + 2]) * factor
end

function onTick()
    local MAF = getSensor("MAF")
    MAF = (MAF == nil and 0 or MAF)
    local TPS = getSensor("Tps1")
    TPS = (TPS == nil and 0 or TPS)
    local PPS = getSensor("AcceleratorPedal")
    PPS = (PPS == nil and 0 or PPS)
    -- print ("MAF " .. MAF .. " TPS " .. TPS .. " PPS " .. PPS)
    local rpmValue = math.floor(getSensor("RPM") + 0.5)
	local RPMread = rpmValue / 3.125
	local RPMhi = RPMread / 256
	local RPMlo = RPMread
	cltValue = getSensor("CLT")
	cltValue = (cltValue == nil and 0 or cltValue)

	--print('ac out = ' ..globalAcOut)
	if globalAcOut == 1 and rpmValue > 250 then
		fanPayloadOff = { 0x88, 0x00 }
		fanPayloadLo = { 0x88, 0x00 }
		fanPayloadHi = { 0x88, 0x00 }
	else
		--print('ac off payload')
		fanPayloadOff = { 0x00, 0x00 }
		fanPayloadLo = { 0x40, 0x00 }
		fanPayloadHi = { 0x80, 0x00 }
	end

	cltGauge = 0x00

	-- clt gauge stuff
	if cltValue < 115 then
		cltGauge = math.floor(cltValue * 1.5 + 0.5)
	else
		cltGauge = 0xF0
	end
	-- print('clt gauge = '..cltGauge)
	-- rpm fun stuff
	PPS256 = PPS * 256 / 100
	TPScapped = (TPS > 0.5 and 0.5 or TPS)
	if t : getElapsedSeconds() < 2 then
		CLTandRPM_D = { 0x00, PPS256, 0x0C, 0x01, 0x0A, 0x87, 0xFF, 0xFF }
	else
		CLTandRPM_D = { 0x00, PPS256, 0x0c, RPMlo, RPMhi, 0x87, 0xFF, cltGauge }
	end

    state_233 = rpmValue > 250 and 0x10 or 0x18
	CLTandRPM_3     = { cltGauge, 0x8C, 0x20, state_233, RPMlo, 0x00, 0x00, RPMhi }

	txCan(1, OUT_233, 0, CLTandRPM_3) -- transmit CLT and RPM for who knows whom
	txCan(1, OUT_23D, 0, CLTandRPM_D) -- transmit CLT and RPM for gauge cluster


	if rpmValue > 250 then
	    setPwmDuty(HEATER_OUTOUT_INDEX, 1)
		if cltValue <= 80 then
			txCan(1, OUT_1F9, 0, fanPayloadOff)
		elseif cltValue < 85 then
		    -- send nothing
		elseif cltValue < 90 then
			txCan(1, OUT_1F9, 0, fanPayloadLo)
		else
			txCan(1, OUT_1F9, 0, fanPayloadHi)
		end
	else
	    setPwmDuty(HEATER_OUTOUT_INDEX, 0)
		txCan(1, OUT_1F9, 0, fanPayloadOff)
	end
	-- print('CLT temp' ..cltValue)
end

function onCanRxAbs1(bus, id, dlc, data)
    kph = getTwoBytesMSB(data, 0, 0.005)
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

)", efi::size(config->luaScript));
