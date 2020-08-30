#include "globalaccess.h"
#include "io_pins.h"

class ButtonDebounce {
public:
    ButtonDebounce(int t, brain_pin_e p, pin_input_mode_e m);
    bool readPinEvent();
private:
    bool checkThreshold(efitimems_t);
    int threshold;
    efitimems_t timeLast;
    brain_pin_e pin;
    iomode_t mode;
    bool readValue;
};
