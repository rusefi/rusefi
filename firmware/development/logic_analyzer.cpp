/**
 * @file	logic_analyzer.cpp
 * @brief	Initialization of Input Capture pins used for rusEfi console sniffer
 *
 * This file is responsible for sniffing of external digital signals and registering
 * these digital events in WaveChart used by the Engine Sniffer tab of rusEfi Console.
 *
 * this is rusEfi build-in logic analyzer
 *
 * @date Jan 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "logic_analyzer.h"

#include "eficonsole.h"
#include "trigger_central.h"
#include "os_util.h"
#include "rpm_calculator.h"
#include "engine_sniffer.h"
#include "digital_input_exti.h"

#if EFI_LOGIC_ANALYZER

#define CHART_RESET_DELAY 1

/**
 * Difference between current 1st trigger event and previous 1st trigger event.
 */
static volatile uint32_t engineCycleDurationUs;
static volatile efitimeus_t previousEngineCycleTimeUs = 0;

class WaveReader {
public:
	void onFallEvent();

	ioline_t line = 0;

	int laIndex;
	volatile int fallEventCounter = 0;
	volatile int riseEventCounter = 0;

	int currentRevolutionCounter = 0;

	/**
	 * Total ON time during last engine cycle
	 */
	efitimeus_t prevTotalOnTimeUs = 0;

	efitimeus_t totalOnTimeAccumulatorUs = 0;

	volatile efitimeus_t lastActivityTimeUs = 0; // timestamp in microseconds ticks
	/**
	 * time of signal fall event, in microseconds
	 */
	volatile efitimeus_t periodEventTimeUs = 0;
	volatile efitimeus_t widthEventTimeUs = 0; // time of signal rise in microseconds

	volatile efitimeus_t signalPeriodUs = 0; // period between two signal rises in microseconds

	/**
	 * offset from engine cycle start in microseconds
	 */
	volatile efitimeus_t waveOffsetUs = 0;
	volatile efitimeus_t last_wave_low_widthUs = 0; // time period in systimer ticks
	volatile efitimeus_t last_wave_high_widthUs = 0; // time period in systimer ticks
};

static WaveReader readers[LOGIC_ANALYZER_CHANNEL_COUNT];

static void riseCallback(WaveReader *reader) {
	efitick_t nowUs = getTimeNowUs();
	reader->riseEventCounter++;
	reader->lastActivityTimeUs = nowUs;
	assertIsrContext(ObdCode::CUSTOM_ERR_6670);
	addEngineSnifferLogicAnalyzerEvent(reader->laIndex, true);

	uint32_t width = nowUs - reader->periodEventTimeUs;
	reader->last_wave_low_widthUs = width;

	reader->signalPeriodUs = nowUs - reader->widthEventTimeUs;
	reader->widthEventTimeUs = nowUs;
}

void WaveReader::onFallEvent() {
	efitick_t nowUs = getTimeNowUs();
	fallEventCounter++;
	lastActivityTimeUs = nowUs;
	assertIsrContext(ObdCode::CUSTOM_ERR_6670);
	addEngineSnifferLogicAnalyzerEvent(laIndex, false);

	efitick_t width = nowUs - widthEventTimeUs;
	last_wave_high_widthUs = width;

#if EFI_SHAFT_POSITION_INPUT
	int revolutionCounter = getRevolutionCounter();
#else
	int revolutionCounter = 0;
#endif

	totalOnTimeAccumulatorUs += width;
	if (currentRevolutionCounter != revolutionCounter) {
		/**
		 * We are here in case of a new engine cycle
		 */
		currentRevolutionCounter = revolutionCounter;
		prevTotalOnTimeUs = totalOnTimeAccumulatorUs;
		totalOnTimeAccumulatorUs = 0;

		waveOffsetUs = nowUs - previousEngineCycleTimeUs;
	}

	periodEventTimeUs = nowUs;
}

void logicAnalyzerCallback(void* arg, efitick_t /*stamp*/) {
	WaveReader* instance = reinterpret_cast<WaveReader*>(arg);

	bool rise = palReadLine(instance->line) == PAL_HIGH;

	if (rise) {
		riseCallback(instance);
	} else {
		instance->onFallEvent();
	}
}

static void initWave(size_t index) {
	brain_pin_e brainPin = engineConfiguration->logicAnalyzerPins[index];

	efiAssertVoid(ObdCode::CUSTOM_ERR_6655, index < efi::size(readers), "too many ICUs");
	WaveReader *reader = &readers[index];

	if (!isBrainPinValid(brainPin)) {
		/**
		 *  in case we are running, and we select none for a channel that was running, 
		 *  this way we ensure that we do not get false report from that channel 
		 **/
		reader->line = 0;
		return;
	}

	reader->laIndex = index;

	reader->line = PAL_LINE(getHwPort("logic", brainPin), getHwPin("logic", brainPin));

	efiExtiEnablePin("logic", brainPin, PAL_EVENT_MODE_BOTH_EDGES, logicAnalyzerCallback, (void*)reader);

	efiPrintf("wave%d input on %s", index, hwPortname(brainPin));
}

