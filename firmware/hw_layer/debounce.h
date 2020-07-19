#include "global.h"

class ButtonDebounce {
public:
    bool readPin(brain_pin_e);
private:
    int threshold;
    efitimems_t timeLast;
    brain_pin_e pin;
    iomode_t mode;
}
