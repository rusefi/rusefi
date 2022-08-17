/*
    ChibiOS - Copyright (C) 2014-2015 Fabio Utzig

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

/**
 * @file    GPIOv2/hal_pal_lld.c
 * @brief   PAL subsystem low level driver.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup PAL
 * @{
 */

#include "osal.h"
#include "hal.h"

#include <string.h>


#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#include "gpio_s6e2_common.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

// port P8x doesn't have PCR register, so we fake it for 'fm4_gpio_ports' table consistency.
static __IO uint32_t FM4_FAKE_REG = 0;

static uint16_t defMask0[PADS_PER_PORT] = { 0 };
static uint16_t defMask1[PADS_PER_PORT] = { 1<<0, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7, 1<<8, 1<<9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15, };

// special bitmasks for paralleled power pins on Hellen-cypress board
#if HAL_USE_PAL_POWER_PINS
static uint16_t pc3PowerMask1[PADS_PER_PORT] = { 1<<0, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7, 1<<8, 
												(1<<9)|(1<<10), (1<<9)|(1<<10),  // P39 || P3A
												(1<<11)|(1<<12), (1<<11)|(1<<12),// P3B || P3C
												(1<<13)|(1<<14), (1<<13)|(1<<14),// P3D || P3E
												1<<15, };
static uint16_t pc4PowerMask1[PADS_PER_PORT] = { (1<<0)|(1<<1), (1<<0)|(1<<1),   // P40 || P41
												(1<<2)|(1<<3), (1<<2)|(1<<3),    // P42 || P43
												(1<<4)|(1<<5), (1<<4)|(1<<5),    // P44 || P45
												1<<6, 1<<7, 1<<8, 1<<9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15,};
#else
#error Are you sure you are using the right halconf.h config?
#define pc3PowerMask1 defMask1
#define pc4PowerMask1 defMask1
#endif

