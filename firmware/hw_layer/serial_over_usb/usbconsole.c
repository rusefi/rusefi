/**
 * @file    usbconsole.c
 * @brief	USB-over-serial configuration
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"

#if EFI_USB_SERIAL || defined(__DOXYGEN__)

#include "usbconsole.h"
#include "usbcfg.h"
#include "efifeatures.h"


static bool isUsbSerialInitialized = false;

void usb_serial_start(void) {
	/*
	 * Initializes a serial-over-USB CDC driver.
	 */
	sduObjectInit(&SDU1);
	sduStart(&SDU1, &serusbcfg);

	/*
	 * Activates the USB driver and then the USB bus pull-up on D+.
	 * Note, a delay is inserted in order to not have to disconnect the cable
	 * after a reset.
	 */
	usbDisconnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(1500);
	usbStart(serusbcfg.usbp, &usbcfg);
	usbConnectBus(serusbcfg.usbp);
	
#if HAL_USE_SERIAL
	/**
	 * todo: start taking USB pins from configuration?
	 * at the moment USB pinout is hard-coded in board.h file
	 *
	 * PA10/PA11/PA12
	 * #define GPIOA_OTG_FS_ID             10
	 * #define GPIOA_OTG_FS_DM             11
	 * #define GPIOA_OTG_FS_DP             12
	 */
	/*
	 * Activates the serial driver 2 using the driver default configuration.
	 */
	sdStart(&SD2, NULL);
#endif

	isUsbSerialInitialized = true;
}

bool is_usb_serial_ready(void) {
	return isUsbSerialInitialized && SDU1.config->usbp->state == USB_ACTIVE;
}

#else
bool is_usb_serial_ready(void) {
	return false;
}

#endif /* EFI_USB_SERIAL */
