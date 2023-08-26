
#include "pch.h"

#include "hardware.h"

int main(void) {
	halInit();
	chSysInit();

	// set base pin configuration based on the board
	setDefaultBasePins();
/* at the moment SD card is not needed by bootloader
	// set SD card configuration also
	setDefaultSdCardParameters();
*/

	while (true) {
		chThdSleepMilliseconds(1);
	}
}
