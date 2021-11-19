/**
 * @file trigger_emulator_algo.h
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

class PwmConfig;
class MultiChannelStateSequence;

void initTriggerEmulator();
void startTriggerEmulatorPins();
void stopTriggerEmulatorPins();
void setTriggerEmulatorRPM(int value);
void onConfigurationChangeRpmEmulatorCallback(engine_configuration_s *previousConfiguration);

// Start & stop trigger emulation
void enableTriggerStimulator();
void enableExternalTriggerStimulator();
void disableTriggerStimulator();

class TriggerEmulatorHelper {
public:
    TriggerEmulatorHelper();
	void handleEmulatorCallback(const MultiChannelStateSequence& mcss, int stateIndex);
};

void initTriggerEmulatorLogic();

int getPreviousIndex(const int currentIndex, const int size);
bool needEvent(const int currentIndex, const MultiChannelStateSequence& mcss, int channelIndex);

