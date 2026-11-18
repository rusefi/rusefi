/**
 * @file boards/f746-discovery/board.h
 *
 * Stock ChibiOS STM32F746G-Discovery board.h: full pinmux (including the
 * USB3320 ULPI PHY on OTG_HS), 25 MHz HSE bypass and BOARD_OTG2_USES_ULPI.
 *
 * This wrapper must exist in the board directory: ChibiOS rules.mk sorts the
 * -I list alphabetically, so pointing BOARDINC at the ChibiOS board directory
 * loses to the F767 Nucleo board.h in hw_layer/ports/stm32/stm32f7
 * ("./config/boards/..." sorts before "./hw_layer/...", "ChibiOS/..." after).
 */

#pragma once

#include "../../../ChibiOS/os/hal/boards/ST_STM32F746G_DISCOVERY/board.h"
