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

TEST_F(ConfigurationWizardTest, VinFilledWasEmpty_ShouldBecomeNotEmpty) {
    // given: VIN string is non-empty, but flag currently says empty
    strcpy(engineConfiguration->vinNumber, "1M8GDM9AXKP042788");
    engineConfiguration->vinIsEmpty = true;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn();

    // then: since VIN is filled, flag should flip to false
    ASSERT_FALSE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinFilledWasNotEmpty_ShouldStayNotEmpty) {
    // given: VIN string is non-empty, and flag already says not empty
    strcpy(engineConfiguration->vinNumber, "1M8GDM9AXKP042788");
    engineConfiguration->vinIsEmpty = false;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn();

    // then: no change expected
    ASSERT_FALSE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinEmptyWasEmpty_ShouldStayEmpty) {
    // given: VIN string is empty, and flag already says empty
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->vinIsEmpty = true;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn();

    // then: no change expected
    ASSERT_TRUE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinEmptyWasNotEmpty_ShouldBecomeEmpty) {
    // given: VIN string is empty, but flag currently says not empty
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->vinIsEmpty = false;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn();

    // then: since VIN is empty, flag should flip to true
    ASSERT_TRUE(engineConfiguration->vinIsEmpty);
}
