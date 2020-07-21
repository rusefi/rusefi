#include "global.h"
#include "io_pins.cpp"

class GearControllerBase {
public:
    void update();
    gear_e getDesiredGear();
private:
    gear_e setDesiredGear(gear_e);
    gear_e desiredGear;
    void postState(TunerStudioOutputChannels);
}

class TransmissionControllerBase {
public:
    void update(gear_e);
    gear_e getCurrentGear();
private:
    gear_e currentGear;
    void postState(TunerStudioOutputChannels);
}