// we store it as a pre-defined const table to save some RAM
const GPIO_TypeDef fm4_gpio_ports[TOTAL_PORTS] = {
// port 0x0
	{ 0,  &FM4_GPIO_PFR0, &FM4_GPIO_PCR0, &FM4_GPIO_PZR0, &FM4_GPIO_DDR0, &FM4_GPIO_PDIR0, &FM4_GPIO_PDOR0, &bFM4_GPIO_PDIR0_P0, &bFM4_GPIO_PDOR0_P0, { defMask0, defMask1 } },
// port 0x1
	{ 1,  &FM4_GPIO_PFR1, &FM4_GPIO_PCR1, &FM4_GPIO_PZR1, &FM4_GPIO_DDR1, &FM4_GPIO_PDIR1, &FM4_GPIO_PDOR1, &bFM4_GPIO_PDIR1_P0, &bFM4_GPIO_PDOR1_P0, { defMask0, defMask1 } },
// port 0x2
	{ 2,  &FM4_GPIO_PFR2, &FM4_GPIO_PCR2, &FM4_GPIO_PZR2, &FM4_GPIO_DDR2, &FM4_GPIO_PDIR2, &FM4_GPIO_PDOR2, &bFM4_GPIO_PDIR2_P0, &bFM4_GPIO_PDOR2_P0, { defMask0, defMask1 } },
// port 0x3
	{ 3,  &FM4_GPIO_PFR3, &FM4_GPIO_PCR3, &FM4_GPIO_PZR3, &FM4_GPIO_DDR3, &FM4_GPIO_PDIR3, &FM4_GPIO_PDOR3, &bFM4_GPIO_PDIR3_P0, &bFM4_GPIO_PDOR3_P0, { defMask0, pc3PowerMask1 } },
// port 0x4
	{ 4,  &FM4_GPIO_PFR4, &FM4_GPIO_PCR4, &FM4_GPIO_PZR4, &FM4_GPIO_DDR4, &FM4_GPIO_PDIR4, &FM4_GPIO_PDOR4, &bFM4_GPIO_PDIR4_P0, &bFM4_GPIO_PDOR4_P0, { defMask0, pc4PowerMask1 } },
// port 0x5
	{ 5,  &FM4_GPIO_PFR5, &FM4_GPIO_PCR5, &FM4_GPIO_PZR5, &FM4_GPIO_DDR5, &FM4_GPIO_PDIR5, &FM4_GPIO_PDOR5, &bFM4_GPIO_PDIR5_P0, &bFM4_GPIO_PDOR5_P0, { defMask0, defMask1 } },
// port 0x6
	{ 6,  &FM4_GPIO_PFR6, &FM4_GPIO_PCR6, &FM4_GPIO_PZR6, &FM4_GPIO_DDR6, &FM4_GPIO_PDIR6, &FM4_GPIO_PDOR6, &bFM4_GPIO_PDIR6_P0, &bFM4_GPIO_PDOR6_P0, { defMask0, defMask1 } },
// port 0x7
	{ 7,  &FM4_GPIO_PFR7, &FM4_GPIO_PCR7, &FM4_GPIO_PZR7, &FM4_GPIO_DDR7, &FM4_GPIO_PDIR7, &FM4_GPIO_PDOR7, &bFM4_GPIO_PDIR7_P0, &bFM4_GPIO_PDOR7_P0, { defMask0, defMask1 } },
// port 0x8
	{ 8,  &FM4_GPIO_PFR8, &FM4_FAKE_REG,  &FM4_GPIO_PZR8, &FM4_GPIO_DDR8, &FM4_GPIO_PDIR8, &FM4_GPIO_PDOR8, &bFM4_GPIO_PDIR8_P0, &bFM4_GPIO_PDOR8_P0, { defMask0, defMask1 } },
// port 0x9
	{ 9,  &FM4_GPIO_PFR9, &FM4_GPIO_PCR9, &FM4_GPIO_PZR9, &FM4_GPIO_DDR9, &FM4_GPIO_PDIR9, &FM4_GPIO_PDOR9, &bFM4_GPIO_PDIR9_P0, &bFM4_GPIO_PDOR9_P0, { defMask0, defMask1 } },
// port 0xA
	{ 10, &FM4_GPIO_PFRA, &FM4_GPIO_PCRA, &FM4_GPIO_PZRA, &FM4_GPIO_DDRA, &FM4_GPIO_PDIRA, &FM4_GPIO_PDORA, &bFM4_GPIO_PDIRA_P0, &bFM4_GPIO_PDORA_P0, { defMask0, defMask1 } },
// port 0xB
	{ 11, &FM4_GPIO_PFRB, &FM4_GPIO_PCRB, &FM4_GPIO_PZRB, &FM4_GPIO_DDRB, &FM4_GPIO_PDIRB, &FM4_GPIO_PDORB, &bFM4_GPIO_PDIRB_P0, &bFM4_GPIO_PDORB_P0, { defMask0, defMask1 } },
// port 0xC
	{ 12, &FM4_GPIO_PFRC, &FM4_GPIO_PCRC, &FM4_GPIO_PZRC, &FM4_GPIO_DDRC, &FM4_GPIO_PDIRC, &FM4_GPIO_PDORC, &bFM4_GPIO_PDIRC_P0, &bFM4_GPIO_PDORC_P0, { defMask0, defMask1 } },
// port 0xD
	{ 13, &FM4_GPIO_PFRD, &FM4_GPIO_PCRD, &FM4_GPIO_PZRD, &FM4_GPIO_DDRD, &FM4_GPIO_PDIRD, &FM4_GPIO_PDORD, &bFM4_GPIO_PDIRD_P0, &bFM4_GPIO_PDORD_P0, { defMask0, defMask1 } },
// port 0xE
	{ 14, &FM4_GPIO_PFRE, &FM4_GPIO_PCRE, &FM4_GPIO_PZRE, &FM4_GPIO_DDRE, &FM4_GPIO_PDIRE, &FM4_GPIO_PDORE, &bFM4_GPIO_PDIRE_P0, &bFM4_GPIO_PDORE_P0, { defMask0, defMask1 } },
// port 0xF
	{ 15, &FM4_GPIO_PFRF, &FM4_GPIO_PCRF, &FM4_GPIO_PZRF, &FM4_GPIO_DDRF, &FM4_GPIO_PDIRF, &FM4_GPIO_PDORF, &bFM4_GPIO_PDIRF_P0, &bFM4_GPIO_PDORF_P0, { defMask0, defMask1 } },
};

#if (PAL_USE_WAIT == TRUE) || (PAL_USE_CALLBACKS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Event records for the 16 GPIO EXTI channels.
 */
palevent_t _pal_events[CYPRESS_GPIO_NUM_LINES];

#define ExintNone ExintInstanceIndexMax

