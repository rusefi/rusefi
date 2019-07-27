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

#if HAL_USE_PAL || defined(__DOXYGEN__)

#include "fsl_port.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if (PAL_USE_WAIT == TRUE) || (PAL_USE_CALLBACKS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Event records for the 16 GPIO EXTI channels.
 */
palevent_t _pal_events[KINETIS_GPIO_NUM_LINES];
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#ifdef KE1xF

void _pal_lld_irq_handler(PORT_Type *base) {
	uint32_t flags = PORT_GetPinsInterruptFlags(base);
	for (int i = 0; i < 16; i++) {
		if (flags & (1 << i)) {
			// todo: add port-dependent lines?
			_pal_isr_code(i);
		}
	}
	PORT_ClearPinsInterruptFlags(base, flags);
}

#endif /* KE1xF */

PORT_TypeDef *_pal_lld_get_port_config(ioportid_t port) {
  PORT_TypeDef *portcfg = NULL;
  if (port == IOPORT1)
    portcfg = PORTA;
  else if (port == IOPORT2)
    portcfg = PORTB;
  else if (port == IOPORT3)
    portcfg = PORTC;
  else if (port == IOPORT4)
    portcfg = PORTD;
  else if (port == IOPORT5)
    portcfg = PORTE;
  return portcfg;
}

/**
 * @brief Reads a logical state from an I/O pad.
 * @note The @ref PAL provides a default software implementation of this
 * functionality, implement this function if can optimize it by using
 * special hardware functionalities or special coding.
 *
 * @param[in] port port identifier
 * @param[in] pad pad number within the port
 * @return The logical state.
 * @retval PAL_LOW low logical state.
 * @retval PAL_HIGH high logical state.
 *
 * @notapi
 */
uint8_t _pal_lld_readpad(ioportid_t port,
                         uint8_t pad) {

  return (port->PDIR & ((uint32_t) 1 << pad)) ? PAL_HIGH : PAL_LOW;
}

/**
 * @brief Writes a logical state on an output pad.
 * @note This function is not meant to be invoked directly by the
 * application code.
 * @note The @ref PAL provides a default software implementation of this
 * functionality, implement this function if can optimize it by using
 * special hardware functionalities or special coding.
 *
 * @param[in] port port identifier
 * @param[in] pad pad number within the port
 * @param[in] bit logical value, the value must be @p PAL_LOW or
 * @p PAL_HIGH
 *
 * @notapi
 */
