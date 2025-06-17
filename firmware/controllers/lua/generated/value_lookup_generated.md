### startButtonSuppressOnStartUpMs
null

### sensorSnifferRpmThreshold
Desabilitar sensor sniffer acima desta rpm

### launchRpm
Um limitador de giros secundario acionado pelo piloto para ajudar o veiculo a acelerar mais rapido

### rpmHardLimit
set rpm_hard_limit X

### engineSnifferRpmThreshold
Engine sniffer seria desabilitado acima desta rpm\nset engineSnifferRpmThreshold X

### multisparkMaxRpm
Desabilitar multispark acima desta velocidade do motor.

### maxAcRpm
Acima desta RPM, desabilitar ar condicionado. Configure 0 para desabilitar verificacao.

### maxAcTps
Acima deste TPS, desabilitar ar condicionado. Configure 0 para desabilitar verificacao.

### maxAcClt
Acima desta CLT, desabilitar ar condicionado para prevenir superaquecimento do motor. Configure 0 para desabilitar verificacao.

### compressionRatio
Apenas para referencia, nao levado em conta por nenhuma logica neste ponto

### wastegatePositionMin
Voltagem quando a wastegate esta fechada.\nVoce provavelmente nao tem uma dessas!

### wastegatePositionMax
Voltagem quando a wastegate esta totalmente aberta.\nVoce provavelmente nao tem uma dessas!\n1 volt = 1000 unidades

### idlePositionMin
Voltagem quando a valvula de marcha lenta esta fechada.\nVoce provavelmente nao tem uma dessas!

### idlePositionMax
Voltagem quando a valvula de marcha lenta esta aberta.\nVoce provavelmente nao tem uma dessas!\n1 volt = 1000 unidades

### ltitEnabled
Enable LTIT (Long Term Idle Trim) learning

### ltitEmaAlpha
Constante de filtro EMA para LTIT (0-255)

### ltitStableRpmThreshold
Faixa de RPM para considerar marcha lenta estavel

### ltitStableTime
Tempo minimo de marcha lenta estavel antes do aprendizado

### ltitCorrectionRate
Taxa de aprendizado LTIT

### ltitIgnitionOnDelay
Atraso apos ignicao ligada antes do aprendizado/aplicacao LTIT

### ltitIgnitionOffSaveDelay
Atraso apos ignicao desligada antes de salvar LTIT

### ltitClampMin
Valor minimo de correcao multiplicativa LTIT

### ltitClampMax
Valor maximo de correcao multiplicativa LTIT

### ltitSmoothingIntensity
Intensidade de suavizacao regional da tabela LTIT (0=sem suavizacao)

### ltitIntegratorThreshold
Threshold minimo do integrador PID para correcao LTIT

### launchFuelAdderPercent


### etbJamTimeout
Tempo apos o qual o acelerador e considerado travado.

### acIdleExtraOffset
Porcentagem adicional de marcha lenta enquanto ar condicionado estiver ativo

### multisparkMaxSparkingAngle
Este parametro define o mais tardio que o ultimo multispark pode ocorrer apos o evento principal de ignicao. Por exemplo, se o tempo de ignicao e 30 graus BTDC, e este parametro e definido como 45, nenhum multispark sera disparado apos 15 graus ATDC.

### multisparkMaxExtraSparkCount
Configura o numero maximo de fagulhas extras para disparar (nao inclui fagulha principal)

### vvtControlMinRpm


### injector.flow
Este e o fluxo do seu injetor na pressao de combustivel usada no veiculo\nVeja configuracao de unidades abaixo

### injectorSecondary.flow
Este e o fluxo do seu injetor na pressao de combustivel usada no veiculo\nVeja configuracao de unidades abaixo

### isForcedInduction
Does the vehicle have a turbo or supercharger?

### useFordRedundantTps
On some Ford and Toyota vehicles one of the throttle sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.

### enableKline


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
TLE7209 and L6205 use two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.

### isDoubleSolenoidIdle
Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.

### useEeprom


### useCicPidForIdle
Switch between Industrial and Cic PID implementation

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

### displayLogicLevelsInEngineSniffer
Shall we display real life signal or just the part consumed by trigger decoder.\nApplies to both trigger and cam/vvt input.\n\nenable logic_level_trigger

### useTLE8888_stepper


### usescriptTableForCanSniffingFiltering


### verboseCan
Print incoming and outgoing first bus CAN messages in evoTech console

### artificialTestMisfire
Experimental setting that will cause a misfire\nDO NOT ENABLE.

### useFordRedundantPps
On some Ford and Toyota vehicles one of the pedal sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.

### cltSensorPulldown


### iatSensorPulldown


### allowIdenticalPps


### overrideVvtTriggerGaps


### useSpiImu
If enabled - use onboard SPI Accelerometer, otherwise listen for CAN messages

### enableStagedInjection


### Fan1AcThresholdOn
Theshold to enable fan 1 after ac turned on

### Fan1AcThresholdOff
Theshold to disable fan 1 after ac tuned off

### Fan2AcThresholdOn
Theshold to enable fan 2 after ac turned on

### Fan2AcThresholdOff
Theshold to disable fan 2 after ac tuned off

### useIdleAdvanceWhileCoasting
Use idle ignition table while coasting;

### tpsMin
Closed throttle, 1 volt = 200 units.\nSee also tps1_1AdcChannel\nset tps_min X

### tpsMax
Full throttle.\nSee also tps1_1AdcChannel\nset tps_max X

### tpsErrorDetectionTooLow
TPS error detection: what throttle % is unrealistically low?\nAlso used for accelerator pedal error detection if so equipped.

### tpsErrorDetectionTooHigh
TPS error detection: what throttle % is unrealistically high?\nAlso used for accelerator pedal error detection if so equipped.

### cranking.baseFuel
Massa base do combustivel por cilindro injetado durante a partida. Isso e modificado pelos multiplicadores para CLT, IAT, TPS etc, para dar a largura final do pulso de partida.\nUm ponto de partida razoavel e 60mg por litro por cilindro.\nex: 2 litros 4 cil = 500cc/cil, entao 30mg combustivel de partida.

