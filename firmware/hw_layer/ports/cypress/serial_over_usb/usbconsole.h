/**
 * @file    usbconsole.h
 *
 * @date Jan 27, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 */

#ifndef USBCONSOLE_H_
#define USBCONSOLE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void usb_serial_start(void);
bool is_usb_serial_ready(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* USBCONSOLE_H_ */
