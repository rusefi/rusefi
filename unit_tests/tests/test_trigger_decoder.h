/**
 * @file	test_trigger_decoder.h
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TEST_TRIGGER_DECODER_H_
#define TEST_TRIGGER_DECODER_H_

#include "engine_test_helper.h"

void testTriggerDecoder2(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty);
void setupSimpleTestEngineWithMafAndTT_ONE_trigger(EngineTestHelper *eth, injection_mode_e injMode = IM_BATCH);

void assertRpm(const char *msg, int expectedRpm DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* TEST_TRIGGER_DECODER_H_ */
