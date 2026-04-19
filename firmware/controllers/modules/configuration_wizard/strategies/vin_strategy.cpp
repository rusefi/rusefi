/*
 * @file vin_strategy.cpp
 *
 * @date: nov 11, 2025
 * @author FDSoftware
 */
#include <cstring>

#include "pch.h"
#include "../configuration_wizard.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif

/*
 * here we need to track if the wizard is active (vin is empty) and trigger TS page refresh
 */
void vinStrategy(bool isRunningOnBurn) {

    if (isRunningOnBurn) {
        return;
    }

    bool isVinFilled = static_cast<bool>(strlen(engineConfiguration->vinNumber));
    int vinWizard = djb2lowerCase(DIALOG_NAME_VEHICLE_INFORMATION);

    // the check with wizardPanelToShow is for not overriding an existing panel
    // (i.e., we need to run more than one wizard with the current ecu configuration)
    bool changed = false;
    if (!isVinFilled && engineConfiguration->wizardPanelToShow == -1) {
        efiPrintf("VinStrategy, showing VIN wizard");
        engineConfiguration->wizardPanelToShow = vinWizard;
        changed = true;
    } else if (isVinFilled && engineConfiguration->wizardPanelToShow == vinWizard) {
        engineConfiguration->wizardPanelToShow = -1;
        changed = true;
    }

    // Only trigger page reset when wizard panel actually changed, not on every CRC check.
    // Unconditional onApplyPreset() here caused TS to reload page 1 on every CRC check,
    // discarding pending user edits and creating a rapid alternation loop on the burning indicator on TS [tag:popular_vehicle]
#if EFI_TUNER_STUDIO
    if (changed) {
        onApplyPreset();
    }
#endif // EFI_TUNER_STUDIO

}