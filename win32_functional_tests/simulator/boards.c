/**
 * @file board.c
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "boards.h"
#include "engine_configuration.h"
#include "adc_math.h"

static Logging logger;
extern engine_configuration_s *engineConfiguration;

static float fakeAdcValues[16];

int getAdcValue(int hwChannel) {
	return fakeAdcValues[hwChannel];
}

static void setVoltage(int hwChannel, float voltage) {
	scheduleMsg(&logger, "fake voltage: channel %d value %f", hwChannel, voltage);
	fakeAdcValues[hwChannel] = voltsToAdc(voltage);
}

static void setCltVoltage(float voltage) {
	setVoltage(engineConfiguration->cltAdcChannel, voltage);
}

static void setIatVoltage(float voltage) {
	setVoltage(engineConfiguration->iatAdcChannel, voltage);
}

static void setMafVoltage(float voltage) {
	setVoltage(engineConfiguration->mafAdcChannel, voltage);
}

static void setAfrVoltage(float voltage) {
	setVoltage(engineConfiguration->afrSensor.afrAdcChannel, voltage);
}

static void setTpsVoltage(float voltage) {
	setVoltage(engineConfiguration->tpsAdcChannel, voltage);
}

static void setMapVoltage(float voltage) {
	setVoltage(engineConfiguration->map.sensor.hwChannel, voltage);
}

void initFakeBoard(void) {
	initLogging(&logger, "simulator board");

	addConsoleActionF("set_mock_clt_voltage", setCltVoltage);
	addConsoleActionF("set_mock_iat_voltage", setIatVoltage);
	addConsoleActionF("set_mock_maf_voltage", setMafVoltage);
	addConsoleActionF("set_mock_afr_voltage", setAfrVoltage);
	addConsoleActionF("set_mock_tps_voltage", setTpsVoltage);
	addConsoleActionF("set_mock_map_voltage", setMapVoltage);
}


