### sensorSnifferRpmThreshold
Disable sensor sniffer above this rpm

### launchRpm
A secondary Rev limit engaged by the driver to help launch the vehicle faster

### rpmHardLimit


### engineSnifferRpmThreshold
Engine sniffer would be disabled above this rpm

### multisparkMaxRpm
Disable multispark above this engine speed.

### maxAcRpm
Above this RPM, disable AC. Set to 0 to disable check.

### maxAcTps
Above this TPS, disable AC. Set to 0 to disable check.

### maxAcClt
Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.

### multisparkMaxSparkingAngle
This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.

### multisparkMaxExtraSparkCount
Configures the maximum number of extra sparks to fire (does not include main spark)

### injector.flow
This is your injector flow at the fuel pressure used in the vehicle. cc/min, cubic centimetre per minute\nBy the way, g/s = 0.125997881 * (lb/hr)\ng/s = 0.125997881 * (cc/min)/10.5\ng/s = 0.0119997981 * cc/min

### isForcedInduction
Does the vehicle have a turbo or supercharger?

### useFordRedundantTps
On some Ford and Toyota vehicles one of the throttle sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.

### overrideTriggerGaps


### enableFan1WithAc
Turn on this fan when AC is on.

### enableFan2WithAc
Turn on this fan when AC is on.

### disableFan1WhenStopped
Inhibit operation of this fan while the engine is not running.

### disableFan2WhenStopped
Inhibit operation of this fan while the engine is not running.

### enableTrailingSparks
Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).

### etb_use_two_wires
TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.

### isDoubleSolenoidIdle
Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.

### useEeprom


### useTLE8888_cranking_hack


### kickStartCranking


### useSeparateIdleTablesForCrankingTaper
This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).

### launchControlEnabled


### doNotFilterTriggerEdgeNoise
"Detect double trigger edges"

### antiLagEnabled


### useRunningMathForCranking
For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).

### useTLE8888_stepper


### usescriptTableForCanSniffingFiltering


### verboseCan
Print incoming and outgoing first bus CAN messages in rusEFI console

### artificialTestMisfire
Experimental setting that will cause a misfire\nDO NOT ENABLE.

### useFordRedundantPps
On some Ford and Toyota vehicles one of the pedal sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.

### cltSensorPulldown


### iatSensorPulldown


### allowIdenticalPps


### tpsMin
Closed throttle, 1 volt = 200 units

### tpsMax
Full throttle, 1 volt = 200 units

### tpsErrorDetectionTooLow
TPS error detection: what throttle % is unrealistically low?\nAlso used for accelerator pedal error detection if so equiped.

### tpsErrorDetectionTooHigh
TPS error detection: what throttle % is unrealistically high?\nAlso used for accelerator pedal error detection if so equiped.

### cranking.baseFuel
Base mass of the per-cylinder fuel injected during cranking. This is then modified by the multipliers for CLT, IAT, TPS ect, to give the final cranking pulse width.\nA reasonable starting point is 60mg per liter per cylinder.\nex: 2 liter 4 cyl = 500cc/cyl, so 30mg cranking fuel.

### cranking.rpm
This sets the RPM limit below which the ECU will use cranking fuel and ignition logic, typically this is around 350-450rpm.

### ignitionDwellForCrankingMs
Dwell duration while cranking

### etbRevLimitStart
Once engine speed passes this value, start reducing ETB angle.

### etbRevLimitRange
This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.

### map.sensor.lowValue
kPa value at low volts

### map.sensor.highValue
kPa value at high volts

### clt.config.tempC_1
these values are in Celcius

### clt.config.tempC_2


### clt.config.tempC_3


### clt.config.resistance_1


### clt.config.resistance_2


### clt.config.resistance_3


### clt.config.bias_resistor
Pull-up resistor value on your board

### iat.config.tempC_1
these values are in Celcius

### iat.config.tempC_2


### iat.config.tempC_3


### iat.config.resistance_1


### iat.config.resistance_2


### iat.config.resistance_3


### iat.config.bias_resistor
Pull-up resistor value on your board

### launchTimingRetard


### knockBandCustom
We calculate knock band based of cylinderBore\n Use this to override - kHz knock band override

### displacement
Engine displacement in litres

### triggerSimulatorRpm


### cylindersCount
Number of cylinder the engine has.

### benchTestCount
How many test bench pulses do you want

### benchTestOnTime
Duration of each test pulse

