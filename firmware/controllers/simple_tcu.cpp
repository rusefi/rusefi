#include "simple_tcu.h"
#include "efi_gpio.h"
#include "global.h"
#include "engine_configuration.h"

EXTERN_CONFIG;

OutputPin tcuSolenoids[efi::size(CONFIG(tcu_solenoid))];

SimpleTransmissionController::SimpleTransmissionController() {
    for (int i = 0; i < efi::size(CONFIG(tcu_solenoid)); i++) {
scheduleMsg(&sharedLogger, "foo");
//        tcuSolenoids[i].initPin("Transmission Solenoid", CONFIG(tcu_solenoid[i]), &CONFIG(tcu_solenoid_mode[i]));
    }
}

void SimpleTransmissionController::update(gear_e gear) {
    for (int i = 0; i < efi::size(CONFIG(tcu_solenoid)); i++) {
        tcuSolenoids[i].setValue(config->tcuSolenoidTable[i][static_cast<int>(gear)]);
    }
    setCurrentGear(gear);
    postState();
}