// todo: The tables below must match the PDL_PERIPHERAL_ENABLE_EXINTx definitions inside pdl_user.h! Maybe add the compile-time checks?
#if 0
#if (PDL_EXINT_INSTANCE_COUNT != 11)
#error Please update the tables below if you changed the PDL_PERIPHERAL_ENABLE_EXINTx settings!
#endif
#endif
static en_exint_instance_index_t instanceIdxFromLine[CYPRESS_GPIO_NUM_LINES] = {
	/* 0 */ ExintNone, ExintNone, ExintNone, ExintNone, ExintNone, ExintNone, ExintNone, ExintNone,
	/* 8 */ ExintInstanceIndexExint8, ExintInstanceIndexExint9, ExintInstanceIndexExint10, ExintInstanceIndexExint11, ExintNone, ExintInstanceIndexExint13, ExintNone, ExintNone, 
	/* 16*/ ExintNone, ExintNone, ExintNone, ExintInstanceIndexExint19, ExintNone, ExintNone, ExintNone, ExintNone, 
	/* 24*/ ExintInstanceIndexExint24, ExintInstanceIndexExint25, ExintInstanceIndexExint26, ExintInstanceIndexExint27, ExintNone, ExintNone, ExintNone, ExintInstanceIndexExint31, 
};

static const uint8_t P1xLines[] = { 
	8, PAL_NOLINE, PAL_NOLINE, PAL_NOLINE, PAL_NOLINE, PAL_NOLINE, PAL_NOLINE, PAL_NOLINE, 
	10, 24,  PAL_NOLINE, 11,  PAL_NOLINE,  PAL_NOLINE, 26, 27 
};

ioportid_t _pal_linePorts[CYPRESS_GPIO_NUM_LINES] = { 
	/* 0 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
	/* 8 */ GPIOB, GPIOC, GPIOB, GPIOB, NULL, GPIOJ, NULL, NULL, 
	/* 16*/ NULL, NULL, NULL, GPIOD, NULL, NULL, NULL, NULL, 
	/* 24*/ GPIOB, GPIOC, GPIOB, GPIOB, NULL, NULL, NULL, NULL, 
};

iopadid_t _pal_linePads[CYPRESS_GPIO_NUM_LINES] = { 
	/* 0 */  0, 0, 0, 0, 0, 0, 0, 0, 
	/* 8 */  0, 9, 8, 11, 0, 7, 0, 0, 
	/* 16 */ 0, 0, 0, 2, 0, 0, 0, 0, 
	/* 24 */ 9, 5, 14, 15, 0, 0, 0, 0, 
};

static en_exint_level_t bothEdges[CYPRESS_GPIO_NUM_LINES];

stc_exint_config_t stcExtIntConfig;
bool wasExtIntInit = false;

static void _pal_lld_irq_handler(uint8_t channel) {
	// change the polarity if needed
	if (bothEdges[channel] != 0) {
		int isHigh = pal_lld_readpad(_pal_linePorts[channel], _pal_linePads[channel]);

		if (isHigh && bothEdges[channel] == ExIntRisingEdge) {
	  		bothEdges[channel] = ExIntFallingEdge;
			Exint_SetDetectMode(channel, bothEdges[channel]);

			_pal_isr_code(channel);
		}
		else if (!isHigh && bothEdges[channel] == ExIntFallingEdge) {
	  		bothEdges[channel] = ExIntRisingEdge;
			Exint_SetDetectMode(channel, bothEdges[channel]);

			_pal_isr_code(channel);
		}
	} else {
		_pal_isr_code(channel);
	}
}

ioline_t _pal_lld_getpadline(ioportid_t port, iopadid_t pad) {
	if (port == GPIOB) /* P1x */ {
		return P1xLines[pad];
	}
	else if (port == GPIOC) /* P2x */ {
		if (pad == 5)
			return 25;
		else if (pad == 9)
			return 9;
	}
	else if (port == GPIOD) /* P3x */ {
		if (pad == 2)
			return 19;
	}
	else if (port == GPIOJ) /* PCx */ {
		if (pad == 7)
			return 13;
	}
	return PAL_NOLINE;
}

#endif

#define AN_NONE -1

