/******************************************************************************
* \file             canfd.c
*
* \version          1.20
*
* \brief            Controller Area Network with Flexible Data Rate(CANFD) driver 
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

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "can/canfd.h"

// [andreika]: fix #elseif -> #elif for GCC

#if (defined(PDL_PERIPHERAL_CANFD_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/// Macro to return the number of enabled CAN FD instances
#define CANFD_INSTANCE_COUNT (uint32_t)(sizeof(m_astcCanfdInstanceDataLut) / sizeof(m_astcCanfdInstanceDataLut[0]))

/// Controller dependent constants.

/// Message RAM
#define	CANFD_MSGRAM_OFFSET		(0x00008000UL)	// Message RAM address offset from the base address of related controller
#define	CANFD_MSGRAM_SIZEB		(16384)			// Byte size of message RAM.
#define	CANFD_MSGRAM_SIZEW		(CANFD_MSGRAM_SIZEB >> 2)	// Word size of message RAM.

/// Tx buffer transmission requested marks
#define	CANFD_TX_NOT_REQUESTED	(0x00U)			// not requested
#define	CANFD_TX_REQUESTED		(0x96U)			// requested

/// Bit masks for IR register
												// error bits
#define	CANFD_IR_MASK_ERROR		(CANFD_ERROR_STE  | \
								 CANFD_ERROR_FOE  | \
								 CANFD_ERROR_ACKE | \
								 CANFD_ERROR_BE   | \
								 CANFD_ERROR_CRCE | \
								 CANFD_ERROR_WDI  | \
								 CANFD_ERROR_ELO  | \
								 CANFD_ERROR_BEU  | \
								 CANFD_ERROR_BEC  | \
								 CANFD_ERROR_TOO  | \
								 CANFD_ERROR_MRAF | \
								 CANFD_ERROR_TEFL | \
								 CANFD_ERROR_RF1L | \
								 CANFD_ERROR_RF0L)
#define	CANFD_IR_MASK_STATUS	(0x03800000UL)	// bus status bits
#define	CANFD_IR_MASK_RXBUF	(0x00080000UL)	// dedicated Rx buffer related bits
#define	CANFD_IR_MASK_RXFIFO0	(0x00000001UL)	// Rx FIFO0 related bits
#define	CANFD_IR_MASK_RXFIFO1	(0x00000010UL)	// Rx FIFO1 related bits
												// all of reception related bits
#define	CANFD_IR_MASK_RX		(CANFD_IR_MASK_RXBUF | CANFD_IR_MASK_RXFIFO0 | CANFD_IR_MASK_RXFIFO1)
#define	CANFD_IR_MASK_TX		(0x00000200UL)	// transmission related bits

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/**
 *****************************************************************************
 ** \brief CAN FD Rx buffer and FIFO element.
 **
 ** This structure defines a dedicated Rx buffer and Rx FIFO element placed on
 ** the message RAM.
 *****************************************************************************/
#define	CANFD_RXBUF_INFOSIZE	(2)		// Rx buffer header size
typedef	struct stc_canfd_rx_buffer
{
	union {
		struct {
			uint32_t	id		: 29;	// Identifier
			uint32_t	rtr		: 1;	// Remote Transmission Request
			uint32_t	xtd		: 1;	// Extended Identifier
			uint32_t	esi		: 1;	// Error State Indicator
			uint32_t	rxts	: 16;	// Rx Timestamp
			uint32_t	dlc		: 4;	// Data Length Code
			uint32_t	brs		: 1;	// Bit Rate Switch
			uint32_t	edl		: 1;	// Extended Data Length
			uint32_t	rsv		: 2;	// (reserved)
			uint32_t	fidx	: 7;	// Filter Index
			uint32_t	anmf	: 1;	// Accepted Non-matching Frame
		};
		uint32_t	w[2];				// to access as word
	};
	uint32_t	data[16];				// data buffer (defined by maximum size)
} stc_canfd_rx_buffer_t;

/**
 *****************************************************************************
 ** \brief CAN FD Tx buffer element.
 **
 ** This structure defines a dedicated Tx buffer, queue and FIFO element
 ** placed on the message RAM.
 *****************************************************************************/
#define	CANFD_TXBUF_INFOSIZE	(2)		// Tx buffer header size
typedef	struct stc_canfd_tx_buffer
{
	union {
		struct {
			uint32_t	id		: 29;	// Identifier
			uint32_t	rtr		: 1;	// Remote Transmission Request
			uint32_t	xtd		: 1;	// Extended Identifier
			uint32_t	rsv1	: 1;	// (reserved)
			uint32_t	rsv3	: 16;	// (reserved)
			uint32_t	dlc		: 4;	// Data Length Code
			uint32_t	rsv2	: 3;	// (reserved)
			uint32_t	efc		: 1;	// Event FIFO Control
			uint32_t	mm		: 8;	// Message Marker
		};
		uint32_t	w[2];				// to access as word
	};
	uint32_t	data[16];				// data buffer (defined by maximum size)
} stc_canfd_tx_buffer_t;

/**
 *****************************************************************************
 ** \brief CAN FD Tx event FIFO element.
 **
 ** This structure defines a dedicated Tx event FIFO element placed on the
 ** message RAM.
 *****************************************************************************/
typedef	struct stc_canfd_tx_event
{
	union {
		struct {
			uint32_t	id		: 29;	// Identifier
			uint32_t	rtr		: 1;	// Remote Transmission Request
			uint32_t	xtd		: 1;	// Extended Identifier
			uint32_t	esi		: 1;	// Error State Indicator
			uint32_t	txts	: 16;	// Tx Timestamp
			uint32_t	dlc		: 4;	// Data Length Code
			uint32_t	brs		: 1;	// Bit Rate Switch
			uint32_t	edl		: 1;	// Extended Data Length
			uint32_t	et		: 2;	// Event Type
			uint32_t	mm		: 16;	// Message Marker
		};
		uint32_t	w[2];				// to access as word
	};
} stc_canfd_tx_event_t;

/**
 *****************************************************************************
 ** \brief CAN FD standard message ID filter element.
 **
 ** This structure defines a standard ID filtering element placed on the
 ** message RAM.
 *****************************************************************************/
#define	CANFD_STDID_INFOSIZE	(1)
typedef struct stc_canfd_stdid_filter
{
	union {
		struct {
			uint32_t	sfid2	: 11;	// Standard Filter ID 2
			uint32_t	rsv		: 5;	// (reserved)
			uint32_t	sfid1	: 11;	// Standard Filter ID 1
			uint32_t	sfec	: 3;	// Standard Filter Element Configuration
			uint32_t	sft		: 2;	// Standard Filter Type
		};
		uint32_t	w;					// to access as word
	};
} stc_canfd_stdid_filter_t;

/**
 *****************************************************************************
 ** \brief CAN FD extended message ID filter element.
 **
 ** This structure defines an extended ID filtering element placed on the
 ** message RAM.
 *****************************************************************************/
#define	CANFD_EXTID_INFOSIZE	(2)
typedef struct stc_canfd_extid_filter
{
	union {
		struct {
			uint32_t	efid1	: 29;	// Extended Filter ID 1
			uint32_t	efec	: 3;	// Extended Filter Element Configuration
			uint32_t	efid2	: 29;	// Extended Filter ID 2
			uint32_t	rsv		: 1;	// (reserved)
			uint32_t	eft		: 2;	// Extended Filter Type
		};
		uint32_t	w[2];				// to access as word
	};
} stc_canfd_extid_filter_t;


/**
 *****************************************************************************
 ** \brief CAN FD internal data
 **
 ** This structure defines an extended ID filtering element placed on the
 ** message RAM.
 *****************************************************************************/
///
typedef	enum en_canfd_confirmation_method
{
	CanfdConfirmPolling   = 0,	// Confirm by polling.
	CanfdConfirmInterrupt = 1		// Confirm by interrupt.
} en_canfd_confirmation_method_t;

///
typedef struct stc_canfd_rx_confirmation
{
	en_canfd_confirmation_method_t	enRxBuffer;
	en_canfd_confirmation_method_t	enRxFIFO0;
	en_canfd_confirmation_method_t	enRxFIFO1;
	en_canfd_confirmation_method_t	enTxBuffer;
	en_canfd_confirmation_method_t	enBusStatus;
	en_canfd_confirmation_method_t	enErrorStatus;
} stc_canfd_rx_confirmation_t;

/// CAN FD instance internal data, storing internal information for each enabled CAN FD instance.
typedef struct stc_canfd_intern_data
{
	en_canfd_mode_t					enCanfdMode;
	en_canfd_clock_t				enCanfdClock;
	stc_canfd_interrupt_handling_t	stcCanfdInterruptHandling;
	stc_canfd_notification_t       	stcCanfdNotificationCb;
	stc_canfd_rx_confirmation_t		stcCanfdRxConfirmation;
	uint8_t							u8TxReqMarker[CANFD_MESSAGE_TXBUFFER_COUNT];
} stc_canfd_intern_data_t;

/// CAN FD instance data type
typedef struct stc_canfd_instance_data
{
	volatile stc_canfdn_t*  pstcInstance;	// pointer to registers of an instance
	stc_canfd_intern_data_t stcInternData;	// module internal data of instance
} stc_canfd_instance_data_t;


/**
 *****************************************************************************
 ** \brief Rx FIFO ID.
 *****************************************************************************/
typedef enum en_canfd_rxfifo_number
{
	CanfdRxFIFO0 = 0,		// Rx FIFO0
	CanfdRxFIFO1 = 1		// Rx FIFO1
} en_canfd_rxfifo_number_t;

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

static stc_canfd_intern_data_t* CanfdGetInternDataPtr( volatile stc_canfdn_t* pstcCanfd );

