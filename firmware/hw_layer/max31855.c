/**
 * @file max31855.c
 * @brief MAX31855 Thermocouple-to-Digital Converter driver
 *
 *
 * http://datasheets.maximintegrated.com/en/ds/MAX31855.pdf
 *
 *
 * Read-only communication over 5MHz SPI
 *
 * @date Sep 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "max31855.h"
#include "pin_repository.h"

#if EFI_MAX_31855

static Logging logger;

static void showEgtInfo(board_configuration_s *boardConfiguration) {
	for (int i = 0; i < MAX31855_CS_COUNT; i++) {
		if (boardConfiguration->max31855_cs[i] != GPIO_NONE) {
			scheduleMsg(&logger, "%d ETG @ %s", i, hwPortname(boardConfiguration->max31855_cs[i]));

		}

	}
}

void initMax31855(board_configuration_s *boardConfiguration) {
	initLogging(&logger, "EGT");

	addConsoleActionP("egtinfo", (VoidPtr) showEgtInfo, boardConfiguration);

	for (int i = 0; i < MAX31855_CS_COUNT; i++) {
		if (boardConfiguration->max31855_cs[i] != GPIO_NONE) {

		}

	}

}

#endif /* EFI_MAX_31855 */
