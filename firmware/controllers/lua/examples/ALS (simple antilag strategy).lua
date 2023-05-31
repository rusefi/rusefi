-- Simple ANTILAG strategy by TurboMarian (use at your own risk!)

-- index 0, 100Hz, zero duty inititally
startPwm(0, 100, 0)            -- Output for external ALS valve (output 0)

function onTick()
	als_valve = getSensor("RPM") > 1500 and getSensor("CLT") < 100 and getSensor("TPS1") < 5

	if getSensor("RPM") > 1500 and getSensor("CLT") < 100 and getSensor("TPS1") < 5 then

		setIdleAdd(30)     -- Add idle valve air
		setTimingAdd(-30)  -- Add timing
		setFuelAdd(5)      -- Add fuel
		setEtbAdd(0)       -- Add air ETB

	else
		setIdleAdd(0)
		setTimingAdd(0)
		setFuelAdd(0)
		setEtbAdd(0)

	end
	-- lua does not have ternary ? : operator, this here means "1 if als_valve and 0 otherwise"
	setPwmDuty(0, als_valve and 1 or 0)


end
