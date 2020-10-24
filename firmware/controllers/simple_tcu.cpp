#include "simple_tcu.h"
#include "efi_gpio.h"
#include "engine_configuration.h"

EXTERN_CONFIG;

OutputPin tcuSolenoids[TCU_SOLENOID_COUNT];

void SimpleTransmissionController::init() {
    for (int i = 0; i < efi::size(CONFIG(tcu_solenoid)); i++) {
        tcuSolenoids[i].initPin("Transmission Solenoid", CONFIG(tcu_solenoid)[i], &CONFIG(tcu_solenoid_mode)[i]);
    }
}

void SimpleTransmissionController::update(gear_e gear) {
    for (int i = 0; i < efi::size(CONFIG(tcu_solenoid)); i++) {
#ifndef EFI_UNIT_TEST
        tcuSolenoids[i].setValue(config->tcuSolenoidTable[static_cast<int>(gear) + 1][i]);
#endif
    }
    setCurrentGear(gear);
    postState();
}
