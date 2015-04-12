/**
 * @file    accel_enrichment.cpp
 * @brief   Acceleration enrichment calculator
 *
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "trigger_central.h"
#include "accel_enrichment.h"
#include "engine_state.h"
#include "engine_math.h"
#include "signal_executor.h"

EXTERN_ENGINE
;

//#if EFI_PROD_CODE
//static THD_WORKING_AREA(aeThreadStack, UTILITY_THREAD_STACK_SIZE);
//#endif

static Logging *logger;

void AccelEnrichmemnt::updateDiffEnrichment(engine_configuration_s *engineConfiguration, float engineLoad) {
//	for (int i = 3; i == 1; i--)
//		array[i] = array[i - 1];
//
//	array[0] = engineLoad;
//
//	diffEnrichment = ((3 * (array[0] - array[1]) + (array[2] - array[3])) / 4)
//			* (engineConfiguration->diffLoadEnrichmentCoef);
}

//float AccelEnrichmemnt::getDiffEnrichment() {
//	return diffEnrichment;
//}

floatms_t AccelEnrichmemnt::getTpsEnrichment(DECLARE_ENGINE_PARAMETER_F) {
	float d = cb.maxValue(cb.getSize());
	if (d > engineConfiguration->tpsAccelEnrichmentThreshold) {
		return d * engineConfiguration->tpsAccelEnrichmentMultiplier;
	}
	return 0;

}

float AccelEnrichmemnt::getMapEnrichment(DECLARE_ENGINE_PARAMETER_F) {
	float d = cb.maxValue(cb.getSize());
	if (d > engineConfiguration->mapAccelEnrichmentThreshold) {
		return d * engineConfiguration->mapAccelEnrichmentMultiplier;
	}
//	if (d < engineConfiguration->deaccelEnrichmentThreshold) {
//		return d * engineConfiguration->deaccelEnrichmentMultiplier;
//	}
	return 0;
}

void AccelEnrichmemnt::reset() {
	delta = 0;
	currentValue = NAN;
}

void AccelEnrichmemnt::onNewValue(float currentValue DECLARE_ENGINE_PARAMETER_S) {
	if (!cisnan(this->currentValue)) {
		delta = currentValue - this->currentValue;
		FuelSchedule *fs = &engine->engineConfiguration2->injectionEvents;
		cb.add(delta * fs->eventsCount);
	}

	this->currentValue = currentValue;
}

void AccelEnrichmemnt::onEngineCycleTps(DECLARE_ENGINE_PARAMETER_F) {
	onNewValue(getTPS(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER);
}

void AccelEnrichmemnt::onEngineCycle(DECLARE_ENGINE_PARAMETER_F) {
	onNewValue(getEngineLoadT(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER);
}

AccelEnrichmemnt::AccelEnrichmemnt() {
	reset();
	cb.setSize(4);
}

//#if EFI_PROD_CODE
//
//static msg_t DiffEnrichmentThread(int param) {
//	chRegSetThreadName("Diff Enrichment");
//	while (TRUE) {
//		instance.updateDiffEnrichment(engineConfiguration, getEngineLoadT(PASS_ENGINE_PARAMETER));
//		chThdSleepMilliseconds(100);
//	}
//#if defined __GNUC__
//	return -1;
//#endif
//}

//void initDiffEnrichment(void) {
//	chThdCreateStatic(aeThreadStack, sizeof(aeThreadStack), LOWPRIO, (tfunc_t) DiffEnrichmentThread, NULL);
//}

//#endif


#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)

static void accelInfo() {
//	scheduleMsg(logger, "MAP accel length=%d", mapInstance.cb.getSize());
	scheduleMsg(logger, "MAP accel th=%f/mult=%f", engineConfiguration->mapAccelEnrichmentThreshold, engineConfiguration->mapAccelEnrichmentMultiplier);
	scheduleMsg(logger, "MAP decel th=%f/mult=%f", engineConfiguration->decelEnrichmentThreshold, engineConfiguration->decelEnrichmentMultiplier);

//	scheduleMsg(logger, "TPS accel length=%d", tpsInstance.cb.getSize());
	scheduleMsg(logger, "TPS accel th=%f/mult=%f", engineConfiguration->tpsAccelEnrichmentThreshold, engineConfiguration->tpsAccelEnrichmentMultiplier);
}

static void setMapAccelThr(float value) {
	engineConfiguration->mapAccelEnrichmentThreshold = value;
	accelInfo();
}

static void setMapAccelMult(float value) {
	engineConfiguration->mapAccelEnrichmentMultiplier = value;
	accelInfo();
}

static void setTpsAccelThr(float value) {
	engineConfiguration->tpsAccelEnrichmentThreshold = value;
	accelInfo();
}

static void setTpsAccelMult(float value) {
	engineConfiguration->tpsAccelEnrichmentMultiplier = value;
	accelInfo();
}

static void setDecelThr(float value) {
	engineConfiguration->decelEnrichmentThreshold = value;
	accelInfo();
}

static void setDecelMult(float value) {
	engineConfiguration->decelEnrichmentMultiplier = value;
	accelInfo();
}

static void setTpsAccelLen(int len) {
	engine->tpsAccelEnrichment.cb.setSize(len);
	accelInfo();
}

static void setMapAccelLen(int len) {
	engine->mapAccelEnrichment.cb.setSize(len);
	accelInfo();
}

void initAccelEnrichment(Logging *sharedLogger) {
	logger = sharedLogger;
	addConsoleActionI("set_tps_accel_len", setTpsAccelLen);
	addConsoleActionF("set_tps_accel_threshold", setTpsAccelThr);
	addConsoleActionF("set_tps_ccel_multiplier", setTpsAccelMult);

	addConsoleActionI("set_map_accel_len", setMapAccelLen);
	addConsoleActionF("set_map_accel_threshold", setMapAccelThr);
	addConsoleActionF("set_map_ccel_multiplier", setMapAccelMult);
	addConsoleActionF("set_decel_threshold", setDecelThr);
	addConsoleActionF("set_decel_multiplier", setDecelMult);
	addConsoleAction("accelinfo", accelInfo);

	setMapAccelLen(engineConfiguration->mapAccelLength);
	setTpsAccelLen(engineConfiguration->tpsAccelLength);
}
#endif /* ! EFI_UNIT_TEST */
