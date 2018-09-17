/**
 * @file board.cpp
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "boards.h"
#include "engine.h"
#include "adc_math.h"
#include "engine_sniffer.h"

static LoggingWithStorage logger("simulator board");
extern engine_configuration_s *engineConfiguration;
extern Engine *engine;

int getAdcValue(const char *msg, int hwChannel) {
	return engine->engineState.mockAdcState.getMockAdcValue(hwChannel);
}
