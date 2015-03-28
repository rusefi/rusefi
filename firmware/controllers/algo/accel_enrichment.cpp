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

static AccelEnrichmemnt instance;
static Logging *logger;

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)

static void accelInfo() {
	scheduleMsg(logger, "accel length=%d", instance.cb.getSize());
	scheduleMsg(logger, "accel th=%f/mult=%f", engineConfiguration->accelEnrichmentThreshold, engineConfiguration->accelEnrichmentMultiplier);
	scheduleMsg(logger, "decel th=%f/mult=%f", engineConfiguration->decelEnrichmentThreshold, engineConfiguration->decelEnrichmentMultiplier);
}

static void setAccelThr(float value) {
	engineConfiguration->accelEnrichmentThreshold = value;
	accelInfo();
}

static void setAccelMult(float value) {
	engineConfiguration->accelEnrichmentMultiplier = value;
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

static void setAccelLen(int len) {
	instance.cb.setSize(len);
	accelInfo();
}

void initAccelEnrichment(Logging *sharedLogger) {
	logger = sharedLogger;
	addConsoleActionF("set_accel_th", setAccelThr);
	addConsoleActionF("set_accel_mult", setAccelMult);
	addConsoleActionF("set_decel_th", setDecelThr);
	addConsoleActionF("set_decel_mult", setDecelMult);
	addConsoleActionI("set_accel_len", setAccelLen);
	addConsoleAction("accelinfo", accelInfo);

	setAccelLen(engineConfiguration->accelLength);

}
#endif /* ! EFI_UNIT_TEST */

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

float AccelEnrichmemnt::getEnrichment(DECLARE_ENGINE_PARAMETER_F) {
	float d = cb.maxValue(cb.getSize());
	if (d > engineConfiguration->accelEnrichmentThreshold) {
		return d * engineConfiguration->accelEnrichmentMultiplier;
	}
//	if (d < engineConfiguration->deaccelEnrichmentThreshold) {
//		return d * engineConfiguration->deaccelEnrichmentMultiplier;
//	}
	return 0;
}

void AccelEnrichmemnt::reset() {
	maxDelta = 0;
	minDelta = 0;
	delta = 0;
	currentEngineLoad = NAN;
}

void AccelEnrichmemnt::onEngineCycle(DECLARE_ENGINE_PARAMETER_F) {
	float currentEngineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);

	if (!cisnan(this->currentEngineLoad)) {
		delta = currentEngineLoad - this->currentEngineLoad;
		cb.add(delta);
		maxDelta = maxF(maxDelta, delta);
		minDelta = minF(minDelta, delta);
	}

	this->currentEngineLoad = currentEngineLoad;
}

AccelEnrichmemnt::AccelEnrichmemnt() {
	reset();
	cb.setSize(4);
//	for (int i = 0; i < MAX_ACCEL_ARRAY_SIZE; i++)
//		array[i] = 0;
	diffEnrichment = 0;
}

//float getAccelEnrichment(void) {
//	return instance.getDiffEnrichment();
//}

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