static int8_t P1xAnalogChannels[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

static int8_t P2xAnalogChannels[] = {
	AN_NONE, AN_NONE, 31, 30, 29, 28, 27, 26, 25, 24, AN_NONE, AN_NONE, AN_NONE, AN_NONE, AN_NONE, AN_NONE
};

static int8_t getAnalogChannel(ioportid_t port, iopadid_t pad) {
	if (port == GPIOB) /* P1x */ {
		return P1xAnalogChannels[pad];
	}
	else if (port == GPIOC) /* P2x */ {
		return P2xAnalogChannels[pad];
	}
	// todo: add more cases
	return AN_NONE;
}

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void resetAnalogChannel(int adcChannel) {
	if (adcChannel >= 0 && adcChannel < 32)
		*(&bFM4_GPIO_ADE_AN00 + 4 * adcChannel) = 0;
}

// todo: Is there a better way to configure pins? The Cypress PDL leaves us no choice with those SetPinFunc_XXX() macros?
// this is based on gpio_s6e2c5xl.h 
static void setPadModeAlternate(ioportid_t port, uint8_t pad, iomode_t mode) {
	switch (mode) {
	case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_ANALOG):
		if (port == GPIOB) {	// P1x
			switch (pad) {
			case 0:	// P10
				SetPinFunc_AN00();
				break;
			case 1:	// P11
				SetPinFunc_AN01();
				break;
			case 2:	// P12
				SetPinFunc_AN02();
				break;
			case 3:	// P13
				SetPinFunc_AN03();
				break;
			case 4:	// P14
				SetPinFunc_AN04();
				break;
			case 5:	// P15
				SetPinFunc_AN05();
				break;
			case 6:	// P16
				SetPinFunc_AN06();
				break;
			case 7:	// P17
				SetPinFunc_AN07();
				break;
			case 8:	// P18
				SetPinFunc_AN08();
				break;
			case 9:	// P19
				SetPinFunc_AN09();
				break;
			case 10:// P1A
				SetPinFunc_AN10();
				break;
			case 11:// P1B
				SetPinFunc_AN11();
				break;
			case 12:// P1C
				SetPinFunc_AN12();
				break;
			case 13:// P1D
				SetPinFunc_AN13();
				break;
			case 14:// P1E
				SetPinFunc_AN14();
				break;
			case 15:// P1F
				SetPinFunc_AN15();
				break;
			}
			break;
		}
		else if (port == GPIOC) {	// P2x
			switch (pad) {
			case 2:	// P22
				SetPinFunc_AN31();
				break;
			case 3:	// P23
				SetPinFunc_AN30();
				break;
			case 4:	// P24
				SetPinFunc_AN29();
				break;
			case 5:	// P25
				SetPinFunc_AN28();
				break;
			case 7:	// P27
				SetPinFunc_AN27();
				break;
			case 8:	// P28
				SetPinFunc_AN26();
				break;
			case 9:	// P29
				SetPinFunc_AN25();
				break;
			case 10:// P2A
				SetPinFunc_AN24();
				break;
			}
		}
  		break;
	case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_SPI):
		if (port == GPIOG) {	// P7x
			switch (pad) {
			case 8:	// P78 (SPI6_MISO)
				SetPinFunc_SIN6_0();
				break;
			case 9:	// P79 (SPI6_MOSI)
				SetPinFunc_SOT6_0();
				break;
			case 10:// P7A (SPI6_SCK)
				SetPinFunc_SCK6_0();
				break;
			case 11:// P7B (SPI6_CS0)
				SetPinFunc_SCS60_0();
				break;
			}
		}
		else if (port == GPIOH) {	// PAx
			switch (pad) {
			case 8:	// PA8 (SPI7_MISO)
				SetPinFunc_SIN7_0();
				break;
			case 9:	// PA9 (SPI7_MOSI)
				SetPinFunc_SOT7_0();
				break;
			case 10:// PAA (SPI7_SCK)
				SetPinFunc_SCK7_0();
				break;
			case 11:// PAB (SPI7_CS0)
				SetPinFunc_SCS70_0();
				break;
			case 12:// PAC (SPI7_CS1)
				SetPinFunc_SCS71_0();
				break;
			}
		}
		break;
	case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_I2C):
		if (port == GPIOH) {	// PAx
			switch (pad) {
			case 13:// PAD (I2C3_SCL)
				SetPinFunc_SCK3_0();
				break;
			case 14:// PAE (I2C3_SDA)
				SetPinFunc_SOT3_0();
				break;
			}
		}
		break;
	case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_UART):
		if (port == GPIOC) {	// P2x
			switch (pad) {
			case 1:	// P21 (UART0_RX)
				SetPinFunc_SIN0_0();
				break;
			case 2:	// P22 (UART0_TX)
				SetPinFunc_SOT0_0();
				break;
			}
		}
		break;
	case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_CAN):
		// this is S6E2Gx version
		if (port == GPIOB) {	// P1x
			switch (pad) {
			case 6:	// P16 (CAN0_RX)
				SetPinFunc_RX0_0();
				break;
			case 7:	// P17 (CAN0_TX)
				SetPinFunc_TX0_0();
				break;
			}
		}
		// this is S6E2Cx version
		else if (port == GPIOG) {	// P7x
			switch (pad) {
			case 13:// P7D (CAN2_RX)
				SetPinFunc_RX2_0();
				break;
			case 14:// P7E (CAN2_TX)
				SetPinFunc_TX2_0();
				break;
			}
		}
		break;
	case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_EXTINT):
		if (port == GPIOB) {	// P1x
			switch (pad) {
			case 0:// P10 (CRANK)
				SetPinFunc_INT08_0(0u);
				break;
			case 8:// P18 (CAM)
				SetPinFunc_INT10_0(0u);
				break;
			case 9:// P19 (VSS)
				SetPinFunc_INT24_1(0u);
				break;
			case 11:// P1B (AUX1)
				SetPinFunc_INT11_0(0u);
				break;
			case 14:// P1E (AUX2)
				SetPinFunc_INT26_1(0u);
				break;
			case 15:// P1F (AUX3)
				SetPinFunc_INT27_1(0u);
				break;
			}
		} 
		else if (port == GPIOC) {	// P2x
			switch (pad) {
			case 5:// P25 (AUX4)
				SetPinFunc_INT25_0(0u);
				break;
			case 9:// P29 (RES_IN)
#ifdef SetPinFunc_INT09_2
				SetPinFunc_INT09_2(0u);
#endif
				break;
			}
		}
		else if (port == GPIOD) {	// P3x
			switch (pad) {
			case 2:// P32 (DRV_MISO)
				SetPinFunc_INT19_0(0u);
				break;
			}
		}
		else if (port == GPIOJ) {	// PCx
			switch (pad) {
			case 7:// PC7 (RES2)
				SetPinFunc_INT13_0(0u);
				break;
			}
		}
		
		// we need to re-configure the edge interrupt
		int line = PAL_LINE(port, pad);
		if (bothEdges[line] != 0) {
			int isHigh = pal_lld_readpad(port, pad);
			bothEdges[line] = (isHigh != 0) ? ExIntFallingEdge : ExIntRisingEdge;
			Exint_SetDetectMode(line, bothEdges[line]);
		}			
		break;
	}
}

