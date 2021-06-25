/**
 * @file trigger_emulator_algo.h
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine.h"

class PwmConfig;
class MultiChannelStateSequence;

void initTriggerEmulator(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void startTriggerEmulatorPins();
void stopTriggerEmulatorPins();
void setTriggerEmulatorRPM(int value DECLARE_ENGINE_PARAMETER_SUFFIX);
void onConfigurationChangeRpmEmulatorCallback(engine_configuration_s *previousConfiguration);

// Start & stop trigger emulation
void enableTriggerStimulator();
void enableExternalTriggerStimulator();
void disableTriggerStimulator();

class TriggerEmulatorHelper {
public:
    TriggerEmulatorHelper();
	void handleEmulatorCallback(PwmConfig *state, int stateIndex DECLARE_ENGINE_PARAMETER_SUFFIX);
};

void initTriggerEmulatorLogic(DECLARE_ENGINE_PARAMETER_SIGNATURE);

int getPreviousIndex(const int currentIndex, const int size);
bool needEvent(const int currentIndex, const int size, const MultiChannelStateSequence& mcss, int channelIndex);