### cylinderBore
Cylinder diameter in mm.

### ALSMaxTPS


### boostControlMinRpm
Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.

### boostControlMinTps
Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.

### boostControlMinMap
Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.

### crankingTimingAngle
Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.\nThere is tapering towards running timing advance

### gapTrackingLengthOverride
How many consecutive gap rations have to match expected ranges for sync to happen

### maxIdleVss
Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).

### minOilPressureAfterStart
Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.

### fixedModeTiming
This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing\nThis mode is useful when adjusting distributor location.

### globalTriggerAngleOffset
Angle between Top Dead Center (TDC) and the first trigger event.\nPositive value in case of synchnization point before TDC and negative in case of synchnization point after TDC\n.Knowing this angle allows us to control timing and other angles in reference to TDC.

### analogInputDividerCoefficient
Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.

### vbattDividerCoeff
This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.

### fanOnTemperature
Cooling fan turn-on temperature threshold, in Celsius

### fanOffTemperature
Cooling fan turn-off temperature threshold, in Celsius

### driveWheelRevPerKm
Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.

### canSleepPeriodMs
CANbus thread period in ms

### byFirmwareVersion


### idle_derivativeFilterLoss
0.1 is a good default value

### trailingSparkAngle
just a temporary solution

### trigger.customTotalToothCount


### trigger.customSkippedToothCount


### airByRpmTaper
Extra air taper amount

### boostControlSafeDutyCycle
Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.

### acrRevolutions


### globalFuelCorrection


### adcVcc


### mapCamDetectionAnglePosition


### afr.v1


### afr.value1


### afr.v2


### afr.value2


### baroSensor.lowValue
kPa value at low volts

### baroSensor.highValue
kPa value at high volts

### idle.solenoidFrequency


### manIdlePosition
Value between 0 and 100 used in Manual mode

### knockRetardAggression
Ignition timing to remove when a knock event occurs.

### knockRetardReapplyRate
After a knock event, reapply timing at this rate.

### vssFilterReciprocal
Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.

### vssGearRatio
Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.

### vssToothCount
Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.

### idle_antiwindupFreq
0.1 is a good default value

### mc33_t_min_boost
Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.

### acIdleExtraOffset
Additional idle % while A/C is active

### finalGearRatio
Ratio between the wheels and your transmission output.

### tcuInputSpeedSensorTeeth
null

### wastegatePositionMin
Voltage when the wastegate is closed.\nYou probably don't have one of these!

### wastegatePositionMax
Voltage when the wastegate is fully open.\nYou probably don't have one of these!\n1 volt = 1000 units

### tunerStudioSerialSpeed
Secondary TTL channel baud rate

### compressionRatio
Just for reference really, not taken into account by any logic at this point

### fordInjectorSmallPulseSlope


### is_enabled_spi_1


### is_enabled_spi_2


### is_enabled_spi_3


### isSdCardEnabled
enable sd/disable sd

### rusefiVerbose29b
Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.

### isVerboseAlternator


### useStepperIdle
This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.

### enabledStep1Limiter


### verboseTLE8888


### enableVerboseCanTx
CAN broadcast using custom rusEFI protocol\nenable can_broadcast/disable can_broadcast

### etb1configured


### etb2configured


### measureMapOnlyInOneCylinder
Useful for individual intakes

### stepperForceParkingEveryRestart


### isFasterEngineSpinUpEnabled
If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.

### coastingFuelCutEnabled
This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.

### useIacTableForCoasting
Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.

### useIdleTimingPidControl


### disableEtbWhenEngineStopped
Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.

### is_enabled_spi_4


### pauseEtbControl
Disable the electronic throttle motor and DC idle motor for testing.\nThis mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.

### alignEngineSnifferAtTDC


### enableAemXSeries
AEM X-Series or rusEFI Wideband

### verboseCanBaseAddress


### mc33_hvolt
Boost Voltage

### minimumBoostClosedLoopMap
Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.

### vehicleWeight


### idlePidRpmUpperLimit
How far above idle speed do we consider idling?\nFor example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.

### applyNonlinearBelowPulse
Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.

### stoichRatioSecondary
Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0

### etbMaximumPosition
Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.

### sdCardLogFrequency
Rate the ECU will log to the SD card, in hz (log lines per second).

### mapMinBufferLength
This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.

### idlePidDeactivationTpsThreshold
Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.

### stepperParkingExtraSteps


### tps1SecondaryMin