### cranking.rpm
Isso define o limite de RPM abaixo do qual a ECU usara combustivel e logica de ignicao de partida, tipicamente isso e cerca de 350-450rpm. \nset cranking_rpm X

### ignitionDwellForCrankingMs
Duracao do dwell durante a partida

### etbRevLimitStart
Uma vez que a velocidade do motor passe este valor, comece a reduzir o angulo ETB.

### etbRevLimitRange
Esta distancia acima do 'Inicio do limitador suave', feche totalmente a borboleta. Na parte inferior da faixa, o controle da borboleta e normal. Na parte superior da faixa, a borboleta esta totalmente fechada.

### map.sensor.lowValue
Valor kPa em voltagem baixa

### map.sensor.highValue
Valor kPa em voltagem alta

### clt.config.tempC_1
estes valores estao em Celsius

### clt.config.tempC_2


### clt.config.tempC_3


### clt.config.resistance_1


### clt.config.resistance_2


### clt.config.resistance_3


### clt.config.bias_resistor
Valor do resistor pull-up em sua placa

### iat.config.tempC_1
estes valores estao em Celsius

### iat.config.tempC_2


### iat.config.tempC_3


### iat.config.resistance_1


### iat.config.resistance_2


### iat.config.resistance_3


### iat.config.bias_resistor
Valor do resistor pull-up em sua placa

### launchTimingRetard


### hip9011Prescaler
value '6' for 8MHz hw osc\nread hip9011 datasheet for details\ntodo split into two bit fields

### tuningDetector
Valor zero significa nao detectar ajuste

### alternator_iTermMin
Valor minimo iTerm

### alternator_iTermMax
Valor maximo iTerm

### displacement
@@DISPLACEMENT_TOOLTIP@@

### triggerSimulatorRpm


### cylindersCount
Number of cylinder the engine has.

### justATempTest
null

### mapSyncThreshold
Delta kPa para sincronizacao MAP

### torqueReductionIgnitionCut
Quantos % dos eventos de ignicao serao cortados

### cylinderBore
@@CYLINDER_BORE_TOOLTIP@@

### ALSMaxTPS


### boostControlMinRpm
Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.

### boostControlMinTps
Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.

### boostControlMinMap
Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.

### crankingTimingAngle
Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.\nThere is tapering towards running timing advance\nset cranking_timing_angle X

### gapTrackingLengthOverride
Quantas razoes de lacuna consecutivas tem que corresponder aos intervalos esperados para que a sincronizacao aconteca

### maxIdleVss
Acima desta velocidade, desabilitar controle de marcha lenta em malha fechada. Configure 0 para desabilitar (permitir marcha lenta em malha fechada em qualquer velocidade).

### camDecoder2jzPrecision
null

### minOilPressureAfterStart
Pressao de oleo esperada apos ligar o motor. Se a pressao de oleo nao atingir este nivel dentro de 5 segundos apos a partida do motor, o combustivel sera cortado. Configure 0 para desabilitar e sempre permitir a partida.

### enableSanderoPump
This option enables sandelo eletrohydraulic steering pump to be CAN controller by the ECU;

### fixedModeTiming
This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing\nThis mode is useful when adjusting distributor location.

### globalTriggerAngleOffset
Angulo entre o Ponto Morto Superior (TDC) e o primeiro evento de trigger.\nValor positivo no caso de ponto de sincronizacao antes do TDC e negativo no caso de ponto de sincronizacao apos TDC\n.Conhecer este angulo nos permite controlar tempo e outros angulos em referencia ao TDC.\nCOMO FAZER:\n1: Mude para modo de tempo fixo no dialogo de configuracao de ignicao\n2: use uma pistola de ponto real para calibrar\n3: adicione/subtraia ate que a pistola de ponto confirme o valor de tempo fixo desejado!'

### analogInputDividerCoefficient
Razao/coeficiente dos divisores de voltagem de entrada em sua PCB. Por exemplo, use '2' se sua placa dividir 5v em 2.5v. Use '1.66' se sua placa dividir 5v em 3v.

### vbattDividerCoeff
Esta e a razao dos resistores para a voltagem da bateria, meca a voltagem na bateria e entao ajuste este numero ate que o medidor corresponda a leitura.

### fanOnTemperature
Limite de temperatura para ligar o ventilador de resfriamento, em Celsius

### fanOffTemperature
Limite de temperatura para desligar o ventilador de resfriamento, em Celsius

### driveWheelRevPerKm
Numero de revolucoes por quilometro para as rodas onde seu sensor de velocidade do veiculo esta conectado. Use uma calculadora online para determinar isso baseado no tamanho do seu pneu.

### canSleepPeriodMs
Periodo da thread CANbus em ms

### byFirmwareVersion


### idle_derivativeFilterLoss
0.1 is a good default value

### trigger.customTotalToothCount


### trigger.customSkippedToothCount


### airByRpmTaper
Quantidade de afunilamento de ar extra

### boostControlSafeDutyCycle
Ciclo de trabalho para usar em caso de falha do sensor. Este ciclo de trabalho deve produzir a menor quantidade possivel de boost. Este ciclo tambem e usado caso qualquer das condicoes minimas de RPM/TPS/MAP nao sejam atendidas.

### acrRevolutions


### calibrationBirthday
null

### adcVcc


### mapCamDetectionAnglePosition
Fase magica do motor: comparamos MAP instantaneo em X com MAP instantaneo em x+360 graus em um ciclo completo

### afr.v1


### afr.value1


### afr.v2


### afr.value2


### etbSplit
Limite de erro TPS/PPS

### baroSensor.lowValue
Valor kPa em voltagem baixa

### baroSensor.highValue
Valor kPa em voltagem alta

### idle.solenoidFrequency


### knockRetardAggression
Tempo de ignicao para remover quando um evento de detonacao ocorrer. Conselho: 5% (leve), 10% (turbo/alta compressao), 15% (alta detonacao, ex. GDI), 20% (bem agressivo)

### knockRetardReapplyRate
Apos um evento de detonacao, reaplicar tempo nesta taxa.

### vssGearRatio
Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.

### vssFilterReciprocal
Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.

### vssToothCount
Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.

