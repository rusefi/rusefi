#pragma once

#include "fan_control_generated.h"

struct FanController : public fan_control_s {
	void update(bool acActive);

private:
	bool getState(bool acActive, bool lastState);

protected:
	virtual OutputPin& getPin() = 0;
	virtual float getFanOnTemp() = 0;
	virtual float getFanOffTemp() = 0;
	virtual bool enableWithAc() = 0;
	virtual bool disableWhenStopped() = 0;
};

void updateFans(bool acActive);
