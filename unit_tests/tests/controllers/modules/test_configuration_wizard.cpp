/*
 * @file test_configuration_wizard.cpp
 *
 * @date: nov 11, 2025
 * @author FDSoftware
 */

#include "pch.h"
#include "tests/util/test_base.h"

class ConfigurationWizardTest : public TestBase<> {
};

static void setVin(const char* value) {
    strncpy(engineConfiguration->vinNumber, value, VIN_NUMBER_SIZE);
    engineConfiguration->vinNumber[VIN_NUMBER_SIZE - 1] = '\0';
}

TEST_F(ConfigurationWizardTest, VinFilledWasEmpty_ShouldBecomeNotEmpty) {
    // given: VIN string is non-empty, but flag currently says empty
    setVin("1M8GDM9AXKP042788");
    engineConfiguration->vinIsEmpty = true;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn();

    // then: since VIN is filled, flag should flip to false
    ASSERT_FALSE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinFilledWasNotEmpty_ShouldStayNotEmpty) {
    // given: VIN string is non-empty, and flag already says not empty
    setVin("JM1NA3512R0512345");
    engineConfiguration->vinIsEmpty = false;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn();

    // then: no change expected
    ASSERT_FALSE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinEmptyWasEmpty_ShouldStayEmpty) {
    // given: VIN string is empty, and flag already says empty
    setVin("");
    engineConfiguration->vinIsEmpty = true;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn();

    // then: no change expected
    ASSERT_TRUE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinEmptyWasNotEmpty_ShouldBecomeEmpty) {
    // given: VIN string is empty, but flag currently says not empty
    setVin("");
    engineConfiguration->vinIsEmpty = false;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn();

    // then: since VIN is empty, flag should flip to true
    ASSERT_TRUE(engineConfiguration->vinIsEmpty);
}
