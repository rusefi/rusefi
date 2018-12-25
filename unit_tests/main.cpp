/**
 * @file main.cpp
 * @file First step towards unit-testing rusEfi algorithms
 *
 * @author Andrey Belomutskiy (c) 2012-2018
 */


#include <stdlib.h>

#include "global.h"
#include "test_accel_enrichment.h"
#include "test_interpolation_3d.h"
#include "test_find_index.h"
#include "test_sensors.h"
#include "test_speed_density.h"

#include "test_fuel_map.h"
#include "fuel_math.h"
#include "test_fuelCut.h"
#include "test_logic_expression.h"
#include "test_pid_auto.h"
#include "engine_configuration.h"

#include "test_idle_controller.h"
#include "afm2mapConverter.h"
#include "test_signal_executor.h"
#include "trigger_central.h"
#include "test_startOfCrankingPrimingPulse.h"
#include "test_fasterEngineSpinningUp.h"
#include "test_util.h"
#include "map_resize.h"
#include "engine_math.h"
#include "test_engine_math.h"
#include "test_trigger_decoder.h"
#include "test_table.h"

typedef int32_t         msg_t;

#include "hal_streams.h"
#include "memstreams.h"

int timeNowUs = 0;

efitimeus_t getTimeNowUs(void) {
	return timeNowUs;
}

efitick_t getTimeNowNt(void) {
	return getTimeNowUs() * US_TO_NT_MULTIPLIER;
}

LoggingWithStorage sharedLogger("main");

extern int revolutionCounterSinceBootForUnitTest;

int getRevolutionCounter(void) {
	return revolutionCounterSinceBootForUnitTest;
}
extern bool printTriggerDebug;

int main(void) {
//	printTriggerDebug = true;

	testMisc();
	testDifferentInjectionModes();
	/**
	 * PID
	 */
	testPidAuto();
	testPidController();
	/**
	 * Larger-scale engine control
	 */
	testMissedSpark299();
	testSparkReverseOrderBug319();
	testFuelSchedulerBug299smallAndLarge();
	testFuelSchedulerBug299smallAndMedium();
	testLogicExpressions(); // fsio
	testPlainCrankingWithoutAdvancedFeatures();
	testStartOfCrankingPrimingPulse();
	testFasterEngineSpinningUp();
	testGpsParser();
	testFuelMap();
	testFuelCut();
	testEngineMath();
	testIgnitionPlanning();
	testSensors();
	/**
	 * Data structures and general methods
	 */
	testCyclicBuffer();
	testCrc();
	testOverflow64Counter();
	testFindIndex();
	testInterpolate3d();
	testInterpolate2d();
	test_table();
	testSetTableValue();
	testHistogram();
	testFLStack();

	testSignalExecutor();
	testPwmGenerator();


	/**
	 * Business logic tests
	 */
	testMalfunctionCentral();

	testConsoleLogic();

	testAngleResolver();

	testPinHelper();

	testAccelEnrichment();

	testSpeedDensity();

	testMenuTree();
	testMafLookup();
	testIgnitionMapGenerator();
	testMafFuelMath();

	testTriggerDecoder();

	//	resizeMap();
	printf("Success 20181206\r\n");
	printAllTriggers();
//	printConvertedTable();


	return EXIT_SUCCESS;
}

void print(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void initLogging(LoggingWithStorage *logging, const char *name) {
}

void scheduleMsg(Logging *logging, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	printf("\r\n");
}
