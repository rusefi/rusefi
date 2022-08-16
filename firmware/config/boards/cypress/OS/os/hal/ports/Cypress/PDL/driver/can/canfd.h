/******************************************************************************
* \file             canfd.h
*
* \version          1.20
*
* \brief            Header file for Controller Area Network with Flexible Data Rate(CANFD) functions  
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
* CYPRESS PROVIDES THIS SOFTWARE "AS IS" AND MAKES NO WARRANTY
* OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
* PURPOSE.
*******************************************************************************/

#ifndef __CANFD_H__
#define __CANFD_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl_user.h"
#include "mcu.h"

#if (defined(PDL_PERIPHERAL_CANFD_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupCANFD Controller Area Network with Flexible Data Rate(CANFD)
* \{
* \defgroup GroupCANFD_Macros Macros
* \defgroup GroupCANFD_Functions Functions
* \defgroup GroupCANFD_DataStructures Data Structures
* \defgroup GroupCANFD_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupCANFD
* \{ 
* Controller Area Network (CAN) is widely used in various industries
* such as automobile and factory automation. CAN with Flexible Data Rate(CANFD)
* complies with ISO11898-1 (CAN specification 
* Rev. 2.0 part A, B) and with CAN FD specification V1.0 (Bosch CAN FD 
* Specification V1.0). It doesn't comply with ISO11898-1 
* (CAN FD specification).<br>
*
* You can configure the CAN FD Controller to support 8-, 16-, and 32-bit wide 
* data. The CAN FD Controller's clock domain concept allows the separation 
* between the two input clocks, the CAN clock and the Bus clock.<br>
*
* <b>Features of the CAN FD Controller</b><br>
* - Conforms with ISO11898-1 (CAN specification Rev. 2.0 part A, B) and the CAN FD specification V1.0
* （Bosch CAN FD Specification V1.0）<br>
* - CAN FD with up to 64 data bytes supported <br>
* - CAN Error Logging <br>
* - Acceptance filtering <br>
* - Two configurable Receive FIFOs <br>
* - Separate signaling on reception of High Priority Messages <br>
* - Up to 64 dedicated Receive Buffers <br>
* - Up to 32 dedicated Transmit Buffers <br>
* - Configurable Transmit FIFO <br>
* - Configurable Transmit Queue <br>
* - Configurable Transmit Event FIFO <br>
* - Direct Message RAM access for CPU <br>
* - Programmable loop-back test mode <br>
* - Maskable interrupts <br>
* - Two clock domains (CAN clock and Bus clock) <br>
* - Power-down support <br>
* - Debug on CAN support <br>
*
* \section SectionCANFD_ConfigurationConsideration Configuration Consideration
* Before initializing the CANFD module, call Canpre_Init(). You must set the value of the divider for 
* the CAN prescaler clock. This is supplied to the CANFD as a CANFD 
* system clock.<br>
*  
* Set fields in the stc_canfd_config_t structure to configure the peripheral. 
* For example, set the CAN operation mode in enCanfdMode, and the clock frequency in enCanfdClock.<br>
* After setting the configuration structure to the desired values, call Canfd_Init() 
* to configure the CANFD peripheral.<br>
* The callback functions pfnStatusCallback and pfnErrorCallback are optional, but recommended. Otherwise there is no
* report to the API in case of any reception, transmission, bus state and error
* event.<br>
* Use Canfd_DeInit() if you need to change any configuration settings. 
* Make the required changes and then call Canfd_Init() again.
* Canfd_DeInit() completely disables the CAN FD module.
* All CAN FD related register values are reset to their default
* values. Any pending or ongoing transmission or reception will be
* aborted.<br>
*  Call Canfd_Start() to start communication. You typically use Canfd_Restart() to recover 
* after an error. Use Canfd_TransmitMsg() and Canfd_ReceiveMsg() for sending and receiving messages. 
* The stc_canfd_msg_t structure contains the message and message identifier. <br>
* Each CAN FD module has dedicated message buffers for receiving and transmitting. 
* You identify the buffer number to use when you transmit a message. <br>
* The receiving buffer consists of dedicated Rx buffer, Rx FIFO0 and Rx FIFO1.
* In the same way, the transmission buffer consists of dedicated Tx buffer and 
* Tx FIFO/QUEUE.<br>
*
* \note The numbers of the message buffers used in this driver are shown as
* follows:<br>
* - dedicated Rx buffer : 1 element, indexed 0.<br>
* - Rx FIFO0            : 2 elements.<br>
* - Rx FIFO1            : not used.<br>
* - Tx buffer           : 1 element, indexed 0.<br>
* - Tx FIFO/QUEUE       : not used.<br>
*
* \section SectionCANFD_MoreInfo More Information
* For more information on the CANFD  Controller, refer to:<br>
* <a href="http://www.cypress.com/file/222976/download">FM4 Family 32-Bit MICROCONTROLLER PERIPHERAL MANUAL Communication Macro Part</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets can be downloaded</a>
*/
 
/**
* \addtogroup GroupCANFD_Macros
* \{
*/

/******************************************************************************
 * Global definitions
 ******************************************************************************/

// CANFD controller
#define stc_canfdn_t  FM_CANFD_TypeDef
#define CANFD0      (*((volatile stc_canfdn_t *)FM_CANFD0_BASE))

/// Size of data bytes in a receive or transmit operation
#define CANFD_MESSAGE_DATA_BUFFER_SIZE			64
/// Size of data word in a receive or transmit operation
#define CANFD_MESSAGE_DATA_BUFFER_SIZEW			(CANFD_MESSAGE_DATA_BUFFER_SIZE / 4)

/// Number of possible dedicated Rx buffer for receive
#define CANFD_MESSAGE_RXBUFFER_COUNT    		64

/// Number of possible Rx FIFO for receive messages
#define CANFD_MESSAGE_RXFIFO_COUNT      		32

/// Number of possible dedicated Tx buffer, Tx Queue and Tx FIFO for transmit messages
#define CANFD_MESSAGE_TXBUFFER_COUNT    		32

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief The CAN FD driver uses definitions of the structure of registers on
 ** the CAN FD controller that were defined in the "REGISTER DEFINITION HEADER"
 ** for the target MCU.
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief CAN FD Unit configuration structures
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief CAN FD received message index.
 **
 ** These are used to specify CAN operation mode.
 ******************************************************************************/
#define	CANFD_MSGIDX_MIN		(0U)		// minimum index of dedicated Rx buffer.
#define	CANFD_MSGIDX_MAX		(63U)		// maximum index of dedicated Rx buffer.
#define	CANFD_MSGIDX_FIFO0		(254U)		// received message stored into Rx FIFO0.
#define	CANFD_MSGIDX_FIFO1		(255U)		// received message stored into Rx FIFO1.

#define	CANFD_MAX_STDDLC		(8)			// maximum data length in standard CAN operation.

/**
 ******************************************************************************
 ** \brief CAN FD error.
 **
 ** These error values are used to report any CAN FD related error to the API via
 ** the CAN FD error callback function.
 ** The parameter of this callback function is uint32_t type, and it includes 
 ** all of detected error flags as shown below;
 ******************************************************************************/
#define	CANFD_ERROR_STE		(1UL << 31)		// [bit31] STE : Stuff Error / More than 5 equal bits in a sequence occurred.
#define	CANFD_ERROR_FOE		(1UL << 30)		// [bit30] FOE : Format Error / A fixed format part of a received frame has the wrong format.
#define	CANFD_ERROR_ACKE	(1UL << 29)		// [bit29] ACKE: Acknowledge Error / A transmitted message was not acknowledged by another node.
#define	CANFD_ERROR_BE		(1UL << 28)		// [bit28] BE  : Bit Error / CAN FD Controller wanted to send a recessive/dominant level, but monitored bus level was dominant/recessive.
#define	CANFD_ERROR_CRCE	(1UL << 27)		// [bit27] CRCE: CRC Error / Received CRC did not match the calculated CRC.
#define	CANFD_ERROR_WDI		(1UL << 26)		// [bit26] WDI : Watchdog Interrupt / Message RAM Watchdog event due to missing READY from Message RAM.
#define	CANFD_ERROR_ELO		(1UL << 22)		// [bit22] ELO : Error Logging Overflow / Overflow of CAN Error Logging Counter (ECR.CEL[7:0]) occurred.
#define	CANFD_ERROR_BEU		(1UL << 21)		// [bit21] BEU : Bit Error Uncorrected / Bit error detected, but could not be corrected.
#define	CANFD_ERROR_BEC		(1UL << 20)		// [bit20] BEC : Bit Error Corrected / Bit error detected and corrected by ECC logic.
#define	CANFD_ERROR_TOO		(1UL << 18)		// [bit18] TOO : Timeout Occurred / Timeout reached.
#define	CANFD_ERROR_MRAF	(1UL << 17)		// [bit17] MRAF: Message RAM Access Failure / Message RAM access failure occurred.
#define	CANFD_ERROR_TEFL	(1UL << 15)		// [bit15] TEFL: Tx Event FIFO Element Lost / Tx Event FIFO element lost. Also set after write attempt to Tx Event FIFO of size zero.
#define	CANFD_ERROR_RF1L	(1UL <<  7)		// [bit7]  RF1L: Rx FIFO 1 Message Lost / Rx FIFO 1 message lost. Also set after write attempt to Rx FIFO 1 of size zero.
#define	CANFD_ERROR_RF0L	(1UL <<  3)		// [bit3]  RF0L: Rx FIFO 0 Message Lost / Rx FIFO 0 message lost. Also set after write attempt to Rx FIFO 0 of size zero.

/** \} GroupCANFD_Macros */  

/**
* \addtogroup GroupCANFD_Types
* \{
*/
/**
 ******************************************************************************
 ** \brief CAN operation mode.
 **
 ** These are used to specify CAN operation mode.
 ******************************************************************************/
typedef enum en_canfd_mode
{
	CanfdModeClassic = 0,	// classic CAN (CAN2.0)
	CanfdModeFDFixed = 1,	// CAN-FD with fixed data rate
	CanfdModeFDFlex  = 2	// CAN-FD with flexible data rate
} en_canfd_mode_t;

/**
 ******************************************************************************
 ** \brief CAN FD operation clock frequency.
 **
 ** These are used to determin clock frequency of CAN FD controller.
 ******************************************************************************/
typedef enum en_canfd_clock
{
	CanfdClock32MHz = 0,	// 32MHz
	CanfdClock40MHz = 1		// 40MHz
} en_canfd_clock_t;


/**
 ******************************************************************************
 ** \brief CAN FD module status.
 **
 ** These state values represent the current CAN FD module state, and are used 
 ** in the CAN FD status changed callback function (canfd_status_chg_func_ptr_t).
 ******************************************************************************/
typedef enum en_canfd_status
{
	CanfdBusOff  = 0,   // The CAN FD module is in busoff state.
	CanfdWarning = 1    // At least one error counter has reached error warning limit of 96.
} en_canfd_status_t;

/**
 *****************************************************************************
 ** \brief Message transmission complete callback function (canfd_tx_msg_func_ptr_t).
 **
 ** Signals a successful completed transmission.
 *****************************************************************************/
typedef void (*canfd_tx_msg_func_ptr_t)( uint8_t u8MsgBuf );


/**
 *****************************************************************************
 ** \brief Status changed callback function (canfd_status_chg_func_ptr_t).
 **
 ** Any status change will be reported to the API (see #en_canfd_status_t for
 ** possible status change codes).
 *****************************************************************************/
typedef void (*canfd_status_chg_func_ptr_t)( en_canfd_status_t enCanfdStatus );


/**
 *****************************************************************************
 ** \brief Error callback function (canfd_error_func_ptr_t).
 **
 ** Any error will be reported to the API 
 *****************************************************************************/
typedef void (*canfd_error_func_ptr_t)( uint32_t u32CanfdError );


/** \}GroupCANFD_Types */

/**
* \addtogroup GroupCANFD_DataStructures
* \{
*/
/**
 ******************************************************************************
 ** \brief CAN message identifier
 **
 ** This structure is used to set the CAN message identifier for transmit and
 ** receive operations.
 ******************************************************************************/
typedef struct stc_canfd_msg_id
{
	uint32_t	u32Identifier;    	// 11- or 29-bit identifier. The valid bit length depends on bExtended.
	boolean_t	bExtended;			// TRUE: 29-bit identifier,  FALSE: 11-bit identifier.
} stc_canfd_msg_id_t;


/**
 ******************************************************************************
 ** \brief CAN message data.
 **
 ** Data structure for transmit and receive operations, maximum value for
 ** parameter u8DataLengthCode is CANFD_MESSAGE_DATA_BUFFER_SIZE.
 ******************************************************************************/
typedef struct stc_canfd_msg_data
{
	union
	{
		uint32_t	au32Data[CANFD_MESSAGE_DATA_BUFFER_SIZEW];	// Data of CAN message.
		uint8_t		au8Data[CANFD_MESSAGE_DATA_BUFFER_SIZE];	// Data of CAN message.
	};
	uint8_t	u8DataLengthCode;                       			// Number of valid bytes in au8Data and DLC of CAN message.
} stc_canfd_msg_data_t;


/**
 ******************************************************************************
 ** \brief CAN message.
 **
 ** This structure stores a CAN or CAN FD message, including the identifier, 
 ** data and data length code (DLC). It also contains an overflow and new flag
 ** which indicates the message state for received messages.
 ******************************************************************************/
typedef struct stc_canfd_msg
{
	stc_canfd_msg_id_t		stcIdentifier;  // 11- or 29-bit identifier (ID).
	stc_canfd_msg_data_t	stcData;		// Data and DLC.
	boolean_t				bCanfd;			// TRUE : DLC is extended, FALSE : Normal DLC
											// (valid received message only.)
} stc_canfd_msg_t;

/**
 *****************************************************************************
 ** \brief Message reception callback function (canfd_rx_msg_func_ptr_t).
 **
 ** Signals that CAN has received a new message.
 *****************************************************************************/
typedef void (*canfd_rx_msg_func_ptr_t)( uint8_t u8MsgBuf, stc_canfd_msg_t* pstcRxMsg );


/**
 *****************************************************************************
 ** \brief CAN FD interrupt pointer structure
 **
 ** Holds some pointers that point to callback functions for message reception
 ** and transmission.
 *****************************************************************************/
typedef struct stc_canfd_interrupt_handling
{
	canfd_tx_msg_func_ptr_t	pfnCanfdTxInterruptFunction; 	// pointer to transmit interrupt callback
	canfd_rx_msg_func_ptr_t	pfnCanfdRxInterruptFunction; 	// pointer to receive interrupt callback
} stc_canfd_interrupt_handling_t;


/**
 *****************************************************************************
 ** \brief CAN FD notification pointer structure
 **
 ** Holds some pointers to callback functions for status and error notification
 *****************************************************************************/
typedef struct stc_canfd_notification
{
	canfd_status_chg_func_ptr_t	pfnCanfdStatusInterruptFunction ;
	canfd_error_func_ptr_t		pfnCanfdErrorInterruptFunction ;
} stc_canfd_notification_t;


/**
 ******************************************************************************
 ** \brief CAN FD configuration.
 **
 ** Is used to set the CAN operation mode and CAN FD clock frequency.
 ** Optionally reception, transmission, error and status notification callback
 ** functions can be set.
 ******************************************************************************/
typedef struct stc_canfd_config
{
	en_canfd_mode_t				enCanfdMode;			// CAN operation mode.
	en_canfd_clock_t			enCanfdClock;			// CAN FD operation clock (prescaler output).
	canfd_rx_msg_func_ptr_t		pfnReceiveMsgCallback; 	// Callback function for receive message, can be NULL.
	canfd_tx_msg_func_ptr_t		pfnTransmitMsgCallback; // Callback function for transmit message, can be NULL.
	canfd_status_chg_func_ptr_t	pfnStatusCallback;      // Callback function for CAN FD status changes, can be NULL.
	canfd_error_func_ptr_t		pfnErrorCallback;       // Callback function for CAN FD related errors, can be NULL.
} stc_canfd_config_t;

/** \} GroupCANFD_DataStructures */

/******************************************************************************/
/* Global variable declarations ('extern', definition in C source)            */
/******************************************************************************/

/**
* \addtogroup GroupCANFD_Functions
* \{
*/ 

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/

extern void CanfdIrqHandler( volatile stc_canfdn_t* pstcCanfd );

extern en_result_t Canfd_Init( volatile stc_canfdn_t* pstcCanfd,
							   const stc_canfd_config_t* pstcConfig
							 );

extern en_result_t Canfd_DeInit( volatile stc_canfdn_t* pstcCanfd );

extern en_result_t Canfd_Start( volatile stc_canfdn_t* pstcCanfd );

extern en_result_t Canfd_Stop( volatile stc_canfdn_t* pstcCanfd );

extern en_result_t Canfd_Restart( volatile stc_canfdn_t* pstcCanfd );

extern en_result_t Canfd_TransmitMsg( volatile stc_canfdn_t* pstcCanfd,
									  uint8_t u8MsgBuf,
									  stc_canfd_msg_t* pstcMsg
									);

extern en_result_t Canfd_ReceiveMsg( volatile stc_canfdn_t* pstcCanfd );

extern en_result_t Canfd_GetBusStatus( volatile stc_canfdn_t* pstcCanfd );

/** \}GroupCANFD_Functions */
/** \} GroupCANFD */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_CANFD_ACTIVE))

#endif /* __CANFD_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
