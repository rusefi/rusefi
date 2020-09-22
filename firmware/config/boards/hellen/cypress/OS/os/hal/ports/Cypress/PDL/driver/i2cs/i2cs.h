/*******************************************************************************
* \file              i2cs.h
* 
* \version           1.20
*
* \brief             Headerfile for I2C Slave functions.
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

#ifndef __I2CS_H__
#define __I2CS_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_I2CS_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupI2CS I2C Slave (I2CS)
* \{ 
* \defgroup GroupI2CS_Macros Macros
* \defgroup GroupI2CS_Functions Functions
* \defgroup GroupI2CS_GlobalVariables Global Variables
* \defgroup GroupI2CS_DataStructures Data Structures
* \defgroup GroupI2CS_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupI2CS
* \{
* I2C Slave (I2CS) implements the slave function of I2C. The microcontroller can wake up when the 
* slave address matches.<br>
* I2CS supports four interrupts.<br>
* - Data transmit
* - Data receive
* - I2CS condition
* - I2CS status<br>
* If the value of the I2C Bus Slave Address matches the received slave address masked by the 
* I2C Bus Slave Mask Address, I2C Slave acknowledges automatically. It is possible to disable the 
* detection of the slave address.<br>
* I2C Slave can wake up the microcontroller when its address or one of the reserved addresses is received. 
* The I2C reserved addresses are used for special purposes per I2C Specification (2000). 
* Software judges whether the reserved address is supported or not.
* \note I2C Slave supports the Standard-mode (Sm), Fast-mode (Fm) and Fast-mode Plus
* (Fm+).<br>
* I2C Slave doesn't support the High-speed mode (Hs-mode) and Ultra Fast-mode
* (UFm).<br>
* I2C Slave doesn't support the following protocol. CBUS, SMBus, PMBus, IPMI, ATCA, DDC.
*
* \section SectionI2CS_ConfigurationConsideration Configuration Consideration
* To set up an I2CS, you provide configuration parameters in the stc_i2cs_config_t structure. 
* For example, you provide the slave address and mask, enable slave address detection and 
* reserved address detection. You also specify which interrupts to enable in an stc_i2cs_irq_en_t 
* structure. Provide the required callback functions in an stc_i2cs_irq_cb_t structure.
* The config structure has fields for the addresses of the enable and callback structures. <br>
* Then call I2cs_Init().<br>
* You can enable or disable interrupts after configuring. You can also send and receive a byte of data.<br>
* Use I2cs_ConfigAck() to send an ACK or NACK signal when receiving data. I2cs_GetAck() gets the ACK signal status 
* after receiving an ACK.<br>
* Use I2cs_GetStatus() to poll for a variety of conditions. Use values defined in en_i2cs_status_t. For 
* example, use this function to get interrupt flags, to determine if the Rx buffer is full, and so on.<br>
* Use  I2cs_ClrStatus() to clear the I2C Slave status selected. Some status can only be cleared by hardware automatically.<br>
* I2cs_GetDataDir() gets the data direction of I2CS in the slave mode.
* 
*
* \section SectionI2CS_MoreInfo More Information
* For more information on the I2C Slave peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223031/download">FM0+ Peripheral Manual - Communication Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222976/download">FM4 Peripheral Manual - Communication Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupI2CS_Macros
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
#define stc_i2csn_t FM_I2CSLAVE_TypeDef 

#define I2CS0       (*((volatile stc_i2csn_t *) FM_I2CSLAVE_BASE))
   
#define I2CS_INSTANCE_COUNT                                \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_I2CS0 == PDL_ON) 
  
#define I2CS0_DATA_REG_ADDR   (uint32_t)(&FM_I2CS0->TDR)
    
/******************************************************************************
 * Global type definitions
 ******************************************************************************/

#define I2CS_IRQ_COND_RESTART_DETECTION        0u
#define I2CS_IRQ_COND_STOP_DETECTION           1u          

/** \} GroupI2CS_Macros */

/**
* \addtogroup GroupI2CS_Types
* \{
*/
         
/**
 ******************************************************************************
 ** \brief I2CS ACK types
 ******************************************************************************/ 
typedef enum en_i2cs_ack
{
    I2csAck =  0u,   ///< I2CS normal ACK
    I2csNAck = 1u,   ///< I2CS NACK 

}en_i2cs_ack_t; 