### tps1SecondaryMax


### antiLagRpmTreshold


### startCrankingDuration
Maximum time to crank starter when start/stop button is pressed

### clutchUpPinInverted


### clutchDownPinInverted


### useHbridgesToDriveIdleStepper
If enabled we use two H-bridges to drive stepper idle air valve

### multisparkEnable


### enableLaunchRetard


### enableCanVss
Read VSS from OEM CAN bus according to selected CAN vehicle configuration.

### enableInnovateLC2


### showHumanReadableWarning


### stftIgnoreErrorMagnitude
If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.

### enableSoftwareKnock


### verboseVVTDecoding
Verbose info in console below engineSnifferRpmThreshold\nenable vvt_details

### invertCamVVTSignal
get invertCamVVTSignal

### knockBankCyl1


### knockBankCyl2


### knockBankCyl3


### knockBankCyl4


### knockBankCyl5


### knockBankCyl6


### knockBankCyl7


### knockBankCyl8


### knockBankCyl9


### knockBankCyl10


### knockBankCyl11


### knockBankCyl12


### tcuEnabled


### canBroadcastUseChannelTwo


### useRawOutputToDriveIdleStepper
If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coilss

### verboseCan2
Print incoming and outgoing second bus CAN messages in rusEFI console

### boostPid.pFactor


### boostPid.iFactor


### boostPid.dFactor


### boostPid.offset
Linear addition to PID logic

### boostPid.periodMs
PID dTime

### boostPid.minValue
Output Min Duty Cycle

### boostPid.maxValue
Output Max Duty Cycle

### boostPwmFrequency


### launchSpeedThreshold
Disabled above this speed

### launchTimingRpmRange
Range from Launch RPM for Timing Retard to activate

### launchFuelAdded
Extra Fuel Added

### launchBoostDuty
Duty Cycle for the Boost Solenoid

### hardCutRpmRange
Range from Launch RPM to activate Hard Cut

### turbochargerFilter
null

### launchTpsThreshold


### launchActivateDelay


### stft.maxIdleRegionRpm
Below this RPM, the idle region is active

### stft.maxOverrunLoad
Below this engine load, the overrun region is active

### stft.minPowerLoad
Above this engine load, the power region is active

### stft.deadband
When close to correct AFR, pause correction. This can improve stability by not changing the adjustment if the error is extremely small, but is not required.

### stft.minClt
Below this temperature, correction is disabled.

### stft.minAfr
Below this AFR, correction is paused

### stft.maxAfr
Above this AFR, correction is paused

### stft.startupDelay
Delay after starting the engine before beginning closed loop correction.

### tps2SecondaryMin


### tps2SecondaryMax


### widebandOnSecondBus
Select which bus the wideband controller is attached to.

### fuelClosedLoopCorrectionEnabled
Enables lambda sensor closed loop feedback for fuelling.

### isVerboseIAC
Print details into rusEFI console\nenable verbose_idle

### boardUseTachPullUp


### boardUseTempPullUp


### yesUnderstandLocking


### silentTriggerError
Sometimes we have a performance issue while printing error

### useLinearCltSensor


### canReadEnabled
enable can_read/disable can_read

### canWriteEnabled
enable can_write/disable can_write

### useLinearIatSensor


### boardUse2stepPullDown


### tachPulseDurationAsDutyCycle
Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%

### isAlternatorControlEnabled
This enables smart alternator control and activates the extra alternator settings.

### invertPrimaryTriggerSignal
https://wiki.rusefi.com/Trigger-Configuration-Guide\nThis setting flips the signal from the primary engine speed sensor.

### invertSecondaryTriggerSignal
https://wiki.rusefi.com/Trigger-Configuration-Guide\nThis setting flips the signal from the secondary engine speed sensor.

### cutFuelOnHardLimit


### cutSparkOnHardLimit
Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.

### launchFuelCutEnable


### launchSparkCutEnable
This is the Cut Mode normally used

### boardUseCrankPullUp


### boardUseCamPullDown


### boardUseCamVrPullUp


### boardUseD2PullDown


### boardUseD3PullDown


### boardUseD4PullDown


### boardUseD5PullDown


### verboseIsoTp


### engineSnifferFocusOnInputs


### launchActivateInverted


### twoStroke


### skippedWheelOnCam
Where is your primary skipped wheel located?

### etbNeutralPosition
Expected neutral position

### isInjectionEnabled


