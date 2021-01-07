#include "simple_tcu.h"
#include "efi_gpio.h"
#include "tunerstudio_outputs.h"
#include "engine_configuration.h"

EXTERN_ENGINE;

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

    if (engineConfiguration->debugMode == DBG_ETB_AUTOTUNE) {
        tsOutputChannels.debugIntField1 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][0];
        tsOutputChannels.debugIntField2 = config->tcuSolenoidTable[static_cast<int>(gear) + 1][1];
    }
}
