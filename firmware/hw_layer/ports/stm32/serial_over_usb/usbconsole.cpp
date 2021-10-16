/**
 * @file    usbconsole.c
 * @brief	USB-over-serial configuration
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"

#if EFI_USB_SERIAL

#include "usbconsole.h"
#include "usbcfg.h"
#include "mpu_util.h"

static bool isUsbSerialInitialized = false;

/**
 * start USB serial using hard-coded communications pins (see comments inside the code)
 */
void usb_serial_start(void) {
	usbPopulateSerialNumber(MCU_SERIAL_NUMBER_LOCATION, MCU_SERIAL_NUMBER_BYTES);

	efiSetPadMode("USB DM", EFI_USB_SERIAL_DM, PAL_MODE_ALTERNATE(EFI_USB_AF));
	efiSetPadMode("USB DP", EFI_USB_SERIAL_DP, PAL_MODE_ALTERNATE(EFI_USB_AF));

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
// See also https://github.com/rusefi/rusefi/issues/705
#ifndef EFI_SKIP_USB_DISCONNECT
	usbDisconnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(250);
#endif/* EFI_SKIP_USB_DISCONNECT */
	usbStart(serusbcfg.usbp, &usbcfg);
	usbConnectBus(serusbcfg.usbp);

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
