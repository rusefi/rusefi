-- analog knob controls desired launch RPM and boost
desiredRpmCurve = findCurveIndex("desired_rpm")
desiredBoostCurve = findCurveIndex("desired_boost")
-- we lookup initial torque reduction based on desired RPM and boost
initialTorqueReductionTable = findTableIndex("torque_red")
-- we have tables for spark skip ratio and spark retard based on desired torque reduction
sparkCutByTorqueCurve = findCurveIndex("spark_cut")
sparkRetardByTorqueCurve = findCurveIndex("spark_retard")

previousLaunchButtonState = 0

-- 50Hz
setTickRate(50)

-- we PID torque reduction TODO hard code better PID settings?
-- p, i, d, min, max
pid = Pid.new(2, 0, 0, -100, 100)

function onTick()
	launchButtonState = getAuxAnalog(0) > 1.5
	launchStrength = getAuxAnalog(1)

	if previousLaunchButtonState == 1 and launchButtonState == 0 then
		print "Exiting LC"
	elseif previousLaunchButtonState == 0 and launchButtonState == 1 then
		desiredRPM = curve(desiredRpmCurve, launchStrength)
		desiredBoost = curve(desiredBoostCurve, launchStrength)

		initialTorqueReduction = table3d(initialTorqueReductionTable, desiredRPM, desiredBoost)
		pid : setOffset(initialTorqueReduction)a


		print ("Running LC " ..desiredRPM .." boost=" ..desiredBoost ..' t=' ..initialTorqueReduction)

	elseif launchButtonState == 1 then
		print ("Running LC desired " ..desiredRPM .." boost=" ..desiredBoost)

		rpm = getSensor("RPM")
		-- handle nil RPM, todo: change firmware to avoid nil RPM
		rpm = (rpm == nil and 0 or rpm)

		print ("Running current " ..rpm .." boost=" ..desiredBoost)

		torqueReduction = pid : get(desiredRPM, rpm)
		sparkCut = curve(sparkCutByTorqueCurve, torqueReduction)
		sparkRetard = curve(sparkRetardByTorqueCurve, torqueReduction)

		setTimingAdd(sparkRetard)
		setSparkSkipRatio(sparkRetard)
		print ("Setting current " ..sparkRetard .." boost=" ..sparkRetard)

	else
		print "Not running LC"
		setTimingAdd(0)
		setSparkSkipRatio(0)
	end

end

