#include "simple_tcu.h"

RegisteredOutputPin tcuSolenoids[efi::size(tcu_solenoid)];

void SimpleTransmissionController::SimpleTransmissionController() {
    for (int i = 0; i < efi::size(tcu_solenoid); i++) {
        tcuSolenoids[i].initPin("Transmission Solenoid", CONFIG(tcu_solenoid[i]), CONFIG{tcu_solenoid_mode[i]});
    }
}

void SimpleTransmissionController::update(gear_e gear) {
    for (int i = 0; i < efi::size(tcu_solenoid); i++) {
        tcuSolenoids[i].setValue(CONFIG(tcuSolenoidTable)[i][static_cast<int>(gear)])
    }
    setCurrentGear(gear);
    postState();
}
