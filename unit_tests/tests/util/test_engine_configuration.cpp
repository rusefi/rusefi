//
// Created by kifir on 11/4/24.
//

#include "pch.h"

#include "test_engine_configuration.h"

#include "engine_configuration_defaults.h"

TestEngineConfiguration& TestEngineConfiguration::getInstance() {
    return instance;
}

void TestEngineConfiguration::configureClutchDownPin(const std::optional<switch_input_pin_e> pin) {
    if (pin.has_value()) {
        engineConfiguration->clutchDownPin = pin.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->clutchDownPin,
            engine_configuration_defaults::CLUTCH_DOWN_PIN
        ); // check default value
    }
}

void TestEngineConfiguration::configureClutchDownPinMode(const std::optional<pin_input_mode_e> pinMode) {
    if (pinMode.has_value()) {
        engineConfiguration->clutchDownPinMode = pinMode.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->clutchDownPinMode,
                engine_configuration_defaults::CLUTCH_DOWN_PIN_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureClutchUpPin(const std::optional<switch_input_pin_e> pin) {
    if (pin.has_value()) {
        engineConfiguration->clutchUpPin = pin.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->clutchUpPin,
                engine_configuration_defaults::CLUTCH_UP_PIN
        ); // check default value
    }
}

void TestEngineConfiguration::configureClutchUpPinMode(const std::optional<pin_input_mode_e> pinMode) {
    if (pinMode.has_value()) {
        engineConfiguration->clutchUpPinMode = pinMode.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->clutchUpPinMode,
                engine_configuration_defaults::CLUTCH_UP_PIN_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureLaunchControlEnabled(const std::optional<bool> launchControlEnabled) {
    if (launchControlEnabled.has_value()) {
        engineConfiguration->launchControlEnabled = launchControlEnabled.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchControlEnabled); // check default value
    }
}

void TestEngineConfiguration::configureLaunchActivationMode(
    const std::optional<launchActivationMode_e> launchActivationMode
) {
    if (launchActivationMode.has_value()) {
        engineConfiguration->launchActivationMode = launchActivationMode.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->launchActivationMode,
            engine_configuration_defaults::LAUNCH_ACTIVATION_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureLaunchSpeedThreshold(const std::optional<int> launchSpeedThreshold) {
    if (launchSpeedThreshold.has_value()) {
        engineConfiguration->launchSpeedThreshold = launchSpeedThreshold.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->launchSpeedThreshold,
            engine_configuration_defaults::LAUNCH_SPEED_THRESHOLD
        ); // check default value
    }
}

void TestEngineConfiguration::configureLaunchRpm(const std::optional<int> launchRpm) {
    if (launchRpm.has_value()) {
        engineConfiguration->launchRpm = launchRpm.value();
    } else {
        ASSERT_EQ(engineConfiguration->launchRpm, 3000); // check default value
    }
}

void TestEngineConfiguration::configureLaunchRpmWindow(const std::optional<int> launchRpmWindow) {
    if (launchRpmWindow.has_value()) {
        engineConfiguration->launchRpmWindow = launchRpmWindow.value();
    } else {
        ASSERT_EQ(engineConfiguration->launchRpmWindow, 500); // check default value
    }
}

void TestEngineConfiguration::configureLaunchCorrectionsEndRpm(const std::optional<int> launchCorrectionsEndRpm) {
    if (launchCorrectionsEndRpm.has_value()) {
        engineConfiguration->launchCorrectionsEndRpm = launchCorrectionsEndRpm.value();
    } else {
        ASSERT_EQ(engineConfiguration->launchCorrectionsEndRpm, 0); // check default value
    }
}

void TestEngineConfiguration::configureIgnitionRetardEnable(std::optional<bool> ignitionRetardEnable) {
    if (ignitionRetardEnable.has_value()) {
        engineConfiguration->enableLaunchRetard = ignitionRetardEnable.value();
    } else {
        ASSERT_FALSE(engineConfiguration->enableLaunchRetard); // check default value
    }
}

void TestEngineConfiguration::configureIgnitionRetard(std::optional<float> ignitionRetard) {
    if (ignitionRetard.has_value()) {
        engineConfiguration->launchTimingRetard = ignitionRetard.value();
    } else {
        ASSERT_EQ(engineConfiguration->launchTimingRetard, 0); // check default value
    }
}

void TestEngineConfiguration::configureSmoothRetardMode(std::optional<bool> smoothRetardMode) {
    if (smoothRetardMode.has_value()) {
        engineConfiguration->launchSmoothRetard = smoothRetardMode.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchSmoothRetard); // check default value
    }
}

void TestEngineConfiguration::configureEnableIgnitionCut(const std::optional<bool> enableIgnitionCut) {
    if (enableIgnitionCut.has_value()) {
        engineConfiguration->launchSparkCutEnable = enableIgnitionCut.value();
    } else {
        ASSERT_FALSE(engineConfiguration->launchSparkCutEnable); // check default value
    }
}

void TestEngineConfiguration::configureInitialIgnitionCutPercent(const std::optional<int> initialIgnitionCutPercent) {
    if (initialIgnitionCutPercent.has_value()) {
        engineConfiguration->initialIgnitionCutPercent = initialIgnitionCutPercent.value();
    } else {
        ASSERT_EQ(engineConfiguration->initialIgnitionCutPercent, 0); // check default value
    }
}

void TestEngineConfiguration::configureFinalIgnitionCutPercentBeforeLaunch(
    const std::optional<int> finalIgnitionCutPercentBeforeLaunch
) {
    if (finalIgnitionCutPercentBeforeLaunch.has_value()) {
        engineConfiguration->finalIgnitionCutPercentBeforeLaunch = finalIgnitionCutPercentBeforeLaunch.value();
    } else {
        ASSERT_EQ(engineConfiguration->finalIgnitionCutPercentBeforeLaunch, 0); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionEnabled(const std::optional<bool> torqueReductionEnabled) {
    if (torqueReductionEnabled.has_value()) {
        engineConfiguration->torqueReductionEnabled = torqueReductionEnabled.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionEnabled,
            engine_configuration_defaults::ENABLE_SHIFT_TORQUE_REDUCTION
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionActivationMode(
    const std::optional<torqueReductionActivationMode_e> activationMode
) {
    if (activationMode.has_value()) {
        engineConfiguration->torqueReductionActivationMode = activationMode.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionActivationMode,
            engine_configuration_defaults::TORQUE_REDUCTION_ACTIVATION_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionTriggerPin(const std::optional<switch_input_pin_e> pin) {
    if (pin.has_value()) {
        engineConfiguration->torqueReductionTriggerPin = pin.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionTriggerPin,
            engine_configuration_defaults::TORQUE_REDUCTION_TRIGGER_PIN
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionButtonMode(const std::optional<pin_input_mode_e> pinMode) {
    if (pinMode.has_value()) {
        engineConfiguration->torqueReductionTriggerPinMode = pinMode.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionTriggerPinMode,
            engine_configuration_defaults::TORQUE_REDUCTION_TRIGGER_PIN_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureLaunchActivatePin(const std::optional<switch_input_pin_e> pin) {
    if (pin.has_value()) {
        engineConfiguration->launchActivatePin = pin.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->launchActivatePin,
            engine_configuration_defaults::LAUNCH_ACTIVATE_PIN
        ); // check default value
    }
}

void TestEngineConfiguration::configureLaunchActivateMode(const std::optional<pin_input_mode_e> pinMode) {
    if (pinMode.has_value()) {
        engineConfiguration->launchActivatePinMode = pinMode.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->launchActivatePinMode,
            engine_configuration_defaults::LAUNCH_ACTIVATE_PIN_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureLimitTorqueReductionTime(std::optional<bool> limitTorqueReductionTime) {
    if (limitTorqueReductionTime.has_value()) {
        engineConfiguration->limitTorqueReductionTime = limitTorqueReductionTime.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->limitTorqueReductionTime,
            engine_configuration_defaults::LIMIT_TORQUE_REDUCTION_TIME
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionTime(std::optional<float> timeout) {
    if (timeout.has_value()) {
        setTable(config->torqueReductionTimeTable, timeout.value());
    } else {
        const int expected_TORQUE_REDUCTION_TIME[] = { 0, 0, 0, 0, 0, 0 };
        EXPECT_THAT(
            config->torqueReductionTimeTable[0],
            testing::ElementsAreArray(expected_TORQUE_REDUCTION_TIME)
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionArmingRpm(const std::optional<float> armingRpm) {
    if (armingRpm.has_value()) {
        engineConfiguration->torqueReductionArmingRpm = armingRpm.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionArmingRpm,
            engine_configuration_defaults::TORQUE_REDUCTION_ARMING_RPM
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionArmingApp(const std::optional<float> armingApp) {
    if (armingApp.has_value()) {
        engineConfiguration->torqueReductionArmingApp = armingApp.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->torqueReductionArmingApp,
            engine_configuration_defaults::TORQUE_REDUCTION_ARMING_APP
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionIgnitionCut(const std::optional<int8_t> ignitionCut) {
    if (ignitionCut.has_value()) {
        setTable(config->torqueReductionIgnitionCutTable, ignitionCut.value());
    } else {
        const int expected_TORQUE_REDUCTION_IGNITION_CUT[] = { 0, 0, 0, 0, 0, 0 };
        EXPECT_THAT(
            config->torqueReductionIgnitionCutTable[0],
            testing::ElementsAreArray(expected_TORQUE_REDUCTION_IGNITION_CUT)
        ); // check default value
    }
}

void TestEngineConfiguration::configureTorqueReductionIgnitionRetard(const std::optional<int8_t> ignitionRetard) {
    if (ignitionRetard.has_value()) {
        setTable(config->torqueReductionIgnitionRetardTable, ignitionRetard.value());
    } else {
        const int expected_TORQUE_REDUCTION_IGNITION_RETARD[] = { 0, 0, 0, 0, 0, 0 };
        EXPECT_THAT(
            config->torqueReductionIgnitionRetardTable[0],
            testing::ElementsAreArray(expected_TORQUE_REDUCTION_IGNITION_RETARD)
        ); // check default value
    }
}

void TestEngineConfiguration::configureFuelPressureSensorMode(
    const std::optional<fuel_pressure_sensor_mode_e> fuelPressureSensorMode
) {
    if (fuelPressureSensorMode.has_value()) {
        engineConfiguration->fuelPressureSensorMode = fuelPressureSensorMode.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->fuelPressureSensorMode,
            engine_configuration_defaults::FUEL_PRESSURE_SENSOR_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureInjectorFlowAsMassFlow(const std::optional<bool> injectorFlowAsMassFlow) {
    if (injectorFlowAsMassFlow.has_value()) {
        engineConfiguration->injectorFlowAsMassFlow = injectorFlowAsMassFlow.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->injectorFlowAsMassFlow,
            engine_configuration_defaults::INJECTOR_FLOW_AS_MASS_FLOW
        ); // check default value
    }
}

void TestEngineConfiguration::configureInjectorFlow(const std::optional<float> flow) {
    if (flow.has_value()) {
        engineConfiguration->injector.flow = flow.value();
    } else {
        ASSERT_EQ(engineConfiguration->injector.flow, engine_configuration_defaults::INJECTOR_FLOW); // check default value
    }
}

static_assert(std::tuple_size<BattLagCorrTable> {} == sizeof(engineConfiguration->injector.battLagCorrTable) / sizeof(engineConfiguration->injector.battLagCorrTable[0]));

void TestEngineConfiguration::configureInjectorBattLagCorr(const std::optional<BattLagCorrTable> battLagCorr) {
    if (battLagCorr.has_value()) {
        copyTable(engineConfiguration->injector.battLagCorrTable, battLagCorr.value());
    } else {
        for (size_t row = 0; row < std::tuple_size<BattLagCorrTable> {}; ++row) {
            EXPECT_THAT(
                engineConfiguration->injector.battLagCorrTable[row],
                testing::ElementsAreArray(engine_configuration_defaults::INJECTOR_BATT_LAG_CURR[row])
            );
        }
    }
}
void TestEngineConfiguration::configureFuelReferencePressure(const std::optional<float> fuelReferencePressure) {
    if (fuelReferencePressure.has_value()) {
        engineConfiguration->fuelReferencePressure = fuelReferencePressure.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->fuelReferencePressure,
            engine_configuration_defaults::FUEL_REFERENCE_PRESSURE
        ); // check default value
    }
}

void TestEngineConfiguration::configureInjectorCompensationMode(
    const std::optional<injector_compensation_mode_e> injectorCompensationMode
) {
    if (injectorCompensationMode.has_value()) {
        engineConfiguration->injectorCompensationMode = injectorCompensationMode.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->injectorCompensationMode,
            engine_configuration_defaults::INJECTOR_COMPENSATION_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureInjectorSecondaryFlow(const std::optional<float> flow) {
    if (flow.has_value()) {
        engineConfiguration->injectorSecondary.flow = flow.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->injectorSecondary.flow,
            engine_configuration_defaults::INJECTOR_SECONDARY_FLOW
        ); // check default value
    }
}

void TestEngineConfiguration::configureInjectorSecondaryBattLagCorr(const std::optional<BattLagCorrTable> battLagCorr) {
    if (battLagCorr.has_value()) {
        for (size_t i = 0; i < VBAT_INJECTOR_CURVE_PRESSURE_SIZE; i++) {
            std::copy(
                std::begin(battLagCorr.value()[i]),
                std::end(battLagCorr.value()[i]),
                std::begin(engineConfiguration->injectorSecondary.battLagCorrTable[i])
            );
        }
    } else {
       for (size_t row = 0; row < std::tuple_size<BattLagCorrTable> {}; ++row) {
            EXPECT_THAT(
                engineConfiguration->injector.battLagCorrTable[row],
                testing::ElementsAreArray(engine_configuration_defaults::INJECTOR_SECONDARY_BATT_LAG_CURR[row])
            );
        }
    }
}

void TestEngineConfiguration::configureSecondaryInjectorFuelReferencePressure(
    const std::optional<float> secondaryInjectorFuelReferencePressure
) {
    if (secondaryInjectorFuelReferencePressure.has_value()) {
        engineConfiguration->secondaryInjectorFuelReferencePressure = secondaryInjectorFuelReferencePressure.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->secondaryInjectorFuelReferencePressure,
            engine_configuration_defaults::SECONDARY_INJECTOR_FUEL_REFERENCE_PRESSURE
        ); // check default value
    }
}

void TestEngineConfiguration::configureSecondaryInjectorCompensationMode(
    const std::optional<injector_compensation_mode_e> secondaryInjectorCompensationMode
) {
    if (secondaryInjectorCompensationMode.has_value()) {
        engineConfiguration->secondaryInjectorCompensationMode = secondaryInjectorCompensationMode.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->secondaryInjectorCompensationMode,
            engine_configuration_defaults::SECONDARY_INJECTOR_COMPENSATION_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureEnableStagedInjection(const std::optional<bool> isStagedInjectionEnabled) {
    if (isStagedInjectionEnabled.has_value()) {
        engineConfiguration->enableStagedInjection = isStagedInjectionEnabled.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->enableStagedInjection,
            engine_configuration_defaults::ENABLE_STAGED_INJECTION
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousControlEnabled(const std::optional<bool> nitrousControlEnabled) {
    if (nitrousControlEnabled.has_value()) {
        engineConfiguration->nitrousControlEnabled = nitrousControlEnabled.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousControlEnabled,
            engine_configuration_defaults::NITROUS_CONTROL_ENABLED
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousControlArmingMethod(
    const std::optional<nitrous_arming_method_e> armingMethod
) {
    if (armingMethod.has_value()) {
        engineConfiguration->nitrousControlArmingMethod = armingMethod.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousControlArmingMethod,
            engine_configuration_defaults::NITROUS_CONTROL_ARMING_METHOD
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousControlTriggerPin(const std::optional<switch_input_pin_e> triggerPin) {
    if (triggerPin.has_value()) {
        engineConfiguration->nitrousControlTriggerPin = triggerPin.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousControlTriggerPin,
            engine_configuration_defaults::NITROUS_CONTROL_TRIGGER_PIN
        ); // check default value
    }
}
void TestEngineConfiguration::configureNitrousControlTriggerPinMode(const std::optional<pin_input_mode_e> pinMode) {
    if (pinMode.has_value()) {
        engineConfiguration->nitrousControlTriggerPinMode = pinMode.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousControlTriggerPinMode,
            engine_configuration_defaults::NITROUS_CONTROL_TRIGGER_PIN_MODE
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousLuaGauge(const std::optional<lua_gauge_e> luaGauge) {
    if (luaGauge.has_value()) {
        engineConfiguration->nitrousLuaGauge = luaGauge.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousLuaGauge,
            engine_configuration_defaults::NITROUS_LUA_GAUGE
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousLuaGaugeMeaning(const std::optional<lua_gauge_meaning_e> luaGaugeMeaning) {
    if (luaGaugeMeaning.has_value()) {
        engineConfiguration->nitrousLuaGaugeMeaning = luaGaugeMeaning.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousLuaGaugeMeaning,
            engine_configuration_defaults::NITROUS_LUA_GAUGE_MEANING
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousLuaGaugeArmingValue(const std::optional<float> luaGaugeArmingValue) {
    if (luaGaugeArmingValue.has_value()) {
        engineConfiguration->nitrousLuaGaugeArmingValue = luaGaugeArmingValue.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousLuaGaugeArmingValue,
            engine_configuration_defaults::NITROUS_LUA_GAUGE_ARMING_VALUE
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousMinimumVehicleSpeed(
    const std::optional<uint16_t> nitrousMinimumVehicleSpeed
) {
    if (nitrousMinimumVehicleSpeed.has_value()) {
        engineConfiguration->nitrousMinimumVehicleSpeed = nitrousMinimumVehicleSpeed.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->nitrousMinimumVehicleSpeed,
                engine_configuration_defaults::NITROUS_MINIMUM_VEHICLE_SPEED
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousMinimumTps(const std::optional<int> nitrousMinimumTps) {
    if (nitrousMinimumTps.has_value()) {
        engineConfiguration->nitrousMinimumTps = nitrousMinimumTps.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousMinimumTps,
            engine_configuration_defaults::NITROUS_MINIMUM_TPS
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousMinimumClt(const std::optional<uint8_t> nitrousMinimumClt) {
    if (nitrousMinimumClt.has_value()) {
        engineConfiguration->nitrousMinimumClt = nitrousMinimumClt.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousMinimumClt,
            engine_configuration_defaults::NITROUS_MINIMUM_CLT
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousMaximumMap(const std::optional<uint16_t> nitrousMaximumMap) {
    if (nitrousMaximumMap.has_value()) {
        engineConfiguration->nitrousMaximumMap = nitrousMaximumMap.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->nitrousMaximumMap,
            engine_configuration_defaults::NITROUS_MAXIMUM_MAP
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousMaximumAfr(const std::optional<float> nitrousMaximumAfr) {
    if (nitrousMaximumAfr.has_value()) {
        engineConfiguration->nitrousMaximumAfr = nitrousMaximumAfr.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->nitrousMaximumAfr,
                engine_configuration_defaults::NITROUS_MAXIMUM_AFR
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousActivationRpm(const std::optional<uint16_t> nitrousActivationRpm) {
    if (nitrousActivationRpm.has_value()) {
        engineConfiguration->nitrousActivationRpm = nitrousActivationRpm.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->nitrousActivationRpm,
                engine_configuration_defaults::NITROUS_ACTIVATION_RPM
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousDeactivationRpm(const std::optional<uint16_t> nitrousDeactivationRpm) {
    if (nitrousDeactivationRpm.has_value()) {
        engineConfiguration->nitrousDeactivationRpm = nitrousDeactivationRpm.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->nitrousDeactivationRpm,
                engine_configuration_defaults::NITROUS_DEACTIVATION_RPM
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousDeactivationRpmWindow(
    const std::optional<uint16_t> nitrousDeactivationRpmWindow
) {
    if (nitrousDeactivationRpmWindow.has_value()) {
        engineConfiguration->nitrousDeactivationRpmWindow = nitrousDeactivationRpmWindow.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->nitrousDeactivationRpmWindow,
                engine_configuration_defaults::NITROUS_DEACTIVATION_RPM_WINDOW
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousFuelAdderPercent(const std::optional<int8_t> nitrousFuelAdderPercent) {
    if (nitrousFuelAdderPercent.has_value()) {
        engineConfiguration->nitrousFuelAdderPercent = nitrousFuelAdderPercent.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->nitrousFuelAdderPercent,
                engine_configuration_defaults::NITROUS_FUEL_ADDER_PERCENT
        ); // check default value
    }
}

void TestEngineConfiguration::configureNitrousIgnitionRetard(const std::optional<float> nitrousIgnitionRetard) {
    if (nitrousIgnitionRetard.has_value()) {
        engineConfiguration->nitrousIgnitionRetard = nitrousIgnitionRetard.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->nitrousIgnitionRetard,
                engine_configuration_defaults::NITROUS_IGNITION_RETARD
        ); // check default value
    }
}

void TestEngineConfiguration::configureFuelLevelAveragingAlpha(const std::optional<float> alpha) {
    if (alpha.has_value()) {
        engineConfiguration->fuelLevelAveragingAlpha = alpha.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->fuelLevelAveragingAlpha,
                engine_configuration_defaults::FUEL_LEVEL_AVERAGING_ALPHA
        ); // check default value
    }
}

void TestEngineConfiguration::configureFuelLevelUpdatePeriodSec(const std::optional<float> seconds) {
    if (seconds.has_value()) {
        engineConfiguration->fuelLevelUpdatePeriodSec = seconds.value();
    } else {
        ASSERT_EQ(
            engineConfiguration->fuelLevelUpdatePeriodSec,
            engine_configuration_defaults::FUEL_LEVEL_UPDATE_PERIOD_SEC
        ); // check default value
    }
}

void TestEngineConfiguration::configureFuelLevelLowThresholdVoltage(const std::optional<float> thresholdVoltage) {
    if (thresholdVoltage.has_value()) {
        engineConfiguration->fuelLevelLowThresholdVoltage = thresholdVoltage.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->fuelLevelLowThresholdVoltage,
                engine_configuration_defaults::FUEL_LEVEL_LOW_THRESHOLD_VOLTAGE
        ); // check default value
    }
}

void TestEngineConfiguration::configureFuelLevelHighThresholdVoltage(const std::optional<float> thresholdVoltage) {
    if (thresholdVoltage.has_value()) {
        engineConfiguration->fuelLevelHighThresholdVoltage = thresholdVoltage.value();
    } else {
        ASSERT_EQ(
                engineConfiguration->fuelLevelHighThresholdVoltage,
                engine_configuration_defaults::FUEL_LEVEL_HIGH_THRESHOLD_VOLTAGE
        ); // check default value
    }
}

TestEngineConfiguration::TestEngineConfiguration() {
}

TestEngineConfiguration TestEngineConfiguration::instance;
