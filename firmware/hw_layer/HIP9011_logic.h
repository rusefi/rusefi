/*
 * @file HIP9011_logic.h
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef HW_LAYER_HIP9011_LOGIC_H_
#define HW_LAYER_HIP9011_LOGIC_H_

#include "rusefi_enums.h"

/**
 * this interface defines SPI communication channel with HIP9011 chip
 */
class HIP9011SpiChannel {
public:
	void sendCommand(unsigned char command);
};

class HIP9011 {
public:
	HIP9011();
	bool needToInit;
	/**
	 * Int/Hold pin is controlled from scheduler call-backs which are set according to current RPM
	 *
	 * The following state makes sure that we only have SPI communication while not integrating and that we take
	 * a good ADC reading after integrating.
	 *
	 * Once integration window is over, we wait for the 2nd ADC callback and then initiate SPI communication if needed
	 *
	 * hipOutput should be set to used FAST adc device
	 */
	hip_state_e state;
};


#endif /* HW_LAYER_HIP9011_LOGIC_H_ */
