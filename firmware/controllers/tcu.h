#include "global.h"
#include "io_pins.h"
#include "tunerstudio.h"

class TransmissionControllerBase {
public:
    void update(gear_e);
    gear_e getCurrentGear();
private:
    gear_e currentGear = NEUTRAL;
protected:
    void postState(TunerStudioOutputChannels*);
};

class GearControllerBase {
public:
    void update();
    gear_e getDesiredGear();
private:
    gear_e desiredGear = NEUTRAL;
protected:
    gear_e setDesiredGear(gear_e);
    void postState(TunerStudioOutputChannels*);
    TransmissionControllerBase transmissionController;
};
