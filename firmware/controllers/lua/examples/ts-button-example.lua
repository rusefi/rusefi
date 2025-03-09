-- for test use `lua_button 1` command in console

t = Timer.new()

count = getTsButtonCount(1)

finderIndex = 0
isOn = false

startPwm(finderIndex, 100, 0)

function onTick()
	if isOn and t : getElapsedSeconds() > 0.5 then
		setPwmDuty(finderIndex, 0)
		isOn = false
		setLuaGauge(2, 0)
		print("Turning OFF")
	end


	local current = getTsButtonCount(1)
	--    print ('Button counter: ' ..  current)
	if (current > count) then
		t : reset()
		isOn = true
		print("Turning ON")
		setPwmDuty(finderIndex, 100)
		count = current
		setLuaGauge(1, count)
		setLuaGauge(2, 1)
	end
end

