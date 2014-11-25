/*
 * @brief dead code
 *
 *
 * map_multiplier.cpp
 *
 * @date Jul 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "map_multiplier_thread.h"
#include "map_adjuster.h"
#include "rpm_calculator.h"
#include "main_trigger_callback.h"
#include "allsensors.h"
#include "engine_math.h"
#include "engine.h"

extern engine_configuration_s *engineConfiguration;

static Logging logger;

EXTERN_ENGINE;

static THD_WORKING_AREA(maThreadStack, UTILITY_THREAD_STACK_SIZE);

static void mapCallback(int rpm, float key, float value) {
	Logging *logging = &logger;
	appendPrintf(logging, "msg%s", DELIMETER);

	appendPrintf(logging, "map_adjusted: ");
	appendPrintf(logging, "%d", rpm);
	appendPrintf(logging, " ");
	appendPrintf(logging, "%d", 100 * key);
	appendPrintf(logging, " ");
	appendPrintf(logging, "%d", 100 * value);

	appendMsgPostfix(logging);
	scheduleLogging(logging);
}

static int timeAtNotRunning = 0;

static int isNewState = TRUE;

static void maThread(int param) {
	chRegSetThreadName("map adjustment");

	while (TRUE) {
		chThdSleepMilliseconds(100);

		systime_t now = chTimeNow();
//todo?		if (!isRunning()) {
//			timeAtNotRunning = now;
//			continue;
//		}

		int wasNotRunningRecently = overflowDiff(now, timeAtNotRunning) < 60 * CH_FREQUENCY;
		if (!wasNotRunningRecently)
			continue;
		if (isNewState)
			scheduleMsg(&logger, "starting fuel map adjustment at %d", now);
		isNewState = FALSE;

		// ideally this should be atomic, but hopefully it's good enough
		int rpm = getRpm();
		float load = getEngineLoadT(PASS_ENGINE_PARAMETER);
		float afr = getAfr();

		addAfr(rpm, load, afr);
		int total = runMapAdjustments(mapCallback);
		if (total > 0) {
//			scheduleSimpleMsg(&logger, "map adjusted for maf ", 100 * key);
		}
	}
}

void initMapAdjusterThread(void) {
	initLogging(&logger, "Map self learning thread");

	initMapAdjuster();

	chThdCreateStatic(maThreadStack, sizeof(maThreadStack), NORMALPRIO, (tfunc_t)maThread, NULL);
}
