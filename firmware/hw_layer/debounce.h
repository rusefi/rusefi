include "global.h"

class ButtonDebounce {
public:
    bool readPin(brain_pin_e);
private:
    int threshold;
    efitimems_t time;
}
