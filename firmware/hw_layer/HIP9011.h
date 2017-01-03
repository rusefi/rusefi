/**
 * @file	HIP9011.h
 * @brief	HIP9011/TPIC8101 driver
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef HIP9011_H_
#define HIP9011_H_

typedef enum {
	NOT_READY,
	/**
	 * the step after this one is always IS_INTEGRATING
	 */
	READY_TO_INTEGRATE,
	/**
	 * the step after this one is always WAITING_FOR_ADC_TO_SKIP
	 */
	IS_INTEGRATING,
	/**
	 * the step after this one is always WAITING_FOR_RESULT_ADC
	 */
	WAITING_FOR_ADC_TO_SKIP,
	/**
	 * the step after this one is always IS_SENDING_SPI_COMMAND or READY_TO_INTEGRATE
	 */
	WAITING_FOR_RESULT_ADC,
	/**
	 * the step after this one is always READY_TO_INTEGRATE
	 */
	IS_SENDING_SPI_COMMAND,
} hip_state_e;

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

// 0b01110001
#define SET_ADVANCED_MODE 0x71

#define HIP_THREAD_PERIOD 100

void initHip9011(Logging *sharedLogger);
void setHip9011FrankensoPinout(void);
void hipAdcCallback(adcsample_t value);

#endif /* HIP9011_H_ */
