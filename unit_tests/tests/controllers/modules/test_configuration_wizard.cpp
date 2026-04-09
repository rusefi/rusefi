/*
 * @file test_configuration_wizard.cpp
 *
 * @date: nov 11, 2025
 * @author FDSoftware
 */

#include "pch.h"
#include "tests/util/test_base.h"
#include "tunerstudio.h"

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

TEST_F(ConfigurationWizardTest, VinEmpty_PanelNotSet_ShouldCallOnApplyPreset) {
    // given: VIN is empty and no wizard is active — first-time detection
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->wizardPanelToShow = -1;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: panel was set (state changed) so onApplyPreset was called → TS refresh pending
    ASSERT_EQ(djb2lowerCase(DIALOG_NAME_VEHICLE_INFORMATION), engineConfiguration->wizardPanelToShow);
    ASSERT_TRUE(needToTriggerTsRefresh());
}

TEST_F(ConfigurationWizardTest, VinEmpty_PanelAlreadySet_ShouldNotCallOnApplyPreset) {
    // given: wizard already active — no state change on a repeat call (e.g. next CRC check)
    const int vinPanel = djb2lowerCase(DIALOG_NAME_VEHICLE_INFORMATION);
    strcpy(engineConfiguration->vinNumber, "");
    engineConfiguration->wizardPanelToShow = vinPanel;

    // ensure engineTypeChangeTimer is in "already elapsed" state before the call
    engine->engineTypeChangeTimer.init();

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: no state change → onApplyPreset must NOT have been called
    ASSERT_FALSE(needToTriggerTsRefresh());
}

TEST_F(ConfigurationWizardTest, VinFilled_WizardActive_ShouldCallOnApplyPreset) {
    // given: VIN just filled while the VIN wizard was showing
    const int vinPanel = djb2lowerCase(DIALOG_NAME_VEHICLE_INFORMATION);
    strcpy(engineConfiguration->vinNumber, "1HGBH41JXMN109186");
    engineConfiguration->wizardPanelToShow = vinPanel;

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: wizard dismissed (state changed) → onApplyPreset called → TS refresh pending
    ASSERT_EQ(-1, engineConfiguration->wizardPanelToShow);
    ASSERT_TRUE(needToTriggerTsRefresh());
}

TEST_F(ConfigurationWizardTest, VinFilled_PanelAlreadyClear_ShouldNotCallOnApplyPreset) {
    // given: VIN already filled and panel already cleared — nothing to do
    strcpy(engineConfiguration->vinNumber, "1HGBH41JXMN109186");
    engineConfiguration->wizardPanelToShow = -1;

    engine->engineTypeChangeTimer.init();

    // when
    ConfigurationWizard::onConfigOnStartUpOrBurn(false);

    // then: no state change → onApplyPreset must NOT have been called
    ASSERT_FALSE(needToTriggerTsRefresh());
}
