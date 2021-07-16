/**
 * @file trigger_emulator_algo.h
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"

struct engine_configuration_s;
class PwmConfig;
class MultiChannelStateSequence;

void initTriggerEmulator(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void startTriggerEmulatorPins(DECLARE_ENGINE_PARAMETER_SIGNATURE);
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
	void handleEmulatorCallback(const int size, const MultiChannelStateSequence& mcss, int stateIndex DECLARE_ENGINE_PARAMETER_SUFFIX);
};

void initTriggerEmulatorLogic(DECLARE_ENGINE_PARAMETER_SIGNATURE);

int getPreviousIndex(const int currentIndex, const int size);
bool needEvent(const int currentIndex, const int size, const MultiChannelStateSequence& mcss, int channelIndex);