### gapVvtTrackingLengthOverride
Quantas razoes de lacuna VVT consecutivas tem que corresponder aos intervalos esperados para que a sincronizacao aconteca

### idle_antiwindupFreq
0.1 is a good default value

### mc33_t_min_boost
Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.

### finalGearRatio
Ratio between the wheels and your transmission output.

### tcuInputSpeedSensorTeeth
null

### wwEnableAdaptiveLearning
Enable adaptive learning for wall wetting parameters

### wwMinCoolantTemp
Minimum coolant temperature for learning

### wwAquinoAccelThresh
Acceleration transient threshold for adaptive detection

### wwAquinoDecelThresh
Deceleration transient threshold for adaptive detection

### wwAquinoMapAccelThresh
MAP acceleration threshold for adaptive detection

### wwAquinoMapDecelThresh
MAP deceleration threshold for adaptive detection

### wwAquinoMinLambdaErr
Minimum lambda error magnitude for adaptation

### wwAquinoTransMinDuration
Minimum transient duration for analysis

### wwAquinoAnalysisMaxDuration
Maximum analysis window duration

### wwAquinoBetaAccelGain
Beta correction gain for acceleration transients

### wwAquinoBetaDecelGain
Beta correction gain for deceleration transients

### wwAquinoBetaLeanThresh
Beta lean threshold for correction

### wwAquinoBetaRichThresh
Beta rich threshold for correction

### wwAquinoTauSettleThresh
Lambda settle threshold for tau analysis

### wwAquinoTauIdealFactor
Ideal settle time factor (multiplied by tau)

### wwAquinoTauToleranceFactor
Tolerance margin factor for settle time

### wwAquinoTauOvershootMinDur
Minimum overshoot duration

### wwAquinoTauOvershootMinMag
Minimum overshoot magnitude

### wwAquinoTauOvershootGain
Tau correction gain for overshoot

### wwAquinoTauSlowGain
Tau correction gain for slow settling

### wwAquinoTauFastGain
Tau correction gain for fast settling

### fordInjectorSmallPulseSlope


### lambdaProtectionMinRpm


### lambdaProtectionMinLoad


### is_enabled_spi_1


### is_enabled_spi_2


### is_enabled_spi_3


### isSdCardEnabled
enable sd/disable sd

### rusefiVerbose29b
Use 11 bit (standard) or 29 bit (extended) IDs for evoTech verbose CAN format.

### rethrowHardFault


### isHip9011Enabled


### requireFootOnBrakeToCrank


### verboseQuad


### useStepperIdle
This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.

### enabledStep1Limiter


### useTpicAdvancedMode


### lambdaProtectionEnable


### verboseTLE8888


### enableVerboseCanTx
CAN broadcast using custom evoTech protocol\nenable can_broadcast/disable can_broadcast

### externalRusEfiGdiModule


### flipWboChannels


### measureMapOnlyInOneCylinder
Useful for individual intakes

### stepperForceParkingEveryRestart


### isFasterEngineSpinUpEnabled
If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.

### coastingFuelCutEnabled
This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.

### useNoiselessTriggerDecoder


### useIdleTimingPidControl


### disableEtbWhenEngineStopped
Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.

### is_enabled_spi_4


### pauseEtbControl
Disable the electronic throttle motor and DC idle motor for testing.\nThis mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.

### tpsTpsPercentMode


### verboseKLine


### idleIncrementalPidCic


### enableAemXSeries
AEM X-Series or evoTech Wideband

### sanderoPumpSpeed


### modeledFlowIdle


### verboseCanBaseAddress


### mc33_hvolt
Voltagem de Boost

### minimumBoostClosedLoopMap
MAP minimo antes que o boost em malha fechada seja habilitado. Use para prevenir comportamento inadequado ao entrar em boost.

### initialIgnitionCutPercent


### finalIgnitionCutPercentBeforeLaunch


### idlePidRpmUpperLimit
How far above idle speed do we consider idling, i.e. coasting detection threshold.\nFor example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.

### applyNonlinearBelowPulse
Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.

### torqueReductionArmingRpm
Como o pino de reducao de torque e geralmente compartilhado com controle de largada, a maioria das pessoas tem uma RPM onde o comportamento abaixo disso e Controle de Largada, acima disso e Reducao de Torque

### stoichRatioSecondary
Razao estequiometrica para seu combustivel secundario. Este valor e usado quando o sensor Flex Fuel indica E100, tipicamente 9.0

### etbMaximumPosition
Posicao maxima permitida do ETB. Algumas borboletas passam da totalmente aberta, entao isso permite limitar a totalmente aberta.

### sdCardLogFrequency
Taxa que a ECU ira registrar no cartao SD, em hz (linhas de log por segundo).

### launchCorrectionsEndRpm


### lambdaProtectionRestoreRpm


### mapMinBufferLength
Esta quantidade de amostras MAP e usada para estimar o MAP atual. Esta quantidade de amostras e considerada, e o minimo e tomado. Valor recomendado e 1 para motores de borboleta unica, e seu numero de cilindros para corpos de borboleta individuais.

### idlePidDeactivationTpsThreshold
Abaixo desta posicao da borboleta, o motor e considerado em marcha lenta. Se voce tem uma borboleta eletronica, isso verifica a posicao do pedal do acelerador em vez da posicao da borboleta, e deve ser definido para 1-2%.

### stepperParkingExtraSteps


### tps1SecondaryMin


### tps1SecondaryMax


### antiLagRpmTreshold


### startCrankingDuration
Tempo maximo para girar o motor de partida quando o botao start/stop e pressionado

### lambdaProtectionMinTps


### lambdaProtectionTimeout
Only respond once lambda is out of range for this period of time. Use to avoid transients triggering lambda protection when not needed

### useHbridgesToDriveIdleStepper
If enabled we use two H-bridges to drive stepper idle air valve

### multisparkEnable


### enableLaunchRetard


### canInputBCM


### consumeObdSensors
This property is useful if using evoTech as TCM or BCM only

### enableCanVss
Read VSS from OEM CAN bus according to selected CAN vehicle configuration.

### suppressWboWorkaround7048


### stftIgnoreErrorMagnitude
If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.

### vvtBooleanForVerySpecialCases


### enableSoftwareKnock


