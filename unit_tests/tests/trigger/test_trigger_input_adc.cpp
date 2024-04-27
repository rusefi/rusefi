/**
 * @file	test_trigger_input_adc.cpp
 *
 * @date Jul 24, 2021
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2023
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

static int triggerChangedRisingCnt = 0, triggerChangedFallingCnt = 0;


void setTriggerAdcMode(triggerAdcMode_t adcMode) {
	trigAdcState.curAdcMode = adcMode;
}

void onTriggerChanged(efitick_t stamp, bool isPrimary, bool isRising) {
	if (isRising)
		triggerChangedRisingCnt++;
	else
		triggerChangedFallingCnt++;

	hwHandleShaftSignal(isPrimary ? 0 : 1, isRising, stamp);
}

static void simulateTrigger(EngineTestHelper &eth, TriggerAdcDetector &trigAdcState, CsvReader &reader, float voltageDiv, float adcMaxVoltage) {
	static const float Vil = 0.3f * adcMaxVoltage;
	static const float Vih = 0.7f * adcMaxVoltage;

	efitimeus_t startUs = getTimeNowUs();

	int prevLogicValue = -1;
	while (reader.haveMore()) {
		double value = 0;
		double stamp = reader.readTimestampAndValues(&value);
		efitimeus_t stampUs = (efitick_t)(stamp * 1'000'000) + startUs;
		eth.setTimeAndInvokeEventsUs(stampUs);
		efitick_t stampNt = US2NT(stampUs);

		// convert into mcu-adc voltage
		value = minF(maxF(value / voltageDiv, 0), adcMaxVoltage);
		if (trigAdcState.curAdcMode == TRIGGER_ADC_EXTI) {
			int logicValue = 0;
			// imitate Schmitt trigger input
			if (value < Vil || value > Vih) {
				logicValue = value > Vih;
				// we need at least two values to detect an edge
				if (prevLogicValue != -1) {
//					printf("--> DIGITAL %d %d\r\n", logicValue, prevLogicValue);

					trigAdcState.digitalCallback(stampNt, true, logicValue > prevLogicValue ? true : false);
				}
				prevLogicValue = logicValue;
			}
		} else if (trigAdcState.curAdcMode == TRIGGER_ADC_ADC) {
			triggerAdcSample_t sampleValue = value * ADC_MAX_VALUE / adcMaxVoltage;
			
//			printf("--> ANALOG %d\r\n", sampleValue);

			trigAdcState.analogCallback(stampNt, sampleValue);
		}
	}
}

static void testOnCsvData(const char *fileName, int finalRpm, int risingCnt, int fallingCnt, int errCnt) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	engineConfiguration->adcVcc = 3.3f;
	engineConfiguration->analogInputDividerCoefficient = 2.0f;
	
	// we'll test on 60-2 wheel
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL_60_2);

	// we generate the data that way
	engineConfiguration->invertPrimaryTriggerSignal = true;

	ASSERT_EQ(0, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(0,  Sensor::getOrZero(SensorType::Rpm)) << "testTriggerInputAdc RPM #1 on " << fileName;

	trigAdcState.init();

	// disable weak signal detector for this test
	trigAdcState.setWeakSignal(false);

	setTriggerAdcMode(TRIGGER_ADC_ADC);

	// reset counters
	triggerChangedRisingCnt = 0; triggerChangedFallingCnt = 0;

	// skip some time to avoid conflicts with ADC sampling time correction
	eth.moveTimeForwardUs(NT2US(trigAdcState.stampCorrectionForAdc));

	CsvReader reader(1, 0);

	reader.open(fileName);
	simulateTrigger(eth, trigAdcState, reader, 2.0f, 3.3f);

	ASSERT_EQ(errCnt,  engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(risingCnt,  triggerChangedRisingCnt);
	ASSERT_EQ(fallingCnt,  triggerChangedFallingCnt);
	ASSERT_NEAR(finalRpm,  Sensor::getOrZero(SensorType::Rpm), 0.5f) << "testTriggerInputAdc RPM #2 on " << fileName;
}

/*
TEST(big, testTriggerInputAdc1) {
	printf("====================================================================================== testTriggerInputAdc 1\r\n");

	testOnCsvData("tests/trigger/resources/trigger_adc_1.csv", 1524, 74, 73, 0);
}



TEST(big, testTriggerInputAdc750) {
	printf("====================================================================================== testTriggerInputAdc 750\r\n");

	testOnCsvData("tests/trigger/resources/trigger_adc_750.csv", 750, 144, 144, 0);
}



TEST(big, testTriggerInputAdc1000) {
	printf("====================================================================================== testTriggerInputAdc 1000\r\n");

	testOnCsvData("tests/trigger/resources/trigger_adc_1000.csv", 1000, 194, 194, 0);
}

TEST(big, testTriggerInputAdcIncrDecr) {
	printf("====================================================================================== testTriggerInputAdc 1000\r\n");

	testOnCsvData("tests/trigger/resources/trigger_adc_incr.csv", 1330, 155, 154, 0);
	testOnCsvData("tests/trigger/resources/trigger_adc_decr.csv", 419, 142, 141, 0);
}
*/

TEST(big, testTriggerInputAdcReal) {
	printf("====================================================================================== testTriggerInputAdc 1000\r\n");

	// constant low RPM
	testOnCsvData("tests/trigger/resources/trigger_adc_real1.csv", 322, 924, 924, 2);
	// accelerate and switch from analog to digital mode
	//testOnCsvData("tests/trigger/resources/trigger_adc_real2.csv", 1283, 2398, 2398, 29);
}

