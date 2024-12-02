//
// Created by kifir on 6/18/24.
//

#include "pch.h"

#include "test_base.h"

void TestBase::SetUp() {
    eth = std::make_unique<EngineTestHelper>(engine_type_e::TEST_ENGINE);
}

void TestBase::TearDown() {
    eth.reset();
}

TestEngineConfiguration& TestBase::getTestEngineConfiguration() {
    return TestEngineConfiguration::getInstance();
}

TestPersistentConfiguration& TestBase::getTestPersistentConfiguration() {
    return TestPersistentConfiguration::getInstance();
}

TestEngineState& TestBase::getTestEngineState() {
    return TestEngineState::getInstance();
}

void TestBase::setUpEngineConfiguration(const EngineConfig& config) {
    // Launch Control
    getTestEngineConfiguration().configureLaunchActivatePin(config.getLaunchActivatePin());
    getTestEngineConfiguration().configureLaunchActivateInverted(config.getLaunchActivateInverted());
    getTestEngineConfiguration().configureLaunchControlEnabled(config.getLaunchControlEnabled());
    getTestEngineConfiguration().configureLaunchRpm(config.getLaunchRpm());
    getTestEngineConfiguration().configureLaunchRpmWindow(config.getLaunchRpmWindow());
    getTestEngineConfiguration().configureLaunchCorrectionsEndRpm(config.getLaunchCorrectionsEndRpm());
    getTestEngineConfiguration().configureIgnitionRetardEnable(config.getIgnitionRetardEnable());
    getTestEngineConfiguration().configureIgnitionRetard(config.getIgnitionRetard());
    getTestEngineConfiguration().configureSmoothRetardMode(config.getSmoothRetardMode());
    getTestEngineConfiguration().configureEnableIgnitionCut(config.getEnableIgnitionCut());
    getTestEngineConfiguration().configureInitialIgnitionCutPercent(config.getInitialIgnitionCut());
    getTestEngineConfiguration().configureFinalIgnitionCutPercentBeforeLaunch(config.getFinalIgnitionCutBeforeLaunch());

    // Shift Torque Reduction (Flat Shift)
    getTestEngineConfiguration().configureTorqueReductionEnabled(config.getTorqueReductionEnabled());
    getTestEngineConfiguration().configureTorqueReductionActivationMode(config.getTorqueReductionActivationMode());
    getTestEngineConfiguration().configureTorqueReductionTriggerPin(config.getTorqueReductionTriggerPin());
    getTestEngineConfiguration().configureTorqueReductionButtonInverted(config.getTorqueReductionTriggerPinInverted());
    getTestEngineConfiguration().configureLimitTorqueReductionTime(config.getLimitTorqueReductionTime());
    getTestEngineConfiguration().configureTorqueReductionTime(config.getTorqueReductionTime());
    getTestEngineConfiguration().configureTorqueReductionArmingRpm(config.getTorqueReductionArmingRpm());
    getTestEngineConfiguration().configureTorqueReductionArmingApp(config.getTorqueReductionArmingApp());
    getTestEngineConfiguration().configureTorqueReductionIgnitionCut(config.getTorqueReductionIgnitionCut());
    getTestEngineConfiguration().configureTorqueReductionIgnitionRetard(config.getTorqueReductionIgnitionRetard());

    getTestEngineConfiguration().configureFuelPressureSensorMode(config.getFuelPressureSensorMode());

    // Injector
    getTestEngineConfiguration().configureInjectorFlowAsMassFlow(config.getInjectorFlowAsMassFlow());
    getTestEngineConfiguration().configureInjectorFlow(config.getInjectorFlow());
    getTestEngineConfiguration().configureInjectorBattLagCorr(config.getInjectorBattLagCorr());
    getTestEngineConfiguration().configureFuelReferencePressure(config.getFuelReferencePressure());
    getTestEngineConfiguration().configureInjectorCompensationMode(config.getInjectorCompensationMode());

    // Secondary injector
    getTestEngineConfiguration().configureInjectorSecondaryFlow(config.getInjectorSecondaryFlow());
    getTestEngineConfiguration().configureInjectorSecondaryBattLagCorr(config.getInjectorSecondaryBattLagCorr());
    getTestEngineConfiguration().configureSecondaryInjectorFuelReferencePressure(
        config.getSecondaryInjectorFuelReferencePressure()
    );
    getTestEngineConfiguration().configureSecondaryInjectorCompensationMode(
        config.getSecondaryInjectorCompensationMode()
    );

    // Staged injection
    getTestEngineConfiguration().configureEnableStagedInjection(config.getStagedInjectionEnabled());

    // Nitrous control
    getTestEngineConfiguration().configureNitrousControlEnabled(config.getNitrousControlEnabled());
    getTestEngineConfiguration().configureNitrousControlArmingMethod(config.getNitrousControlArmingMethod());
    getTestEngineConfiguration().configureNitrousControlTriggerPin(config.getNitrousControlTriggerPin());
    getTestEngineConfiguration().configureNitrousControlTriggerPinInverted(
        config.getNitrousControlTriggerPinInverted()
    );
    getTestEngineConfiguration().configureNitrousControlTriggerPinMode(config.getNitrousControlTriggerPinMode());
    getTestEngineConfiguration().configureNitrousLuaGauge(config.getNitrousLuaGauge());
    getTestEngineConfiguration().configureNitrousLuaGaugeMeaning(config.getNitrousLuaGaugeMeaning());
    getTestEngineConfiguration().configureNitrousLuaGaugeArmingValue(config.getNitrousLuaGaugeArmingValue());

    getTestEngineConfiguration().configureNitrousMinimumTps(config.getNitrousMinimumTps());
    getTestEngineConfiguration().configureNitrousMinimumClt(config.getNitrousMinimumClt());
    getTestEngineConfiguration().configureNitrousMaximumMap(config.getNitrousMaximumMap());
    getTestEngineConfiguration().configureNitrousMaximumAfr(config.getNitrousMaximumAfr());
    getTestEngineConfiguration().configureNitrousActivationRpm(config.getNitrousActivationRpm());
    getTestEngineConfiguration().configureNitrousDeactivationRpm(config.getNitrousDeactivationRpm());
    getTestEngineConfiguration().configureNitrousDeactivationRpmWindow(
        config.getNitrousDeactivationRpmWindow()
    );
}

void TestBase::periodicFastCallback() {
    // run the ignition math
    engine->periodicFastCallback();
}

void TestBase::periodicSlowCallback() {
    engine->periodicSlowCallback();
}

void TestBase::updateRpm(const std::optional<float> rpm) {
    updateSensor(SensorType::Rpm, rpm);
}

void TestBase::updateApp(const std::optional<float> app) {
    updateSensor(SensorType::DriverThrottleIntent, app);
}

void TestBase::updateClt(const std::optional<float> clt) {
    updateSensor(SensorType::Clt, clt);
}

void TestBase::updateMap(const std::optional<float> map) {
    updateSensor(SensorType::Map, map);
}

void TestBase::updateLambda1(std::optional<float> lambda1) {
    updateSensor(SensorType::Lambda1, lambda1);
}

void TestBase::updateSensor(const SensorType sensor, const std::optional<float> sensorReading) {
    if (sensorReading.has_value()) {
        Sensor::setMockValue(sensor, sensorReading.value());
    } else {
        Sensor::resetMockValue(sensor);
    }
    periodicFastCallback();
}