### isIgnitionEnabled


### isCylinderCleanupEnabled
When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.

### complexWallModel
Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?

### alwaysInstantRpm


### isMapAveragingEnabled


### overrideCrankingIacSetting
If enabled, use separate temperature multiplier table for cranking idle position.\nIf disabled, use normal running multiplier table applied to the cranking base position.

### useSeparateAdvanceForIdle
This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.

### isWaveAnalyzerEnabled


### useSeparateVeForIdle
This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.

### verboseTriggerSynchDetails
Verbose info in console below engineSnifferRpmThreshold\nenable trigger_details

### isManualSpinningMode
Usually if we have no trigger events that means engine is stopped\nUnless we are troubleshooting and spinning the engine by hand - this case a longer\ndelay is needed

### twoWireBatchInjection
This is needed if your coils are individually wired and you wish to use batch injection.\nenable two_wire_batch_injection

### neverInstantRpm


### useFixedBaroCorrFromMap


### useSeparateAdvanceForCranking
In Constant mode, timing is automatically tapered to running as RPM increases.\nIn Table mode, the "Cranking ignition advance" table is used directly.

### useAdvanceCorrectionsForCranking
This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).\nYou probably don't need this.

### flexCranking
Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.

### useIacPidMultTable
This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller

### isBoostControlEnabled


### launchSmoothRetard
Interpolates the Ignition Retard from 0 to 100% within the RPM Range

### isPhaseSyncRequiredForIgnition
Some engines are OK running semi-random sequential while other engine require phase synchronization

### useCltBasedRpmLimit
If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.

### forceO2Heating
If enabled, don't wait for engine start to heat O2 sensors. WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.

### invertVvtControlIntake
If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.

### invertVvtControlExhaust
If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.

### useBiQuadOnAuxSpeedSensors


### sdTriggerLog
'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.

### ALSActivateInverted


### engineChartSize


### turboSpeedSensorMultiplier


### acIdleRpmBump
Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.

### warningPeriod


### knockDetectionWindowStart


### knockDetectionWindowEnd


### idleStepperReactionTime


### idleStepperTotalSteps


### noAccelAfterHardLimitPeriodSecs
TODO: finish this #413

### mapAveragingSchedulingAtIndex
At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.

### tachPulseDuractionMs
Duration in ms or duty cycle depending on selected mode

### wwaeTau
Length of time the deposited wall fuel takes to dissipate after the start of acceleration.

### alternatorControl.pFactor


### alternatorControl.iFactor


### alternatorControl.dFactor


### alternatorControl.offset
Linear addition to PID logic

### alternatorControl.periodMs
PID dTime

### alternatorControl.minValue
Output Min Duty Cycle

### alternatorControl.maxValue
Output Max Duty Cycle

### etb.pFactor


### etb.iFactor


### etb.dFactor


### etb.offset
Linear addition to PID logic

### etb.periodMs
PID dTime

### etb.minValue
Output Min Duty Cycle

### etb.maxValue
Output Max Duty Cycle

### airTaperRpmRange
RPM range above upper limit for extra air taper,"RPM", 1, 0, 0, 1500, 0

### tps2Min


### tps2Max


### tachPulsePerRev


### mapErrorDetectionTooLow
kPa value which is too low to be true

### mapErrorDetectionTooHigh
kPa value which is too high to be true

### multisparkSparkDuration
How long to wait for the spark to fire before recharging the coil for another spark.

### multisparkDwell
This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.

### idleRpmPid.pFactor


### idleRpmPid.iFactor


### idleRpmPid.dFactor


### idleRpmPid.offset
Linear addition to PID logic

### idleRpmPid.periodMs
PID dTime

### idleRpmPid.minValue
Output Min Duty Cycle

### idleRpmPid.maxValue
Output Max Duty Cycle

### wwaeBeta
0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.

### throttlePedalUpVoltage


### throttlePedalWOTVoltage
Pedal in the floor

### startUpFuelPumpDuration
on IGN voltage detection turn fuel pump on to build fuel pressure

### idlePidRpmDeadZone
If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation

### targetVBatt
This is the target battery voltage the alternator PID control will attempt to maintain

### alternatorOffAboveTps
Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.

### afterCrankingIACtaperDuration
This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.

### iacByTpsTaper
Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value

### tpsAccelLookback
How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.

### coastingFuelCutVssLow
Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.

### coastingFuelCutVssHigh
Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.

