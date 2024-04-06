/**
 * @file    usbconsole.h
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void usb_serial_start();
bool is_usb_serial_ready();

#ifdef __cplusplus
}
#endif /* __cplusplus */
