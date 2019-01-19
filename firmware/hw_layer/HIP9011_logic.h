/*
 * @file HIP9011_logic.h
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef HW_LAYER_HIP9011_LOGIC_H_
#define HW_LAYER_HIP9011_LOGIC_H_

#include "efifeatures.h"
#include "rusefi_enums.h"
#include "hip9011_lookup.h"

/**
 * this interface defines hardware communication layer for HIP9011 chip
 */
class Hip9011HardwareInterface {
public:
	virtual void sendSyncCommand(unsigned char command) = 0;
	virtual void sendCommand(unsigned char command) = 0;
};

#if EFI_PROD_CODE || EFI_SIMULATOR
#define PASS_HIP_PARAMS
#define DEFINE_HIP_PARAMS
#define GET_CONFIG_VALUE(x) CONFIG(x)
#define FORWARD_HIP_PARAMS
#define DEFINE_PARAM_SUFFIX(x)
#else

#define PASS_HIP_PARAMS CONFIG(knockBandCustom), \
		CONFIG(cylinderBore), \
		CONFIG(hip9011Gain), \
		CONFIG(hip9011PrescalerAndSDO), \
		CONFIG(knockDetectionWindowStart), \
		CONFIG(knockDetectionWindowEnd)

#define FORWARD_HIP_PARAMS knockBandCustom, \
		cylinderBore, \
		hip9011Gain, \
		hip9011PrescalerAndSDO, \
		knockDetectionWindowStart, \
		knockDetectionWindowEnd

#define DEFINE_HIP_PARAMS float knockBandCustom,\
		float cylinderBore, \
		float hip9011Gain, \
		int hip9011PrescalerAndSDO, \
		float knockDetectionWindowStart, \
		float knockDetectionWindowEnd


#define GET_CONFIG_VALUE(x) x
#define DEFINE_PARAM_SUFFIX(x) , x
#endif

class HIP9011 {
public:
	HIP9011(Hip9011HardwareInterface *hardware);
	void prepareHip9011RpmLookup(float angleWindowWidth);
	int getIntegrationIndexByRpm(float rpm);
	void setStateAndCommand(unsigned char cmd);
	void setAngleWindowWidth(DEFINE_HIP_PARAMS);
	void handleValue(int rpm DEFINE_PARAM_SUFFIX(DEFINE_HIP_PARAMS));

	/**
	 * band index is only send to HIP chip on startup
	 */
	int currentBandIndex;
	int currentGainIndex;
	int correctResponsesCount;
	int invalidHip9011ResponsesCount;
	float angleWindowWidth;

	int currentIntergratorIndex;
	bool needToInit;
	int settingUpdateCount;
	int totalKnockEventsCount;
	int currentPrescaler;
	Hip9011HardwareInterface *hardware;
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

	float rpmLookup[INT_LOOKUP_SIZE];
};

float getHIP9011Band(DEFINE_HIP_PARAMS);
int getBandIndex(DEFINE_HIP_PARAMS);
int getHip9011GainIndex(DEFINE_HIP_PARAMS);

// 0b01000000
#define SET_PRESCALER_CMD 0x40

// 0b11100000
#define SET_CHANNEL_CMD 0xE0

// 0b11000000
#define SET_INTEGRATOR_CMD 0xC0

// 0b00000000
#define SET_BAND_PASS_CMD 0x0

// 0b10000000
#define SET_GAIN_CMD 0x80

#define _8MHZ_PRESCALER 6

#endif /* HW_LAYER_HIP9011_LOGIC_H_ */
