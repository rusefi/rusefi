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

static Logging *logger;

float AccelEnrichmemnt::getDelta() {
	return cb.maxValue(cb.getSize());
}

floatms_t AccelEnrichmemnt::getTpsEnrichment(DECLARE_ENGINE_PARAMETER_F) {
	float d = getDelta();
	if (d > engineConfiguration->tpsAccelEnrichmentThreshold) {
		return d * engineConfiguration->tpsAccelEnrichmentMultiplier;
	}
	return 0;
}

float AccelEnrichmemnt::getMapEnrichment(DECLARE_ENGINE_PARAMETER_F) {
	float d = getDelta();
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
	if (len < 1) {
		scheduleMsg(logger, "Length shoold be positive");
		return;
	}
	engine->tpsAccelEnrichment.cb.setSize(len);
	accelInfo();
}

static void setMapAccelLen(int len) {
	if (len < 1) {
		scheduleMsg(logger, "Length shoold be positive");
		return;
	}
	engine->mapAccelEnrichment.cb.setSize(len);
	accelInfo();
}

void initAccelEnrichment(Logging *sharedLogger) {
	logger = sharedLogger;
	addConsoleActionI("set_tps_accel_len", setTpsAccelLen);
	addConsoleActionF("set_tps_accel_threshold", setTpsAccelThr);
	addConsoleActionF("set_tps_accel_multiplier", setTpsAccelMult);

	addConsoleActionI("set_map_accel_len", setMapAccelLen);
	addConsoleActionF("set_map_accel_threshold", setMapAccelThr);
	addConsoleActionF("set_map_accel_multiplier", setMapAccelMult);
	addConsoleActionF("set_decel_threshold", setDecelThr);
	addConsoleActionF("set_decel_multiplier", setDecelMult);
	addConsoleAction("accelinfo", accelInfo);

	setMapAccelLen(engineConfiguration->mapAccelLength);
	setTpsAccelLen(engineConfiguration->tpsAccelLength);
}
#endif /* ! EFI_UNIT_TEST */
