/**
 *
 * \file
 *
 * \brief This module contains SAMG55 BSP APIs implementation.
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "asf.h"
#include "conf_winc.h"

static tpfNmBspIsr gpfIsr;

static void chip_isr(uint32_t id, uint32_t mask)
{
	if ((id == CONF_WINC_SPI_INT_PIO_ID) && (mask == CONF_WINC_SPI_INT_MASK)) {
		if (gpfIsr) {
			gpfIsr();
		}
	}
}

/*
 *	@fn		init_chip_pins
 *	@brief	Initialize reset, chip enable and wake pin
 */
static void init_chip_pins(void)
{
	ioport_init();
	ioport_set_pin_dir(CONF_WINC_PIN_RESET, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CONF_WINC_PIN_RESET, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(CONF_WINC_PIN_CHIP_ENABLE, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CONF_WINC_PIN_CHIP_ENABLE, IOPORT_PIN_LEVEL_HIGH);
}

/*
*	@fn		nm_bsp_init
*	@brief	Initialize BSP
*	@return	0 in case of success and -1 in case of failure
*/
sint8 nm_bsp_init(void)
{
	gpfIsr = NULL;

	/* Initialize chip IOs. */
	init_chip_pins();

    /* Make sure a 1ms Systick is configured. */
    if (!(SysTick->CTRL & SysTick_CTRL_ENABLE_Msk && SysTick->CTRL & SysTick_CTRL_TICKINT_Msk)) {
	    delay_init();
    }

	/* Perform chip reset. */
	nm_bsp_reset();

	return 0;
}

/**
*   @fn      nm_bsp_deinit
*   @brief   De-iInitialize BSP
*   @return  0 in case of success and -1 in case of failure
*/
sint8 nm_bsp_deinit(void)
{
	ioport_set_pin_level(CONF_WINC_PIN_CHIP_ENABLE, false);
	ioport_set_pin_level(CONF_WINC_PIN_RESET, false);
	return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_reset
 *	@brief	Reset WINC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 */
void nm_bsp_reset(void)
{
	ioport_set_pin_level(CONF_WINC_PIN_CHIP_ENABLE, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(CONF_WINC_PIN_RESET, IOPORT_PIN_LEVEL_LOW);
	nm_bsp_sleep(1);
	ioport_set_pin_level(CONF_WINC_PIN_CHIP_ENABLE, IOPORT_PIN_LEVEL_HIGH);
	nm_bsp_sleep(10);
	ioport_set_pin_level(CONF_WINC_PIN_RESET, IOPORT_PIN_LEVEL_HIGH);
}

/*
*	@fn		nm_bsp_sleep
*	@brief	Sleep in units of mSec
*	@param[IN]	u32TimeMsec
*				Time in milliseconds
*/
void nm_bsp_sleep(uint32 u32TimeMsec)
{
	while(u32TimeMsec--) {
		delay_ms(1);
	}
}

/*
*	@fn		nm_bsp_register_isr
*	@brief	Register interrupt service routine
*	@param[IN]	pfIsr
*				Pointer to ISR handler
*/
void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
	gpfIsr = pfIsr;

	/* Configure PGIO pin for interrupt from SPI slave, used when slave has data to send. */
	pmc_enable_periph_clk(CONF_WINC_SPI_INT_PIO_ID);
	pio_configure_pin(CONF_WINC_SPI_INT_PIN, PIO_TYPE_PIO_INPUT);
	pio_pull_up(CONF_WINC_SPI_INT_PIO, CONF_WINC_SPI_INT_MASK, PIO_PULLUP);
	/*Interrupt on falling edge*/
	pio_handler_set(CONF_WINC_SPI_INT_PIO, CONF_WINC_SPI_INT_PIO_ID,
	CONF_WINC_SPI_INT_MASK, PIO_PULLUP | PIO_IT_FALL_EDGE, chip_isr);
	pio_get_interrupt_status(CONF_WINC_SPI_INT_PIO);
	pio_enable_interrupt(CONF_WINC_SPI_INT_PIO, CONF_WINC_SPI_INT_MASK);
	NVIC_EnableIRQ((IRQn_Type) CONF_WINC_SPI_INT_PIO_ID);
	pio_handler_set_priority(CONF_WINC_SPI_INT_PIO, (IRQn_Type)CONF_WINC_SPI_INT_PIO_ID,
			CONF_WINC_SPI_INT_PRIORITY);
}

/*
*	@fn		nm_bsp_interrupt_ctrl
*	@brief	Enable/Disable interrupts
*	@param[IN]	u8Enable
*				'0' disable interrupts. '1' enable interrupts
*/
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
	if (u8Enable) {
		pio_get_interrupt_status(CONF_WINC_SPI_INT_PIO);
		pio_enable_interrupt(CONF_WINC_SPI_INT_PIO, CONF_WINC_SPI_INT_MASK);
	}
	else {
		pio_disable_interrupt(CONF_WINC_SPI_INT_PIO, CONF_WINC_SPI_INT_MASK);
	}
}

