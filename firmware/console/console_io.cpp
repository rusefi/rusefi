	/**
 * @file	console_io.cpp
 *
 * @date Dec 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEFI can communicate with external universe via native USB or some sort of TTL mode
 * We have an interesting situation with TTL communication channels, we have
 * 1) SERIAL - this one was implemented first simply because the code was readily available (works on stm32)
 *    this one is most suitable for streaming HAL API
 *    this one is not great since each byte requires an IRQ and with enough IRQ delay we have a risk of data loss
 * 2) UART DMA - the best one since FIFO buffer reduces data loss (works on stm32)
 *    We have two halves of DMA buffer - one is used for TTL while rusEFI prepares next batch of data in the other side.
 *    We need idle support in order to not wait for the complete buffer to get full in order to recieve a message.
 *    Back when we were implementing this STM32_DMA_CR_HTIE was not available in ChibiOS driver so we have added it.
 *    we have custom rusEFI changes to ChibiOS HAL driver v1
 *    F7 uses driver v2 which currently does not have rusEFI changes.
 *    open question if fresh ChibiOS is better in this regard.
 * 3) UART this one is useful on platforms with hardware FIFO buffer like Kinetis.
 *    stm32 does not have such buffer so for stm32 UART without DMA has no advantages
 *
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "pch.h"

#include "console_io.h"
#include "os_util.h"
#include "tunerstudio.h"
#include "connector_uart_dma.h"
#include "thread_priority.h"

#if EFI_SIMULATOR
#include "rusEfiFunctionalTest.h"
#endif /*EFI_SIMULATOR */

bool consoleByteArrived = false;

void onDataArrived(bool valid) {
	consoleByteArrived = valid;
}

CommandHandler console_line_callback;

void startConsole(CommandHandler console_line_callback_p) {
	console_line_callback = console_line_callback_p;
}
