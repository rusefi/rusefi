#include "simple_tcu.h"

OutputPin tcuSolenoids[TCU_SOLENOID_COUNT];

void SimpleTransmissionController::SimpleTransmissionController() {
 1 <= TCU_SOLENOID_COUNT ? tcuSolenoids[1].initPin("Transmission Solenoid", CONFIG(tcu_solenoid1), OM_DEFAULT);
 2 <= TCU_SOLENOID_COUNT ? tcuSolenoids[2].initPin("Transmission Solenoid", CONFIG(tcu_solenoid2), OM_DEFAULT);
 3 <= TCU_SOLENOID_COUNT ? tcuSolenoids[3].initPin("Transmission Solenoid", CONFIG(tcu_solenoid3), OM_DEFAULT);
 4 <= TCU_SOLENOID_COUNT ? tcuSolenoids[4].initPin("Transmission Solenoid", CONFIG(tcu_solenoid4), OM_DEFAULT);
 5 <= TCU_SOLENOID_COUNT ? tcuSolenoids[5].initPin("Transmission Solenoid", CONFIG(tcu_solenoid5), OM_DEFAULT);
 6 <= TCU_SOLENOID_COUNT ? tcuSolenoids[6].initPin("Transmission Solenoid", CONFIG(tcu_solenoid6), OM_DEFAULT);
 7 <= TCU_SOLENOID_COUNT ? tcuSolenoids[7].initPin("Transmission Solenoid", CONFIG(tcu_solenoid7), OM_DEFAULT);
 8 <= TCU_SOLENOID_COUNT ? tcuSolenoids[8].initPin("Transmission Solenoid", CONFIG(tcu_solenoid8), OM_DEFAULT);
}

void SimpleTransmissionController::update(gear_e gear) {
    for (int i = 1; i <= TCU_SOLENOID_COUNT; i++) {
        i <= TCU_SOLENOID_COUNT ? tcuSolenoids[i].setValue(CONFIG(tcuSolenoidTable)[i][static_cast<int>(gear)])
    }
    setCurrentGear(gear);
    postState();
}
