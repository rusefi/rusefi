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
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: since VIN is filled, flag should flip to false
    ASSERT_FALSE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinFilledWasNotEmpty_ShouldStayNotEmpty) {
    // given: VIN string is non-empty, and flag already says not empty
    strcpy(engineConfiguration->vinNumber, "1M8GDM9AXKP042788");
    engineConfiguration->vinIsEmpty = false;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: no change expected
    ASSERT_FALSE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinEmptyWasEmpty_ShouldStayEmpty) {
    // given: VIN string is empty, and flag already says empty
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->vinIsEmpty = true;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: no change expected
    ASSERT_TRUE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinEmptyWasNotEmpty_ShouldBecomeEmpty) {
    // given: VIN string is empty, but flag currently says not empty
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->vinIsEmpty = false;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: since VIN is empty, flag should flip to true
    ASSERT_TRUE(engineConfiguration->vinIsEmpty);
}

TEST_F(ConfigurationWizardTest, VinFilledMismatch_ShouldClearWizardPanel) {
    // given: VIN string is non-empty, but flag currently says empty (mismatch triggers state change)
    strcpy(engineConfiguration->vinNumber, "1M8GDM9AXKP042788");
    engineConfiguration->vinIsEmpty = true;
    engineConfiguration->wizardPanelToShow = 42; // some non-default value

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: VIN becomes not empty and wizard panel cleared
    ASSERT_FALSE(engineConfiguration->vinIsEmpty);
    ASSERT_EQ(-1, engineConfiguration->wizardPanelToShow);
}

TEST_F(ConfigurationWizardTest, VinEmptyMismatch_ShouldOpenVehicleInfoPanel) {
    // given: VIN string is empty, but flag currently says not empty (mismatch triggers state change)
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->vinIsEmpty = false;
    engineConfiguration->wizardPanelToShow = -1;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: VIN becomes empty and wizard panel set to Vehicle Information
    ASSERT_TRUE(engineConfiguration->vinIsEmpty);
    ASSERT_EQ(DIALOG_NAME_VEHICLE_INFORMATION_HASH, engineConfiguration->wizardPanelToShow);
}

TEST_F(ConfigurationWizardTest, IsRunningOnBurn_ShouldNotChangeStateOrPanel) {
    // given: mismatch, but running on burn should suppress changes
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->vinIsEmpty = false;
    engineConfiguration->wizardPanelToShow = 7;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(true);

    // then: no changes applied
    ASSERT_FALSE(engineConfiguration->vinIsEmpty);
    ASSERT_EQ(7, engineConfiguration->wizardPanelToShow);
}

TEST_F(ConfigurationWizardTest, NoStateChange_ShouldNotTouchPanel) {
    // given: already consistent empty VIN state
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->vinIsEmpty = true;
    engineConfiguration->wizardPanelToShow = 99;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: panel remains unchanged
    ASSERT_TRUE(engineConfiguration->vinIsEmpty);
    ASSERT_EQ(99, engineConfiguration->wizardPanelToShow);
}
