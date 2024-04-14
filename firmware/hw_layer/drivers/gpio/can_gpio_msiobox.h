/**
 * can_gpio.h
 */
#pragma once

#include "efifeatures.h"

#if EFI_CAN_GPIO
/* not number of pins but number of entities in IO module set */
#define BOARD_CAN_GPIO_COUNT	1
#else
#define BOARD_CAN_GPIO_COUNT	0
#endif

int initCanGpioMsiobox();
