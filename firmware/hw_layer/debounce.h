#include "global.h"

class ButtonDebounce {
public:
    bool readPin();
    bool readEvent();
private:
    bool checkThreshold(efitimems_t);
    int threshold;
    efitimems_t timeLast;
    brain_pin_e pin;
    iomode_t mode;
    bool readValue;
}