/**
 ******************************************************************************
 ** \brief I2CS interrupt selection
 ******************************************************************************/
typedef enum en_i2cs_irq_sel
{
    I2csTxIrq          = 0u,          ///< I2CS TX interrupt
    I2csRxIrq          = 1u,          ///< I2CS RX interrupt
    I2csCondIrq        = 2u,          ///< I2CS condition detection interrupt
    I2csStatusIrq      = 3u,          ///< I2CS status interrupt
  
}en_i2cs_irq_sel_t;

/**
 ******************************************************************************
 ** \brief I2CS status types
 ******************************************************************************/
typedef enum en_i2cs_status
{
    I2csStatus              = 0u,   ///< I2CS status interrupt request flag
    I2csRxFull              = 1u,   ///< I2CS RX buffer full interrupt request flag
    I2csTxEmpty             = 2u,   ///< I2CS TX buffer empty interrupt request flag
    I2csFirstByteDetect     = 3u,   ///< I2CS First byte detection
    I2csReservedByteDetect  = 4u,   ///< I2CS reserved byte detection
	I2csActive              = 5u,   ///< I2CS active
	I2csRestartDetect       = 6u,   ///< I2CS re-start condition detection	
    I2csStopDetect          = 7u,   ///< I2CS stop condition detection
    I2csBusBusy             = 8u,   ///< I2CS Bus busy status
}en_i2cs_status_t;

/**
 ******************************************************************************
 ** \brief I2CS data direction
 ******************************************************************************/
typedef enum en_i2cs_data_dir
{
    I2csDataRx = 0u,  ///< Data from master to slave
    I2csDataTx = 1u,  ///< Data from slave to master
  
}en_i2cs_data_dir_t;

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/
/// Enumeration to define an index for each enabled I2CS instance
typedef enum en_i2cs_instance_index
{  
#if (PDL_PERIPHERAL_ENABLE_I2CS0 == PDL_ON)  
    I2csInstanceIndexI2cs0,
#endif
    I2csInstanceIndexMax,    
    I2csInstanceIndexUnknown = 0xFFu,
    
} en_i2cs_instance_index_t;

/** \} GroupI2CS_Types */

/**
* \addtogroup GroupI2CS_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief I2CS interrupt enable structure
 ******************************************************************************/
typedef struct stc_i2cs_irq_en
{
    boolean_t bTxIrq;         ///< I2CS TX interrupt
    boolean_t bRxIrq;         ///< I2CS RX interrupt
    boolean_t bCondIrq;       ///< I2CS condition interrupt
    boolean_t bStatusIrq;     ///< I2CS condition interrupt	
  
}stc_i2cs_irq_en_t;

/**
 ******************************************************************************
 ** \brief I2CS interrupt callback function
 ******************************************************************************/
typedef struct stc_i2cs_irq_cb
{
    func_ptr_t pfnTxIrqCb;              ///< I2CS TX interrupt callback function pointer
    func_ptr_t pfnRxIrqCb;              ///< I2CS RX interrupt callback function pointer
    func_ptr_arg1_t pfnCondIrqCb;       ///< I2CS Condition interrupt callback function pointer 
    func_ptr_t pfnStatusIrqCb;          ///< I2CS status interrupt callback function pointer	
    
}stc_i2cs_irq_cb_t;

/**
 ******************************************************************************
 ** \brief I2CS configuration structure
 ******************************************************************************/
typedef struct stc_i2cs_config
{
    uint8_t   u8SlaveAddr;           ///< Slave address (This is effective on slave mode.
    uint8_t   u8SlaveMaskAddr;       ///< Slave Mask address (This is effective on slave mode.)
	boolean_t bEnableSlaveAddr;      ///< FLASE: disable slave address comparison, TRUE: enable salve address comparison
	boolean_t bEnableReservedAddr;   ///< FALSE: disable the detection of the reserved addresses (0000xxx or 1111xxx), TRUE: enable the detection of the reserved addresses (0000xxx or 1111xxx)
    boolean_t bWaitSelection;        ///< FALSE: generate interrupt after ACK, TRUE: generate interrupt before ACK
	boolean_t bEnableNoiseFilter;    ///< FALSE: disable noise filter, TRUE: enable noise filter
    uint8_t   u8SetupTime;           ///< Setup time between the I2C data line (SDA) and I2C clock line (SCL)  
	
#if (PDL_INTERRUPT_ENABLE_I2CS0 == PDL_ON)
    stc_i2cs_irq_en_t* pstcIrqEn;    ///< Pointer to I2CS interrupt enable structure, if set to NULL, no interrupt enabled.
    stc_i2cs_irq_cb_t* pstcIrqCb;    ///< Pointer to I2CS interrupt callback functions structurei, if set to NULL, no interrupt callback initialized.
    boolean_t bTouchNvic;            ///< TRUE: enable NVIC, FALSE: don't enable NVIC
#endif    
} stc_i2cs_config_t;

