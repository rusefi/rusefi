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
    bool isVinFilled = static_cast<bool>(strlen(engineConfiguration->vinNumber));

    bool vinStateChanged = isVinFilled == engineConfiguration->vinIsEmpty;

    if (vinStateChanged && !isRunningOnBurn) {
        efiPrintf("VinStrategy, reseting flag");
        engineConfiguration->vinIsEmpty = !isVinFilled;

        if (!isVinFilled) {
            engineConfiguration->wizardPanelToShow = DIALOG_NAME_VEHICLE_INFORMATION_HASH;
        } else {
            engineConfiguration->wizardPanelToShow = -1;
        }

        // trigger page reset, see [tag:popular_vehicle]
#if EFI_TUNER_STUDIO && !EFI_UNIT_TEST
        onApplyPreset();
#endif // EFI_TUNER_STUDIO
    }

}