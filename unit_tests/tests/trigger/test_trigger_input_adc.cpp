/**
 * @file	test_trigger_input_adc.cpp
 *
 * @date Jul 24, 2021
 */

#include "global.h"
#include "engine_test_helper.h"
#include "trigger_decoder.h"
#include "engine_math.h"
#include "allsensors.h"
#include "rpm_calculator.h"
#include "event_queue.h"
#include "trigger_central.h"
#include "main_trigger_callback.h"
#include "engine.h"
#include "advance_map.h"
#include "speed_density.h"
#include "fuel_math.h"
#include "spark_logic.h"
#include "trigger_universal.h"

#include "trigger_input_adc.h"
#include "logicdata_csv_reader.h"


extern TriggerAdcDetector trigAdcState;

static EngineTestHelper *eth = nullptr;

void setTriggerAdcMode(triggerAdcMode_t adcMode) {
	trigAdcState.curAdcMode = adcMode;
	
	if (trigAdcState.curAdcMode == TRIGGER_ADC_ADC)
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! MODE = ANALOG!\r\n");
	else if (trigAdcState.curAdcMode == TRIGGER_ADC_EXTI)
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! MODE = DIGITAL!\r\n");
}

void onTriggerChanged(efitick_t stamp, bool isPrimary, bool isRising) {
	Engine *engine = &eth->engine;
	EXPAND_Engine;

	//!!!!!!!!!!!
	static int cnt = 0;
	static efitick_t prevstamp = 0;
	printf("[%d] %lld\r\n", cnt++, stamp - prevstamp);
	prevstamp = stamp;
	
	hwHandleShaftSignal(isPrimary ? 0 : 1, isRising, stamp PASS_ENGINE_PARAMETER_SUFFIX);
}

static void simulateTrigger(TriggerAdcDetector &trigAdcState, CsvReader &reader, float voltageDiv, float adcMaxVoltage) {
	static const float Vil = 0.3f * adcMaxVoltage;
	static const float Vih = 0.7f * adcMaxVoltage;

	int prevLogicValue = -1;
	while (reader.haveMore()) {
		double value = 0;
		double stamp = reader.readTimestampAndValues(&value);
		long stampUs = (long)(stamp * 1'000'000);
		efitick_t stampNt = getTimeNowNt();
		
		//printf("-- %ld %f\r\n", stamp, (float)value);
		
		eth->setTimeAndInvokeEventsUs(stampUs);
		
		// convert into mcu-adc voltage
		value = minF(maxF(value / voltageDiv, 0), adcMaxVoltage);
		if (trigAdcState.curAdcMode == TRIGGER_ADC_EXTI) {
			int logicValue = 0;
			// imitate Schmitt trigger input
			if (value < Vil || value > Vih) {
				logicValue = value > Vih;
				// we need at least two values to detect an edge
				if (prevLogicValue != -1 && logicValue != prevLogicValue) {
					//printf("--> DIGITAL %d %d\r\n", logicValue, prevLogicValue);

					trigAdcState.digitalCallback(stampNt, true, logicValue > prevLogicValue ? true : false);
				}
				prevLogicValue = logicValue;
			}
		} else if (trigAdcState.curAdcMode == TRIGGER_ADC_ADC) {
			triggerAdcSample_t sampleValue = value * ADC_MAX_VALUE / adcMaxVoltage;
			
			//printf("--> ANALOG %d\r\n", sampleValue);

			trigAdcState.analogCallback(stampNt, sampleValue);
		}
	}
}

template<typename Func>
static void testTriggerSignal(const char *testName, const char *fName, int numTriggerErrors, int numRevolutions, int rpm, Func&& additionalSetup) {
	EngineTestHelper _eth(TEST_ENGINE, std::unordered_map<SensorType, float>{});
	eth = &_eth;
	Engine *engine = &eth->engine;
	EXPAND_Engine;
	
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	engineConfiguration->invertPrimaryTriggerSignal = true;

	engineConfiguration->adcVcc = 3.3f;
	engineConfiguration->analogInputDividerCoefficient = 2.0f;
	
	additionalSetup();
	
	// we'll test on 60-2 wheel
	eth->setTriggerType(TT_TOOTHED_WHEEL_60_2 PASS_ENGINE_PARAMETER_SUFFIX);

	ASSERT_EQ(0, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(0, GET_RPM());

	CsvReader reader(1, 0);
	const int indices[2] = { 0, 1 };

	trigAdcState.init(PASS_ENGINE_PARAMETER_SIGNATURE);
	setTriggerAdcMode(TRIGGER_ADC_ADC);
		
	reader.open(fName, indices);
	simulateTrigger(trigAdcState, reader, 2.0f, 3.3f);
	
	ASSERT_EQ(numTriggerErrors, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(numRevolutions, engine->triggerCentral.triggerState.getTotalRevolutionCounter());
	ASSERT_EQ(rpm, GET_RPM()) << testName;
}


TEST(big, testTriggerInputAdc) {
	printf("====================================================================================== testTriggerInputAdc\r\n");
#if 0	
	// low signal 7500 RPM, analog mode test
	testTriggerSignal("testTriggerInputAdc RPM #1", "tests/trigger/resources/trigger_adc_750.csv", 0, 2, 750, []() { });
	
	// strong signal 1000 RPM, digital mode test
	testTriggerSignal("testTriggerInputAdc RPM #2", "tests/trigger/resources/trigger_adc_1000.csv", 0, 2, 1000, []() { });
	
	// RPM acceleration test (analog mode)
	testTriggerSignal("testTriggerInputAdc RPM #3", "tests/trigger/resources/trigger_adc_incr.csv", 0, 3, 1329, []() { });
	
	// ditital-to-analog deceleration test (teeth: 0-4 = analog, 5-183 = digital, 184-226 = analog)
	testTriggerSignal("testTriggerInputAdc RPM #4", "tests/trigger/resources/trigger_adc_decr.csv", 0, 2, 420, []() { });
	
	// real oscilloscope-recorded signal
	testTriggerSignal("testTriggerInputAdc RPM #5", "tests/trigger/resources/trigger_adc_real1.csv", 0, 15, 322, []() { });
	// real oscilloscope-recorded signal
	testTriggerSignal("testTriggerInputAdc RPM #6", "tests/trigger/resources/trigger_adc_real2.csv", 0, 36, 322, []() { eth->engine.engineConfiguration->useNoiselessTriggerDecoder = true; });
	// real oscilloscope-recorded signal (bad distorted signal)
	testTriggerSignal("testTriggerInputAdc RPM #7", "tests/trigger/resources/trigger_adc_real3.csv", 7, 28, 322, []() { eth->engine.engineConfiguration->useNoiselessTriggerDecoder = true; });
#endif
	// real oscilloscope-recorded signal (bad distorted signal)
	testTriggerSignal("testTriggerInputAdc RPM #8", "tests/trigger/resources/trigger_adc_real6.csv", 0, 9, 322, []() { eth->engine.engineConfiguration->useNoiselessTriggerDecoder = true; });
}

