//
// Created by kifir on 11/21/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    constexpr float TEST_PRIMARY_INJECTOR_FUEL_REFERENCE_PRESSURE = 5423.1f;
    constexpr float TEST_SECONDARY_INJECTOR_FUEL_REFERENCE_PRESSURE = 678.9f;
    constexpr float TEST_MAP = 78.9f;
    constexpr float TEST_BAROMETRIC_PRESSURE = 67.8f;
    constexpr float TEST_FUEL_PRESSURE_INJECTOR = 76.5f;

    constexpr float TEST_PRIMARY_INJECTOR_FIXED_RAIL_FUEL_DIFFERENTIAL_PRESSURE
        = TEST_PRIMARY_INJECTOR_FUEL_REFERENCE_PRESSURE + TEST_BAROMETRIC_PRESSURE - TEST_MAP;
    constexpr float TEST_SECONDARY_INJECTOR_FIXED_RAIL_FUEL_DIFFERENTIAL_PRESSURE
        = TEST_SECONDARY_INJECTOR_FUEL_REFERENCE_PRESSURE + TEST_BAROMETRIC_PRESSURE - TEST_MAP;
    constexpr float TEST_SENSED_RAIL_ABSOLUTE_FUEL_DIFFERENTIAL_PRESSURE
        = TEST_FUEL_PRESSURE_INJECTOR - TEST_MAP;
    constexpr float TEST_SENSED_RAIL_GAUGE_FUEL_DIFFERENTIAL_PRESSURE
        = TEST_FUEL_PRESSURE_INJECTOR + TEST_BAROMETRIC_PRESSURE - TEST_MAP;

    const EngineConfig TEST_ENGINE_CONFIG = EngineConfig()
        .setFuelReferencePressure(TEST_PRIMARY_INJECTOR_FUEL_REFERENCE_PRESSURE)
        .setSecondaryInjectorFuelReferencePressure(TEST_SECONDARY_INJECTOR_FUEL_REFERENCE_PRESSURE);

    class FuelDifferentialPressureTest : public TestBase<> {
    protected:
        void SetUp() override;

        void checkFuelDifferrentialPressure(
            const EngineConfig& engineConfig,
            std::optional<float> primaryInjectorFuelDifferentialPressure,
            std::optional<float> secondaryInjectorFuelDifferentialPressure,
            const char* context
        );
    private:
        expected<float> getPrimaryInjectorFuelDifferentialPressure();
        expected<float> getSecondaryInjectorFuelDifferentialPressure();
    };

    void FuelDifferentialPressureTest::SetUp(){
        TestBase::SetUp();

        Sensor::setMockValue(SensorType::Map, TEST_MAP);
        Sensor::setMockValue(SensorType::BarometricPressure, TEST_BAROMETRIC_PRESSURE);
        Sensor::setMockValue(SensorType::FuelPressureInjector, TEST_FUEL_PRESSURE_INJECTOR);
    }

    void FuelDifferentialPressureTest::checkFuelDifferrentialPressure(
        const EngineConfig& engineConfig,
        const std::optional<float> expectedPrimaryInjectorFuelDifferentialPressure,
        const std::optional<float> expectedSecondaryInjectorFuelDifferentialPressure,
        const char* const context
    ) {
        setUpEngineConfiguration(engineConfig);
        constexpr float INVALID_PRESSURE = -1.0f;
        if (expectedPrimaryInjectorFuelDifferentialPressure.has_value()) {
            EXPECT_NE(expectedPrimaryInjectorFuelDifferentialPressure, INVALID_PRESSURE) << context;
            EXPECT_NEAR(
                expectedPrimaryInjectorFuelDifferentialPressure.value(),
                getPrimaryInjectorFuelDifferentialPressure().value_or(INVALID_PRESSURE),
                EPS5D
            ) << context;
        } else {
            EXPECT_FALSE(getPrimaryInjectorFuelDifferentialPressure().Valid);
        }
        if (expectedSecondaryInjectorFuelDifferentialPressure.has_value()) {
            EXPECT_NE(expectedSecondaryInjectorFuelDifferentialPressure.value(), INVALID_PRESSURE) << context;
            EXPECT_NEAR(
                expectedSecondaryInjectorFuelDifferentialPressure.value(),
                getSecondaryInjectorFuelDifferentialPressure().value_or(INVALID_PRESSURE),
                EPS5D
            ) << context;
        } else {
            EXPECT_FALSE(getSecondaryInjectorFuelDifferentialPressure().Valid);
        }
    }

    expected<float> FuelDifferentialPressureTest::getPrimaryInjectorFuelDifferentialPressure() {
        return getModule<InjectorModelPrimary>().getFuelDifferentialPressure();
    }

    expected<float> FuelDifferentialPressureTest::getSecondaryInjectorFuelDifferentialPressure() {
        return getModule<InjectorModelSecondary>().getFuelDifferentialPressure();
    }

    TEST_F(FuelDifferentialPressureTest, checkDefault) {
        checkFuelDifferrentialPressure(
            TEST_ENGINE_CONFIG,
            {},
            {},
            "default"
        );
    }

    TEST_F(FuelDifferentialPressureTest, checkNone) {
        checkFuelDifferrentialPressure(
            TEST_ENGINE_CONFIG.clone()
                .setInjectorCompensationMode(ICM_None)
                .setSecondaryInjectorCompensationMode(ICM_None),
            {},
            {},
            "primaryInjectorCompensationMode=ICM_None, "
            "secondaryInjectorCompensationMode=ICM_None"
        );
    }

    TEST_F(FuelDifferentialPressureTest, checkFixedRailPressure) {
        checkFuelDifferrentialPressure(
            TEST_ENGINE_CONFIG.clone()
                .setInjectorCompensationMode(ICM_FixedRailPressure)
                .setSecondaryInjectorCompensationMode(ICM_FixedRailPressure),
            { TEST_PRIMARY_INJECTOR_FIXED_RAIL_FUEL_DIFFERENTIAL_PRESSURE },
            { TEST_SECONDARY_INJECTOR_FIXED_RAIL_FUEL_DIFFERENTIAL_PRESSURE },
            "primaryInjectorCompensationMode=ICM_FixedRailPressure, "
            "secondaryInjectorCompensationMode=ICM_FixedRailPressure"
        );
    }

    TEST_F(FuelDifferentialPressureTest, checkSensedRailPressureDefault) {
        checkFuelDifferrentialPressure(
            TEST_ENGINE_CONFIG.clone()
                .setInjectorCompensationMode(ICM_SensedRailPressure)
                .setSecondaryInjectorCompensationMode(ICM_SensedRailPressure),
            { TEST_SENSED_RAIL_ABSOLUTE_FUEL_DIFFERENTIAL_PRESSURE },
            { TEST_SENSED_RAIL_ABSOLUTE_FUEL_DIFFERENTIAL_PRESSURE },
            "primaryInjectorCompensationMode=ICM_SensedRailPressure, "
            "secondaryInjectorCompensationMode=ICM_SensedRailPressure"
        );
    }

    TEST_F(FuelDifferentialPressureTest, checkSensedRailPressureAbsolute) {
        checkFuelDifferrentialPressure(
            TEST_ENGINE_CONFIG.clone()
                .setInjectorCompensationMode(ICM_SensedRailPressure)
                .setSecondaryInjectorCompensationMode(ICM_SensedRailPressure)
                .setFuelPressureSensorMode(FPM_Absolute),
            { TEST_SENSED_RAIL_ABSOLUTE_FUEL_DIFFERENTIAL_PRESSURE },
            { TEST_SENSED_RAIL_ABSOLUTE_FUEL_DIFFERENTIAL_PRESSURE },
            "primaryInjectorCompensationMode=ICM_SensedRailPressure, "
            "secondaryInjectorCompensationMode=ICM_SensedRailPressure, "
            "fuelPressureSensorMode=FPM_Absolute"
        );
    }

    TEST_F(FuelDifferentialPressureTest, checkSensedRailPressureGauge) {
        checkFuelDifferrentialPressure(
            TEST_ENGINE_CONFIG.clone()
                .setInjectorCompensationMode(ICM_SensedRailPressure)
                .setSecondaryInjectorCompensationMode(ICM_SensedRailPressure)
                .setFuelPressureSensorMode(FPM_Gauge),
            { TEST_SENSED_RAIL_GAUGE_FUEL_DIFFERENTIAL_PRESSURE },
            { TEST_SENSED_RAIL_GAUGE_FUEL_DIFFERENTIAL_PRESSURE },
            "primaryInjectorCompensationMode=ICM_SensedRailPressure, "
            "secondaryInjectorCompensationMode=ICM_SensedRailPressure, "
            "fuelPressureSensorMode=FPM_Gauge"
        );
    }

    TEST_F(FuelDifferentialPressureTest, checkSensedRailPressureDifferential) {
        checkFuelDifferrentialPressure(
            TEST_ENGINE_CONFIG.clone()
                .setInjectorCompensationMode(ICM_SensedRailPressure)
                .setSecondaryInjectorCompensationMode(ICM_SensedRailPressure)
                .setFuelPressureSensorMode(FPM_Differential),
            { TEST_FUEL_PRESSURE_INJECTOR },
            { TEST_FUEL_PRESSURE_INJECTOR },
            "primaryInjectorCompensationMode=ICM_SensedRailPressure, "
            "secondaryInjectorCompensationMode=ICM_SensedRailPressure, "
            "fuelPressureSensorMode=FPM_Differential"
        );
    }

    TEST_F(FuelDifferentialPressureTest, checkMix) {
        checkFuelDifferrentialPressure(
            TEST_ENGINE_CONFIG.clone()
                .setInjectorCompensationMode(ICM_FixedRailPressure)
                .setSecondaryInjectorCompensationMode(ICM_SensedRailPressure),
            { TEST_PRIMARY_INJECTOR_FIXED_RAIL_FUEL_DIFFERENTIAL_PRESSURE },
            { TEST_SENSED_RAIL_ABSOLUTE_FUEL_DIFFERENTIAL_PRESSURE },
            "primaryInjectorCompensationMode=ICM_FixedRailPressure, "
            "secondaryInjectorCompensationMode=ICM_SensedRailPressure"
        );
    }
}