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
 * @file    GPIOv2/hal_pal_lld.h
 * @brief   PAL subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup PAL
 * @{
 */

#ifndef HAL_PAL_LLD_H_
#define HAL_PAL_LLD_H_

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

// STM32-compatible macro
#define PAL_MODE_ALTERNATE(x)       (0x10 + x)

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

#define TOTAL_PORTS                 16
#define PADS_PER_PORT               16

/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 16

/**
 * @brief   Whole port mask.
 * @brief   This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFFFFFFFF)

/* Specifies palInit() without parameter, required until all platforms will
   be updated to the new style.*/
#define PAL_NEW_INIT

/**
 * @brief   Digital I/O port sized unsigned type.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief   Digital I/O modes.
 */
typedef uint32_t iomode_t;

/**
 * @brief   Type of an I/O line.
 */
typedef uint32_t ioline_t;

/**
 * @brief   Type of an event mode.
 */
typedef uint32_t ioeventmode_t;


/**
 * @brief   Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */

 #ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
// Parts taken from FM4_GPIO_TypeDef
typedef struct {
	uint8_t idx;
	__IO uint32_t * const PFR;		// port function
	__IO uint32_t * const PCR;		// pull-up
	__IO uint32_t * const PZR;		// open-drain
	__IO uint32_t * const DDR;		// direction (input/output)
	__IO uint32_t * const portDIR;	// read port input 
	__IO uint32_t * const portDOR;	// write port output
	__IO uint8_t  * const padDIR;	// read pad
	__IO uint8_t  * const padDOR;	// write pad
	uint16_t *padMask[2];			// write mask for pal_lld_writepad() [PADS_PER_PORT]
} GPIO_TypeDef;

extern const GPIO_TypeDef fm4_gpio_ports[TOTAL_PORTS];
#ifdef __cplusplus
}
#endif /* __cplusplus */

typedef GPIO_TypeDef *ioportid_t;

// port 0x0
#define GPIOA	((GPIO_TypeDef *)&fm4_gpio_ports[0x0])
// port 0x1
#define GPIOB	((GPIO_TypeDef *)&fm4_gpio_ports[0x1])
// port 0x2
#define GPIOC	((GPIO_TypeDef *)&fm4_gpio_ports[0x2])
// port 0x3
#define GPIOD	((GPIO_TypeDef *)&fm4_gpio_ports[0x3])
// port 0x4
#define GPIOE	((GPIO_TypeDef *)&fm4_gpio_ports[0x4])
// port 0x5
#define GPIOF	((GPIO_TypeDef *)&fm4_gpio_ports[0x5])
// port 0x6
// --- skip port 0x6
// port 0x7
#define GPIOG	((GPIO_TypeDef *)&fm4_gpio_ports[0x7])
// port 0x8
// --- skip port 0x8
// port 0x9
// --- skip port 0x9
// port 0xa
#define GPIOH	((GPIO_TypeDef *)&fm4_gpio_ports[0xa])
// port 0xb
#define GPIOI	((GPIO_TypeDef *)&fm4_gpio_ports[0xb])
// port 0xc
#define GPIOJ	((GPIO_TypeDef *)&fm4_gpio_ports[0xc])
// port 0xd
// --- skip port 0xd
// port 0xe
// --- skip port 0xe
// port 0xf
#define GPIOK	((GPIO_TypeDef *)&fm4_gpio_ports[0xf])

/**
 * @brief   Type of an pad identifier.
 */
typedef uint32_t iopadid_t;

/**
 * @brief   Handler used by _pal_lld_setpadeventhandler().
 */
typedef void         (*pal_padeventhandler_t)(uint8_t);

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief   GPIO port identifiers.
 */
#define IOPORT1          GPIOA
#define IOPORT2          GPIOB
#define IOPORT3          GPIOC
#define IOPORT4          GPIOD
#define IOPORT5          GPIOE
#define IOPORT6          GPIOF
#define IOPORT7          GPIOG
#define IOPORT8          GPIOH
#define IOPORT9          GPIOI
#define IOPORT10         GPIOJ
#define IOPORT11         GPIOK

/**
 * @name    Line handling macros
 * @{
 */
/**
 * @brief   Forms a line identifier.
 * @details A port/pad pair are encoded into an @p ioline_t type. The encoding
 *          of this type is platform-dependent.
 */
#define PAL_LINE(port, pad) ((ioline_t)_pal_lld_getpadline((port), (pad)))

/**
 * @brief   Decodes a port identifier from a line identifier.
 */
#define PAL_PORT(line) (_pal_linePorts[(line)])
  
/**
 * @brief   Decodes a pad identifier from a line identifier.
 */
#define PAL_PAD(line) (_pal_linePads[(line)])

/**
 * @brief   Decodes a port index (0..4) from a port identifier.
 */
#define PAL_PORT_INDEX(port) ((uint32_t)(port->idx))

/**
 * @brief   Value identifying an invalid line.
 */
