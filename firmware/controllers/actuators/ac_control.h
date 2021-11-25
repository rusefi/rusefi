#pragma once

#include "ac_control_generated.h"

class AcState final : public ac_control_s {
public:
	// Returns true if AC is currently active
	bool updateAc();

private:
	bool getAcState();
};
