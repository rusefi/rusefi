#include "simple_tcu.h"
#include "efi_gpio.h"
#include "engine_configuration.h"

EXTERN_CONFIG;

#ifndef EFI_UNIT_TEST
OutputPin tcuSolenoids[efi::size(CONFIG(tcu_solenoid))];
#endif

void SimpleTransmissionController::init() {
    for (int i = 0; i < efi::size(CONFIG(tcu_solenoid)); i++) {
#ifndef EFI_UNIT_TEST
        tcuSolenoids[i].initPin("Transmission Solenoid", CONFIG(tcu_solenoid)[i], &CONFIG(tcu_solenoid_mode)[i]);
#endif
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
