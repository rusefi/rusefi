include "global.h"

class GearControllerBase {
public:
    void update();
    gear_e getDesiredGear();
private:
    gear_e setDesiredGear(gear_e);
    gear_e desiredGear;
}

class TransmissionControllerBase {
public:
    void update(gear_e);
private:
    gear_e currentGear;
}