#define PAL_NOLINE                      0U
/** @} */

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   Low level PAL subsystem initialization.
 *
 * @param[in] config    architecture-dependent ports configuration
 *
 * @notapi
 */
#define pal_lld_init(config) _pal_lld_init(config)

/**
 * @brief   Reads a group of bits.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @return              The group logical states.
 *
 * @notapi
 */
#define pal_lld_readgroup(port, mask, offset) 0

/**
 * @brief   Writes a group of bits.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] bits      bits to be written. Values exceeding the group width
 *                      are masked.
 *
 * @notapi
 */
#define pal_lld_writegroup(port, mask, offset, bits) (void)bits

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] mode      group mode
 *
 * @notapi
 */
#define pal_lld_setgroupmode(port, mask, offset, mode)                      \
  _pal_lld_setgroupmode(port, mask << offset, mode)

/**
 * @brief   Reads a logical state from an I/O pad.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @return              The logical state.
 * @retval PAL_LOW      low logical state.
 * @retval PAL_HIGH     high logical state.
 *
 * @notapi
 */
#define pal_lld_readpad(port, pad) *((port)->padDIR + pad * 4)

/**
 * @brief   Writes a logical state on an output pad.
 * @note    This function is not meant to be invoked directly by the
 *          application  code.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] bit       logical value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 *
 * @notapi
 */
// [andreika]: We use portDOR instead of padDOR because we have twin power pads (connected on board for more current)
// [andreika]: For twin power pads, we use combined mask with 2 bits set simultaneously.
#define pal_lld_writepad(port, pad, bit) *((port)->portDOR) = ((*((port)->portDIR) & ~((port)->padMask[1][pad])) | (port)->padMask[bit][pad])
//#define pal_lld_writepad(port, pad, bit) *((port)->padDOR + pad * 4) = bit

/**
 * @brief   Sets a pad logical state to @p PAL_HIGH.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
//#define pal_lld_setpad(port, pad) *((port)->padDOR + pad * 4) = 1
#define pal_lld_setpad(port, pad) pal_lld_writepad(port, pad, 1)

/**
 * @brief   Clears a pad logical state to @p PAL_LOW.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
//#define pal_lld_clearpad(port, pad) *((port)->padDOR + pad * 4) = 0
#define pal_lld_clearpad(port, pad) pal_lld_writepad(port, pad, 0)

/**
 * @brief   Toggles a pad logical state.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_togglepad(port, pad) pal_lld_writepad(port, pad, ~(pal_lld_readpad(port, pad)))

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
#define pal_lld_setpadmode(port, pad, mode)                                 \
    _pal_lld_setpadmode(port, pad, mode)


#if (PAL_USE_WAIT == TRUE) || (PAL_USE_CALLBACKS == TRUE)

#define CYPRESS_GPIO_NUM_PORTS 16
#define CYPRESS_GPIO_NUM_PADS  16
#define CYPRESS_GPIO_NUM_LINES 32

/**
 * @brief   Pad event enable.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] mode      pad event mode
 *
 * @notapi
 */
#define pal_lld_enablepadevent(port, pad, mode)                             \
  _pal_lld_enablepadevent(port, pad, mode)

/**
 * @brief   Pad event disable.
 * @details This function disables previously programmed event callbacks.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_disablepadevent(port, pad)                                  \
  _pal_lld_disablepadevent(port, pad)

/**
 * @brief   Returns a PAL event structure associated to a pad.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_get_pad_event(port, pad)                                    \
  &_pal_events[PAL_LINE((port), (pad))]
 
/**
 * @brief   Returns a PAL event structure associated to a line.
 *
 * @param[in] line      line identifier
 *
 * @notapi
 */
#define pal_lld_get_line_event(line)                                        \
	&_pal_events[(line)]

extern palevent_t _pal_events[CYPRESS_GPIO_NUM_LINES];
extern ioportid_t _pal_linePorts[CYPRESS_GPIO_NUM_LINES];
extern iopadid_t _pal_linePads[CYPRESS_GPIO_NUM_LINES];
#endif /* PAL_USE_WAIT */


#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_init(void);
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             iomode_t mode);
  void _pal_lld_setpadmode(ioportid_t port,
                           uint8_t pad,
                           iomode_t mode);
  uint8_t _pal_lld_readpad(ioportid_t port,
                           uint8_t pad);
  void _pal_lld_writepad(ioportid_t port,
                         uint8_t pad,
                         uint8_t bit);
#if PAL_USE_CALLBACKS || PAL_USE_WAIT
  ioline_t _pal_lld_getpadline(ioportid_t port, iopadid_t pad);

  void _pal_lld_enablepadevent(ioportid_t port,
                               iopadid_t pad,
                               ioeventmode_t mode);
  void _pal_lld_disablepadevent(ioportid_t port, iopadid_t pad);

  void _pal_lld_setpadeventhandler(ioline_t line, int edgeLevel, pal_padeventhandler_t handler);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL */

#endif /* HAL_PAL_LLD_H_ */

/** @} */
