canRxAdd(0x35d)

OUT_1F9 = 0x1F9
OUT_233 = 0x233
OUT_23D = 0x23D

setTickRate(100)
t = Timer.new()
t : reset()

globalAcOut = 0

function onTick()
    local rpmValue = math.floor(getSensor("RPM") + 0.5)
	local RPMread = rpmValue / 3.15
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
	if t : getElapsedSeconds() < 2 then
		CLTandRPM_D = { 0x00, 0x18, 0x0C, 0x01, 0x0A, 0x87, 0xFF, 0xFF }
	else
		CLTandRPM_D = { 0x00, 0x18, 0x0c, RPMlo, RPMhi, 0x87, 0xFF, cltGauge }
	end

    state_233 = rpmValue > 250 and 0x10 or 0x18
	CLTandRPM_3     = { cltGauge, 0x8C, 0x20, state_233, RPMlo, 0x00, 0x00, RPMhi }

	txCan(1, OUT_233, 0, CLTandRPM_3) -- transmit CLT and RPM for who knows whom
	txCan(1, OUT_23D, 0, CLTandRPM_D) -- transmit CLT and RPM for gauge cluster


	if rpmValue > 250 then
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
		txCan(1, OUT_1F9, 0, fanPayloadOff)
	end
	-- print('CLT temp' ..cltValue)
end


function onCanRx(bus, id, dlc, data)
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