### verboseVVTDecoding
Verbose info in console below engineSnifferRpmThreshold\nenable vvt_details

### invertCamVVTSignal


### alphaNUseIat
In Alpha-N mode, compensate for air temperature.

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
If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coils

### verboseCan2
Print incoming and outgoing second bus CAN messages in evoTech console

### boostPid.pFactor


### boostPid.iFactor


### boostPid.dFactor


### boostPid.offset
Adicao linear a logica PID

### boostPid.periodMs
Tempo PID dTime

### boostPid.minValue
Ciclo de Trabalho Minimo de Saida

### boostPid.maxValue
Ciclo de Trabalho Maximo de Saida

### standbyTimeout
How long it takes to enter standby mode, in seconds. Any value below 60 will cause the ECU to not enter standby, with an average consumption of 80mA

### boostPwmFrequency


### tpsAccelLookback
How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.

### noFuelTrimAfterDfcoTime
Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.

### noFuelTrimAfterAccelTime
Pause closed loop fueling after acceleration fuel occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel accel.

### launchSpeedThreshold
Launch desabilitado acima desta velocidade se a configuracao estiver acima de zero

### launchRpmWindow
Janela de RPM de Launch inicial para ativar (subtrai do RPM de Launch)

### triggerEventsTimeoutMs


### mapExpAverageAlpha


### magicNumberAvailableForDevTricks
null

### turbochargerFilter
null

### launchTpsThreshold


### launchActivateDelay


### stft.maxIdleRegionRpm
Abaixo desta RPM, a regiao de marcha lenta esta ativa, marcha lenta+300 seria um bom valor

### stft.maxOverrunLoad
Abaixo desta carga do motor, a regiao de desaceleracao esta ativa\nQuando ajustando por MAP as unidades sao kPa, ex. 30 significaria 30kPa. Quando ajustando TPS, 30 seria 30%

### stft.minPowerLoad
Acima desta carga do motor, a regiao de potencia esta ativa\nQuando ajustando por MAP as unidades sao kPa

### stft.deadband
Quando proximo do AFR correto, pausar correcao. Isso pode melhorar a estabilidade nao alterando o ajuste se o erro for extremamente pequeno, mas nao e obrigatorio.

### stft.minClt
Abaixo desta temperatura, a correcao e desabilitada.

### stft.minAfr
Abaixo deste AFR, a correcao e pausada

### stft.maxAfr
Acima deste AFR, a correcao e pausada

### stft.startupDelay
Atraso apos ligar o motor antes de iniciar a correcao em malha fechada.

### tps2SecondaryMin


### tps2SecondaryMax


### widebandOnSecondBus
Select which bus the wideband controller is attached to.

### fuelClosedLoopCorrectionEnabled
Enables lambda sensor closed loop feedback for fuelling.

### alwaysWriteSdCard
Write SD card log even when powered by USB

### knockDetectionUseDoubleFrequency
Second harmonic (aka double) is usually quieter background noise

### yesUnderstandLocking


### silentTriggerError
Sometimes we have a performance issue while printing error

### useLinearCltSensor


### canReadEnabled
enable can_read/disable can_read

### canWriteEnabled
enable can_write/disable can_write. See also can1ListenMode

### useLinearIatSensor


### enableOilPressureProtect


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

### torqueReductionEnabled


### limitTorqueReductionTime


### verboseIsoTp
Are you a developer troubleshooting TS over CAN ISO/TP?

### engineSnifferFocusOnInputs


### twoStroke


### skippedWheelOnCam
Where is your primary skipped wheel located?

### etbNeutralPosition
Posicao neutra esperada

### isInjectionEnabled


### isIgnitionEnabled


### isCylinderCleanupEnabled
When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.

### complexWallModel
Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?

### alwaysInstantRpm
RPM is measured based on last 720 degrees while instant RPM is measured based on the last 90 degrees of crank revolution

### isMapAveragingEnabled


### useSeparateAdvanceForIdle
This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra advance at low idle speeds will prevent stalling and extra retard at high idle speeds can help reduce engine power and slow the idle speed.

### isWaveAnalyzerEnabled


### useSeparateVeForIdle
This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.

### verboseTriggerSynchDetails
Verbose info in console below engineSnifferRpmThreshold\nenable trigger_details

### hondaK


### twoWireBatchIgnition
This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).

### useFixedBaroCorrFromMap
Read MAP sensor on ECU start-up to use as baro value.

### useSeparateAdvanceForCranking
In Constant mode, timing is automatically tapered to running as RPM increases.\nIn Table mode, the "Cranking ignition advance" table is used directly.

### useAdvanceCorrectionsForCranking
This enables the various ignition corrections during cranking (IAT, CLT and PID idle).\nYou probably don't need this.

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
If enabled, don't wait for engine start to heat O2 sensors.\nWARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.

### invertVvtControlIntake
If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.

### invertVvtControlExhaust
If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.

### useBiQuadOnAuxSpeedSensors


### sdTriggerLog
'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.

### stepper_dc_use_two_wires


### watchOutForLinearTime


### engineChartSize


### turboSpeedSensorMultiplier


### acIdleRpmTarget
Idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.

### warningPeriod
definir warningPeriod X

### knockDetectionWindowStart


### idleStepperReactionTime


### idleStepperTotalSteps


### torqueReductionArmingApp
Posicao do pedal para perceber que precisamos reduzir o torque quando o pino de trigger e acionado

### tachPulseDuractionMs
Duracao em ms ou ciclo de trabalho dependendo do modo selecionado

### wwaeTau
Length of time the deposited wall fuel takes to dissipate after the start of acceleration.

### alternatorControl.pFactor


### alternatorControl.iFactor


### alternatorControl.dFactor


### alternatorControl.offset
Adicao linear a logica PID

### alternatorControl.periodMs
Tempo PID dTime

### alternatorControl.minValue
Ciclo de Trabalho Minimo de Saida

### alternatorControl.maxValue
Ciclo de Trabalho Maximo de Saida

### etb.pFactor


### etb.iFactor


### etb.dFactor


### etb.offset
Adicao linear a logica PID

### etb.periodMs
Tempo PID dTime

### etb.minValue
Ciclo de Trabalho Minimo de Saida