static void CanfdInitInterrupt( volatile stc_canfdn_t* pstcCanfd );
static void CanfdDeInitInterrupt( volatile stc_canfdn_t* pstcCanfd );

static en_result_t CanfdCheckAndNotifyRxMsg( volatile stc_canfdn_t* pstcCanfd, en_canfd_confirmation_method_t enMethod, uint32_t* pu32Accepted );
static stc_canfd_stdid_filter_t* CanfdCalcStdIdFilterAddress( volatile stc_canfdn_t* pstcCanfd, uint8_t u8Index );
static stc_canfd_extid_filter_t* CanfdCalcExtIdFilterAddress( volatile stc_canfdn_t* pstcCanfd, uint8_t u8Index );
static stc_canfd_rx_buffer_t* CanfdCalcRxBufferAddress( volatile stc_canfdn_t* pstcCanfd, uint8_t u8Index );
static en_result_t CanfdGetRxFIFO( volatile stc_canfdn_t* pstcCanfd, en_canfd_rxfifo_number_t enNumber, stc_canfd_rx_buffer_t* pstcRxMsg );
static en_result_t CanfdConvRxBuffer( stc_canfd_rx_buffer_t* pstcRxBuffer, stc_canfd_msg_t* pstcRxMsg );
static stc_canfd_tx_buffer_t* CanfdCalcTxBufferAddress( volatile stc_canfdn_t* pstcCanfd, uint8_t u8Index );
static en_result_t CanfdClearTxRequestMarker( volatile stc_canfdn_t* pstcCanfd );

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

// Look-up table for all enabled CAN FD instances and their internal data
static stc_canfd_instance_data_t m_astcCanfdInstanceDataLut[] =
{
	#if (PDL_PERIPHERAL_ENABLE_CANFD0 == PDL_ON)
	{
		&CANFD0  	// pstcInstance
		// [andreika]: fix gcc
		,{ 0 }		// stcInternData (not initialized yet)
	},
	#endif
};

// convert Rx/Tx buffer size parameter to real buffer size in word
static const uint8_t u8BufSizeInWord[] = {
	2,			//  2 words ( 8 bytes)
	3,			//  3 words (12 bytes)
	4,			//  4 words (16 bytes)
	5,			//  5 words (20 bytes)
	6,			//  6 words (24 bytes)
	8,			//  8 words (32 bytes)
	12,			// 12 words (48 bytes)
	16			// 16 words (64 bytes)
};

// convert DLC to data size in byte
static const uint8_t u8DlcInByte[] = {
	 0,			//  0 byte
	 1,			//  1 byte
	 2,			//  2 bytes
	 3,			//  3 bytes
	 4,			//  4 bytes
	 5,			//  5 bytes
	 6,			//  6 bytes
	 7,			//  7 bytes
	 8,			//  8 bytes
	12,			// 12 bytes
	16,			// 16 bytes
	20,			// 20 bytes
	24,			// 24 bytes
	32,			// 32 bytes
	48,			// 48 bytes
	64,			// 64 bytes
};

/******************************************************************************/
/* Function implementation - global (no 'static') and local ('static')        */
/******************************************************************************/

/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain CAN FD instance.
 **
 ** \param pstcCanfd Pointer to CAN FD instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_canfd_intern_data_t* CanfdGetInternDataPtr( volatile stc_canfdn_t* pstcCanfd )
{
	uint32_t					u32Instance;
	stc_canfd_intern_data_t*	pstcIntarnData;

	// Initialize address of corresponding intarnal data (NULL).
	pstcIntarnData = NULL;

	// Search internal data.
	for (u32Instance = 0; NULL == pstcIntarnData && CANFD_INSTANCE_COUNT > u32Instance; u32Instance++)
	{
		// If corresponding data was found, ...
		if (pstcCanfd == m_astcCanfdInstanceDataLut[u32Instance].pstcInstance)
		{
			// Set valid address to result pointer.
			pstcIntarnData = &m_astcCanfdInstanceDataLut[u32Instance].stcInternData;
		}
	}

	// Return result.
	return pstcIntarnData;
}

/**
 ******************************************************************************
 ** \brief Set NVIC Interrupt depending on CAN FD instance
 **
 ** \param pstcCanfd Pointer to CAN FD instance
 **
 ******************************************************************************/

static void CanfdInitInterrupt( volatile stc_canfdn_t* pstcCanfd )
{
#if(defined(IRQ_CAN1_AVAILABLE)) 
    NVIC_ClearPendingIRQ(CAN1_IRQn);
    NVIC_EnableIRQ(CAN1_IRQn);
    NVIC_SetPriority(CAN1_IRQn, PDL_IRQ_LEVEL_CAN1_CANFD0);
#elif(defined(IRQ_CAN1_CANFD0_AVAILABLE))    
    NVIC_ClearPendingIRQ(CAN1_CANFD0_IRQn);
    NVIC_EnableIRQ(CAN1_CANFD0_IRQn);
    NVIC_SetPriority(CAN1_CANFD0_IRQn, PDL_IRQ_LEVEL_CAN1_CANFD0);
#elif(defined(IRQ_CANFD0_AVAILABLE))    
    NVIC_ClearPendingIRQ(CANFD0_IRQn);
    NVIC_EnableIRQ(CANFD0_IRQn);
    NVIC_SetPriority(CANFD0_IRQn, PDL_IRQ_LEVEL_CAN1_CANFD0);
#endif
}

/**
 ******************************************************************************
 ** \brief Clear NVIC Interrupt depending on CAN FD instance
 **
 ** \param pstcCanfd Pointer to CAN FD instance
 **
 ******************************************************************************/
