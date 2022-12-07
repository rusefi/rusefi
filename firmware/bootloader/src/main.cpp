
#include "pch.h"

#include "hardware.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <ch.h>
#include <hal.h>
#include <stm32f4xx.h>

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "dfu.h"

static bool wasCommand = false;

static THD_WORKING_AREA(waBootloaderSerial, 128);
static THD_FUNCTION(thBootloaderSerial, arg) {
   	(void)arg;
	chRegSetThreadName("BootloaderSerial");

	// start our DFU emulator
	wasCommand = dfuStartLoop();

	chThdExit(MSG_OK);
}

int main(void) {
	// run ChibiOS
	halInit();
	chSysInit();
	// set base pin configuration based on the board
	setDefaultBasePins();
/* at the moment SD card is not needed by bootloader
	// set SD card configuration also
	setDefaultSdCardParameters();
*/

	// start UART
	getTsChannel()->start(38400);	// TODO: should bootloader serial speed be configurable?

	// start a serial port reader thread
	thread_t *thrSerial = chThdCreateStatic(waBootloaderSerial, sizeof(waBootloaderSerial), NORMALPRIO, thBootloaderSerial, NULL);
	
	// wait for the thread to finish
	chThdWait(thrSerial);
	
#if 0
	if (wasCommand)	// abnormal termination of the bootloader thread
		chSysHalt("Bootloader DFU FAIL");
#endif

	// Run application
	dfuJumpToApp(APPLICATION_ADDR);

	return 0;
}