### etb.maxValue
Ciclo de Trabalho Maximo de Saida

### airTaperRpmRange
Faixa de RPM acima do limite superior para afunilamento de ar extra

### tps2Min
Closed throttle#2. todo: extract these two fields into a structure\nSee also tps2_1AdcChannel

### tps2Max
Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!\nSee also tps1_1AdcChannel

### tachPulsePerRev


### mapErrorDetectionTooLow
Valor kPa que e muito baixo para ser verdadeiro

### mapErrorDetectionTooHigh
Valor kPa que e muito alto para ser verdadeiro

### multisparkSparkDuration
How long to wait for the spark to fire before recharging the coil for another spark.

### multisparkDwell
This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.

### idleRpmPid.pFactor


### idleRpmPid.iFactor


### idleRpmPid.dFactor


### idleRpmPid.offset
Adicao linear a logica PID

### idleRpmPid.periodMs
Tempo PID dTime

### idleRpmPid.minValue
Ciclo de Trabalho Minimo de Saida

### idleRpmPid.maxValue
Ciclo de Trabalho Maximo de Saida

### wwaeBeta
0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.

### throttlePedalUpVoltage


### throttlePedalWOTVoltage
Pedal no chao

### startUpFuelPumpDuration
na deteccao de voltagem IGN ligar bomba de combustivel para construir pressao de combustivel

### idlePidRpmDeadZone
If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation

### idleMaximumAirmass
Maximum commanded airmass for the idle controller.

### torqueReductionTime
For how long after the pin has been triggered will the cut/reduction stay active. After that, even if the pin is still triggered, torque is re-introduced

### mc33810DisableRecoveryMode
See Over/Undervoltage Shutdown/Retry bit in documentation

### mc33810Gpgd0Mode


### mc33810Gpgd1Mode


### mc33810Gpgd2Mode


### mc33810Gpgd3Mode


### enableExtendedCanBroadcast
Send out board statistics

### luaCanRxWorkaround
global_can_data performance hack

### flexSensorInverted


### useHardSkipInTraction


### useAuxSpeedForSlipRatio
Use Aux Speed 1 as one of speeds for wheel slip ratio?

### useVssAsSecondWheelSpeed
VSS and Aux Speed 1 or Aux Speed 1 with Aux Speed 2?

### is_enabled_spi_5


### is_enabled_spi_6


### enableAemXSeriesEgt
AEM X-Series EGT gauge kit or evoTech EGT sensor from Wideband controller

### startRequestPinInverted


### tcu_rangeSensorPulldown


### devBit01


### devBit0


### devBit1


### devBit2


### devBit3


### devBit4


### devBit5


### devBit6


### devBit7


### invertExhaustCamVVTSignal


### enableKnockSpectrogram
"Available via TS Plugin see https://rusefi.com/s/knock"

### enableKnockSpectrogramFilter


### afterCrankingIACtaperDuration
This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.\nShould be 100 once tune is better

### iacByTpsTaper
This value is an added for base idle value. Idle Value added when coasting and transitioning into idle.

### coastingFuelCutVssLow
Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.

### coastingFuelCutVssHigh
Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.

### tpsAccelEnrichmentThreshold
Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.

### totalGearsCount


### fan1ExtraIdle
Additional idle % when fan #1 is active

### uartConsoleSerialSpeed
Taxa de banda para TTL primario

### tpsDecelEnleanmentThreshold
For decel we simply multiply delta of TPS and tFor decel we do not use table?!

### tpsDecelEnleanmentMultiplier
Magic multiplier, we multiply delta of TPS and get fuel squirt duration

### torqueReductionIgnitionRetard
Quantos graus de avanco de tempo serao reduzidos durante o Tempo de Reducao de Torque

### throttlePedalSecondaryUpVoltage


### throttlePedalSecondaryWOTVoltage
Pedal no chao

### mc33_hpfp_i_peak


### mc33_hpfp_i_hold


### mc33_hpfp_i_hold_off
Quanto tempo desativar a energia quando a corrente de manutencao e atingida antes de aplicar energia novamente

### mc33_hpfp_max_hold
Quantidade maxima de tempo que o solenoide pode estar ativo antes de assumir um erro de programacao

### stepperDcInvertedPins
Habilite se o driver de motor DC (ponte H) inverter os sinais (ex: RZ7899 em placas Hellen)

### canOpenBLT
Permitir OpenBLT no CAN Primário

### can2OpenBLT
Permitir OpenBLT no CAN Secundário

### injectorFlowAsMassFlow
Selecione se deseja configurar o fluxo do injetor em fluxo volumétrico (padrão, cc/min) ou fluxo de massa (g/s).

### boardUseCanTerminator


### kLineDoHondaSend


### can1ListenMode
ListenMode é sobre reconhecer tráfego CAN no nível do protocolo. Diferente de canWriteEnabled

### can2ListenMode


### camDecoder2jzPosition
null

### benchTestOnTime
Duração de cada pulso de teste

### lambdaProtectionRestoreTps


### lambdaProtectionRestoreLoad


### boostCutPressure
Valor de MAP acima do qual o combustível é cortado em caso de overboost. Defina como 0 para desabilitar o corte de overboost.

### fixedTiming
Fixed timing, useful for TDC testing

### mapLowValueVoltage
MAP voltage for low point

### mapHighValueVoltage
MAP voltage for low point

### egoValueShift
EGO value correction

### crankingIACposition
Esta é a posição do IAC durante a partida, alguns motores dão partida melhor se receberem mais ar durante a partida para melhorar o enchimento dos cilindros.

### tChargeMinRpmMinTps


### tChargeMinRpmMaxTps


### tChargeMaxRpmMinTps


### tChargeMaxRpmMaxTps


### minimumIgnitionTiming
Minimim timing advance allowed. No spark on any cylinder will ever fire after this angle BTDC. For example, setting -10 here means no spark ever fires later than 10 deg ATDC. Note that this only concerns the primary spark: any trailing sparks or multispark may violate this constraint.

### maximumIgnitionTiming
Maximum timing advance allowed. No spark on any cylinder will ever fire before this angle BTDC. For example, setting 45 here means no spark ever fires earlier than 45 deg BTDC

