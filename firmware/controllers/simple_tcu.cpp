#include "simple_tcu.h"
#include "efi_gpio.h"
#include "engine_configuration.h"

EXTERN_CONFIG;

OutputPin tcuSolenoids[TCU_SOLENOID_COUNT];

void SimpleTransmissionController::init() {
    for (size_t i = 0; i < efi::size(CONFIG(tcu_solenoid)); i++) {
        tcuSolenoids[i].initPin("Transmission Solenoid", CONFIG(tcu_solenoid)[i], &CONFIG(tcu_solenoid_mode)[i]);
    }
}

void SimpleTransmissionController::update(gear_e gear) {
    for (size_t i = 0; i < efi::size(CONFIG(tcu_solenoid)); i++) {
#if ! EFI_UNIT_TEST
        tcuSolenoids[i].setValue(config->tcuSolenoidTable[i][static_cast<int>(gear) + 1]);
#endif
    }
    setCurrentGear(gear);
    postState();
}
