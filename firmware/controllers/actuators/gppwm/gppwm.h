#pragma once

#include "rusefi_enums.h"

void initGpPwm();
void updateGppwm();

// H-bridge repurposed as a general-purpose GPPWM output, see https://github.com/rusefi/rusefi/issues/9673
inline bool isDcGppwmFunction(dc_function_e func) {
	return func >= DC_Gppwm1 && func <= DC_Gppwm4;
}