### alternatorPwmFrequency


### fan2ExtraIdle
Marcha lenta adicional % quando o ventilador #2 está ativo

### primingDelay
Atraso para permitir que a pressão de combustível se estabilize antes de disparar o pulso de priming.

### fuelReferencePressure
Esta é a pressão na qual o fluxo do seu injetor é conhecido.\nPor exemplo, se seus injetores fluem 400cc/min a 3,5 bar, insira 350kpa aqui.

### auxTempSensor1.config.tempC_1
estes valores estao em Celsius

### auxTempSensor1.config.tempC_2


### auxTempSensor1.config.tempC_3


### auxTempSensor1.config.resistance_1


### auxTempSensor1.config.resistance_2


### auxTempSensor1.config.resistance_3


### auxTempSensor1.config.bias_resistor
Valor do resistor pull-up em sua placa

### auxTempSensor2.config.tempC_1
estes valores estao em Celsius

### auxTempSensor2.config.tempC_2


### auxTempSensor2.config.tempC_3


### auxTempSensor2.config.resistance_1


### auxTempSensor2.config.resistance_2


### auxTempSensor2.config.resistance_3


### auxTempSensor2.config.bias_resistor
Valor do resistor pull-up em sua placa

### knockSamplingDuration


### etbFreq


### etbWastegatePid.pFactor


### etbWastegatePid.iFactor


### etbWastegatePid.dFactor


### etbWastegatePid.offset
Adicao linear a logica PID

### etbWastegatePid.periodMs
Tempo PID dTime

### etbWastegatePid.minValue
Ciclo de Trabalho Minimo de Saida

### etbWastegatePid.maxValue
Ciclo de Trabalho Maximo de Saida

### stepperMinDutyCycle
Use para limitar a corrente quando o motor de passo está parado, não se movendo (100% = no limit)

### stepperMaxDutyCycle
Use para limitar a corrente máxima através do motor de passo (100% = no limit)

### idlePidActivationTime


### oilPressure.v1


### oilPressure.value1


### oilPressure.v2


### oilPressure.value2


### fan2OnTemperature
Limite de temperatura para ligar o ventilador de resfriamento #2, em Celsius

### fan2OffTemperature
Limite de temperatura para desligar o ventilador de resfriamento #2, em Celsius

### auxFrequencyFilter


### coastingFuelCutRpmHigh
Define o RPM acima do qual o corte de combustível está ativo.

### coastingFuelCutRpmLow
Define o RPM abaixo do qual o corte de combustível é desativado, isso previne solavancos ou problemas ao transitar para marcha lenta

### coastingFuelCutTps
Posição do acelerador abaixo da qual o corte de combustível está ativo. Com acelerador eletrônico habilitado, isso verifica a posição do pedal em vez da borboleta, e deve ser definido para 1-2%.

### coastingFuelCutClt
O corte de combustível é desabilitado quando o motor está frio.

### pidExtraForLowRpm
Aumenta a reação do PID para RPM<alvo adicionando percentual extra ao erro do PID

### coastingFuelCutMap
Valor de MAP acima do qual a injeção de combustível é reabilitada.

### highPressureFuel.v1


### highPressureFuel.value1


### highPressureFuel.v2


### highPressureFuel.value2


### lowPressureFuel.v1


### lowPressureFuel.value1


### lowPressureFuel.v2


### lowPressureFuel.value2


### tChargeAirCoefMin
Coeficiente de transferência de calor em fluxo zero.\n0 means the air charge is fully heated to the same temperature as CLT.\n1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.

### tChargeAirCoefMax
Coeficiente de transferência de calor em alto fluxo, conforme definido por "fluxo máximo de ar".\n0 means the air charge is fully heated to the same temperature as CLT.\n1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.

### tChargeAirFlowMax
Ponto de alto fluxo para estimativa de transferência de calor.\nDefina isso para talvez 50-75% do seu fluxo máximo de ar em WOT.

### tChargeAirIncrLimit
Taxa máxima permitida de aumento para a temperatura estimada da carga de ar

### tChargeAirDecrLimit
Taxa máxima permitida de diminuição para a temperatura estimada da carga de ar

### hip9011Gain


### etb_iTermMin
Valor mínimo de iTerm

### etb_iTermMax
Valor máximo de iTerm

### idleTimingPid.pFactor


### idleTimingPid.iFactor


### idleTimingPid.dFactor


### idleTimingPid.offset
Adicao linear a logica PID

### idleTimingPid.periodMs
Tempo PID dTime

### idleTimingPid.minValue
Ciclo de Trabalho Minimo de Saida

### idleTimingPid.maxValue
Ciclo de Trabalho Maximo de Saida

### idleTimingSoftEntryTime
Ao entrar em marcha lenta, e as configurações do PID são agressivas, é bom fazer uma entrada suave ao entrar em malha fechada

### tpsAccelFractionPeriod
Atraso em ciclos entre porções de enriquecimento de combustível

### tpsAccelFractionDivisor
Divisor de fração: 1 ou menos = toda a porção de uma vez, ou dividido em frações decrescentes

### idlerpmpid_iTermMin
Valor mínimo de iTerm

### stoichRatioPrimary
Razão estequiométrica para seu combustível primário. Quando Flex Fuel está habilitado, este valor é usado quando o sensor Flex Fuel indica E0.\nE0 = 14.7\nE10 = 14.1\nE85 = 9.9\nE100 = 9.0

### idlerpmpid_iTermMax
Valor máximo de iTerm

### etbIdleThrottleRange
Define o alcance do controle de marcha lenta no ETB. Em 100% da posição de marcha lenta, o valor especificado aqui define a posição base do ETB.

### triggerCompCenterVolt
Ponto central de tensão do comparador de trigger

### triggerCompHystMin
Tensão de histerese do comparador de trigger (Mín)

### triggerCompHystMax
Tensão de histerese do comparador de trigger (Máx)

### triggerCompSensorSatRpm
RPM de saturação do sensor VR

### disableFan1AtSpeed
null

### disableFan2AtSpeed
null

### mc33_i_boost
Corrente de Boost

### mc33_i_peak
Corrente de Pico

