/**
 *
 * \file
 *
 * \brief This module contains SAMD21 BSP APIs implementation.
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
#include "bsp/include/nm_bsp_internal.h"
#include "common/include/nm_common.h"
#include "asf.h"
#include "conf_winc.h"

static tpfNmBspIsr gpfIsr;

static void chip_isr(void)
{
	if (gpfIsr) {
		gpfIsr();
	}
}

/*
 *	@fn		init_chip_pins
 *	@brief	Initialize reset, chip enable and wake pin
 */
static void init_chip_pins(void)
{
	struct port_config pin_conf;

	port_get_config_defaults(&pin_conf);

	/* Configure control pins as output. */
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(CONF_WINC_PIN_RESET, &pin_conf);
	port_pin_set_config(CONF_WINC_PIN_CHIP_ENABLE, &pin_conf);
	port_pin_set_config(CONF_WINC_PIN_WAKE, &pin_conf);
	port_pin_set_output_level(CONF_WINC_PIN_CHIP_ENABLE, false);
	port_pin_set_output_level(CONF_WINC_PIN_RESET, false);
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
#if 0
	/*For uart debug only*/
	/* Perform chip reset. */
	nm_bsp_reset();
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	/* Configure control pins as input with pull up. */
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(PIN_PA07, &pin_conf);
	port_pin_set_config(PIN_PA04, &pin_conf);
	port_pin_set_config(PIN_PA05, &pin_conf);
	port_pin_set_config(PIN_PA06, &pin_conf);
	while(1);
#endif
	system_interrupt_enable_global();

	return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_deinit
 *	@brief	De-iInitialize BSP
 *	@return	0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_deinit(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	/* Configure control pins as input no pull up. */
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_output_level(CONF_WINC_PIN_CHIP_ENABLE, false);
	port_pin_set_output_level(CONF_WINC_PIN_RESET, false);
	port_pin_set_config(CONF_WINC_SPI_INT_PIN, &pin_conf);
	return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_reset
 *	@brief	Reset NMC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 */
void nm_bsp_reset(void)
{
	port_pin_set_output_level(CONF_WINC_PIN_CHIP_ENABLE, false);
	port_pin_set_output_level(CONF_WINC_PIN_RESET, false);
	nm_bsp_sleep(1);
	port_pin_set_output_level(CONF_WINC_PIN_CHIP_ENABLE, true);
	nm_bsp_sleep(10);
	port_pin_set_output_level(CONF_WINC_PIN_RESET, true);
}

/*
 *	@fn		nm_bsp_sleep
 *	@brief	Sleep in units of mSec
 *	@param[IN]	u32TimeMsec
 *				Time in milliseconds
 */
void nm_bsp_sleep(uint32 u32TimeMsec)
{
	while (u32TimeMsec--) {
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
	struct extint_chan_conf config_extint_chan;

	gpfIsr = pfIsr;

	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin = CONF_WINC_SPI_INT_PIN;
	config_extint_chan.gpio_pin_mux = CONF_WINC_SPI_INT_MUX;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;

	extint_chan_set_config(CONF_WINC_SPI_INT_EIC, &config_extint_chan);
	extint_register_callback(chip_isr, CONF_WINC_SPI_INT_EIC,
			EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(CONF_WINC_SPI_INT_EIC,
			EXTINT_CALLBACK_TYPE_DETECT);
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
		extint_chan_enable_callback(CONF_WINC_SPI_INT_EIC,
				EXTINT_CALLBACK_TYPE_DETECT);
	} else {
		extint_chan_disable_callback(CONF_WINC_SPI_INT_EIC,
				EXTINT_CALLBACK_TYPE_DETECT);
	}
}
