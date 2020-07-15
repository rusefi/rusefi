include "global.h"

class TransmissionControllerBase {
public:
	void update();
protected:
	virtual gear_e GetShiftRequest();
private:
	gear_e currentGear;
	gear_e desiredGear;
};