void waTriggerEventListener(trigger_event_e ckpSignalType, uint32_t index, efitick_t edgeTimestamp) {
	(void)ckpSignalType;
	if (index != 0) {
		return;
	}

	efitimeus_t nowUs = NT2US(edgeTimestamp);
	engineCycleDurationUs = nowUs - previousEngineCycleTimeUs;
	previousEngineCycleTimeUs = nowUs;
}

static float getSignalOnTime(int index) {
	WaveReader& reader = readers[index];

	if (getTimeNowUs() - reader.lastActivityTimeUs > 4 * US_PER_SECOND) {
		return 0.0f; // dwell time has expired
	}
	return reader.last_wave_high_widthUs / 1000.0f;
}

static efitimeus_t getWaveOffset(int index) {
	return readers[index].waveOffsetUs;
}

static float getSignalPeriodMs(int index) {
	return readers[index].signalPeriodUs / 1000.0f;
}

static void reportWave(Logging *logging, int index) {
	if (readers[index].line == 0) {
		return;
	}

//	int counter = getEventCounter(index);
//	debugInt2(logging, "ev", index, counter);

	float dwellMs = getSignalOnTime(index);
	float periodMs = getSignalPeriodMs(index);

	logging->appendPrintf("duty%d%s", index, LOG_DELIMITER);
	logging->appendFloat(100.0f * dwellMs / periodMs, 2);
	logging->appendPrintf("%s", LOG_DELIMITER);

	/**
	 * that's the ON time of the LAST signal
	 */
	logging->appendPrintf("dwell%d%s", index, LOG_DELIMITER);
	logging->appendFloat(dwellMs, 2);
	logging->appendPrintf("%s", LOG_DELIMITER);

	/**
	 * that's the total ON time during the previous engine cycle
	 */
	logging->appendPrintf("total_dwell%d%s", index, LOG_DELIMITER);
	logging->appendFloat(readers[index].prevTotalOnTimeUs / 1000.0f, 2);
	logging->appendPrintf("%s", LOG_DELIMITER);

	logging->appendPrintf("period%d%s", index, LOG_DELIMITER);
	logging->appendFloat(periodMs, 2);
	logging->appendPrintf("%s", LOG_DELIMITER);

	efitimeus_t offsetUs = getWaveOffset(index);
	int rpm = Sensor::getOrZero(SensorType::Rpm);
	if (rpm != 0) {
		float oneDegreeUs = getOneDegreeTimeUs(rpm);

		logging->appendPrintf("advance%d%s", index, LOG_DELIMITER);
		float angle = (offsetUs / oneDegreeUs) - tdcPosition();
		fixAngle(angle, "waveAn", ObdCode::CUSTOM_ERR_6564);
		logging->appendFloat(angle, 3);
		logging->appendPrintf("%s", LOG_DELIMITER);
	}
}

void printWave(Logging *logging) {
	reportWave(logging, 0);
	reportWave(logging, 1);
}

void showWaveInfo(void) {
	efiPrintf("logic input #1: %d/%d", readers[0].fallEventCounter, readers[0].riseEventCounter);
}

void initWaveAnalyzer() {
	if (hasFirmwareError()) {
		return;
	}

	addConsoleAction("waveinfo", showWaveInfo);
}

void startLogicAnalyzerPins() {
	for (size_t index = 0; index < LOGIC_ANALYZER_CHANNEL_COUNT; index++) {
		initWave(index);
	}
}

void stopLogicAnalyzerPins() {
	for (int index = 0; index < LOGIC_ANALYZER_CHANNEL_COUNT; index++) {
		brain_pin_e brainPin = activeConfiguration.logicAnalyzerPins[index];

		if (isBrainPinValid(brainPin)) {
			efiExtiDisablePin(brainPin);
		}
	}
}

template <typename TFreq>
static void getChannelFreqAndDuty(int index, float& duty, TFreq& freq) {
	if (readers[index].line == 0) {
		duty = 0.0;
		freq = 0;
	} else {
		float high = getSignalOnTime(index);
		float period = getSignalPeriodMs(index);

		if (period != 0) {

			duty = (high * 1000.0f) /(period * 10.0f);
			freq = (int)(1 / (period / 1000.0f));
		} else {		
			duty = 0.0;
			freq = 0;
		}
	}
}

void reportLogicAnalyzerToTS() {
#if EFI_TUNER_STUDIO
	getChannelFreqAndDuty(0, engine->outputChannels.debugFloatField1, engine->outputChannels.debugIntField1);
	getChannelFreqAndDuty(1, engine->outputChannels.debugFloatField2, engine->outputChannels.debugIntField2);
	getChannelFreqAndDuty(2, engine->outputChannels.debugFloatField3, engine->outputChannels.debugIntField3);
	getChannelFreqAndDuty(3, engine->outputChannels.debugFloatField4, engine->outputChannels.debugIntField4);
#endif	
}

#endif /* EFI_LOGIC_ANALYZER */