### mc33_i_hold
Corrente de Manutenção

### mc33_t_max_boost
Tempo máximo permitido na fase de boost. Se a corrente do injetor não atingir o limite antes desse tempo, assume-se que o injetor está ausente ou com falha em circuito aberto.

### mc33_t_peak_off


### mc33_t_peak_tot
Duração da fase de pico

### mc33_t_bypass


### mc33_t_hold_off


### mc33_t_hold_tot
Duração da fase de manutenção

### maxCamPhaseResolveRpm
Abaixo deste RPM, use a informação do comando para sincronizar a posição do virabrequim para operação totalmente sequencial. Use isto se seu sensor de comando apresentar comportamentos estranhos em altas rotações. Defina como 0 para desabilitar e sempre usar o comando para ajudar a sincronizar o virabrequim.

### dfcoDelay
Atraso antes de cortar o combustível. Defina como 0 para cortar imediatamente sem atraso. Pode causar estouros e pipocos no escapamento...

### acDelay
Atraso antes de acionar o compressor do ar-condicionado. Defina como 0 para acionar imediatamente sem atraso. Use isto para evitar queda de rotação em marcha lenta ao acionar o ar-condicionado.

### fordInjectorSmallPulseBreakPoint


### etbJamDetectThreshold
Limite de erro do ETB (alvo vs. real) acima do qual o temporizador de travamento é iniciado. Se o temporizador atingir o tempo especificado no período de detecção de travamento, a borboleta é considerada travada e a operação do motor é limitada.

### hpfpCamLobes


### acLowRpmLimit
Baixa rotação do motor para o A/C. Motores maiores podem suportar valores menores

### hpfpMinAngle
Se o tempo de ativação solicitado estiver abaixo deste ângulo, não acione a bomba

### hpfpPumpVolume
Tamanho da câmara da bomba em cc. Típica Bosch HDP5 tem 9,0mm de diâmetro, típico BMW N* curso é 4,4mm.

### hpfpActivationAngle
Tempo de ativação da válvula (para permitir que a bomba gere pressão e mantenha a válvula aberta sozinha)

### issFilterReciprocal


### hpfpPidP


### hpfpPidI


### hpfpTargetDecay
A taxa mais rápida que a pressão alvo pode ser reduzida. Isso porque HPFP não tem como aliviar pressão além de injetar combustível.

### vvtActivationDelayMs
We need to give engine time to build oil pressure without diverting it to VVT

### acrDisablePhase
During revolution where ACR should be disabled at what specific angle to disengage

### auxLinear1.v1


### auxLinear1.value1


### auxLinear1.v2


### auxLinear1.value2


### auxLinear2.v1


### auxLinear2.value1


### auxLinear2.v2


### auxLinear2.value2


### etbMinimumPosition
Set a minimum allowed target position to avoid slamming/driving against the hard mechanical stop in the throttle.

### tuneHidingKey


### ALSMinRPM


### ALSMaxRPM


### ALSMaxDuration


### ALSMinCLT


### ALSMaxCLT


### alsMinTimeBetween


### alsEtbPosition


### acRelayAlternatorDutyAdder


### customSentTpsMin
null

### ALSIdleAdd


### ALSEtbAdd


### ALSSkipRatio


### acPressureEnableHyst
Hysterisis: if Pressure High Disable is 240kpa, and acPressureEnableHyst is 20, when the ECU sees 240kpa, A/C will be disabled, and stay disabled until 240-20=220kpa is reached

### tpsSecondaryMaximum
For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%

### ppsSecondaryMaximum
For Toyota ETCS-i, use ~69%

### customSentTpsMax
null

### kLineBaudRate
null

### hpfpPeakPos
Crank angle ATDC of first lobe peak

### kLinePeriodUs


### rpmSoftLimitWindowSize
Window that the correction will be added throughout (example, if rpm limit is 7000, and rpmSoftLimitWindowSize is 200, the corrections activate at 6800RPM, creating a 200rpm window)

### rpmSoftLimitTimingRetard
Degrees of timing REMOVED from actual timing during soft RPM limit window

### rpmSoftLimitFuelAdded
% of fuel ADDED during window

### rpmHardLimitHyst
Hysterisis: if the hard limit is 7200rpm and rpmHardLimitHyst is 200rpm, then when the ECU sees 7200rpm, fuel/ign will cut, and stay cut until 7000rpm (7200-200) is reached

### benchTestOffTime
Time between bench test pulses

### boostCutPressureHyst
Hysterisis: if hard cut is 240kpa, and boostCutPressureHyst is 20, when the ECU sees 240kpa, fuel/ign will cut, and stay cut until 240-20=220kpa is reached

### benchTestCount
How many test bench pulses do you want

### iacByTpsHoldTime
How long initial idle adder is held before starting to decay.

### iacByTpsDecayTime
How long it takes to remove initial IAC adder to return to normal idle.

### canVssScaling
Scale the reported vehicle speed value from CAN. Example: Parameter set to 1.1, CAN VSS reports 50kph, ECU will report 55kph instead.

### oilTempSensor.config.tempC_1
estes valores estao em Celsius

### oilTempSensor.config.tempC_2


### oilTempSensor.config.tempC_3


### oilTempSensor.config.resistance_1


### oilTempSensor.config.resistance_2


### oilTempSensor.config.resistance_3


### oilTempSensor.config.bias_resistor
Valor do resistor pull-up em sua placa

### fuelTempSensor.config.tempC_1
estes valores estao em Celsius

### fuelTempSensor.config.tempC_2


### fuelTempSensor.config.tempC_3


### fuelTempSensor.config.resistance_1


### fuelTempSensor.config.resistance_2


### fuelTempSensor.config.resistance_3


### fuelTempSensor.config.bias_resistor
Valor do resistor pull-up em sua placa

### ambientTempSensor.config.tempC_1
estes valores estao em Celsius

### ambientTempSensor.config.tempC_2


### ambientTempSensor.config.tempC_3


### ambientTempSensor.config.resistance_1


### ambientTempSensor.config.resistance_2


### ambientTempSensor.config.resistance_3


### ambientTempSensor.config.bias_resistor
Valor do resistor pull-up em sua placa