### noFuelTrimAfterDfcoTime
Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.

### tpsAccelEnrichmentThreshold
Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.

### totalGearsCount


### fan1ExtraIdle
Additional idle % when fan #1 is active

### uartConsoleSerialSpeed
Band rate for primary TTL

### tpsDecelEnleanmentThreshold
For decel we simply multiply delta of TPS and tFor decel we do not use table?!

### tpsDecelEnleanmentMultiplier
Magic multiplier, we multiply delta of TPS and get fuel squirt duration

### auxSerialSpeed


### throttlePedalSecondaryUpVoltage


### throttlePedalSecondaryWOTVoltage
Pedal in the floor

### mc33_hpfp_i_peak


### mc33_hpfp_i_hold


### mc33_hpfp_i_hold_off
How long to deactivate power when hold current is reached before applying power again

### mc33_hpfp_max_hold
Maximum amount of time the solenoid can be active before assuming a programming error

### stepperDcInvertedPins
Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)

### canOpenBLT
Allow OpenBLT on Primary CAN

### can2OpenBLT
Allow OpenBLT on Secondary CAN

### injectorFlowAsMassFlow
Select whether to configure injector flow in volumetric flow (defualt, cc/min) or mass flow (g/s).

### benchTestOffTime
Time between bench test pulses

### boostCutPressure
MAP value above which fuel is cut in case of overboost.\nSet to 0 to disable overboost cut.

### fixedTiming
Fixed timing, useful for TDC testing

### mapLowValueVoltage
MAP voltage for low point

### mapHighValueVoltage
MAP voltage for low point

### egoValueShift
EGO value correction

### crankingIACposition
This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.

### tChargeMinRpmMinTps


### tChargeMinRpmMaxTps


### tChargeMaxRpmMinTps


### tChargeMaxRpmMaxTps


### alternatorPwmFrequency


### fan2ExtraIdle
Additional idle % when fan #2 is active

### primingDelay
Delay to allow fuel pressure to build before firing the priming pulse.

### fuelReferencePressure
This is the pressure at which your injector flow is known.\nFor example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.

### postCrankingFactor
Fuel multiplier (enrichment) immediately after engine start

### postCrankingDurationSec
Time over which to taper out after start enrichment

### auxTempSensor1.config.tempC_1
these values are in Celcius

### auxTempSensor1.config.tempC_2


### auxTempSensor1.config.tempC_3


### auxTempSensor1.config.resistance_1


### auxTempSensor1.config.resistance_2


### auxTempSensor1.config.resistance_3


### auxTempSensor1.config.bias_resistor
Pull-up resistor value on your board

### auxTempSensor2.config.tempC_1
these values are in Celcius

### auxTempSensor2.config.tempC_2


### auxTempSensor2.config.tempC_3


### auxTempSensor2.config.resistance_1


### auxTempSensor2.config.resistance_2


### auxTempSensor2.config.resistance_3


### auxTempSensor2.config.bias_resistor
Pull-up resistor value on your board

### knockSamplingDuration


### etbFreq


### etbWastegatePid.pFactor


### etbWastegatePid.iFactor


### etbWastegatePid.dFactor


### etbWastegatePid.offset
Linear addition to PID logic

### etbWastegatePid.periodMs
PID dTime

### etbWastegatePid.minValue
Output Min Duty Cycle

### etbWastegatePid.maxValue
Output Max Duty Cycle

### stepperMinDutyCycle
Use to limit the current when the stepper motor is idle, not moving (100% = no limit)

### stepperMaxDutyCycle
Use to limit the max.current through the stepper motor (100% = no limit)

### idlePidActivationTime


### oilPressure.v1


### oilPressure.value1


### oilPressure.v2


### oilPressure.value2


### fan2OnTemperature
Cooling fan turn-on temperature threshold, in Celsius

### fan2OffTemperature
Cooling fan turn-off temperature threshold, in Celsius

### auxFrequencyFilter
null

### vvtControlMinRpm


### launchFuelAdderPercent
null

### etbJamTimeout
Time required to detect a stuck throttle.

### etbExpAverageLength
By the way ETB PID runs at 500hz, length in 1/500 of second here.

### coastingFuelCutRpmHigh
This sets the RPM above which fuel cut is active.

### coastingFuelCutRpmLow
This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle

### coastingFuelCutTps
Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.

### coastingFuelCutClt
Fuel cutoff is disabled when the engine is cold.

