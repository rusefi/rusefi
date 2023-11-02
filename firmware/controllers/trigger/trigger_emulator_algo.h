/**
 * @file trigger_emulator_algo.h
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

// 1 crank and CAMS_PER_BANK cam channels
#define NUM_EMULATOR_CHANNELS (1 + CAMS_PER_BANK)

class PwmConfig;
class MultiChannelStateSequence;

void initTriggerEmulator();
void startTriggerEmulatorPins();
void stopTriggerEmulatorPins();
void setTriggerEmulatorRPM(int value);
void onConfigurationChangeRpmEmulatorCallback(engine_configuration_s *previousConfiguration);

// Start & stop trigger emulation
void enableTriggerStimulator(bool incGlobalConfiguration = true);
void enableExternalTriggerStimulator();
void disableTriggerStimulator();

class TriggerEmulatorHelper {
public:
    TriggerEmulatorHelper();
	void handleEmulatorCallback(int channel, const MultiChannelStateSequence& mcss, int stateIndex);
};

int getPreviousIndex(const int currentIndex, const int size);
bool needEvent(const int currentIndex, const MultiChannelStateSequence& mcss, int channelIndex);

