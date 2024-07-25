-- scriptname launch_control.lua

-- uncomment functions setIgnDisabled and setFuelDisabled below to use script with rusefi_simulator:
--[[
function setIgnDisabled(value) end
function setFuelDisabled(value) end
]]--

local SPEED_THRESHOLD = 30 -- emulates `Speed Threshold(Kph)` setting on `Launch Control` dialog in TunerStudio
local LAUNCH_RPM = 3000 -- emulates `Launch RPM(rpm)` setting on `Launch Control` dialog in TunerStudio
local LAUNCH_CONTROL_WINDOW = 500 -- emulates `Launch Control Window(RPM)` setting on `Launch Control` dialog in TunerStudio
local TPS_THRESHOLD  = 0 -- emulates `Speed Threshold(Kph)` setting on `Launch Control` dialog in TunerStudio
local LAUNCH_CORRECTIONS_END_RPM = 0 -- emulates `Launch Corrections End RPM` setting on `Launch Control` dialog in TunerStudio
local IGNITION_CUT = true -- emulates `Ignition Cut` setting on `Launch Control` dialog in TunerStudio
local INITIAL_IGNITION_CUT = 0 -- emulates `Initial Ignition Cut %` setting on `Launch Control` dialog in TunerStudio
local FINAL_IGNITION_CUT_BEFORE_LAUNCH = 0 -- emulates `Final Ignition Cut % Before launch (%)` setting on `Launch Control` dialog in TunerStudio
local FUEL_CUT = false -- emulates `Fuel Cut` setting on `Launch Control` dialog in TunerStudio

local isSwitchConditionSatisfied
function isSwitchConditionSatisfied()
	--[[ Here we assume that Launch Control is already activated, i.e. condition specified by the following settings on `Launch Control` dialog in TunerStudio:
	   * `Enable Launch Control`
	   * `Activation Mode`
	   * `Launch Button`
	   * `Launch Button inverted`
	   * `Launch Button mode`
	   * `Clutch Down`
	   * `Clutch Down inverted`
	   * `Clutch Down mode`
	   - is already satisfied
	]]--
	return true
end

print('SPEED_THRESHOLD = ' .. SPEED_THRESHOLD)

local isSpeedConditionSatisfied
function isSpeedConditionSatisfied()
 	if SPEED_THRESHOLD == 0 then
		print('SPEED_THRESHOLD = ' .. tostring(SPEED_THRESHOLD))
		return true
	else
		local currentVehicleSpeed = getSensor("VehicleSpeed")
		if currentVehicleSpeed == nil then
			print('currentVehicleSpeed is undefined, so we are using default zero value')
			currentVehicleSpeed = 0
		else
			print('currentVehicleSpeed = ' .. currentVehicleSpeed)
		end
		return currentVehicleSpeed < SPEED_THRESHOLD
	end
end

local isTpsConditionSatisfied
function isTpsConditionSatisfied()
	local currentDriverThrotrrentDriverThrottleIntetleIntent = getSensor("DriverThrottleIntent")
	if currentDriverThrottleIntent == nil then
		print('currentVehicleSpeed is undefined, so TPS condition is not satisfied')
		return false
	else
		print('currentDriverThrottleIntent = ' .. currentDriverThrottleIntent)
	end
	return TPS_THRESHOLD < currentDriverThrottleIntent
end

-- Launch condition enumeration
local LaunchCondition = {
	NOT_MET = {},
	PRE_LAUNCH = {},
	LAUNCH = {}
}

local calculateRpmLaunchCondition
function calculateRpmLaunchCondition(rpm)
	if rpm < LAUNCH_RPM - LAUNCH_CONTROL_WINDOW then
		return LaunchCondition.NOT_MET
	elseif LAUNCH_RPM <= rpm then
		return LaunchCondition.LAUNCH
	else
		return PRE_LAUNCH
	end
end

local calculateLaunchCondition
function calculateLaunchCondition(rpm)
	local switchCondition = isSwitchConditionSatisfied()

 	if not switchCondition then
		print('Switch condition is not satisfied')
	end

	local speedCondition = isSpeedConditionSatisfied()
	if not speedCondition then
		print('Speed condition is not satisfied')
	end

	local tpsCondition = isTpsConditionSatisfied()
	if not tpsCondition then
		print('TPS condition is not satisfied')
	end

	if switchCondition and speedCondition and tpsCondition then
		return calculateRpmLaunchCondition(rpm)
	else
		return LaunchCondition.NOT_MET
	end
end

function interpolateClamped(x1, y1, x2, y2, x)
  if x <= x1 then
    return y1
  elseif x2 <= x then
    return y2
  else
    return interpolate(x1, y1, x2, y2, x)
  end
end

local updateSparkSkipRatio
function updateSparkSkipRatio(launchCondition, rpm)
  local sparkSkipRatio = 0.0
  if launchCondition == LaunchCondition.LAUNCH then
    sparkSkipRatio = 1.0
  elseif launchCondition == LaunchCondition.PRE_LAUNCH then
    local sparkSkipStartRpm = LAUNCH_RPM - LAUNCH_CONTROL_WINDOW
    local sparkSkipEndRpm = LAUNCH_RPM - LAUNCH_CORRECTIONS_END_RPM
    sparkSkipRatio = interpolateClamped(sparkSkipStartRpm, INITIAL_IGNITION_CUT, sparkSkipEndRpm, FINAL_IGNITION_CUT_BEFORE_LAUNCH)
  end
  setSparkHardSkipRatio(sparkSkipRatio)
end

local updateIgnitionCut
function updateIgnitionCut(launchCondition)
  local disableIgnition = (IGNITION_CUT and launchCondition == LaunchCondition.LAUNCH)
  setIgnDisabled(disableIgnition)
end

local updateFuelCut
function updateFuelCut(launchCondition)
  setFuelDisabled(FUEL_CUT and launchCondition == LaunchCondition.LAUNCH)
end

function onTick()
	local currentRpm = getSensor("Rpm")
	print('currentRpm = ' .. currentRpm)

	local launchCondition = calculateLaunchCondition(currentRpm)
	if launchCondition == LaunchCondition.NOT_MET then
		print('NOT_MET')
	elseif launchCondition == LaunchCondition.PRE_LAUNCH then
		print('PRE_LAUNCH')
	else -- launchCondition == LaunchCondition.LAUNCH
		print('LAUNCH')
	end

	updateSparkSkipRatio(launchCondition, currentRpm)
	updateIgnitionCut(launchCondition)
	updateFuelCut(launchCondition)
end
