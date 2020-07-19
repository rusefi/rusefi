#include "global.h"

class ButtonDebounce {
public:
    bool readPin(brain_pin_e);
private:
    int threshold;
    efitimems_t time;
    brain_pin_e pin;
    iomode_t mode;
}
