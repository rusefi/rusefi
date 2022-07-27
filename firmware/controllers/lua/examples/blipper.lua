local currentGear = 0;

function etbAddForRpm(rpm, targetRpm)
	local error = rpm - targetRpm

	-- Throttle position required to just hold open
	local openLoop = curve(1, rpm)

	-- Closed loop +- to get there
	local closedLoop = curve(2, error)

	return openLoop + closedLoop
end

local timer = Timer.new()

function doRevMatching()
	local rpm = getSensor("RPM")

	local clutchDown = getDigital(0)
	local brakeDown = getDigital(2)
	local rpmHighEnough = rpm > 1500

	-- Only activate if clutch AND brake are pressed AND rpm is above 1500
	if clutchDown and brakeDown and rpmHighEnough then
		-- If we've been in the condition for too long, cancel to avoid stuck throttle
		if timer:getElapsedSeconds() > 1 then
			return
		end

		-- Don't rev match on a downshift from 2nd or 1st
		if currentGear <= 2 then
			return
		end

		-- Get the ideal RPM in the next gear down
		local targetRpm = getRpmInGear(currentGear - 1)

		setEtbAdd(etbAddForRpm(rpm, targetRpm))
	else
		setEtbAdd(0)

		-- While the engine is engaged, get the current gear and rpm
		currentGear = getCurrentGear()
		currentRpm = rpm

		timer:reset()
	end
end

function onTick()
	doRevMatching()
end
