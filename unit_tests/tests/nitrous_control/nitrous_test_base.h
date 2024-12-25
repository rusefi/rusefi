//
// Created by kifir on 12/5/24.
//

#pragma once

#include "util/test_base.h"

class NitrousTestBase : public TestBase<> {
protected:
    static constexpr switch_input_pin_e TEST_NITROUS_CONTROL_ARMING_PIN = Gpio::A13;
    static constexpr uint16_t TEST_MIN_VEHICLE_SPEED = 25;
    static constexpr int TEST_MIN_TPS = 34;
    static constexpr uint8_t TEST_MIN_CLT = 51;
    static constexpr int TEST_MAX_MAP = 45;

    static constexpr float TEST_MAXIMUM_AFR = 12.3;
    static constexpr float TEST_LAMBDA1 = TEST_MAXIMUM_AFR / STOICH_RATIO;

    static constexpr uint16_t TEST_ACTIVATION_RPM = 239;
    static constexpr uint16_t TEST_DEACTIVATION_RPM = 932;
    static constexpr uint16_t TEST_DEACTIVATION_RPM_WINDOW = 17;

    void setUpTestConfiguration(
        std::optional<int8_t> nitrousFuelAdderPercent = {},
        std::optional<float> nitrousIgnitionRetard = {}
    );

    void armNitrousControl();
    void satisfySpeedCondition();
    void satisfyTpsCondition();
    void satisfyCltCondition();
    void satisfyMapCondition();
    void satisfyAfrCondition();
    void satisfyRpmCondition();

    void activateNitrousControl();

    void unarmNitrousControl();
    void unsatisfySpeedCondition();
    void unsatisfyTpsCondition();
    void unsatisfyCltCondition();
    void unsatisfyMapCondition();
    void unsatisfyAfrCondition();
    void unsatisfyRpmCondition();

    void deactivateNitrousControl();

    void checkNitrousCondition(bool expected, const char* context);
};