### pidExtraForLowRpm
Increases PID reaction for RPM<target by adding extra percent to PID-error

### coastingFuelCutMap
MAP value above which fuel injection is re-enabled.

### highPressureFuel.v1


### highPressureFuel.value1


### highPressureFuel.v2


### highPressureFuel.value2


### lowPressureFuel.v1


### lowPressureFuel.value1


### lowPressureFuel.v2


### lowPressureFuel.value2


### tChargeAirCoefMin
Heat transfer coefficient at zero flow.\n0 means the air charge is fully heated to the same temperature as CLT.\n1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.

### tChargeAirCoefMax
Heat transfer coefficient at high flow, as defined by "max air flow".\n0 means the air charge is fully heated to the same temperature as CLT.\n1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.

### tChargeAirFlowMax
High flow point for heat transfer estimation.\nSet this to perhaps 50-75% of your maximum airflow at wide open throttle.

### tChargeAirIncrLimit
Maximum allowed rate of increase allowed for the estimated charge temperature

### tChargeAirDecrLimit
Maximum allowed rate of decrease allowed for the estimated charge temperature

### etb_iTermMin
iTerm min value

### etb_iTermMax
iTerm max value

### idleTimingPid.pFactor


### idleTimingPid.iFactor


### idleTimingPid.dFactor


### idleTimingPid.offset
Linear addition to PID logic

### idleTimingPid.periodMs
PID dTime

### idleTimingPid.minValue
Output Min Duty Cycle

### idleTimingPid.maxValue
Output Max Duty Cycle

### etbRocExpAverageLength
By the way ETB PID runs at 500hz, length in 1/500 of second here.

### tpsAccelFractionPeriod
A delay in cycles between fuel-enrich. portions

### tpsAccelFractionDivisor
A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions

### idlerpmpid_iTermMin
iTerm min value

### stoichRatioPrimary
Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.\nE0 = 14.7\nE10 = 14.1\nE85 = 9.9\nE100 = 9.0

### idlerpmpid_iTermMax
iTerm max value

### etbIdleThrottleRange
This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.

### triggerCompCenterVolt
Trigger comparator center point voltage

### triggerCompHystMin
Trigger comparator hysteresis voltage (Min)

### triggerCompHystMax
Trigger comparator hysteresis voltage (Max)

### triggerCompSensorSatRpm
VR-sensor saturation RPM

### mc33_i_boost
Boost Current

### mc33_i_peak
Peak Current

### mc33_i_hold
Hold Current

### mc33_t_max_boost
Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.

### mc33_t_peak_off


### mc33_t_peak_tot
Peak phase duration

### mc33_t_bypass


### mc33_t_hold_off


### mc33_t_hold_tot
Hold phase duration

### maxCamPhaseResolveRpm
Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.

### dfcoDelay
Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...

### acDelay
Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.

### fordInjectorSmallPulseBreakPoint


### etbJamIntegratorLimit


### hpfpCamLobes


### hpfpPeakPos
Crank angle ATDC of first lobe peak

### hpfpMinAngle
If the requested activation time is below this angle, don't bother running the pump

### hpfpPumpVolume
Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.

### hpfpActivationAngle
How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)

### issFilterReciprocal
null

### hpfpPidP


### hpfpPidI


### hpfpTargetDecay
The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.

### vvtActivationDelayMs
We need to give engine time to build oil pressure without diverting it to VVT

### acrDisablePhase


### auxLinear1.v1


### auxLinear1.value1


### auxLinear1.v2


### auxLinear1.value2


### auxLinear2.v1


### auxLinear2.value1


### auxLinear2.v2


### auxLinear2.value2


### etbMinimumPosition


### tuneHidingKey


### ALSMinRPM


### ALSMaxRPM


### ALSMaxDuration


### ALSMinCLT


### ALSMaxCLT


### alsMinTimeBetween


### alsEtbPosition


### acRelayAlternatorDutyAdder
null

### instantRpmRange


### ALSIdleAdd


### ALSEtbAdd


### ALSSkipRatio


### ALSMaxDriverThrottleIntent


### tpsSecondaryMaximum
For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%

### ppsSecondaryMaximum
For Toyota ETCS-i, use ~69%

### rpmHardLimitHyst
If the hard limit is 7200rpm and hysteresis is 200rpm, then when the ECU sees 7200rpm, fuel/ign will cut, and stay cut until 7000rpm (7200-200) is reached

