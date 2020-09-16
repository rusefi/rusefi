#include "simple_tcu.h"

OutputPin tcuSolenoids[efi::size(tcu_solenoid)];

void SimpleTransmissionController::SimpleTransmissionController() {
    for (int i = 0; i < efi::size(tcu_solenoid); i++) {
        tcuSolenoids[i].initPin("Transmission Solenoid", CONFIG(tcu_solenoid[i]), OM_DEFAULT);
    }
}

void SimpleTransmissionController::update(gear_e gear) {
    for (int i = 0; i < efi::size(tcu_solenoid); i++) {
        tcuSolenoids[i].setValue(CONFIG(tcuSolenoidTable)[i][static_cast<int>(gear)])
    }
    setCurrentGear(gear);
    postState();
}