void _pal_lld_writepad(ioportid_t port,
                       uint8_t pad,
                       uint8_t bit) {

  if (bit == PAL_HIGH)
    port->PDOR |= ((uint32_t) 1 << pad);
  else
    port->PDOR &= ~((uint32_t) 1 << pad);
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

  if (mode == PAL_MODE_OUTPUT_PUSHPULL || mode == PAL_MODE_OUTPUT_OPENDRAIN)
    port->PDDR |= ((uint32_t) 1 << pad);
  else
    port->PDDR &= ~((uint32_t) 1 << pad);

  PORT_TypeDef *portcfg = _pal_lld_get_port_config(port);
  osalDbgAssert(portcfg != NULL, "pal_lld_setpadmode() #2, invalid port");

  switch (mode) {
  case PAL_MODE_RESET:
  case PAL_MODE_INPUT:
  case PAL_MODE_OUTPUT_PUSHPULL:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(1);
    break;
#if KINETIS_GPIO_HAS_OPENDRAIN
  case PAL_MODE_OUTPUT_OPENDRAIN:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(1) |
                        PORTx_PCRn_ODE;
    break;
#else
#undef PAL_MODE_OUTPUT_OPENDRAIN
#endif
  case PAL_MODE_INPUT_PULLUP:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(1) |
                        PORTx_PCRn_PE |
                        PORTx_PCRn_PS;
      break;
  case PAL_MODE_INPUT_PULLDOWN:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(1) |
                        PORTx_PCRn_PE;
      break;
  case PAL_MODE_UNCONNECTED:
  case PAL_MODE_INPUT_ANALOG:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(0);
    break;
  case PAL_MODE_ALTERNATIVE_1:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(1);
    break;
  case PAL_MODE_ALTERNATIVE_2:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(2);
    break;
  case PAL_MODE_ALTERNATIVE_3:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(3);
    break;
  case PAL_MODE_ALTERNATIVE_4:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(4);
    break;
  case PAL_MODE_ALTERNATIVE_5:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(5);
    break;
  case PAL_MODE_ALTERNATIVE_6:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(6);
    break;
  case PAL_MODE_ALTERNATIVE_7:
    portcfg->PCR[pad] = PIN_MUX_ALTERNATIVE(7);
    break;
  }
  
  // a little 'hack' for complex pin modes like MISO+PULLUP
  if (mode & PAL_STM32_PUPDR_PULLUP) {
  	portcfg->PCR[pad] |= PAL_STM32_PUPDR_PULLUP;
  }

  // enable high-current drive strength for push-pull outputs (Kinetis-specific feature)
  if (mode & PAL_MODE_OUTPUT_PUSHPULL) {
    // todo: Not all pins are capable of high-drive. Is is a problem?
    PORT_SetPinDriveStrength(portcfg, pad, kPORT_HighDriveStrength);
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
#ifdef KE1xF
void _pal_lld_init(void) {
  /* Enable clocking on all Ports */
  PCC->CLKCFG[PCC_PORTA_INDEX] |= PCC_CLKCFG_CGC_MASK;
  PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC_MASK;
  PCC->CLKCFG[PCC_PORTC_INDEX] |= PCC_CLKCFG_CGC_MASK;
  PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC_MASK;
  PCC->CLKCFG[PCC_PORTE_INDEX] |= PCC_CLKCFG_CGC_MASK;

  nvicEnableVector(PORTA_IRQn, KINETIS_GPIO_IRQ_PRIORITY);
  nvicEnableVector(PORTB_IRQn, KINETIS_GPIO_IRQ_PRIORITY);
  nvicEnableVector(PORTC_IRQn, KINETIS_GPIO_IRQ_PRIORITY);
  nvicEnableVector(PORTD_IRQn, KINETIS_GPIO_IRQ_PRIORITY);
  nvicEnableVector(PORTE_IRQn, KINETIS_GPIO_IRQ_PRIORITY);
}
#else
void _pal_lld_init(const PALConfig *config) {

  int i, j;

  /* Enable clocking on all Ports */
  SIM->SCGC5 |= SIM_SCGC5_PORTA |
                SIM_SCGC5_PORTB |
                SIM_SCGC5_PORTC |
                SIM_SCGC5_PORTD |
                SIM_SCGC5_PORTE;

  /* Initial PORT and GPIO setup */
  for (i = 0; i < TOTAL_PORTS; i++) {
    for (j = 0; j < PADS_PER_PORT; j++) {
      pal_lld_setpadmode(config->ports[i].port,
                         j,
                         config->ports[i].pads[j]);
    }
  }
}
#endif

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

void _pal_lld_enablepadevent(ioportid_t port,
                             iopadid_t pad,
                             ioeventmode_t mode) {
  port_interrupt_t cfg;
  switch (mode) {
  case PAL_EVENT_MODE_RISING_EDGE:
  	cfg = kPORT_InterruptRisingEdge;
  	break;
  case PAL_EVENT_MODE_FALLING_EDGE:
  	cfg = kPORT_InterruptFallingEdge;
  	break;
  case PAL_EVENT_MODE_BOTH_EDGES:
  	cfg = kPORT_InterruptEitherEdge;
  	break;
  case PAL_EVENT_MODE_DISABLED:
  default:
  	cfg = kPORT_InterruptOrDMADisabled;
  }
  
  PORT_TypeDef *portcfg = _pal_lld_get_port_config(port);
  PORT_SetPinInterruptConfig(portcfg, pad, cfg);
 
}

void _pal_lld_disablepadevent(ioportid_t port, iopadid_t pad) {
	PORT_TypeDef *portcfg = _pal_lld_get_port_config(port);
	PORT_SetPinInterruptConfig(portcfg, pad, PAL_EVENT_MODE_DISABLED);
	PORT_ClearPinsInterruptFlags(portcfg, 0xffffffff);
}

#endif /* PAL_USE_CALLBACKS || PAL_USE_WAIT */

#endif /* HAL_USE_PAL */

/** @} */