/**
 * @brief   Pad mode setup.
 * @details This function programs a pad with the specified mode.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] mode      pad mode
 *
 * @notapi
 */
void _pal_lld_setpadmode(ioportid_t port,
                         uint8_t pad,
                         iomode_t mode) {

  osalDbgAssert(pad < PADS_PER_PORT, "pal_lld_setpadmode() #1, invalid pad");
 
  uint32_t padMask = port->padMask[1][pad];

  switch (mode) {
  case PAL_MODE_RESET:
  case PAL_MODE_INPUT:
  case PAL_MODE_INPUT_PULLDOWN:	// todo: we have no pulldown, is it ok to be here?
  	*port->PCR &= ~padMask;	// no pull-up
  	*port->DDR &= ~padMask;	// dir=input
  	*port->PFR &= ~padMask;	// gpio
  	resetAnalogChannel(getAnalogChannel(port, pad));
  	break;
  case PAL_MODE_OUTPUT_PUSHPULL:
  	*port->PZR &= ~padMask;	// no open-drain
  	*port->DDR |= padMask;	// dir=output
  	*port->PFR &= ~padMask;	// gpio
  	resetAnalogChannel(getAnalogChannel(port, pad));
    break;
  case PAL_MODE_OUTPUT_OPENDRAIN:
    *port->PZR |= padMask;	// open-drain (and no-pullup)
  	*port->DDR |= padMask;	// dir=output
  	*port->PFR &= ~padMask;	// gpio
  	resetAnalogChannel(getAnalogChannel(port, pad));
    break;
  case PAL_MODE_INPUT_PULLUP:
    *port->PCR |= padMask;	// pull-up
  	*port->DDR &= ~padMask;	// dir=input
  	*port->PFR &= ~padMask;	// gpio
  	resetAnalogChannel(getAnalogChannel(port, pad));
    break;
  case PAL_MODE_UNCONNECTED:
  case PAL_MODE_INPUT_ANALOG:
  case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_ANALOG):
  	setPadModeAlternate(port, pad, PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_ANALOG));
  	break;
  case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_SPI):
  case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_I2C):
  case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_UART):
  case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_CAN):
  case PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_EXTINT):
  	setPadModeAlternate(port, pad, mode);
  	break;
  }
  
  // a little 'hack' for complex pin modes like MISO+PULLUP
  if (mode & PAL_STM32_PUPDR_PULLUP) {
  	*port->PCR |= padMask;	// pull-up
  }

}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Kinetis I/O ports configuration.
 * @details Ports A-E clocks enabled.
 *
 * @param[in] config    the Kinetis ports configuration
 *
 * @notapi
 */
