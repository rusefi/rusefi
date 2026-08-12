	setLuaScript(GET_BIT_RANGE_LSB TWO_BYTES_LSB PRINT_ARRAY SET_TWO_BYTES_LSB R"(


isUseful = Timer.new()
isUseful : reset()

function onTick()

rpm = getSensor("RPM")
vbat = getSensor("BatteryVoltage")

--		print (vbat .. " " .. rpm)
if (vbat < 8) or (rpm > 200) then
-- keep alive if USB hooked up
isUseful : reset()
end

	if (isUseful : getElapsedSeconds() > 17) then
		mcu_standby()
	end
end

)");