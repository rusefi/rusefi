/**
 * @file max31855.c
 * @brief MAX31855 Thermocouple-to-Digital Converter driver
 *
 *
 * http://datasheets.maximintegrated.com/en/ds/MAX31855.pdf
 *
 * @date Sep 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "max31855.h"
#include "pin_repository.h"

static Logging logger;

static void showEgtInfo(void) {
	for (int i = 0; i < MAX31855_CS_COUNT; i++) {
//		if (boardConfiguration->max31855_cs[i] != GPIO_NONE) {
//			scheduleMsg(&logger, "%d ETG @ %s", i, hwPortname(boardConfiguration->max31855_cs[i]));


//		}

	}
}

void initMax31855(board_configuration_s *boardConfiguration) {
	initLogging(&logger, "EGT");

	addConsoleAction("egtinfo", showEgtInfo);

	for (int i = 0; i < MAX31855_CS_COUNT; i++) {
		if (boardConfiguration->max31855_cs[i] != GPIO_NONE) {

		}

	}

}
