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

TEST_F(ConfigurationWizardTest, VinEmptyMismatch_ShouldOpenVehicleInfoPanel) {
    // given: VIN string is empty,and we aren't running another wizard
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->wizardPanelToShow = -1;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: VIN becomes empty and wizard panel set to Vehicle Information
    ASSERT_EQ(djb2lowerCase(DIALOG_NAME_VEHICLE_INFORMATION), engineConfiguration->wizardPanelToShow);
}

TEST_F(ConfigurationWizardTest, IsRunningOnBurn_ShouldNotChangeStateOrPanel) {
    // given: mismatch, but running on burn should suppress changes
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->wizardPanelToShow = -1;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(true);

    // then: no changes applied
    ASSERT_EQ(-1, engineConfiguration->wizardPanelToShow);
}

TEST_F(ConfigurationWizardTest, NoStateChange_ShouldNotTouchPanel) {
    // given: already empty VIN state
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->wizardPanelToShow = 99;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: panel remains unchanged
    ASSERT_EQ(99, engineConfiguration->wizardPanelToShow);
}
