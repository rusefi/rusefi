/*
 * @file hip9011_logic.h
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "efifeatures.h"
#include "rusefi_enums.h"
#include "hip9011_lookup.h"

#define BAND(bore) (900 / (PIF * (bore) / 2))

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
	explicit HIP9011(Hip9011HardwareInterface *hardware);
	void prepareHip9011RpmLookup(float angleWindowWidth);
	int getIntegrationIndexByRpm(float rpm);
	void setStateAndCommand(unsigned char cmd);
	void setAngleWindowWidth(DEFINE_HIP_PARAMS);
	void handleValue(int rpm DEFINE_PARAM_SUFFIX(DEFINE_HIP_PARAMS));

	/**
	 * band index is only send to HIP chip on startup
	 */
	int currentBandIndex = 0;
	int currentGainIndex = -1;
	int correctResponsesCount = 0;
	int invalidHip9011ResponsesCount = 0;
	float angleWindowWidth = - 1;

	int currentIntergratorIndex = -1;
	bool needToInit = true;
	int totalKnockEventsCount = 0;
	int currentPrescaler = 0;
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
	uint8_t cylinderNumber;

	/* error counters */
	int overrun = 0;

	float rpmLookup[INT_LOOKUP_SIZE];
};

float getHIP9011Band(DEFINE_HIP_PARAMS);
int getBandIndex(DEFINE_HIP_PARAMS);
int getHip9011GainIndex(DEFINE_HIP_PARAMS);

// 0b010x.xxxx
#define SET_PRESCALER_CMD(v) 	(0x40 | ((v) & 0x1f))
// 0b1110.000x
#define SET_CHANNEL_CMD(v) 		(0xE0 | ((v) & 0x01))
// 0b00xx.xxxx
#define SET_BAND_PASS_CMD(v)	(0x00 | ((v) & 0x3f))
// 0b10xx.xxxx
#define SET_GAIN_CMD(v)			(0x80 | ((v) & 0x3f))
// 0b110x.xxxx
#define SET_INTEGRATOR_CMD(v)	(0xC0 | ((v) & 0x1f))
// 0b0111.0001
#define SET_ADVANCED_MODE_CMD	(0x71)

//	D[4:1] = 0000 : 4 MHz
#define HIP_4MHZ_PRESCALER		(0x0 << 1)
//	D[4:1] = 0001 : 5 MHz
#define HIP_5MHZ_PRESCALER		(0x1 << 1)
//	D[4:1] = 0010 : 6 MHz
#define HIP_6MHZ_PRESCALER		(0x2 << 1)
//	D[4:1] = 0011 ; 8 MHz
#define HIP_8MHZ_PRESCALER		(0x3 << 1)
//	D[4:1] = 0100 ; 10 MHz
#define HIP_10MHZ_PRESCALER		(0x4 << 1)
//	D[4:1] = 0101 ; 12 MHz
#define HIP_12MHZ_PRESCALER		(0x5 << 1)
//	D[4:1] = 0110 : 16 MHz
#define HIP_16MHZ_PRESCALER		(0x6 << 1)
//	D[4:1] = 0111 : 20 MHz
#define HIP_20MHZ_PRESCALER		(0x7 << 1)
//	D[4:1] = 1000 : 24 MHz
#define HIP_24MHZ_PRESCALER		(0x8 << 1)

