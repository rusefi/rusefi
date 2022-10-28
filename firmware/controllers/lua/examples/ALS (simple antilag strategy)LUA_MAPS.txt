-- Simple ANTILAG strategy by TurboMarian (use at your own risk!)

-- index 0, 100Hz, zero duty inititally
startPwm(0, 100, 0) -- Output for external ALS valve (output 0)

IdleAddTable = findTableIndex("Idle")
TimingAddTable = findTableIndex("Timing")
FuelAddTable = findTableIndex("Fuel")
SparkSkipAddTable = findTableIndex("SparkSkip")  -- 0.0-1

Rpm = getSensor("Rpm")
Map = getSensor("Map")

IdleAdd = table3d(IdleAddTable, Rpm , Map)
TimingAdd = table3d(TimingAddTable, Rpm , Map)
FuelAdd = table3d(FuelAddTable, Rpm , Map)
SparkSkipAdd = table3d(SparkSkipAddTable, Rpm , Map)

function onTick()
	
als_valve = getSensor("RPM") > 1300 and getSensor("RPM") < 3000 and getSensor("CLT") < 100 and getSensor("CLT") > 60 and getSensor("TPS1") < 2 and getSensor("MAP") < 120 and getDigital(0)



if getSensor("RPM") > 1300 and getSensor("RPM") < 3000 and getSensor("CLT") < 100 and getSensor("CLT") > 60 and getSensor("TPS1") < 2 and getSensor("MAP") < 120 and getDigital(0) then


      -- getDigital(0) Clutch down
            

setIdleAdd(IdleAdd)
setTimingAdd(-TimingAdd)
setFuelAdd(FuelAdd)
setEtbAdd(0)
setSparkSkipRatio(SparkSkipAdd/10)
            

		print ("Antilag")

	else
		setIdleAdd(0)
		setTimingAdd(0)
		setFuelAdd(0)
		setEtbAdd(0)
		setSparkSkipRatio(0)

	end
	-- lua does not have ternary ? : operator, this here means "1 if als_valve and 0 otherwise"
	setPwmDuty(0, als_valve and 1 or 0)


end