static void CanfdDeInitInterrupt( volatile stc_canfdn_t* pstcCanfd )
{
#if(defined(IRQ_CAN1_AVAILABLE)) 
    NVIC_ClearPendingIRQ(CAN1_IRQn);
    NVIC_DisableIRQ(CAN1_IRQn);
    NVIC_SetPriority(CAN1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#elif(defined(IRQ_CAN1_CANFD0_AVAILABLE))   
    NVIC_ClearPendingIRQ(CAN1_CANFD0_IRQn);
    NVIC_DisableIRQ(CAN1_CANFD0_IRQn);
    NVIC_SetPriority(CAN1_CANFD0_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#elif(defined(IRQ_CANFD0_AVAILABLE))   
    NVIC_ClearPendingIRQ(CANFD0_IRQn);
    NVIC_DisableIRQ(CANFD0_IRQn);
    NVIC_SetPriority(CANFD0_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#endif
}


/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/

/**
 *****************************************************************************
 ** \brief CAN FD (Status/Error/Rx/Tx) interrupt ISR
 **
 ** Check for any pending interrupt source and process until no more INTs are
 ** active.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN unit.
 *****************************************************************************/
void CanfdIrqHandler( volatile stc_canfdn_t* pstcCanfd )
{
	stc_canfd_intern_data_t*	pstcCanfdInternData;
	uint8_t                		u8MsgNumber;
	uint32_t					u32IR;
	uint32_t					u32TXBRP;
	uint32_t					u32AccInt;
	uint32_t					u32AccRx;
	en_result_t					enResult;

	// Get pointer to internal data structure.
	pstcCanfdInternData = CanfdGetInternDataPtr( pstcCanfd );

	// Perform interrupt process if specified CAN FD base address was recognized.
	if ( NULL != pstcCanfdInternData )
	{
		// Get interrupt flag register.
		u32IR = pstcCanfd->IR;

		// Clear variables that hold accepted interrupt flags.
		u32AccInt = 0;
		u32AccRx = 0;

		// Check error flags.
		if ( CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enErrorStatus &&
			 NULL != pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdErrorInterruptFunction &&
			 0UL != ( u32IR & CANFD_IR_MASK_ERROR ) )
		{
			// Nortify error status.
			pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdErrorInterruptFunction( u32IR & CANFD_IR_MASK_ERROR );

			// Set value to clear error status change interrupt bit.
			u32AccInt |= CANFD_IR_MASK_ERROR;
		}

		// Check status flags.
		if ( CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enBusStatus &&
			 NULL != pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdStatusInterruptFunction &&
			 0UL != ( u32IR & CANFD_IR_MASK_STATUS ) )
		{
			// Check bus status.
			Canfd_GetBusStatus( pstcCanfd );

			// Set value to clear bus status change interrupt bit.
			u32AccInt |= CANFD_IR_MASK_STATUS;
		}

		// Check reception interrupt.
		if ( 0UL != ( u32IR & CANFD_IR_MASK_RX ) )
		{
			// Check and notify received messages by polling.
			enResult = CanfdCheckAndNotifyRxMsg( pstcCanfd, CanfdConfirmInterrupt, &u32AccRx );

			// If process was done successfully, ...
			if ( Ok == enResult )
			{
				// Set value to clear accepted Rx buffer/FIFO0/FIFO1 new data interrupt bit.
				u32AccInt |= u32AccRx;
			}
		}

		// Check transmission related interrupts.
		if ( CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enTxBuffer )
		{
			// Check transmission finished interrput.
			if ( 0UL != ( u32IR & CANFD_IR_MASK_TX) )
			{
				// Get Tx buffer transmission request pending register to check whether requested transmissions were finished.
				u32TXBRP = pstcCanfd->TXBRP;

				// Loop to check and notify all Tx buffers.
				for ( u8MsgNumber = 0; CANFD_MESSAGE_TXBUFFER_COUNT > u8MsgNumber; u8MsgNumber++ )
				{
					// Check requested transmission state.
					if ( CANFD_TX_REQUESTED == pstcCanfdInternData->u8TxReqMarker[u8MsgNumber] &&
						 0UL == ( u32TXBRP & 1UL ) )
					{
						// Callback if the pointer is valid.
						if ( NULL != pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdTxInterruptFunction )
						{
							pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdTxInterruptFunction( u8MsgNumber );
						}

						// Clear marker.
						pstcCanfdInternData->u8TxReqMarker[u8MsgNumber] = CANFD_TX_NOT_REQUESTED;
					}

					// Shift TXBRP 1bit right to check next Tx buffer.
					u32TXBRP >>= 1;
				}

				// Set value to clear Tx done interrupt bit.
				u32AccInt |= CANFD_IR_MASK_TX;
			}
		}

		// Clear accepted interrupt flags all.
		pstcCanfd->IR = u32AccInt;
	}
} // CanfdIrqHandler

/**
 *****************************************************************************
 ** \brief Initializes the CAN FD module.
 **
 ** This function initializes one specific CAN FD module with the parameters
 ** provided in the given config structure.
 ** After initialization the CAN FD module has Error, Status and Module-Interrupt
 ** enabled and is ready to use.
 **
 ** Canfd_Init() has to be called with the parameter pstcConfig of type
 ** stc_canfd_config_t the basic CAN settings the CAN operation mode, the CAN
 ** baudrate, and the error and status change callback function can be set.
 **
 ** All values in pstcConfig have to be in valid range (see can.h for allowed
 ** ranges of dedicated parameters). The error and status change callback
 ** functions can be NULL. In this case no information of error or status
 ** changes will be reported to the API.
 **
 ** To reset and disable the CAN FD module the function Canfd_DeInit() has to
 ** be used.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 ** \param [in] pstcConfig			CAN FD configuration parameters.
 **
 ** \retval Ok						CAN FD module has been successfully initialized.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfd == NULL
 **   - pstcConfig == NULL
 **   - pstcCanfdInternData == NULL (invalid or disabled CAN FD unit (PDL_PERIPHERAL_ENABLE_CANFD))
 **   - pstcConfig->enCanfdMode parameter out of range
 **   - pstcConfig->enCanfdClock parameter out of range
 *****************************************************************************/
en_result_t Canfd_Init( volatile stc_canfdn_t* pstcCanfd, const stc_canfd_config_t* pstcConfig )
{
	stc_canfd_intern_data_t*  	pstcCanfdInternData;
	uint32_t*					pu32Adrs;
	stc_canfd_stdid_filter_t	stcCanfdStdID;
	uint16_t					u16BRP;				// CAN bit rate prescaler
	uint16_t					u16TSEG1;           // CAN Time segment 1
	uint16_t					u16TSEG2;           // CAN Time segment 2
	uint16_t					u16SJW;				// CAN (Re)synchronization jump width
	uint16_t					u16FBRP = 0u;       // CAN FD bit rate prescaler
	uint16_t					u16TDCO;			// CAN FD transceiver delay compensation offset
	uint16_t					u16TDC;				// CAN FD transceiver delay compensation enable
	uint16_t					u16FTSEG1 = 0u;     // CAN FD Time segment 1
	uint16_t					u16FTSEG2 = 0u;     // CAN FD Time segment 2
	uint16_t					u16FSJW = 0u;       // CAN FD (Re)synchronization jump width
	uint16_t					u16Count;
	en_result_t					enResult;

	// Initialize result value (as 'Ok').
	enResult = Ok;

	// Check for NULL pointers.
	if ( NULL == pstcCanfd ||
		 NULL == pstcConfig
	   )
	{
		enResult = ErrorInvalidParameter;
	}
	// Check CAN operation mode.
	else if ( CanfdModeClassic != pstcConfig->enCanfdMode &&
			  CanfdModeFDFixed != pstcConfig->enCanfdMode &&
			  CanfdModeFDFlex  != pstcConfig->enCanfdMode
			)
	{
		enResult = ErrorInvalidParameter;
	}
	// Check CAN prescaler output clock frequency.
	else if ( CanfdClock32MHz != pstcConfig->enCanfdClock &&
			  CanfdClock40MHz != pstcConfig->enCanfdClock
			)
	{
		enResult = ErrorInvalidParameter;
	}
	// No parameter error.
	else
	{
		// Noting to do.
	}

	// If all parameters are correct, ...
	if ( Ok == enResult )
	{
		// Get pointer to internal data structure.
		pstcCanfdInternData = CanfdGetInternDataPtr( pstcCanfd );

		// Check for NULL.
		if ( NULL == pstcCanfdInternData )
		{
			enResult = ErrorInvalidParameter;
		}
	}

	// If it had been prepared to initialize the CAN FD controller completely, ...
	if ( Ok == enResult )
	{
		// Save CAN operation mode.
		pstcCanfdInternData->enCanfdMode = pstcConfig->enCanfdMode;

		// Initialize Rx confirmation method of Rx buffers and Rx FIFOs (polling).
		pstcCanfdInternData->stcCanfdRxConfirmation.enRxBuffer    = CanfdConfirmPolling;
		pstcCanfdInternData->stcCanfdRxConfirmation.enRxFIFO0     = CanfdConfirmPolling;
		pstcCanfdInternData->stcCanfdRxConfirmation.enRxFIFO1     = CanfdConfirmPolling;
		pstcCanfdInternData->stcCanfdRxConfirmation.enTxBuffer    = CanfdConfirmPolling;
		pstcCanfdInternData->stcCanfdRxConfirmation.enBusStatus   = CanfdConfirmPolling;
		pstcCanfdInternData->stcCanfdRxConfirmation.enErrorStatus = CanfdConfirmPolling;

		// Set receive and transmit callback function.
		pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdTxInterruptFunction = pstcConfig->pfnTransmitMsgCallback;
		pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction = pstcConfig->pfnReceiveMsgCallback;
		// Set notification callback functions
		pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdStatusInterruptFunction = pstcConfig->pfnStatusCallback;
		pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdErrorInterruptFunction  = pstcConfig->pfnErrorCallback;

		// Transit CAN FD controller to initial state.
		pstcCanfd->CCCR_f.INIT = PDL_ON;
		while ( PDL_OFF == pstcCanfd->CCCR_f.INIT )
		{
		}

		// Reset value of configuration registers and enable editing.
		pstcCanfd->CCCR_f.CCE = PDL_ON;

		// Initialize basic controller settings.
		pstcCanfd->CCCR_f.TXP  = 0;	// disable pending transmission
		pstcCanfd->CCCR_f.CME  = 2;	// 10�Fenable full CAN FD operation (long data, flexible Rx/Tx data rate)
		pstcCanfd->CCCR_f.TEST = 0;	// normal operation (not in TEST mode)
		pstcCanfd->CCCR_f.DAR  = 0; // enable automatic retransmission
		pstcCanfd->CCCR_f.MON  = 0;	// disable bus monitor mode
		pstcCanfd->CCCR_f.CSR  = 0;	// disable stopping CAN FD clock
		pstcCanfd->CCCR_f.ASM  = 0;	// not in restricted operation mode

		// Setup bit configuration parameters.
		//   Currently, the bit configuration is applied with constant configuration as follows;
		//
		//   CAN, CAN FD arbitration bits and CAN FD fixed rate data bits configuration:
		//                     	32MHz	40MHz
		//     Tq frequency		8MHz	<-
		//     PropSeg+Tseg1	12Tq	<-
		//     Tseg2			4Tq		<-
		//     SJW				4Tq		<-
		//     Bit rate			500kHz	<-
		//     Sampling point   75.0%	<-
		//
		//   CAN FD flexible rate data bits configuration:
		//                     	32MHz	40MHz
		//     Tq frequency		32MHz	40MHz
		//     PropSeg+Tseg1	6Tq		8Tq
		//     Tseg2			2Tq		<-
		//     SJW				2Tq		<-
		//     Bit rate			4MHz	<-
		//     Sampling point   75.0%	80.0%
		//
		if ( CanfdClock32MHz == pstcConfig->enCanfdClock )
		{
			u16BRP   = (4 - 1);			// CAN bit rate prescaler
			u16TSEG1 = (11 - 1);		// CAN Tseg1
			u16TSEG2 = (4 - 1);			// CAN Tseg2
			u16SJW   = (4 - 1);			// CAN SJW
			if ( CanfdModeFDFixed == pstcConfig->enCanfdMode )
			{
				u16FBRP   = (4 - 1);				// CAN FD bit rate prescaler
				u16TDCO   = 2;						// Transceiver Delay Compensation Offset
				u16TDC    = (0 == u16TDCO) ? 0 : 1;	// Transceiver Delay Compensation enable
				u16FTSEG1 = (11 - 1);				// CAN FD Tseg1
				u16FTSEG2 = (4 - 1);				// CAN FD Tseg2
				u16FSJW   = (4 - 1);				// CAN FD SJW
			}
			else if ( CanfdModeFDFlex == pstcConfig->enCanfdMode )
			{
				u16FBRP   = (1 - 1);				// CAN FD bit rate prescaler
				u16TDCO   = 2;						// Transceiver Delay Compensation Offset
				u16TDC    = (0 == u16TDCO) ? 0 : 1;	// Transceiver Delay Compensation enable
				u16FTSEG1 = (5 - 1);				// CAN FD Tseg1
				u16FTSEG2 = (2 - 1);				// CAN FD Tseg2
				u16FSJW   = (2 - 1);				// CAN FD SJW
			}
		}
		else // if ( CanfdClock40MHz == pstcConfig->enCanfdClock )
		{
			u16BRP   = (5 - 1);			// CAN bit rate prescaler
			u16TSEG1 = (11 - 1);		// CAN Tseg1
			u16TSEG2 = (4 - 1);			// CAN Tseg2
			u16SJW   = (4 - 1);			// CAN SJW
			if ( CanfdModeFDFixed == pstcConfig->enCanfdMode )
			{
				u16FBRP   = (5 - 1);				// CAN FD bit rate prescaler
				u16TDCO   = 2;						// Transceiver Delay Compensation Offset
				u16TDC    = (0 == u16TDCO) ? 0 : 1;	// Transceiver Delay Compensation enable
				u16FTSEG1 = (11 - 1);				// CAN FD Tseg1
				u16FTSEG2 = (4 - 1);				// CAN FD Tseg2
				u16FSJW   = (4 - 1);				// CAN FD SJW
			}
			else if ( CanfdModeFDFlex == pstcConfig->enCanfdMode )
			{
				u16FBRP   = (1 - 1);				// CAN FD bit rate prescaler
				u16TDCO   = 2;						// Transceiver Delay Compensation Offset
				u16TDC    = (0 == u16TDCO) ? 0 : 1;	// Transceiver Delay Compensation enable
				u16FTSEG1 = (7 - 1);				// CAN FD Tseg1
				u16FTSEG2 = (2 - 1);				// CAN FD Tseg2
				u16FSJW   = (2 - 1);				// CAN FD SJW
			}
		}

		// Setup bit configuration.
		// (common parameters)
		pstcCanfd->BTP_f.BRP   = u16BRP;	// bit rate prescaler
		pstcCanfd->BTP_f.TSEG1 = u16TSEG1;	// Tseg1
		pstcCanfd->BTP_f.TSEG2 = u16TSEG2;	// Tseg2
		pstcCanfd->BTP_f.SJW   = u16SJW;	// SJW
		// (CAN FD parameters)
		if ( CanfdModeFDFixed == pstcConfig->enCanfdMode || CanfdModeFDFlex == pstcConfig->enCanfdMode )
		{
			pstcCanfd->FBTP_f.FBRP   = u16FBRP;		// bit rate prescaler
			pstcCanfd->FBTP_f.TDCO   = u16TDCO;		// Transceiver Delay Compensation Offset
			pstcCanfd->FBTP_f.TDC    = u16TDC;		// Transceiver Delay Compensation enable
			pstcCanfd->FBTP_f.FTSEG1 = u16FTSEG1;	// Tseg1
			pstcCanfd->FBTP_f.FTSEG2 = u16FTSEG2;	// Tseg2
			pstcCanfd->FBTP_f.FSJW   = u16FSJW;		// SJW
		}

		// interrupt configuration.
		pstcCanfd->IE  = 0x00000000UL;		// disable CAN FD interrupts all
		pstcCanfd->ILE = 0x00000000UL;		// disable CAN FD line interrupts all
		pstcCanfd->ILS = 0x00000000UL;		// assign all CAN FD interrupts to the interrupt line #0

		// global filter configuration.
		pstcCanfd->GFC = 0x00000000UL;
		pstcCanfd->GFC_f.ANFS = 3;			// discard message if its standard ID doesn't match any filters.
		pstcCanfd->GFC_f.ANFE = 3;			// discard message if its extended ID doesn't match any filters.
		pstcCanfd->GFC_f.RRFS = 1;			// discard remote frame with standard ID.
		pstcCanfd->GFC_f.RRFE = 1;			// discard remote frame with extended ID.

		// standard ID filter basic configuration.
		pstcCanfd->SIDFC = 0x00000000UL;
		pstcCanfd->SIDFC_f.LSS    = 2;		// number of standard ID filter.
		pstcCanfd->SIDFC_f.FLSSA = 0x0000;	// offset address of standard ID filter section.

		// extended ID filter basic configuration.
		pstcCanfd->XIDFC = 0x00000000UL;
		pstcCanfd->XIDFC_f.LSE   = 0;			// number of standard ID filter.
		pstcCanfd->XIDFC_f.FLESA = 0x0010;		// offset address of extended ID filter section.
		pstcCanfd->XIDAM_f.EIDM  = 0x1fffffff;	// no pre-filtering

		// Rx FIFO0 basic configration.
		pstcCanfd->RXF0C = 0x00000000UL;
		pstcCanfd->RXF0C_f.F0OM = 0;		// blocking mode.
		pstcCanfd->RXF0C_f.F0WM = 0;		// disable watermark interrupt.
		pstcCanfd->RXF0C_f.F0S  = 2;		// number of elements.
		pstcCanfd->RXF0C_f.F0SA = 0x0100;	// offset address of Rx FIFO0 section.

		// Rx FIFO1 basic configration.
		pstcCanfd->RXF1C = 0x00000000UL;
		pstcCanfd->RXF1C_f.F1OM = 0;		// blocking mode.
		pstcCanfd->RXF1C_f.F1WM = 0;		// disable watermark interrupt.
		pstcCanfd->RXF1C_f.F1S  = 0;		// number of elements.
		pstcCanfd->RXF1C_f.F1SA = 0x0200;	// offset address of Rx FIFO0 section.

		// dedicated Rx buffer basic configration.
		pstcCanfd->RXBC = 0x00000000UL;
		pstcCanfd->RXBC_f.RBSA = 0x0300;	// offset address of Rx buffer section.

		// received data size.
		pstcCanfd->RXESC = 0x00000000UL;
		pstcCanfd->RXESC_f.RBDS = 7;		// Rx data maximum length (64bytes).
		pstcCanfd->RXESC_f.F0DS = 7;		// Rx FIFO0 data maximum length (64bytes).
		pstcCanfd->RXESC_f.F1DS = 7;		// Rx FIFO1 data maximum length (64bytes).

		// Tx FIFO/QUEUE configration.
		pstcCanfd->TXEFC = 0x00000000UL;
		pstcCanfd->TXEFC_f.EFWM = 0;		// disable watermark interrupt.
		pstcCanfd->TXEFC_f.EFS  = 0;		// number of elements.
		pstcCanfd->TXEFC_f.EFSA = 0x0400;	// offset address of Tx FIFO/QUEUE.

		// Tx buffer configration.
		pstcCanfd->TXBC = 0x00000000UL;
		pstcCanfd->TXBC_f.TFQM = 0;			// select FIFO or QUEUE (refer to TFQS).
		pstcCanfd->TXBC_f.TFQS = 0;			// use Tx FIFO/QUEUE or not.
		pstcCanfd->TXBC_f.NDTB = 1;			// number of Tx buffer elements.
		pstcCanfd->TXBC_f.TBSA = 0x0500;	// offset address of Tx byffer section.

		// Tx buffer/FIFO/QUEUE configration.
		pstcCanfd->TXESC = 0x00000000UL;
		pstcCanfd->TXESC_f.TBDS = 7;		// maximum length of Tx buffer/FIFO/QUEUE (64bytes)

		// Clear whole of message RAM (by zero).
		//   Note; Message RAM must be initialized by writing before accessing. Or ECC error was detected.
		pu32Adrs = (uint32_t*)((uint8_t*)pstcCanfd + CANFD_MSGRAM_OFFSET);
		for ( u16Count = 0; CANFD_MSGRAM_SIZEW > u16Count; u16Count++ )
		{
			*pu32Adrs++ = 0UL;
		}

		// Driver special configuration.
		//   This driver accepts messages only that has identifier 0x100 or 0x200.
		//   The message that has ID 0x100 is stored into dedicated Rx buffer #0, and it is obtained by interrupt.
		//   The message that has ID 0x200 is stored into Rx FIFO0, and it is obtained by polling.

		// Clear transmission request marker all.
		CanfdClearTxRequestMarker( pstcCanfd );

		// Rx message confiramtion method (interrupt).
		pstcCanfdInternData->stcCanfdRxConfirmation.enRxBuffer = CanfdConfirmInterrupt;
		// [andreika]: fix TX buffer interrupts
		pstcCanfdInternData->stcCanfdRxConfirmation.enTxBuffer = CanfdConfirmInterrupt;

		// Setup ID filters.
		// [andreika]: todo: the following should be controlled externally!

		// (standard ID : message #0)
		stcCanfdStdID.w = 0UL;
		stcCanfdStdID.sft   = 2;		// filter typr : classic (ignored, see SFEC)
		stcCanfdStdID.sfec  = 7;		// stored to : dedicated Rx buffer or debug message
		stcCanfdStdID.sfid1 = 0x100;	// ID : 0x100
		stcCanfdStdID.sfid2 = (0 << 9)	// storage detail : dedicated Rx buffer, index 0
							| 0;
		pu32Adrs = (uint32_t*)CanfdCalcStdIdFilterAddress( pstcCanfd, 0 );
		pu32Adrs[0] = stcCanfdStdID.w;

		// (standard ID : message #1)
		stcCanfdStdID.w = 0UL;
		stcCanfdStdID.sft   = 2;		// filter typr : classic
		stcCanfdStdID.sfec  = 1;		// stored to : Rx FIFO0
		stcCanfdStdID.sfid1 = 0x200;	// ID   : 0x200
		stcCanfdStdID.sfid2 = 0x7FF;	// MASK : 0x7FF (accept 0x200 only)
		pu32Adrs = (uint32_t*)CanfdCalcStdIdFilterAddress( pstcCanfd, 1 );
		pu32Adrs[0] = stcCanfdStdID.w;

		// Initialize CAN FD related interrupt.
		CanfdInitInterrupt(pstcCanfd);
	}

	// Return result.
	return enResult;
} // Canfd_Init

/**
 *****************************************************************************
 ** \brief Deinitializes the CAN FD module.
 ** Any pending transmission or receiption will be aborted and all CAN FD related
 ** registers are reset to their default values.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 **
 ** \retval Ok						CAN FD module has been successfully deinitialized.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfd == NULL
 **   - pstcCanfdInternData == NULL (invalid or disabled CAN FD unit
 **                                 (PDL_PERIPHERAL_ENABLE_CANFD))
 *****************************************************************************/
en_result_t Canfd_DeInit( volatile stc_canfdn_t* pstcCanfd )
{
	stc_canfd_intern_data_t*	pstcCanfdInternData;	// Pointer to internal data
	en_result_t					enResult;

	// Check for NULL pointer.
	if ( NULL == pstcCanfd )
	{
		enResult = ErrorInvalidParameter;
	}
	// Get pointer to internal data structure.
	else if ( NULL == ( pstcCanfdInternData = CanfdGetInternDataPtr( pstcCanfd ) ) )
	{
		enResult = ErrorInvalidParameter;
	}
	// If it had been prepared to initialize, ...
	else
	{
		// Transit CAN FD controller to initial state.
		pstcCanfd->CCCR_f.INIT = PDL_ON;
		while ( PDL_OFF == pstcCanfd->CCCR_f.INIT )
		{
		}

		// Deinitialize CAN FD related interrupt.
		CanfdDeInitInterrupt(pstcCanfd);

		// Reset value of configuration registers and enable editing.
		pstcCanfd->CCCR_f.CCE = PDL_ON;

		// Set initial value to all registers included to the CAN FD controller.
		pstcCanfd->CCCR &= ~0x00000003UL;	// CCR (initialize except for INIT and CCE)
											// CREL (read only)
											// ENDN (read only)
											// CUST (not present)
		pstcCanfd->FBTP   = 0x00000A33UL;	// FBTP
		pstcCanfd->TEST   = 0x00000000UL;	// TEST
		pstcCanfd->RWD    = 0x00000000UL;	// RWD
		pstcCanfd->BTP    = 0x00000A33UL;	// BTP
		pstcCanfd->TSCC   = 0x00000000UL;	// TSCC
		pstcCanfd->TSCV   = 0x00000000UL;	// TSCV
		pstcCanfd->TOCC   = 0x00000000UL;	// TOCC
		pstcCanfd->TOCV   = 0x00000000UL;	// TOCV
											// ECR (read only)
											// PSR (read only)
		pstcCanfd->IE     = 0x00000000UL;	// IE (initialize before IR)
		pstcCanfd->IR     = 0xFFFFFFFFUL;	// IR (write '1' to clear)
		pstcCanfd->ILS    = 0x00000000UL;	// ILS
		pstcCanfd->ILE    = 0x00000000UL;	// ILE
		pstcCanfd->GFC    = 0x00000000UL;	// GFC
		pstcCanfd->SIDFC  = 0x00000000UL;	// SIDFC
		pstcCanfd->XIDFC  = 0x00000000UL;	// XIDFC
		pstcCanfd->XIDAM  = 0x1FFFFFFFUL;	// XIDAM
											// HPMS (read only)
		pstcCanfd->NDAT1  = 0xFFFFFFFFUL;	// NDAT1 (write '1' to clear)
		pstcCanfd->NDAT2  = 0xFFFFFFFFUL;	// NDAT2 (write '1' to clear)
		pstcCanfd->RXF0C  = 0x00000000UL;	// RXF0C
											// RXF0S (read only)
											// RXF0A (to write to this affects other registers)
		pstcCanfd->RXBC   = 0x00000000UL;	// RXBC
		pstcCanfd->RXF1C  = 0x00000000UL;	// RXF1C
											// RXF1S (read only)
											// RXF1A (to write to this affects other registers)
		pstcCanfd->RXESC  = 0x00000000UL;	// RXESC
		pstcCanfd->TXBC   = 0x00000000UL;	// TXBC
											// TXFQS (read only)
		pstcCanfd->TXESC  = 0x00000000UL;	// TXESC
											// TXBRP (read only)
											// TXBAR (to write to this triggers the function)
											// TXBCR (to write to this triggers the function)
											// TXBTO (read only)
											// TXBCF (read only)
		pstcCanfd->TXBTIE = 0x00000000UL;	// TXBTIE
		pstcCanfd->TXBCIE = 0x00000000UL;	// TXBCIE
		pstcCanfd->TXEFC  = 0x00000000UL;	// TXEFC
											// TXEFS (read only)
											// TXEFA (to write to this affects other registers)
		pstcCanfd->FDECR   = 0x00UL;		// FDECR
											// FDESR (read only)
											// FDSEAR (read only)
											// FDESCR (to write to this affects other registers)
											// FDDEAR (read only)

		// Disable access to any configuration registers.
		pstcCanfd->CCCR_f.CCE = PDL_OFF;

		// Reset receive and transmit callback function.
		pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdTxInterruptFunction = NULL;
		pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction = NULL;

		// Reset notification callback functions
		pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdStatusInterruptFunction = NULL;
		pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdErrorInterruptFunction  = NULL;

		// Clear transmission request marker all.
		CanfdClearTxRequestMarker( pstcCanfd );

		// Set 'Ok' to result.
		enResult = Ok;
	}

	// Return result
	return enResult;
} // Canfd_DeInit

/**
 *****************************************************************************
 ** \brief Start communication.
 ** Start (enable) CAN communication.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 **
 ** \retval Ok						CAN FD module has been successfully started.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfd == NULL
 **   - pstcCanfdInternData == NULL
 *****************************************************************************/
en_result_t Canfd_Start( volatile stc_canfdn_t* pstcCanfd )
{
	stc_canfd_intern_data_t*	pstcCanfdInternData;	// Pointer to internal data
	uint32_t					u32ActiveInt;
	en_result_t					enResult;

	// Check for NULL pointer.
	if ( NULL == pstcCanfd )
	{
		enResult = ErrorInvalidParameter;
	}
	// Get pointer to internal data structure.
	else if ( NULL == ( pstcCanfdInternData = CanfdGetInternDataPtr( pstcCanfd ) ) )
	{
		enResult = ErrorInvalidParameter;
	}
	// Start communication.
	else
	{
		// Leave from initial state.
		pstcCanfd->CCCR_f.INIT = PDL_OFF;
		while ( PDL_ON == pstcCanfd->CCCR_f.INIT )
		{
		}

		// Change CAN operation mode to desired one.
		// (classic CAN (CAN2.0))
		if ( CanfdModeClassic == pstcCanfdInternData->enCanfdMode )
		{
			// 11�FISO11898-1 standard operation.
			pstcCanfd->CCCR_f.CMR = 3;
		}
		// (CAN FD with fixed data rate)
		else if ( CanfdModeFDFixed == pstcCanfdInternData->enCanfdMode )
		{
			// 01�FCAN FD (fixed data rate) operation.
			pstcCanfd->CCCR_f.CMR = 1;
		}
		// (CAN FD with flexible data rate)
		else if ( CanfdModeFDFlex == pstcCanfdInternData->enCanfdMode )
		{
			// 10�FCAN FD (flexible data rate) operation.
			pstcCanfd->CCCR_f.CMR = 2;
		}
		// (others (invalid parameters))
		else
		{
			// 11�FISO11898-1 standard operation.
			pstcCanfd->CCCR_f.CMR = 3;
		}
		// (Wait until the operation mode is changed to specified mode)
		while ( 0 != pstcCanfd->CCCR_f.CMR )
		{
		}

		// Enable CAN FD interrupts.
		u32ActiveInt = ( ( CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enRxBuffer    ) ? CANFD_IR_MASK_RXBUF   : 0UL )
					 | ( ( CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enRxFIFO0     ) ? CANFD_IR_MASK_RXFIFO0 : 0UL )
					 | ( ( CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enRxFIFO1     ) ? CANFD_IR_MASK_RXFIFO1	: 0UL )
					 | ( ( CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enTxBuffer    ) ? CANFD_IR_MASK_TX	    : 0UL )
					 | ( ( CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enBusStatus   ) ? CANFD_IR_MASK_STATUS  : 0UL )
					 | ( ( CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enErrorStatus ) ? CANFD_IR_MASK_ERROR   : 0UL );
		pstcCanfd->IE  = u32ActiveInt;	// enable CAN interrupts
		pstcCanfd->ILE = 0x00000001UL;	// enable CAN line 0 interrupts
		
		// [andreika]: fix TX interrupts
		if (CanfdConfirmInterrupt == pstcCanfdInternData->stcCanfdRxConfirmation.enTxBuffer) {
			pstcCanfd->TXBTIE = 0xffffffffUL;	// Tx Buffer Transmission Interrupt Enable
			pstcCanfd->TXBCIE = 0xffffffffUL;	// Tx Buffer Cancellation Finished Interrupt Enable (TXBCIE)
		}

		// Set 'Ok' to result.
		enResult = Ok;
	}

	// Return result.
	return enResult;
} // Canfd_Start

/**
 *****************************************************************************
 ** \brief Stop communication.
 ** Stop (disable) CAN communication.
 ** Any pending transmission or receiption will be aborted and all CAN FD statuses
 ** are reset (cleared).
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 **
 ** \retval Ok						CAN FD module has been successfully stopped.
 ** \retval ErrorInvalidParameter	If following condition is met:
 **   - pstcCanfd == NULL
 *****************************************************************************/
en_result_t Canfd_Stop( volatile stc_canfdn_t* pstcCanfd )
{
	en_result_t					enResult;

	// Check for NULL pointer.
	if ( NULL == pstcCanfd )
	{
		enResult = ErrorInvalidParameter;
	}
	// Stop communication.
	else
	{
		// Transit CAN FD controller to initial state.
		pstcCanfd->CCCR_f.INIT = PDL_ON;
		while ( PDL_OFF == pstcCanfd->CCCR_f.INIT )
		{
		}

		// Set CCE to clear current status and to prepare to restart correctly.
		pstcCanfd->CCCR_f.CCE = PDL_ON;

		// Clear ASM to leave from the restricted operation mode if CAN FD controller was in that mode.
		pstcCanfd->CCCR_f.ASM = PDL_OFF;

		// disable CAN FD interrupts.
		pstcCanfd->IE  = 0x00000000UL;	// disable CAN FD interrupts all
		pstcCanfd->ILE = 0x00000000UL;	// disable CAN FD line interrupts all

		// Set 'Ok' to result.
		enResult = Ok;
	}

	// Return result.
	return enResult;
} // Canfd_Stop

/**
 *****************************************************************************
 ** \brief Restart communication.
 ** Restart CAN communication.
 ** This funnction stops and starts communication.
 ** Any pending transmission or receiption will be aborted and all CAN FD statuses
 ** are reset (cleared) in the stop process.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 **
 ** \retval Ok						CAN FD module has been successfully restarted.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfd == NULL (Canfd_Stop)
 **   - pstcCanfdInternData == NULL (Canfd_Start)
 *****************************************************************************/
en_result_t Canfd_Restart( volatile stc_canfdn_t* pstcCanfd )
{
	en_result_t enResult;

	// Stop communication.
	enResult = Canfd_Stop( pstcCanfd );

	// If communication was stopped successfully, ...
	if ( Ok == enResult )
	{
		// Clear interrupt flags and new data arrival flags all.
		pstcCanfd->IR     = 0xFFFFFFFFUL;	// IR (write '1' to clear)
		pstcCanfd->NDAT1  = 0xFFFFFFFFUL;	// NDAT1 (write '1' to clear)
		pstcCanfd->NDAT2  = 0xFFFFFFFFUL;	// NDAT2 (write '1' to clear)

		// Clear transmission request marker all.
		CanfdClearTxRequestMarker( pstcCanfd );

		// (Re)Start communication.
		enResult = Canfd_Start( pstcCanfd );
	}

	// Return result
	return enResult;
} // Canfd_Restart

/**
 *****************************************************************************
 ** \brief Configures a message buffer for transmission.
 ** Setting of new values is not possible if a transmission is pending.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 ** \param [in] u8MsgBuf			Message buffer index (0 .. CANFD_MESSAGE_TXBUFFER_COUNT - 1)
 ** \param [in] pstcMsg				CAN message to be transmitted.
 **
 ** \retval Ok						Transmission request has been successfully accepted.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfd == NULL
 **   - u8MsgBuf out of range
 **   - pstcMsg == NULL
 **   - pstcCanfdInternData == NULL (invalid or disabled CAN FD unit (PDL_PERIPHERAL_ENABLE_CANFD))
 ** \retval ErrorOperationInProgress  If following condition is met:
 **   - A transmission is pending.
 *****************************************************************************/
en_result_t Canfd_TransmitMsg( volatile stc_canfdn_t* pstcCanfd,
							   uint8_t u8MsgBuf,
							   stc_canfd_msg_t* pstcMsg
							 )
{
	stc_canfd_intern_data_t*	pstcCanfdInternData; // Pointer to internal data
	stc_canfd_tx_buffer_t*		pstcTxBuffer;
	uint8_t						u8DataSize;
	uint8_t						u8Count;
	en_result_t					enResult;

	// Initialize result value (as 'Ok')
	enResult = Ok;

	// Check parameters.
	if ( NULL == pstcCanfd ||
		 CANFD_MESSAGE_TXBUFFER_COUNT <= u8MsgBuf ||
		 NULL == pstcMsg
	   )
	{
		enResult = ErrorInvalidParameter;
	}
	// Get pointer to internal data structure.
	else if ( NULL == ( pstcCanfdInternData = CanfdGetInternDataPtr( pstcCanfd ) ) )
	{
		enResult = ErrorInvalidParameter;
	}
	// Check whether the target Tx buffer is ready to accept new request.
	else if ( 0UL != ( pstcCanfd->TXBRP & (1UL << u8MsgBuf) ) )
	{
		enResult = ErrorOperationInProgress;
	}
	// If all parameters are correct and Tx buffer is ready to accept new transmission request, ...
	else
	{
		// Get Tx Buffer address
		pstcTxBuffer = CanfdCalcTxBufferAddress( pstcCanfd, u8MsgBuf );

		// If obtained Tx buffer address is invalid, set error to result.
		if ( NULL == pstcTxBuffer )
		{
			enResult = Error;
		}
		// Otherwise, ...
		else
		{
			// Set marker.
			pstcCanfdInternData->u8TxReqMarker[u8MsgBuf] = CANFD_TX_REQUESTED;

			// Set message attributes to Tx buffer.
			pstcTxBuffer->rtr = 0;
			pstcTxBuffer->xtd = ( TRUE == pstcMsg->stcIdentifier.bExtended ) ? 1 : 0;
			pstcTxBuffer->id  = ( 0 == pstcTxBuffer->xtd ) ? ( pstcMsg->stcIdentifier.u32Identifier << 18 )
														   : pstcMsg->stcIdentifier.u32Identifier;
			pstcTxBuffer->efc = 0;
			pstcTxBuffer->mm  = 0;
			pstcTxBuffer->dlc = pstcMsg->stcData.u8DataLengthCode;

			// Convert the DLC to word size.
			u8DataSize = u8DlcInByte[pstcTxBuffer->dlc];
			if ( CanfdModeClassic == pstcCanfdInternData->enCanfdMode &&
				 CANFD_MAX_STDDLC < u8DataSize )
			{
				u8DataSize = CANFD_MAX_STDDLC;
			}
			u8DataSize = ( u8DataSize + 3 ) >> 2;

			// Copy data to Tx buffer.
			for ( u8Count = 0; u8Count < u8DataSize; u8Count++ )
			{
				pstcTxBuffer->data[u8Count] = pstcMsg->stcData.au32Data[u8Count];
			}

			// Request to transmit
			pstcCanfd->TXBAR |= ( 1UL << u8MsgBuf );
		}
	}

	// Return result.
	return enResult;
}

/**
 *****************************************************************************
 ** \brief Get all received messages by polling.
 ** Check and notify all received messages by polling.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 **
 ** \retval Ok						Message buffer for reception has been succesfully set.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfd == NULL
 **   - pstcCanfdInternData == NULL (invalid or disabled CAN FD unit (PDL_PERIPHERAL_ENABLE_CANFD))
 *****************************************************************************/
en_result_t Canfd_ReceiveMsg( volatile stc_canfdn_t* pstcCanfd )
{
	uint32_t	u32Accepted;
	en_result_t	enResult;

	// Check and notify received messages by polling.
	enResult = CanfdCheckAndNotifyRxMsg( pstcCanfd, CanfdConfirmPolling, &u32Accepted );

	// If process was done successfully, ...
	if ( Ok == enResult )
	{
		// Clear Rx FIFO1 new data interrupt bit.
		pstcCanfd->IR = u32Accepted;
	}

	// Return result.
	return enResult;
}

/**
 *****************************************************************************
 ** \brief Get current bus status.
 **
 ** Calculate the top of address of standard ID filter element that is
 ** corresponding to specified index.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 **
 ** \retval Ok						Process succeeded.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfd == NULL
 **   - pstcCanfdInternData == NULL
 *****************************************************************************/
en_result_t Canfd_GetBusStatus( volatile stc_canfdn_t* pstcCanfd )
{
	stc_canfd_intern_data_t* pstcCanfdInternData;	// Pointer to internal data
	en_result_t enResult;

	// Check parameters; if parameter error was detected, ...
	if ( NULL == pstcCanfd )
	{
		// Invalid parameter.
		enResult = ErrorInvalidParameter;
	}
	// Otherwise, ...
	else
	{
		// Get pointer to internal data structure.
		pstcCanfdInternData = CanfdGetInternDataPtr( pstcCanfd );

		// If the internal data pointer was gotten, ...
		if ( NULL != pstcCanfdInternData )
		{
			// Notify current bus status by calling user hook function if it is available.
			if ( NULL != pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdStatusInterruptFunction )
			{
				// Check and notify current bus status by calling user hook function.
				if ( PDL_ON == pstcCanfd->IR_f.BO )
				{
					// BusOff status.
					pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdStatusInterruptFunction(CanfdBusOff);
				}
				else if ( PDL_ON == pstcCanfd->IR_f.EW )
				{
					// ErrorWarning status.
					pstcCanfdInternData->stcCanfdNotificationCb.pfnCanfdStatusInterruptFunction(CanfdWarning);
				}
				else
				{
					// Do nothing.
				}
			}

			// Finish process successfully.
			enResult = Ok;
		}
		// Otherwise, ...
		else
		{
			// Invalid parameter.
			enResult = ErrorInvalidParameter;
		}

		// Clear bus status related bits in IR.
		pstcCanfd->IR = CANFD_IR_MASK_STATUS;
	}

	// Return result
	return enResult;
}


/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/

/**
 *****************************************************************************
 ** \brief Check and notify received messages.
 **
 ** Check and notify received messages via Rx buffer, FIFO0 and FIFO1.
 ** The target is the object that its confirmation method is same as the
 ** parameter 'enMethod'.
 ** Note that the value of the parameter 'pstcCanfd' is not checked.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 ** \param [in] enMethod			Confirmation method.
 ** \param [in] pu32Accepted		Address to store flags that were accepted in this function.
 **
 ** \retval Ok						Process was done successfully.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfdInternData == NULL
 **   - pu32Accepted == NULL
 *****************************************************************************/
static en_result_t CanfdCheckAndNotifyRxMsg( volatile stc_canfdn_t* pstcCanfd,
											 en_canfd_confirmation_method_t enMethod,
											 uint32_t* pu32Accepted )
{
	stc_canfd_intern_data_t*	pstcCanfdInternData;
	uint8_t                		u8MsgNumber;
	stc_canfd_rx_buffer_t*		pstcRxBuffer;
	stc_canfd_rx_buffer_t		stcRxBufTemp;
	union {
		uint32_t				IR;
		stc_canfd_ir_field_t	IR_f;
	} unCanfdIrRegister;
	uint32_t					u32ND1;
	uint32_t					u32ND2;
	uint32_t					u32NDMask;
	stc_canfd_msg_t				stcRxMsg;
	uint32_t					u32AccInt;
	en_result_t					enResult;

	// Get pointer to internal data structure.
	pstcCanfdInternData = CanfdGetInternDataPtr( pstcCanfd );

	// Return immediately when specified CAN FD base address could not be recognized.
	if ( NULL == pstcCanfdInternData ||
		 NULL == pu32Accepted )
	{
		enResult = ErrorInvalidParameter;
	}
	// Otherwise, ...
	else
	{
		// Get interrupt flag register.
		unCanfdIrRegister.IR = pstcCanfd->IR;

		// Clear accepted interrupt flags.
		u32AccInt = 0UL;

		// If some messages were received and stored into dedicated Rx buffer, ...
		if ( enMethod == pstcCanfdInternData->stcCanfdRxConfirmation.enRxBuffer &&
			 0 != unCanfdIrRegister.IR_f.DRX )
		{
			// Get new data arrived flags.
			u32ND1 = pstcCanfd->NDAT1;
			u32ND2 = pstcCanfd->NDAT2;

			// Check dedicated Rx buffer #0 to #31.
			for ( u8MsgNumber = 0, u32NDMask = 1UL; 0UL != u32NDMask; u8MsgNumber++, u32NDMask <<= 1 )
			{
				// Check one buffer
				if ( 0UL != ( u32ND1 & u32NDMask ) )
				{
					// If reception event collback is available, ...
					if ( NULL != pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction )
					{
						// Get address of target message.
						pstcRxBuffer = CanfdCalcRxBufferAddress( pstcCanfd, u8MsgNumber );
						// Convert raw Rx buffer content to common CAN message content.
						CanfdConvRxBuffer( pstcRxBuffer, &stcRxMsg );
						// Notify message reception event.
						pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction( u8MsgNumber, &stcRxMsg );
					}

					// Clear new data bit.
					pstcCanfd->NDAT1 |= u32NDMask;
				}
			}

			// Check dedicated Rx buffer #32 to #63.
			for ( u32NDMask = 1UL; 0UL != u32NDMask; u8MsgNumber++, u32NDMask <<= 1 )
			{
				// Check one buffer.
				if ( 0UL != ( u32ND2 & u32NDMask ) )
				{
					// If reception event collback is available, ...
					if ( NULL != pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction )
					{
						// Get address of target message.
						pstcRxBuffer = CanfdCalcRxBufferAddress( pstcCanfd, u8MsgNumber );
						// Convert raw Rx buffer content to common CAN message content.
						CanfdConvRxBuffer( pstcRxBuffer, &stcRxMsg );
						// Notify message reception event.
						pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction( u8MsgNumber, &stcRxMsg );
					}

					// Clear new data bit.
					pstcCanfd->NDAT2 |= u32NDMask;
				}
			}

			// Set accepted flag to clear dedicated Rx biffer new data bit.
			u32AccInt |= CANFD_IR_MASK_RXBUF;
		}

		// Check and notify Rx FIFO0 messages.
		if ( enMethod == pstcCanfdInternData->stcCanfdRxConfirmation.enRxFIFO0 &&
			 0 != unCanfdIrRegister.IR_f.RF0N )
		{
			// If reception event collback is available, ...
			if ( NULL != pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction )
			{
				// Get all received messages.
				while ( Ok == CanfdGetRxFIFO( pstcCanfd, CanfdRxFIFO0, &stcRxBufTemp ) )
				{
					// Convert raw Rx FIFO content to common CAN message content.
					CanfdConvRxBuffer( &stcRxBufTemp, &stcRxMsg );
					// Notify message reception event.
					pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction( CANFD_MSGIDX_FIFO0, &stcRxMsg );
				}
			}

			// Set accepted flag to clear Rx FIFO0 new data interrupt bit.
			u32AccInt |= CANFD_IR_MASK_RXFIFO0;
		}

		// Check and notify Rx FIFO1 messages.
		if ( enMethod == pstcCanfdInternData->stcCanfdRxConfirmation.enRxFIFO1 &&
			 0 != unCanfdIrRegister.IR_f.RF1N )
		{
			// If reception event collback is available, ...
			if ( NULL != pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction )
			{
				// Get all received messages.
				while ( Ok == CanfdGetRxFIFO( pstcCanfd, CanfdRxFIFO1, &stcRxBufTemp ) )
				{
					// Convert raw Rx FIFO content to common CAN message content.
					CanfdConvRxBuffer( &stcRxBufTemp, &stcRxMsg );
					// Notify message reception event.
					pstcCanfdInternData->stcCanfdInterruptHandling.pfnCanfdRxInterruptFunction( CANFD_MSGIDX_FIFO1, &stcRxMsg );
				}
			}

			// Set accepted flag to clear Rx FIFO1 new data interrupt bit.
			u32AccInt |= CANFD_IR_MASK_RXFIFO1;
		}

		// Store value of accepted flags to specified address.
		*pu32Accepted = u32AccInt;

		// Set result : Ok.
		enResult = Ok;
	}

	// Return result.
	return enResult;
}

/**
 *****************************************************************************
 ** \brief Calculate address of the element of specified standard ID filter.
 **
 ** Calculate the top of address of standard ID filter element that is
 ** corresponding to specified index.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 ** \param [in] u8Index				Standard ID filter index.
 **
 ** \retval NULL					Invalid parameter(s).
 ** \retval not NULL				Address of specified filter element
 *****************************************************************************/
static stc_canfd_stdid_filter_t* CanfdCalcStdIdFilterAddress( volatile stc_canfdn_t* pstcCanfd,
															  uint8_t u8Index )
{
	stc_canfd_stdid_filter_t* pstcFilter;

	// Check parameters; if parameter error was detected, ...
	if ( NULL == pstcCanfd )
	{
		pstcFilter = NULL;
	}
	// Otherwise, ...
	else
	{
		// Calculate address of specified standard ID element.
		pstcFilter = (stc_canfd_stdid_filter_t*)((uint8_t*)pstcCanfd + CANFD_MSGRAM_OFFSET +
												 ((pstcCanfd->SIDFC_f.FLSSA + CANFD_STDID_INFOSIZE * u8Index) << 2));
	}

	// Return result
	return pstcFilter;
} // CanfdCalcStdIdFilterAddress

/**
 *****************************************************************************
 ** \brief Calculate address of the element of specified extended ID filter.
 **
 ** Calculate the top of address of exended ID filter element that is
 ** corresponding to specified index.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 ** \param [in] u8Index				Extended ID filter index.
 **
 ** \retval NULL					Invalid parameter(s).
 ** \retval not NULL				Address of specified filter element
 *****************************************************************************/
static stc_canfd_extid_filter_t* CanfdCalcExtIdFilterAddress( volatile stc_canfdn_t* pstcCanfd,
															  uint8_t u8Index )
{
	stc_canfd_extid_filter_t* pstcFilter;

	// Check parameters; if parameter error was detected, ...
	if ( NULL == pstcCanfd )
	{
		pstcFilter = NULL;
	}
	// Otherwise, ...
	else
	{
		// Calculate address of specified extended ID element.
		pstcFilter = (stc_canfd_extid_filter_t*)((uint8_t*)pstcCanfd + CANFD_MSGRAM_OFFSET +
												 ((pstcCanfd->XIDFC_f.FLESA + CANFD_EXTID_INFOSIZE * u8Index) << 2));
	}

	// Return result
	return pstcFilter;
} // CanfdCalcExtIdFilterAddress

/**
 *****************************************************************************
 ** \brief Calculate address of the element of specified dedicated Rx Buffer.
 **
 ** Calculate the top of address of dedicated Rx buffer element that is
 ** corresponding to specified index.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 ** \param [in] u8Index				Rx buffer index.
 **
 ** \retval NULL					Invalid parameter(s).
 ** \retval not NULL				Address of specified Rx buffer element
 *****************************************************************************/
static stc_canfd_rx_buffer_t* CanfdCalcRxBufferAddress( volatile stc_canfdn_t* pstcCanfd,
														uint8_t u8Index )
{
	stc_canfd_rx_buffer_t*	pstcRxBuf;
	uint8_t					u8RBDS;

	// Check parameters; if parameter error was detected, ...
	if ( NULL == pstcCanfd )
	{
		pstcRxBuf = NULL;
	}
	// Otherwise, ...
	else
	{
		// Calculate address of specified Rx buffer.
		u8RBDS = pstcCanfd->RXESC_f.RBDS;
		pstcRxBuf = (stc_canfd_rx_buffer_t*)((uint8_t*)pstcCanfd + CANFD_MSGRAM_OFFSET +
											 ((pstcCanfd->RXBC_f.RBSA +
											  (CANFD_RXBUF_INFOSIZE + u8BufSizeInWord[u8RBDS]) * u8Index) << 2));
	}

	// Return result.
	return pstcRxBuf;
} // CanfdCalcRxBufferAddress

/**
 *****************************************************************************
 ** \brief Get one received message from specified Rx FIFO.
 **
 ** Get one received message from specified Rx FIFO.
 ** The 'GetIndex' is updated by this process if it was done successfully.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 ** \param [in] enNumber			Rx FIFO number (FIFO0/1).
 ** \param [in] pstcRxBuffer		Address of Rx (FIFO) buffer element.
 **
 ** \retval Ok						Get Rx FIFO message succesfully.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfdInternData == NULL
 **   - enNumber out of range
 **   - pstcRxBuffer == NULL
 ** \retval Error					FIFO is not available or has no message.
 *****************************************************************************/
static en_result_t CanfdGetRxFIFO( volatile stc_canfdn_t* pstcCanfd,
								   en_canfd_rxfifo_number_t enNumber,
								   stc_canfd_rx_buffer_t* pstcRxBuffer )
{
	union {
		uint32_t				RXFC;
		stc_canfd_rxf0c_field_t	RXFC_f;
	} unRXFC;
	union {
		uint32_t				RXFS;
		stc_canfd_rxf0s_field_t	RXFS_f;
	} unRXFS;
	stc_canfd_rx_buffer_t*	pstcMsg;
	uint8_t         u8Size;
	uint8_t         u8Idx;
	en_result_t     enResult;
        uint8_t         F0GI;
        uint8_t         F0PI;
        
	// Check parameters; if parameter error was detected, ...
	if ( NULL == pstcCanfd ||
		 ( CanfdRxFIFO0 != enNumber && CanfdRxFIFO1 != enNumber ) ||
		 NULL==  pstcRxBuffer )
	{
		enResult = ErrorInvalidParameter;
	}
	// Otherwise, ...
	else
	{
		// Get specified FIFO information.
		if ( CanfdRxFIFO0 == enNumber )
		{
			// Rx FIFO0 information
			unRXFC.RXFC = pstcCanfd->RXF0C;
			unRXFS.RXFS = pstcCanfd->RXF0S;
			u8Size = u8BufSizeInWord[pstcCanfd->RXESC_f.F0DS];
		}
		else
		{
			// Rx FIFO1 information
			unRXFC.RXFC = pstcCanfd->RXF1C;
			unRXFS.RXFS = pstcCanfd->RXF1S;
			u8Size = u8BufSizeInWord[pstcCanfd->RXESC_f.F1DS];
		}

		// If FIFO is not active or has no message, ...
                F0GI = unRXFS.RXFS_f.F0GI;
                F0PI = unRXFS.RXFS_f.F0PI;
		if ((0 == unRXFC.RXFC_f.F0S) || 
                    ((PDL_OFF == unRXFS.RXFS_f.F0F) && (F0GI == F0PI)))
		{//
			enResult = Error;
		}
		// Otherwise, ...
		else
		{
			// Get a message at the top of the FIFO.
			// (Calculate element address)
			pstcMsg = (stc_canfd_rx_buffer_t*)((uint8_t*)pstcCanfd + 
                                   CANFD_MSGRAM_OFFSET +
                                   ((unRXFC.RXFC_f.F0SA + (CANFD_RXBUF_INFOSIZE + u8Size) * F0GI) << 2));
                        
			// (Get message information header)
			pstcRxBuffer->w[0] = pstcMsg->w[0];
			pstcRxBuffer->w[1] = pstcMsg->w[1];
			// (Get message data)
			for ( u8Idx = 0; u8Idx < u8Size; u8Idx++ )
			{
				pstcRxBuffer->data[u8Idx] = pstcMsg->data[u8Idx];
			}

			// Acknowledge a message.
			if ( CanfdRxFIFO0 == enNumber )
			{
				pstcCanfd->RXF0A_f.F0AI = unRXFS.RXFS_f.F0GI;
			}
			else
			{
				pstcCanfd->RXF1A_f.F1AI = unRXFS.RXFS_f.F0GI;
			}

			// Finish process successfully.
			enResult = Ok;
		}
	}

	// Return result.
	return enResult;
} // CanfdGetRxFIFO

/**
 *****************************************************************************
 ** \brief Convert MCAN Rx buffer to driver message.
 **
 ** Convert contents of MCAN Rx buffer on the message RAM to the driver defined
 ** message structure.
 **
 ** \param [in] pstcRxBuffer		Pointer to Rx buffer/FIFO element
 ** \param [in] pstcRxMsg			Pointer to message structure
 **
 ** \retval Ok						Done conversion successfully.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcRxBuffer == NULL
 **   - pstcRxMsg == NULL
 **
 ** Note: This function premises that the byte order is 'little endian'.
 *****************************************************************************/
static en_result_t CanfdConvRxBuffer( stc_canfd_rx_buffer_t* pstcRxBuffer,
									  stc_canfd_msg_t* pstcRxMsg )
{
	uint8_t		u8DataSize;
	uint8_t		u8Count;
	uint32_t	u32Data;
	en_result_t	enResult;

	// Check parameters.
	if ( NULL == pstcRxBuffer || NULL == pstcRxMsg )
	{
		enResult = ErrorInvalidParameter;
	}
	// If all parameters are valid, ...
	else
	{
		// XTD : Extended Identifier.
		pstcRxMsg->stcIdentifier.bExtended = pstcRxBuffer->xtd;

		// ID : RxID.
		if ( 0 == pstcRxMsg->stcIdentifier.bExtended )
		{
			pstcRxMsg->stcIdentifier.u32Identifier = pstcRxBuffer->id >> 18;
		}
		else
		{
			pstcRxMsg->stcIdentifier.u32Identifier = pstcRxBuffer->id;
		}

		// EDL : Extended Data Length.
		pstcRxMsg->bCanfd = ( 0 == pstcRxBuffer->edl ) ? FALSE : TRUE;

		// DLC : Data Length Code.
		pstcRxMsg->stcData.u8DataLengthCode = pstcRxBuffer->dlc;

		// Copy 0-64 byte of data area.
		u8DataSize = u8DlcInByte[pstcRxMsg->stcData.u8DataLengthCode];

		if ( FALSE == pstcRxMsg->bCanfd && CANFD_MAX_STDDLC < u8DataSize )
		{
			u8DataSize = CANFD_MAX_STDDLC;
		}
		for ( u8Count = 0; 4 <= u8DataSize; u8Count++, u8DataSize -= 4 )
		{
			pstcRxMsg->stcData.au32Data[u8Count] = pstcRxBuffer->data[u8Count];
		}
		if ( 0 < u8DataSize )
		{
			u32Data = pstcRxBuffer->data[u8Count];
			for ( u8Count *= 4; 0 < u8DataSize; u8Count++, u8DataSize-- )
			{
				pstcRxMsg->stcData.au8Data[u8Count] = (uint8_t)u32Data;
				u32Data >>= 8;
			}
		}

		// Set 'Ok' to result.
		enResult = Ok;
	}

	// Return result.
	return enResult;
}


/**
 *****************************************************************************
 ** \brief Calculate address of the element of specified Tx Buffer.
 **
 ** Calculate the top of address of Tx buffer element that is corresponding
 ** to specified index.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 ** \param [in] u8Index				Tx buffer index.
 **
 ** \retval NULL					Invalid parameter(s).
 ** \retval not NULL				Address of specified Tx buffer element
 *****************************************************************************/
static stc_canfd_tx_buffer_t* CanfdCalcTxBufferAddress( volatile stc_canfdn_t* pstcCanfd,
														uint8_t u8Index )
{
	stc_canfd_tx_buffer_t*	pstcTxBuf;
	uint8_t					u8TBDS;

	// Check parameters; if parameter error was detected, ...
	if ( NULL == pstcCanfd )
	{
		pstcTxBuf = NULL;
	}
	// Otherwise, ...
	else
	{
		// Calculate address of specified Tx buffer.
		u8TBDS = pstcCanfd->TXESC_f.TBDS;
		pstcTxBuf = (stc_canfd_tx_buffer_t*)((uint8_t*)pstcCanfd + CANFD_MSGRAM_OFFSET +
											 ((pstcCanfd->TXBC_f.TBSA +
											   (CANFD_TXBUF_INFOSIZE + u8BufSizeInWord[u8TBDS]) * u8Index) << 2));
	}

	// Return result.
	return pstcTxBuf;
} // CanfdCalcTxBufferAddress


/**
 *****************************************************************************
 ** \brief Clear transmission request marker.
 **
 ** Clear transmission request marker of all Tx buffers.
 ** Transmission request marker holds in the MM field of Tx buffer.
 **
 ** \param [in] pstcCanfd			Pointer to register area of a CAN FD unit.
 **
 ** \retval Ok						Done process successfully.
 ** \retval ErrorInvalidParameter	If one of the following conditions are met:
 **   - pstcCanfd == NULL
 **   - pstcCanfdInternData == NULL
 *****************************************************************************/
static en_result_t CanfdClearTxRequestMarker( volatile stc_canfdn_t* pstcCanfd )
{
	stc_canfd_intern_data_t*	pstcCanfdInternData;	// Pointer to internal data
	uint8_t						u8Num;
	en_result_t					enResult;

	// Check parameter.
	if ( NULL == pstcCanfd )
	{
		enResult = ErrorInvalidParameter;
	}
	// Get pointer to internal data structure.
	else if ( NULL == ( pstcCanfdInternData = CanfdGetInternDataPtr( pstcCanfd ) ) )
	{
		enResult = ErrorInvalidParameter;
	}
	// If parameter is valid, ...
	else
	{
		// Loop to clear all markers.
		for ( u8Num = 0; CANFD_MESSAGE_TXBUFFER_COUNT > u8Num; u8Num++ )
		{
			// Clear marker.
			pstcCanfdInternData->u8TxReqMarker[u8Num] = CANFD_TX_NOT_REQUESTED;
		}

		// Set 'Ok' to result.
		enResult = Ok;
	}

	//Return result.
	return enResult;
}

#endif

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
