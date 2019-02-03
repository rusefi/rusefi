/**
 * @file	EfiWave.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef EFI_WAVE_H_
#define EFI_WAVE_H_

#include "global.h"

#define PWM_PHASE_MAX_COUNT 252
#define PWM_PHASE_MAX_WAVE_PER_PWM 3

/**
 * int8_t is probably less efficient then int32_t but we need
 * to reduce memory footprint
 *
 * todo: migrate to bit-array to save memory?
 * this would cost some CPU cycles. see std::vector<bool>
 */
typedef int8_t pin_state_t;

/**
 * This class represents one channel of a digital signal state sequence
 * Each element represents either a HIGH or LOW state - while at the moment this
 * is not implemented using a bit array, it could absolutely be a bit array
 *
 * This sequence does not know anything about signal lengths - only signal state at a given index
 *
 * @brief   PWM configuration for the specific output pin
 */
class SingleWave {
public:
	SingleWave();
	SingleWave(pin_state_t *pinStates);
	void init(pin_state_t *pinStates);
	/**
	 * todo: confirm that we only deal with two states here, no magic '-1'?
	 * @return HIGH or LOW state at given index
	 */
	pin_state_t getState(int switchIndex);
	void setState(int switchIndex, pin_state_t state);

	// todo: make this private by using 'getState' and 'setState' methods
	pin_state_t *pinStates;
};

/**
 * This class represents multi-channel logical signals with shared time axis
 *
 */
class MultiWave {
public:
	void baseConstructor();
	MultiWave();
	MultiWave(float *switchTimes, SingleWave *waves);
	void init(float *switchTimes, SingleWave *waves);
	void reset(void);
	float getSwitchTime(int phaseIndex) const;
	void setSwitchTime(int phaseIndex, float value);
	void checkSwitchTimes(int size);
	int getChannelState(int channelIndex, int phaseIndex) const;

	int findAngleMatch(float angle, int size) const;
	int findInsertionAngle(float angle, int size) const;

	/**
	 * Number of signal channels
	 */
	int waveCount;
	SingleWave *channels;
//private:
	/**
	 * values in the (0..1] range which refer to points within the period at at which pin state should be changed
	 * So, in the simplest case we turn pin off at 0.3 and turn it on at 1 - that would give us a 70% duty cycle PWM
	 */
	float *switchTimes;
};

#endif /* EFI_WAVE_H_ */
