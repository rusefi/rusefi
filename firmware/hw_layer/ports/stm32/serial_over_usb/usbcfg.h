/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#pragma once

#ifndef HAL_USE_USB_CDC_2
#define HAL_USE_USB_CDC_2 FALSE
#endif

#ifndef HAL_USE_USB_CDC_3
#define HAL_USE_USB_CDC_3 FALSE
#endif

#ifndef HAL_USE_USB_CDC_4
#define HAL_USE_USB_CDC_4 FALSE
#endif

#define NUM_CDC_INSTANCES ( (1) + \
                            (HAL_USE_USB_CDC_2 ? 1 : 0) + \
                            (HAL_USE_USB_CDC_3 ? 1 : 0) + \
                            (HAL_USE_USB_CDC_4 ? 1 : 0) )

extern const USBConfig usbcfg;
extern const SerialUSBConfig serusbcfg[NUM_CDC_INSTANCES];
extern SerialUSBDriver SDU[NUM_CDC_INSTANCES];

void usbPopulateSerialNumber(const uint8_t* serialNumber, size_t bytes);
