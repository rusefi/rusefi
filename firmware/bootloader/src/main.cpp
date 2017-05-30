
#include "main.h"
#include "hardware.h"
#include "efiGpio.h"
#include "global.h"

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

persistent_config_container_s persistentState CCM_OPTIONAL;
const persistent_config_s *config = &persistentState.persistentConfiguration;
const engine_configuration_s *engineConfiguration = &persistentState.persistentConfiguration.engineConfiguration;
const board_configuration_s *boardConfiguration = &persistentState.persistentConfiguration.engineConfiguration.bc;

LoggingWithStorage tsLogger("binary");
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
	// set UART pads configuration based on the board
	setDefaultSerialParameters(PASS_ENGINE_PARAMETER_SIGNATURE);
	// set SD card configuration also
	setDefaultSdCardParameters(PASS_ENGINE_PARAMETER_SIGNATURE);

	// start UART
	startTsPort(getTsChannel());
	
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