### compressorDischargeTemperature.config.tempC_1
estes valores estao em Celsius

### compressorDischargeTemperature.config.tempC_2


### compressorDischargeTemperature.config.tempC_3


### compressorDischargeTemperature.config.resistance_1


### compressorDischargeTemperature.config.resistance_2


### compressorDischargeTemperature.config.resistance_3


### compressorDischargeTemperature.config.bias_resistor
Valor do resistor pull-up em sua placa

### speedometerPulsePerKm
Number of speedometer pulses per kilometer travelled.

### maxInjectorDutyInstant
If injector duty cycle hits this value, instantly cut fuel.

### maxInjectorDutySustained
If injector duty cycle hits this value for the specified delay time, cut fuel.

### maxInjectorDutySustainedTimeout
Timeout period for duty cycle over the sustained limit to trigger duty cycle protection.

### auxSpeed1Multiplier
null

### brakeMeanEffectivePressureDifferential
null

### tunerStudioSerialSpeed
Secondary TTL channel baud rate

### anotherCiTest
null

### tcu_rangeSensorBiasResistor


### mc33810Nomi
Nominal coil charge current, 0.25A step

### mc33810Maxi
Maximum coil charge current, 1A step

### acPressure.v1


### acPressure.value1


### acPressure.v2


### acPressure.value2


### minAcPressure
value of A/C pressure in kPa before that compressor is disengaged

### maxAcPressure
value of A/C pressure in kPa after that compressor is disengaged

### minimumOilPressureTimeout
Delay before cutting fuel due to low oil pressure. Use this to ignore short pressure blips and sensor noise.

### auxLinear3.v1


### auxLinear3.value1


### auxLinear3.v2


### auxLinear3.value2


### auxLinear4.v1


### auxLinear4.value1


### auxLinear4.v2


### auxLinear4.value2


### knockSuppressMinTps
Below TPS value all knock suppression will be disabled.

### knockFuelTrimAggression
Fuel to odd when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),

### knockFuelTrimReapplyRate
After a knock event, reapply fuel at this rate.

### knockFuelTrim
Fuel trim when knock, max 30%

### knockSpectrumSensitivity


### knockFrequency
"Estimated knock frequency, ignore cylinderBore if this one > 0"

### secondaryInjectorFuelReferencePressure
This is the pressure at which your injector flow is known.\nFor example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.

### nitrousControlEnabled


### nitrousLuaGaugeArmingValue


### nitrousMinimumTps


### nitrousMinimumClt


### nitrousMaximumMap


### nitrousMaximumAfr


### nitrousActivationRpm


### nitrousDeactivationRpm


### nitrousDeactivationRpmWindow


### dfcoRetardDeg
Retard timing by this amount during DFCO. Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.

### dfcoRetardRampInTime
Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.

### nitrousFuelAdderPercent


### nitrousIgnitionRetard
Retard timing to remove from actual final timing (after all corrections) due to additional air.

### nitrousMinimumVehicleSpeed


### fuelLevelAveragingAlpha
Exponential Average Alpha filtering parameter

### fuelLevelUpdatePeriodSec
How often do we update fuel level gauge

### fuelLevelLowThresholdVoltage
Error below specified value

### fuelLevelHighThresholdVoltage
Error above specified value

### mapExpAverageAfr


### sparkHardwareLatencyCorrection
Compensates for trigger delay due to belt stretch, or other electromechanical issues. beware that raising this value advances ignition timing!

### maxOilPressureTimeout
Delay before cutting fuel due to extra high oil pressure. Use this to ignore short pressure blips and sensor noise.

### maxOilPressure
Maximum allowed oil pressure. If oil pressure exceed this level within <timeout> seconds, fuel will be cut. Set to 0 to disable.

### ltftEnabled


### ltftCRC


### ltftMinModTemp
Minimum temperature to start correcting ltft tables

### ltftMinTemp
Minimum temperature to start using ltft tables

### ltftPermissivity
How much long term fuel trim should act to reduce short term fuel trim, 100 should keep stft in about 3%, 255 in 8% and 33 in 1%, and

### ltftMaxCorrection


### ltftMinCorrection


### ltftEmaAlpha
Constante de tempo do filtro EMA para STFT usado no LTFT

### ltftStftRejectThreshold
Limiar de rejeição para STFT (em %)

### ltftIgnitionOnDelay
Delay pós-ignição ON para aprendizado/aplicação do LTFT

### ltftIgnitionOffSaveDelay
Delay após ignição OFF para salvamento do LTFT

### wwBufferSize
200

### tcu_shiftTime


### dynoRpmStep
@@DYNO_RPM_STEP_TOOLTIP@@

### dynoSaeTemperatureC
@@DYNO_SAE_TEMPERATURE_C_TOOLTIP@@

### dynoSaeRelativeHumidity
@@DYNO_SAE_RELATIVE_HUMIDITY_TOOLTIP@@

### dynoSaeBaro
@@DYNO_SAE_BARO_TOOLTIP@@

### dynoCarWheelDiaInch
@@DYNO_CAR_WHEEL_DIA_INCH_TOOLTIP@@

### dynoCarWheelAspectRatio
@@DYNO_CAR_WHEEL_ASPECT_RATIO_TOOLTIP@@

### dynoCarWheelTireWidthMm
@@DYNO_CAR_WHEEL_TIRE_WIDTH_TOOLTIP@@

### dynoCarGearPrimaryReduction
@@DYNO_CAR_GEAR_PRIMARY_REDUCTION_TOOLTIP@@

### dynoCarGearRatio
@@DYNO_CAR_GEAR_RATIO_TOOLTIP@@

### dynoCarGearFinalDrive
@@DYNO_CAR_GEAR_FINAL_DRIVE_TOOLTIP@@

### dynoCarCarMassKg
@@DYNO_CAR_CAR_MASS_TOOLTIP@@

### dynoCarCargoMassKg
@@DYNO_CAR_CARGO_MASS_TOOLTIP@@

### dynoCarCoeffOfDrag
@@DYNO_CAR_COEFF_OF_DRAG_TOOLTIP@@

### dynoCarFrontalAreaM2
@@DYNO_CAR_FRONTAL_AREA_TOOLTIP@@

