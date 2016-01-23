/**
 * @file board.cpp
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "boards.h"
#include "engine_configuration.h"
#include "adc_math.h"
#include "engine_sniffer.h"

static LoggingWithStorage logger("simulator board");
extern engine_configuration_s *engineConfiguration;

MockAdcState mockAdcState;

int getAdcValue(const char *msg, int hwChannel) {
	return mockAdcState.getMockAdcValue(hwChannel);
}

static void setVoltage(int hwChannel, float voltage) {
	scheduleMsg(&logger, "fake voltage: channel %d value %f", hwChannel, voltage);
	mockAdcState.setMockVoltage(hwChannel, voltage);
}

static void setCltVoltage(float voltage) {
	setVoltage(engineConfiguration->clt.adcChannel, voltage);
}

static void setIatVoltage(float voltage) {
	setVoltage(engineConfiguration->iat.adcChannel, voltage);
}

static void setMafVoltage(float voltage) {
	setVoltage(engineConfiguration->mafAdcChannel, voltage);
}

static void setAfrVoltage(float voltage) {
	setVoltage(engineConfiguration->afr.hwChannel, voltage);
}

static void setTpsVoltage(float voltage) {
	setVoltage(engineConfiguration->tpsAdcChannel, voltage);
}

static void setMapVoltage(float voltage) {
	setVoltage(engineConfiguration->map.sensor.hwChannel, voltage);
}

static void setVBattVoltage(float voltage) {
	setVoltage(engineConfiguration->vbattAdcChannel, voltage);
}

void initFakeBoard(void) {

	addConsoleActionF("set_mock_clt_voltage", setCltVoltage);
	addConsoleActionF("set_mock_iat_voltage", setIatVoltage);
	addConsoleActionF("set_mock_maf_voltage", setMafVoltage);
	addConsoleActionF("set_mock_afr_voltage", setAfrVoltage);
	addConsoleActionF("set_mock_tps_voltage", setTpsVoltage);
	addConsoleActionF("set_mock_map_voltage", setMapVoltage);
	addConsoleActionF("set_mock_vbatt_voltage", setVBattVoltage);
}


