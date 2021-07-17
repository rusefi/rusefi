#include "simple_tcu.h"

void SimpleTransmissionController::init() {
    for (size_t i = 0; i < efi::size(CONFIG(tcu_solenoid)); i++) {
        enginePins.tcuSolenoids[i].initPin("Transmission Solenoid", CONFIG(tcu_solenoid)[i], &CONFIG(tcu_solenoid_mode)[i]);
    }
}

void SimpleTransmissionController::update(gear_e gear) {
    for (size_t i = 0; i < efi::size(CONFIG(tcu_solenoid)); i++) {
#if ! EFI_UNIT_TEST
    	enginePins.tcuSolenoids[i].setValue(config->tcuSolenoidTable[static_cast<int>(gear) + 1][i]);
#endif
    }
    setCurrentGear(gear);
    postState();

#if EFI_TUNER_STUDIO
    if (engineConfiguration->debugMode == DBG_TCU) {
        tsOutputChannels.debugIntField1 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][0];
        tsOutputChannels.debugIntField2 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][1];
        tsOutputChannels.debugIntField3 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][2];
        tsOutputChannels.debugIntField4 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][3];
        tsOutputChannels.debugIntField5 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][4];
    }
#endif
}