/// I2CS module internal data, storing internal information for each enabled I2CS instance.
typedef struct stc_i2cs_intern_data
{
    func_ptr_t pfnTxIrqCb;              ///< I2CS TX interrupt callback function pointer
    func_ptr_t pfnRxIrqCb;              ///< I2CS RX interrupt callback function pointer
    func_ptr_arg1_t pfnCondIrqCb;       ///< I2CS Condition interrupt callback function pointer
    func_ptr_t pfnStatusIrqCb;          ///< I2CS status interrupt callback function pointer	

} stc_i2cs_intern_data_t;

/// I2CS instance data type
typedef struct stc_i2cs_instance_data
{
    volatile stc_i2csn_t*   pstcInstance;     ///< pointer to registers of an instance
    stc_i2cs_intern_data_t stcInternData;     ///< module internal data of instance
} stc_i2cs_instance_data_t;

/** \} GroupI2CS_DataStructures */

/**
* \addtogroup GroupI2CS_GlobalVariables
* \{
*/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/// Look-up table for all enabled I2CS instances and their internal data
extern stc_i2cs_instance_data_t m_astcI2csInstanceDataLut[I2CS_INSTANCE_COUNT];

/** \} GroupI2CS_GlobalVariables */

/**
* \addtogroup GroupI2CS_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
/* I2CS */
#if (PDL_INTERRUPT_ENABLE_I2CS0 == PDL_ON) 
// Interrupt
void I2csIrqHandlerTx(volatile stc_i2csn_t*   pstcI2cs, 
                      stc_i2cs_intern_data_t* pstcInternData);
void I2csIrqHandlerRx(volatile stc_i2csn_t*   pstcI2cs,  
                      stc_i2cs_intern_data_t* pstcInternData);
void I2csIrqHandlerStatus(volatile stc_i2csn_t*   pstcI2cs,   
                          stc_i2cs_intern_data_t* pstcInternData);
en_result_t I2cs_EnableIrq(volatile stc_i2csn_t*   pstcI2cs,   
                           en_i2cs_irq_sel_t enIrqSel);
en_result_t I2cs_DisableIrq(volatile stc_i2csn_t*   pstcI2cs,  
                            en_i2cs_irq_sel_t enIrqSel);
#endif
// Init/De-Init
en_result_t I2cs_Init(volatile stc_i2csn_t*   pstcI2cs,  
                      const stc_i2cs_config_t* pstcConfig);
en_result_t I2cs_DeInit(volatile stc_i2csn_t*   pstcI2cs,   boolean_t bTouchNvic);

// Data read/write
en_result_t I2cs_SendData(volatile stc_i2csn_t*   pstcI2cs, uint8_t u8Data);
uint8_t I2cs_ReceiveData(volatile stc_i2csn_t*   pstcI2cs);

// ACK
en_result_t I2cs_ConfigAck(volatile stc_i2csn_t*   pstcI2cs, en_i2cs_ack_t enAck);
en_i2cs_ack_t I2cs_GetAck(volatile stc_i2csn_t*   pstcI2cs);

// Status read/clear
boolean_t I2cs_GetStatus(volatile stc_i2csn_t*   pstcI2cs, 
                         en_i2cs_status_t enStatus);
en_result_t I2cs_ClrStatus(volatile stc_i2csn_t*   pstcI2cs,
                           en_i2cs_status_t enStatus);

// Get Data direction
en_i2cs_data_dir_t I2cs_GetDataDir(volatile stc_i2csn_t*   pstcI2cs);

/** \} GroupI2CS_Functions */
/** \} GroupI2CS */

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_I2CS_ACTIVE)) */

#endif /* __I2CS_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
