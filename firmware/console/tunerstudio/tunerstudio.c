/**
 * @file	tunerstudio.c
 * @brief Integration with EFI Analytics Tuner Studio software
 *
 * todo: merge this file with tunerstudio_algo.c?
 *
 * @date Aug 26, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "main.h"

#include "engine_state.h"
#include "tunerstudio.h"
#include "pin_repository.h"

#include "main_trigger_callback.h"
#include "flash_main.h"
#include "usbconsole.h"
#include "map_averaging.h"

#include "tunerstudio_algo.h"
#include "tunerstudio_configuration.h"
#include "malfunction_central.h"

#if EFI_TUNER_STUDIO

static Logging logger;

extern engine_configuration_s *engineConfiguration;

extern SerialUSBDriver SDU1;
#define CONSOLE_DEVICE &SDU1

#if EFI_TUNER_STUDIO_OVER_USB
#define ts_serail_ready() is_usb_serial_ready()
#else
#define ts_serail_ready() TRUE
static SerialConfig tsSerialConfig = { TS_SERIAL_SPEED, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };
#endif /* EFI_TUNER_STUDIO_OVER_USB */

static WORKING_AREA(TS_WORKING_AREA, UTILITY_THREAD_STACK_SIZE);

static int tsCounter = 0;
static int writeCounter = 0;

static TunerStudioWriteRequest writeRequest;

extern TunerStudioOutputChannels tsOutputChannels;

extern engine_configuration_s tsContstants;

char *constantsAsPtr = (char *) &tsContstants;

extern TunerStudioState tsState;

static void printStats(void) {
#if EFI_TUNER_STUDIO_OVER_USB
#else
	scheduleMsg(&logger, "TS RX on %s%d", portname(TS_SERIAL_RX_PORT), TS_SERIAL_RX_PIN);
	scheduleMsg(&logger, "TS TX on %s%d", portname(TS_SERIAL_TX_PORT), TS_SERIAL_TX_PIN);
#endif /* EFI_TUNER_STUDIO_OVER_USB */
	scheduleMsg(&logger, "TunerStudio total/error counter=%d/%d", tsCounter, tsState.errorCounter);
	scheduleMsg(&logger, "TunerStudio H counter=%d", tsState.queryCommandCounter);
	scheduleMsg(&logger, "TunerStudio O counter=%d size=%d", tsState.outputChannelsCommandCounter, sizeof(tsOutputChannels));
	scheduleMsg(&logger, "TunerStudio C counter=%d size=%d", tsState.readPageCommandsCounter, sizeof(tsContstants));
	scheduleMsg(&logger, "TunerStudio B counter=%d size=%d", tsState.burnCommandCounter, sizeof(tsContstants));
	scheduleMsg(&logger, "TunerStudio W counter=%d", writeCounter);
}

void tunerStudioWriteData(const uint8_t * buffer, int size) {
	chSequentialStreamWrite(TS_SERIAL_DEVICE, buffer, size);
}

void tunerStudioDebug(char *msg) {
#if EFI_TUNER_STUDIO_VERBOSE
	print("%s\r\n", msg);
	printStats();
#endif
}

/**
 * 'Write' command receives a single value at a given offset
 */
void handleValueWriteCommand(void) {
	writeCounter++;

	tunerStudioDebug("got W (Write)");

	int size = sizeof(TunerStudioWriteRequest);
	print("Reading %d\r\n", size);

	int recieved = chSequentialStreamRead(TS_SERIAL_DEVICE, (uint8_t *)&writeRequest, size);
	print("got %d\r\n", recieved);

//	unsigned char offset = writeBuffer[0];
//	unsigned char value = writeBuffer[1];
//
	print("offset %d: value=%d\r\n", writeRequest.offset, writeRequest.value);
	constantsAsPtr[writeRequest.offset] = writeRequest.value;
}

/**
 * 'Burn' command is a command to commit the changes
 */
void handleBurnCommand(void) {
	tsState.burnCommandCounter++;

	tunerStudioDebug("got B (Burn)");

	// todo: how about some multi-threading?
	memcpy(engineConfiguration, &tsContstants, sizeof(engine_configuration_s));
	writeToFlash();
}

static msg_t tsThreadEntryPoint(void *arg) {
	(void) arg;
	chRegSetThreadName("tunerstudio thread");

	int wasReady = FALSE;
	while (true) {
		int isReady = ts_serail_ready();
		if (!isReady) {
			chThdSleepMilliseconds(10);
			wasReady = FALSE;
			continue;
		}
		if (!wasReady) {
			wasReady = TRUE;
//			scheduleSimpleMsg(&logger, "ts channel is now ready ", chTimeNow());
		}

		short command = (short) chSequentialStreamGet(TS_SERIAL_DEVICE);
		int success = tunerStudioHandleCommand(command);
		if (!success && command != 0)
			print("got unexpected TunerStudio command %c:%d\r\n", command, command);

		tsCounter++;
	}
#if defined __GNUC__
	return 0;
#endif
}

extern engine_configuration_s *engineConfiguration;

void syncTunerStudioCopy(void) {
	memcpy(&tsContstants, engineConfiguration, sizeof(engine_configuration_s));
}

void startTunerStudioConnectivity(void) {
	initLogging(&logger, "tuner studio");
#if EFI_TUNER_STUDIO_OVER_USB
	print("TunerStudio over USB serial");
	usb_serial_start();
#else
	print("TunerStudio over USART");
	mySetPadMode("tunerstudio rx", TS_SERIAL_RX_PORT, TS_SERIAL_RX_PIN, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
	mySetPadMode("tunerstudio tx", TS_SERIAL_TX_PORT, TS_SERIAL_TX_PIN, PAL_MODE_ALTERNATE(TS_SERIAL_AF));

	sdStart(TS_SERIAL_DEVICE, &tsSerialConfig);
#endif

	syncTunerStudioCopy();

	addConsoleAction("tsinfo", printStats);

	chThdCreateStatic(TS_WORKING_AREA, sizeof(TS_WORKING_AREA), NORMALPRIO, tsThreadEntryPoint, NULL);
}

void updateTunerStudioState() {
	tsOutputChannels.rpm = getRpm();
	tsOutputChannels.coolant_temperature = getCoolantTemperature();
	tsOutputChannels.intake_air_temperature = getIntakeAirTemperature();
	tsOutputChannels.throttle_positon = getTPS();
	tsOutputChannels.mass_air_flow = getMaf();
	tsOutputChannels.air_fuel_ratio = getAfr();
	tsOutputChannels.v_batt = getVBatt();
	tsOutputChannels.tpsADC = getTPS10bitAdc();
	tsOutputChannels.atmospherePressure = getAtmosphericPressure();
	tsOutputChannels.manifold_air_pressure = getMap();
	tsOutputChannels.checkEngine = hasErrorCodes();
}

#endif /* EFI_TUNER_STUDIO */
