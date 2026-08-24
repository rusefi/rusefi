template<class GtestBase, engine_type_e EngineType>
template<typename ModuleType>
ModuleType& TestBase<GtestBase, EngineType>::getModule() {
	return engine->module<ModuleType>().unmock();
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::SetUp() {
	eth = std::make_unique<EngineTestHelper>(EngineType);
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::TearDown() {
	eth.reset();
}

template<class GtestBase, engine_type_e EngineType>
TestEngineConfiguration& TestBase<GtestBase, EngineType>::getTestEngineConfiguration() {
	return TestEngineConfiguration::getInstance();
}

template<class GtestBase, engine_type_e EngineType>
TestPersistentConfiguration& TestBase<GtestBase, EngineType>::getTestPersistentConfiguration() {
	return TestPersistentConfiguration::getInstance();
}

template<class GtestBase, engine_type_e EngineType>
TestLuaScriptExecutor& TestBase<GtestBase, EngineType>::getTestLuaScriptExecutor() {
	return TestLuaScriptExecutor::getInstance();
}

template<class GtestBase, engine_type_e EngineType>
TestEngineState& TestBase<GtestBase, EngineType>::getTestEngineState() {
	return TestEngineState::getInstance();
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::setUpEngineConfiguration(const EngineConfig& engineConfig) {
	// Other Sensor Inputs
	getTestEngineConfiguration().configureClutchDownPin(engineConfig.getClutchDownPin());
	getTestEngineConfiguration().configureClutchDownPinMode(engineConfig.getClutchDownPinMode());
	getTestEngineConfiguration().configureClutchUpPin(engineConfig.getClutchUpPin());
	getTestEngineConfiguration().configureClutchUpPinMode(engineConfig.getClutchUpPinMode());

	// Launch Control
	getTestEngineConfiguration().configureLaunchActivatePin(engineConfig.getLaunchActivatePin());
	getTestEngineConfiguration().configureLaunchActivateMode(engineConfig.getLaunchActivatePinMode());
	getTestEngineConfiguration().configureLaunchControlEnabled(engineConfig.getLaunchControlEnabled());
	getTestEngineConfiguration().configureLaunchRpm(engineConfig.getLaunchRpm());
	getTestEngineConfiguration().configureLaunchRpmWindow(engineConfig.getLaunchRpmWindow());
	getTestEngineConfiguration().configureLaunchCorrectionsEndRpm(engineConfig.getLaunchCorrectionsEndRpm());
	getTestEngineConfiguration().configureIgnitionRetardEnable(engineConfig.getIgnitionRetardEnable());
	getTestEngineConfiguration().configureIgnitionRetard(engineConfig.getIgnitionRetard());
	getTestEngineConfiguration().configureSmoothRetardMode(engineConfig.getSmoothRetardMode());
	getTestEngineConfiguration().configureEnableIgnitionCut(engineConfig.getEnableIgnitionCut());
	getTestEngineConfiguration().configureInitialIgnitionCutPercent(engineConfig.getInitialIgnitionCut());
	getTestEngineConfiguration().configureFinalIgnitionCutPercentBeforeLaunch(engineConfig.getFinalIgnitionCutBeforeLaunch());

	// Shift Torque Reduction (Flat Shift)
	getTestEngineConfiguration().configureTorqueReductionEnabled(engineConfig.getTorqueReductionEnabled());
	getTestEngineConfiguration().configureTorqueReductionActivationMode(engineConfig.getTorqueReductionActivationMode());
	getTestEngineConfiguration().configureTorqueReductionTriggerPin(engineConfig.getTorqueReductionTriggerPin());
	getTestEngineConfiguration().configureTorqueReductionButtonMode(engineConfig.getTorqueReductionTriggerPinMode());
	getTestEngineConfiguration().configureLimitTorqueReductionTime(engineConfig.getLimitTorqueReductionTime());
	getTestEngineConfiguration().configureTorqueReductionTime(engineConfig.getTorqueReductionTime());
	getTestEngineConfiguration().configureTorqueReductionArmingRpm(engineConfig.getTorqueReductionArmingRpm());
	getTestEngineConfiguration().configureTorqueReductionArmingApp(engineConfig.getTorqueReductionArmingApp());
	getTestEngineConfiguration().configureTorqueReductionIgnitionCut(engineConfig.getTorqueReductionIgnitionCut());
	getTestEngineConfiguration().configureTorqueReductionIgnitionRetard(engineConfig.getTorqueReductionIgnitionRetard());

	getTestEngineConfiguration().configureFuelPressureSensorMode(engineConfig.getFuelPressureSensorMode());

	// Injector
	getTestEngineConfiguration().configureInjectorFlowAsMassFlow(engineConfig.getInjectorFlowAsMassFlow());
	getTestEngineConfiguration().configureInjectorFlow(engineConfig.getInjectorFlow());
	getTestEngineConfiguration().configureInjectorBattLagCorr(engineConfig.getInjectorBattLagCorr());
	getTestEngineConfiguration().configureFuelReferencePressure(engineConfig.getFuelReferencePressure());
	getTestEngineConfiguration().configureInjectorCompensationMode(engineConfig.getInjectorCompensationMode());

	// Secondary injector
	getTestEngineConfiguration().configureInjectorSecondaryFlow(engineConfig.getInjectorSecondaryFlow());
	getTestEngineConfiguration().configureInjectorSecondaryBattLagCorr(engineConfig.getInjectorSecondaryBattLagCorr());
	getTestEngineConfiguration().configureSecondaryInjectorFuelReferencePressure(
			engineConfig.getSecondaryInjectorFuelReferencePressure()
	);
	getTestEngineConfiguration().configureSecondaryInjectorCompensationMode(
			engineConfig.getSecondaryInjectorCompensationMode()
	);

	// Staged injection
	getTestEngineConfiguration().configureEnableStagedInjection(engineConfig.getStagedInjectionEnabled());

	// Nitrous control
	getTestEngineConfiguration().configureNitrousControlEnabled(engineConfig.getNitrousControlEnabled());
	getTestEngineConfiguration().configureNitrousControlArmingMethod(engineConfig.getNitrousControlArmingMethod());
	getTestEngineConfiguration().configureNitrousControlTriggerPin(engineConfig.getNitrousControlTriggerPin());
	getTestEngineConfiguration().configureNitrousControlTriggerPinMode(engineConfig.getNitrousControlTriggerPinMode()
	);
	getTestEngineConfiguration().configureNitrousControlTriggerPinMode(engineConfig.getNitrousControlTriggerPinMode());
	getTestEngineConfiguration().configureNitrousLuaGauge(engineConfig.getNitrousLuaGauge());
	getTestEngineConfiguration().configureNitrousLuaGaugeMeaning(engineConfig.getNitrousLuaGaugeMeaning());
	getTestEngineConfiguration().configureNitrousLuaGaugeArmingValue(engineConfig.getNitrousLuaGaugeArmingValue());

	getTestEngineConfiguration().configureNitrousMinimumVehicleSpeed(engineConfig.getNitrousMinimumVehicleSpeed());
	getTestEngineConfiguration().configureNitrousMinimumTps(engineConfig.getNitrousMinimumTps());
	getTestEngineConfiguration().configureNitrousMinimumClt(engineConfig.getNitrousMinimumClt());
	getTestEngineConfiguration().configureNitrousMaximumMap(engineConfig.getNitrousMaximumMap());
	getTestEngineConfiguration().configureNitrousMaximumAfr(engineConfig.getNitrousMaximumAfr());
	getTestEngineConfiguration().configureNitrousActivationRpm(engineConfig.getNitrousActivationRpm());
	getTestEngineConfiguration().configureNitrousDeactivationRpm(engineConfig.getNitrousDeactivationRpm());
	getTestEngineConfiguration().configureNitrousDeactivationRpmWindow(
			engineConfig.getNitrousDeactivationRpmWindow()
	);
	getTestEngineConfiguration().configureNitrousFuelAdderPercent(
			engineConfig.getNitrousFuelAdderPercent()
	);
	getTestEngineConfiguration().configureNitrousIgnitionRetard(engineConfig.getNitrousIgnitionRetard());
	getTestEngineConfiguration().configureFuelLevelAveragingAlpha(engineConfig.getFuelLevelAveragingAlpha());
	getTestEngineConfiguration().configureFuelLevelUpdatePeriodSec(engineConfig.getFuelLevelUpdatePeriodSec());
	getTestEngineConfiguration().configureFuelLevelLowThresholdVoltage(engineConfig.getFuelLevelLowThresholdVoltage());
	getTestEngineConfiguration().configureFuelLevelHighThresholdVoltage(engineConfig.getFuelLevelHighThresholdVoltage());
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::periodicFastCallback() {
	// run the ignition math
	engine->periodicFastCallback();
}

template<class GtestBase, engine_type_e EngineType> void
TestBase<GtestBase, EngineType>::periodicSlowCallback() {
	engine->periodicSlowCallback();
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::updateVehicleSpeed(const std::optional<float> speed, void (TestBase::* const postAction)()) {
	updateSensor(SensorType::VehicleSpeed, speed, postAction);
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::updateRpm(const std::optional<float> rpm, void (TestBase::* const postAction)()) {
	updateSensor(SensorType::Rpm, rpm, postAction);
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::updateApp(const std::optional<float> app, void (TestBase::* const postAction)()) {
	updateSensor(SensorType::DriverThrottleIntent, app, postAction);
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::updateClt(const std::optional<float> clt, void (TestBase::* const postAction)()) {
	updateSensor(SensorType::Clt, clt, postAction);
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::updateMap(const std::optional<float> map, void (TestBase::* const postAction)()) {
	updateSensor(SensorType::Map, map, postAction);
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::updateLambda1(std::optional<float> lambda1, void (TestBase::* const postAction)()) {
	updateSensor(SensorType::Lambda1, lambda1, postAction);
}

template<class GtestBase, engine_type_e EngineType>
void TestBase<GtestBase, EngineType>::updateSensor(
		const SensorType sensor,
		const std::optional<float> sensorReading,
		void (TestBase::* const postAction)()
) {
	if (sensorReading.has_value()) {
		Sensor::setMockValue(sensor, sensorReading.value());
	} else {
		Sensor::resetMockValue(sensor);
	}
	(this->*postAction)();
}