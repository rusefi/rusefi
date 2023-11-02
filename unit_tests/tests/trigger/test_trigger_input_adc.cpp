/**
 * @file	test_trigger_input_adc.cpp
 *
 * @date Jul 24, 2021
 */

#include "pch.h"
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

void setTriggerAdcMode(triggerAdcMode_t adcMode) {
	trigAdcState.curAdcMode = adcMode;
}

void onTriggerChanged(efitick_t stamp, bool isPrimary, bool isRising) {
	printf("*\r\n");
}

static void simulateTrigger(TriggerAdcDetector &state, CsvReader &reader, float voltageDiv, float adcMaxVoltage) {
	static const float Vil = 0.3f * adcMaxVoltage;
	static const float Vih = 0.7f * adcMaxVoltage;

	int prevLogicValue = -1;
	while (reader.haveMore()) {
		double value = 0;
		double stamp = reader.readTimestampAndValues(&value);
		efitick_t stampUs = (efitick_t)(stamp * 1'000'000);
//		printf("--simulateTrigger %lld %f\r\n", stamp, (float)value);
		// convert into mcu-adc voltage
		value = minF(maxF(value / voltageDiv, 0), adcMaxVoltage);
		if (state.curAdcMode == TRIGGER_ADC_EXTI) {
			int logicValue = 0;
			// imitate Schmitt trigger input
			if (value < Vil || value > Vih) {
				logicValue = value > Vih;
				// we need at least two values to detect an edge
				if (prevLogicValue != -1) {
//					printf("--> DIGITAL %d %d\r\n", logicValue, prevLogicValue);

					state.digitalCallback(stampUs, true, logicValue > prevLogicValue ? true : false);
				}
				prevLogicValue = logicValue;
			}
		} else if (state.curAdcMode == TRIGGER_ADC_ADC) {
			triggerAdcSample_t sampleValue = value * ADC_MAX_VALUE / adcMaxVoltage;
			
//			printf("--> ANALOG %d\r\n", sampleValue);

			state.analogCallback(stampUs, sampleValue);
		}
	}
}

TEST(big, testTriggerInputAdc) {
	printf("====================================================================================== testTriggerInputAdc\r\n");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	engineConfiguration->adcVcc = 3.3f;
	engineConfiguration->analogInputDividerCoefficient = 2.0f;
	
	// we'll test on 60-2 wheel
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	ASSERT_EQ(0, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(0,  Sensor::getOrZero(SensorType::Rpm)) << "testTriggerInputAdc RPM #1";

	trigAdcState.init();
	setTriggerAdcMode(TRIGGER_ADC_ADC);

	CsvReader reader(1, 0);

	reader.open("tests/trigger/resources/trigger_adc_1.csv");
	simulateTrigger(trigAdcState, reader, 2.0f, 3.3f);

	ASSERT_EQ(0,  engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(0,  Sensor::getOrZero(SensorType::Rpm)) << "testTriggerInputAdc RPM #2";
}