void _pal_lld_init(void) {
	PDL_ZERO_STRUCT(stcExtIntConfig);
	stcExtIntConfig.bTouchNvic = TRUE;

	memset(bothEdges, 0, sizeof(bothEdges));
	
#if PAL_USE_CALLBACKS || PAL_USE_WAIT || defined(__DOXYGEN__)
	unsigned i;

	for (i = 0; i < CYPRESS_GPIO_NUM_LINES; i++) {
		_pal_init_event(i);
	}
#endif

	wasExtIntInit = false;
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {

  int i;

  (void)mask;

  for (i = 0; i < PADS_PER_PORT; i++) {
    pal_lld_setpadmode(port, i, mode);
  }
}

#if PAL_USE_CALLBACKS || PAL_USE_WAIT

void _pal_lld_setpadeventhandler(ioline_t line, int edgeLevel, pal_padeventhandler_t handler) {
	// add a new line to the extint config
	stcExtIntConfig.abEnable[instanceIdxFromLine[line]] = TRUE;   // INT00
	stcExtIntConfig.aenLevel[instanceIdxFromLine[line]] = (en_exint_level_t)edgeLevel;
	stcExtIntConfig.apfnExintCallback[instanceIdxFromLine[line]] = (func_ptr_arg1_t)handler;

	if (wasExtIntInit) {
		Exint_DeInit();
		wasExtIntInit = false;
	}

	if (!wasExtIntInit) {
		Exint_Init(&stcExtIntConfig);
		wasExtIntInit = true;
	}

	Exint_EnableChannel(line);
}

void _pal_lld_enablepadevent(ioportid_t port,
                             iopadid_t pad,
                             ioeventmode_t mode) {
	en_exint_level_t cfg;

	ioline_t line = PAL_LINE(port, pad);
	if (line == PAL_NOLINE)
		return;
	if (instanceIdxFromLine[line] == ExintNone)
		return;
  
	bothEdges[line] = 0;

	switch (mode) {
	case PAL_EVENT_MODE_RISING_EDGE:
		cfg = ExIntRisingEdge;
		break;
	case PAL_EVENT_MODE_FALLING_EDGE:
		cfg = ExIntFallingEdge;
		break;
	case PAL_EVENT_MODE_BOTH_EDGES:
#if 0	// [andreika]: unfortunately our FM4 doesn't support it :(
		cfg = ExIntBothEdge;
#else
		_pal_lld_setpadmode(port, pad, PAL_MODE_INPUT);
		cfg = pal_lld_readpad(port, pad) != 0 ? ExIntFallingEdge : ExIntRisingEdge;
		// save the initial edge state so we can change it in IRQ handler
		bothEdges[line] = cfg;
#endif
		break;
	case PAL_EVENT_MODE_DISABLED:
	default:
		_pal_lld_disablepadevent(port, pad);
		return;
	}

	// Set pin function before enabling external interrupt channel!
	_pal_lld_setpadmode(port, pad, PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_EXTINT));

	_pal_lld_setpadeventhandler(line, cfg, _pal_lld_irq_handler);
}

void _pal_lld_disablepadevent(ioportid_t port, iopadid_t pad) {
	ioline_t line = PAL_LINE(port, pad);
	if (line == PAL_NOLINE)
		return;
	stcExtIntConfig.abEnable[instanceIdxFromLine[line]] = FALSE;
	bothEdges[line] = 0;
	
	Exint_DisableChannel(line);

#if PAL_USE_CALLBACKS || PAL_USE_WAIT
  /* Callback cleared and/or thread reset.*/
  _pal_clear_event(pad);
#endif
}

#endif /* PAL_USE_CALLBACKS || PAL_USE_WAIT */

#endif /* HAL_USE_PAL */

/** @} */
