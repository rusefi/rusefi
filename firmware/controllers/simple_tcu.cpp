#include "simple_tcu.h"

OutputPin tcuSolenoids[TCU_SOLENOID_COUNT];

void SimpleTransmissionController::SimpleTransmissionController() {
    for (int i = 0; i < TCU_SOLENOID_COUNT; i++) {
        tcuSolenoids[i].initPin("Transmission Solenoid", CONFIG(tcu_solenoid[i]), OM_DEFAULT);
    }
}

void SimpleTransmissionController::update(gear_e gear) {
    for (int i = 0; i < TCU_SOLENOID_COUNT; i++) {
        tcuSolenoids[i].setValue(CONFIG(tcuSolenoidTable)[i][static_cast<int>(gear)])
    }
    setCurrentGear(gear);
    postState();
}
