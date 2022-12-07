/******************************************************************************
* \file             dma.h
*
* \version          1.20
*
* \brief            Header file of Direct Memory Access Controller function
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

#ifndef __DMA_H__
#define __DMA_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_DMA_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/**
* \defgroup GroupDMA Direct Memory Access Controller (DMA)
* \{
* \defgroup GroupDMA_Macros Macros
* \defgroup GroupDMA_Functions Functions 
* \defgroup GroupDMA_GlobalVariables Global Variables
* \defgroup GroupDMA_DataStructures Data Structures
* \defgroup GroupDMA_Types Enumerated Types
* \}
*/
/**
* \addtogroup GroupDMA
* \{
* The Direct Memory Access Controller (DMA) transfers data at high speed without 
* using the CPU. DMA has an independent bus, so memory transfers are not affected by
* CPU bus access.<br>
* - DMA supports a maximum of eight channels, so eight different 
* DMA transfers can occur independently<br>
* - You set the address of the transfer destination, the address of the 
* transfer source, the size of transfer data, the source of transfer request, 
* and the transfer mode<br>
* You control the start of transfer operation, the 
* forced termination of a transfer, and can pause the transfer for each channel<br>
* - You can control DMA channels individually, or as a batch<br>
* - You can use BT, EXINT, MFS or ADC peripheral as an interrupt source<br>
* - DMA complies with the system bus (AHB), supporting 32-bit address space 
* (up to 4 Gbytes).<br>
* There are three transfer modes: <br>
* - Block – a number of data elements of the specified width, with a 
* timing gap between blocks; this enables the DMA controller to switch among multiple channels<br>
* - Burst – a number of data elements of the specified width, with no timing gap; 
* this enables the channel to have priority and complete transfer before another channel is enabled<br>
* - Demand – for interrupt-driven transfers; DMA receives a transfer request from the peripheral<br>
* 
* \section SectionDMA_ConfigurationConsideration Configuration Consideration
* You specify configuration options in the stc_dma_config_t structure. Each channel has its 
* own configuration. As noted in the General Description there are several options, including:<br>
* - The interrupt source (including software as the trigger)<br>
* - The source and destination addresses<br>
* - Whether to reload the original source and destination addresses upon completion of the transfer<br>
* - Data width (8-, 16-, or 32-bit)<br>
* - Transfer mode (block, burst, demand)<br>
* If you use interrupt-driven DMA, you also provide configuration structures to enable and identify 
* the required callback functions for the channel.<br>
* Call Dma_InitChannel() to initialize a DMA channel. Use Use Dma_SetChannel() to enable, disable, 
* pause, or trigger a data transfer by software control. As noted, the configuration structure sets 
* up interrupts for the channel. After initializing, you can enable and disable interrupts 
* for a single channel with Dma_EnableIrq() and Dma_DisableIrq() with Dma_EnableIrq() and Dma_DisableIrq().<br>
* For batch control, use Dma_Enable(), Dma_Disable(), Dma-Pause(), or Dma_Resume() to control all channels.<br>
* For FM3 Type 2 parts only, the Dma_SetIrqXXSelector() sets the source selection of IRQXX which is used to 
* trigger DMA, where XX is 10, 11, 24, 25, 26, 27, 30, 31.<br>
*
* \section SectionDMA_MoreInfo More Information
* For more information on the DMA peripheral, refer to:<br> 
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual Core Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual Core Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupDMA_Macros
* \{
*/
  
/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/
#define PDL_DMA_CHANNELS        8u
#define PDL_DMA_INSTANCE_COUNT  (PDL_PERIPHERAL_ENABLE_DMA0 == PDL_ON) + \
                                (PDL_PERIPHERAL_ENABLE_DMA1 == PDL_ON) + \
                                (PDL_PERIPHERAL_ENABLE_DMA2 == PDL_ON) + \
                                (PDL_PERIPHERAL_ENABLE_DMA3 == PDL_ON) + \
                                (PDL_PERIPHERAL_ENABLE_DMA4 == PDL_ON) + \
                                (PDL_PERIPHERAL_ENABLE_DMA5 == PDL_ON) + \
                                (PDL_PERIPHERAL_ENABLE_DMA6 == PDL_ON) + \
                                (PDL_PERIPHERAL_ENABLE_DMA7 == PDL_ON) 
                             
// register address offset
#define DMA_DMACA_OFFSET 0x00ul
#define DMA_DMACB_OFFSET 0x04ul
#define DMA_DMASA_OFFSET 0x08ul
#define DMA_DMADA_OFFSET 0x0Cul
    
/** \} GroupDMA_Macros */

/**
* \addtogroup GroupDMA_Types
* \{
*/
    
/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief DMA transfer data width
 ******************************************************************************/
typedef enum en_dma_transfermode
{
    DmaBlockTransfer  = 0u,  ///< Block Transfer
    DmaBurstTransfer  = 1u,  ///< Burst transfer
    DmaDemandTransfer = 2u   ///< Demand transfer
} en_dma_transfermode_t ;

/**
 ******************************************************************************
 ** \brief DMA transfer data width
 ******************************************************************************/
typedef enum en_dma_transferwidth
{
    Dma8Bit  = 0u,  ///< 8 bit transfer via DMA
    Dma16Bit = 1u,  ///< 16 bit transfer via DMA
    Dma32Bit = 2u   ///< 32 bit transfer via DMA
} en_dma_transferwidth_t ;

#if (PDL_MCU_TYPE == PDL_FM3_TYPE2)
/**
 ******************************************************************************
 ** \brief DMA source selection of IRQ10
 **
 ** \note Only available for FM3 TYPE2 products
 ******************************************************************************/
typedef enum en_dma_irq10_sel
{
    DmaIrq10Bt4Irq0 = 0u,   ///< Base timer ch.4 Interrupt signal of IRQ0
    DmaIrq10Usb1Ep1 = 1u,   ///< USB ch.1 EP1 DRQ interrupt signal
    DmaIrq10Usb1Ep2 = 2u,   ///< USB ch.1 EP2 DRQ interrupt signal
    DmaIrq10Usb1Ep3 = 3u,   ///< USB ch.1 EP3 DRQ interrupt signal
    DmaIrq10Usb1Ep4 = 4u,   ///< USB ch.1 EP4 DRQ interrupt signal
    DmaIrq10Usb1Ep5 = 5u,   ///< USB ch.1 EP5 DRQ interrupt signal
    
}en_dma_irq10_sel_t;

/**
 ******************************************************************************
 ** \brief DMA source selection of IRQ11
 **
 ** \note Only available for FM3 TYPE2 products
 ******************************************************************************/
typedef enum en_dma_irq11_sel
{
    DmaIrq11Bt6Irq0 = 0u,   ///< Base timer ch.6 Interrupt signal of IRQ0
    DmaIrq11Usb1Ep1 = 1u,   ///< USB ch.1 EP1 DRQ interrupt signal
    DmaIrq11Usb1Ep2 = 2u,   ///< USB ch.1 EP2 DRQ interrupt signal
    DmaIrq11Usb1Ep3 = 3u,   ///< USB ch.1 EP3 DRQ interrupt signal
    DmaIrq11Usb1Ep4 = 4u,   ///< USB ch.1 EP4 DRQ interrupt signal
    DmaIrq11Usb1Ep5 = 5u,   ///< USB ch.1 EP5 DRQ interrupt signal
    
}en_dma_irq11_sel_t;

/**
 ******************************************************************************
 ** \brief DMA source selection of IRQ24
 **
 ** \note Only available for FM3 TYPE2 products
 ******************************************************************************/
typedef enum en_dma_irq24_sel
{
    DmaIrq24Mfs6Rx  = 0u,   ///< MFS ch.6 Receive interrupt signal
    DmaIrq24Usb1Ep1 = 1u,   ///< USB ch.1 EP1 DRQ interrupt signal
    DmaIrq24Usb1Ep2 = 2u,   ///< USB ch.1 EP2 DRQ interrupt signal
    DmaIrq24Usb1Ep3 = 3u,   ///< USB ch.1 EP3 DRQ interrupt signal
    DmaIrq24Usb1Ep4 = 4u,   ///< USB ch.1 EP4 DRQ interrupt signal
    DmaIrq24Usb1Ep5 = 5u,   ///< USB ch.1 EP5 DRQ interrupt signal
    
}en_dma_irq24_sel_t;

/**
 ******************************************************************************
 ** \brief DMA source selection of IRQ25
 **
 ** \note Only available for FM3 TYPE2 products
 ******************************************************************************/
typedef enum en_dma_irq25_sel
{
    DmaIrq25Mfs6Tx  = 0u,   ///< MFS ch.6 Send interrupt signal
    DmaIrq25Usb1Ep1 = 1u,   ///< USB ch.1 EP1 DRQ interrupt signal
    DmaIrq25Usb1Ep2 = 2u,   ///< USB ch.1 EP2 DRQ interrupt signal
    DmaIrq25Usb1Ep3 = 3u,   ///< USB ch.1 EP3 DRQ interrupt signal
    DmaIrq25Usb1Ep4 = 4u,   ///< USB ch.1 EP4 DRQ interrupt signal
    DmaIrq25Usb1Ep5 = 5u,   ///< USB ch.1 EP5 DRQ interrupt signal
    
}en_dma_irq25_sel_t;

/**
 ******************************************************************************
 ** \brief DMA source selection of IRQ26
 **
 ** \note Only available for FM3 TYPE2 products
 ******************************************************************************/
typedef enum en_dma_irq26_sel
{
    DmaIrq26Mfs7Rx  = 0u,   ///< MFS ch.7 Receive interrupt signal
    DmaIrq26Usb1Ep1 = 1u,   ///< USB ch.1 EP1 DRQ interrupt signal
    DmaIrq26Usb1Ep2 = 2u,   ///< USB ch.1 EP2 DRQ interrupt signal
    DmaIrq26Usb1Ep3 = 3u,   ///< USB ch.1 EP3 DRQ interrupt signal
    DmaIrq26Usb1Ep4 = 4u,   ///< USB ch.1 EP4 DRQ interrupt signal
    DmaIrq26Usb1Ep5 = 5u,   ///< USB ch.1 EP5 DRQ interrupt signal
    
}en_dma_irq26_sel_t;

/**
 ******************************************************************************
 ** \brief DMA source selection of IRQ27
 **
 ** \note Only available for FM3 TYPE2 products
 ******************************************************************************/
typedef enum en_dma_irq27_sel
{
    DmaIrq27Mfs7Tx  = 0u,   ///< MFS ch.7 Send interrupt signal
    DmaIrq27Usb1Ep1 = 1u,   ///< USB ch.1 EP1 DRQ interrupt signal
    DmaIrq27Usb1Ep2 = 2u,   ///< USB ch.1 EP2 DRQ interrupt signal
    DmaIrq27Usb1Ep3 = 3u,   ///< USB ch.1 EP3 DRQ interrupt signal
    DmaIrq27Usb1Ep4 = 4u,   ///< USB ch.1 EP4 DRQ interrupt signal
    DmaIrq27Usb1Ep5 = 5u,   ///< USB ch.1 EP5 DRQ interrupt signal
    
}en_dma_irq27_sel_t;

/**
 ******************************************************************************
 ** \brief DMA source selection of IRQ30
 **
 ** \note Only available for FM3 TYPE2 products
 ******************************************************************************/
typedef enum en_dma_irq30_sel
{
    DmaIrq30ExtIrq2  = 0u,   ///< External interrupt ch.2 signal
    DmaIrq30Usb1Ep1  = 1u,   ///< USB ch.1 EP1 DRQ interrupt signal
    DmaIrq30Usb1Ep2  = 2u,   ///< USB ch.1 EP2 DRQ interrupt signal
    DmaIrq30Usb1Ep3  = 3u,   ///< USB ch.1 EP3 DRQ interrupt signal
    DmaIrq30Usb1Ep4  = 4u,   ///< USB ch.1 EP4 DRQ interrupt signal
    DmaIrq30Usb1Ep5  = 5u,   ///< USB ch.1 EP5 DRQ interrupt signal
    
}en_dma_irq30_sel_t;

/**
 ******************************************************************************
 ** \brief DMA source selection of IRQ31
 **
 ** \note Only available for FM3 TYPE2 products
 ******************************************************************************/
typedef enum en_dma_irq31_sel
{
    DmaIrq31ExtIrq3  = 0u,   ///< External interrupt ch.3 signal
    DmaIrq31Usb1Ep1  = 1u,   ///< USB ch.1 EP1 DRQ interrupt signal
    DmaIrq31Usb1Ep2  = 2u,   ///< USB ch.1 EP2 DRQ interrupt signal
    DmaIrq31Usb1Ep3  = 3u,   ///< USB ch.1 EP3 DRQ interrupt signal
    DmaIrq31Usb1Ep4  = 4u,   ///< USB ch.1 EP4 DRQ interrupt signal
    DmaIrq31Usb1Ep5  = 5u,   ///< USB ch.1 EP5 DRQ interrupt signal
    
}en_dma_irq31_sel_t;
#endif

/**
 ******************************************************************************
 ** \brief DMA IDREQ number
 ******************************************************************************/
typedef enum en_dma_idreq
{
    Usb0Ep1        = 0u,   ///< Interrupt signal from EP1 DRQ of USB ch.0
    Usb0Ep2        = 1u,   ///< Interrupt signal from EP2 DRQ of USB ch.0
    Usb0Ep3        = 2u,   ///< Interrupt signal from EP3 DRQ of USB ch.0
    Usb0Ep4        = 3u,   ///< Interrupt signal from EP4 DRQ of USB ch.0
    Usb0Ep5        = 4u,   ///< Interrupt signal from EP5 DRQ of USB ch.0 
    Adc0           = 5u,   ///< Scan conversion interrupt signal from A/D converter unit0
    Adc1           = 6u,   ///< Scan conversion interrupt signal from A/D converter unit1
    Adc2           = 7u,   ///< Scan conversion interrupt signal from A/D converter unit2
    Bt0Irq0        = 8u,   ///< Interrupt signal from IRQ0 of base timer ch.0
    Bt2Irq0        = 9u,   ///< Interrupt signal from IRQ0 of base timer ch.2
    Bt4Irq0Sel10   = 10u,  ///< Default: Interrupt signal from IRQ0 of base timer ch.4, it can also be selected by Dma_SetIrq10Selector()
    Bt6Irq0Sel11   = 11u,  ///< Default: Interrupt signal from IRQ0 of base timer ch.6, it can also be selected by Dma_SetIrq11Selector()
    MfsRx0         = 12u,  ///< Receiving interrupt signal from MFS ch.0
    MfsTx0         = 13u,  ///< Sending interrupt signal from MFS ch.0   
    MfsRx1         = 14u,  ///< Receiving interrupt signal from MFS ch.1
    MfsTx1         = 15u,  ///< Sending interrupt signal from MFS ch.1
    MfsRx2         = 16u,  ///< Receiving interrupt signal from MFS ch.2
    MfsTx2         = 17u,  ///< Sending interrupt signal from MFS ch.2
    MfsRx3         = 18u,  ///< Receiving interrupt signal from MFS ch.3
    MfsTx3         = 19u,  ///< Sending interrupt signal from MFS ch.3 
    MfsRx4         = 20u,  ///< Receiving interrupt signal from MFS ch.4
    MfsTx4         = 21u,  ///< Sending interrupt signal from MFS ch.4 
    MfsRx5         = 22u,  ///< Receiving interrupt signal from MFS ch.5
    MfsTx5         = 23u,  ///< Sending interrupt signal from MFS ch.5 
    MfsRx6Sel24    = 24u,  ///< Default: Receiving interrupt signal from MFS ch.6, it can also be selected by Dma_SetIrq24Selector()
    MfsTx6Sel25    = 25u,  ///< Default: Sending interrupt signal from MFS ch.6, it can also be selected by Dma_SetIrq25Selector() 
    MfsRx7Sel26    = 26u,  ///< Default: Receiving interrupt signal from MFS ch.7, it can also be selected by Dma_SetIrq26Selector()
    MfsTx7Sel27    = 27u,  ///< Default: Sending interrupt signal from MFS ch.7, it can also be selected by Dma_SetIrq27Selector() 
    ExtIrq0        = 28u,  ///< Interrupt signal from external interrupt unit ch.0
    ExtIrq1        = 29u,  ///< Interrupt signal from external interrupt unit ch.1    
    ExtIrq2Sel30   = 30u,  ///< Default: Interrupt signal from external interrupt unit ch.2, it can also be selected by Dma_SetIrq30Selector()
    ExtIrq3Sel31   = 31u,  ///< Interrupt signal from external interrupt unit ch.3, it can also be selected by Dma_SetIrq31Selector() 
    Software       = 1234u ///< Software Demand (just a high number)    
} en_dma_idreq_t ;

/**
 ******************************************************************************
 ** \brief DMA stop cause
 ******************************************************************************/
typedef enum dma_stop_cause
{
    DmaStopByAddrOverflow        = 0u,  ///< DMA stops by address overflow
    DmaStopByTransferStopRqst    = 1u,  ///< DMA stops by transfer stop request
    DmaStopBySrcAccessError      = 2u,  ///< DMA stops by source access error
    DmaStopByDstAccessError      = 3u,  ///< DMA stops by destination access error
    DmaStopByTransferComplete    = 4u,  ///< DMA stops by transfer completion
    DmaStopByTransferPause       = 5u,  ///< DMA stops by transfer pause
    
}dma_stop_cause_t;

/** \}GroupDMA_Types */

/**
* \addtogroup GroupDMA_DataStructures
* \{
*/
/**
 ******************************************************************************
 ** \brief DMA interrupt selection
 ******************************************************************************/
typedef struct stc_dma_irq_sel
{
    boolean_t bCompleteIrq;   ///< Select DMA transfer completion interrupt
    boolean_t bErrorIrq;      ///< Select DMA transfer error interrupt
    
}stc_dma_irq_sel_t, stc_dma_irq_en_t;

/**
 ******************************************************************************
 ** \brief DMA interrupt callback function
 ******************************************************************************/
typedef struct stc_dma_irq_cb
{
    func_ptr_t      pfnDmaCompletionIrqCb;         ///< Pointer to DMA transfer completion interrupt callback function
    func_ptr_arg1_t pfnDmaErrorIrqCb;              ///< Pointer to DMA transfer error interrupt callback function
    
}stc_dma_irq_cb_t;

/**
 ******************************************************************************
 ** \brief Datatype for holding internal data needed for DMA
 ******************************************************************************/
typedef struct stc_dma_intern_data
{
    func_ptr_t      apfnDmaIrqCompletionCb[PDL_DMA_INSTANCE_COUNT];   ///< DMA transfer completion callback pointer array
    func_ptr_arg1_t apfnDmaErrorIrqCb[PDL_DMA_INSTANCE_COUNT];        ///< DMA error callback pointer array

} stc_dma_intern_data_t;

/**
 ******************************************************************************
 ** \brief DMA configuration
 ******************************************************************************/
typedef struct stc_dma_config
{
    en_dma_idreq_t         enDmaIdrq ;             ///< ID Request number (see #en_dma_idreq_t for details)
    uint8_t                u8BlockCount ;          ///< Block counter
    uint16_t               u16TransferCount ;      ///< Transfer counter
    en_dma_transfermode_t  enTransferMode ;        ///< DMA transfer mode (see #en_dma_transfermode_t for details)
    en_dma_transferwidth_t enTransferWidth ;       ///< DMA transfer width (see #en_dma_transferwidth_t for details)
    uint32_t               u32SourceAddress;       ///< Source address
    uint32_t               u32DestinationAddress ; ///< Destination address
    boolean_t              bFixedSource ;          ///< TRUE = source address not increased
    boolean_t              bFixedDestination ;     ///< TRUE = destiantion address not increased
    boolean_t              bReloadCount ;          ///< TRUE = count is reloaded
    boolean_t              bReloadSource ;         ///< TRUE = source address is reloaded
    boolean_t              bReloadDestination ;    ///< TRUE = destination address is reloaded
    boolean_t              bEnableBitMask ;        ///< FALSE = Clear EB (bEnable) bit on completion (mandatory for transfer end!)
#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)   
    stc_dma_irq_en_t*      pstcIrqEn;              ///< Pointer to DMA interrupt enable structure
    stc_dma_irq_cb_t*      pstcIrqCb;              ///< Pointer to DMA interrupt callback function structure
    boolean_t              bTouchNvic;             ///< TRUE: enable NVIC, FALSE: disable NVIC
#endif    
    
} stc_dma_config_t ;

/** \} GroupDMA_DataStructures */

/**
* \addtogroup GroupDMA_GlobalVariables
* \{
*/ 
/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
extern stc_dma_intern_data_t stcDmaInternData;
/** \} GroupDMA_GlobalVariables*/

/**
* \addtogroup GroupDMA_Functions
* \{
*/ 
/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/  
#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)  
void DmaIrqHandler(uint8_t u8DmaChannel) ;
en_result_t Dma_EnableIrq(uint8_t u8DmaChannel, 
                          stc_dma_irq_sel_t* pstcIrqSel);
en_result_t Dma_DisableIrq(uint8_t u8DmaChannel, 
                           stc_dma_irq_sel_t* pstcIrqSel);
#endif
/* Init/DeInit a DMA channel */
en_result_t Dma_InitChannel(uint8_t u8DmaChannel, const stc_dma_config_t* pstcConfig);
en_result_t Dma_DeInitChannel(uint8_t u8DmaChannel, boolean_t bTouchNvic);
/* Irq 10,11,24,25,26,27,30,31 Selection */
#if (PDL_MCU_TYPE == PDL_FM3_TYPE2)
en_result_t Dma_SetIrq10Selector(en_dma_irq10_sel_t enDmaIrq10Sel);
en_result_t Dma_SetIrq11Selector(en_dma_irq11_sel_t enDmaIrq11Sel);
en_result_t Dma_SetIrq24Selector(en_dma_irq24_sel_t enDmaIrq24Sel);
en_result_t Dma_SetIrq25Selector(en_dma_irq25_sel_t enDmaIrq25Sel);
en_result_t Dma_SetIrq26Selector(en_dma_irq26_sel_t enDmaIrq26Sel);
en_result_t Dma_SetIrq27Selector(en_dma_irq27_sel_t enDmaIrq27Sel);
en_result_t Dma_SetIrq30Selector(en_dma_irq30_sel_t enDmaIrq30Sel);
en_result_t Dma_SetIrq31Selector(en_dma_irq31_sel_t enDmaIrq30Sel);
#endif
/* Activate a DMA channel */
en_result_t Dma_SetChannel (uint8_t u8DmaChannel, 
                            boolean_t bEnable,
                            boolean_t bPause,
                            boolean_t bSoftwareTrigger) ;
/* Stop casue read/clear */
dma_stop_cause_t Dma_GetStopCause(uint8_t u8DmaChannel);
en_result_t Dma_ClrStopCause (uint8_t u8DmaChannel);
/* Globally enable/disable all channels */
en_result_t Dma_Enable(void) ;
en_result_t Dma_Disable(void) ;
/* Globally pause/resume all channels */
en_result_t Dma_Pause(void) ;
en_result_t Dma_Resume(void) ;

/** \} GroupDMA_Functions */
/** \} GroupDMA */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_DMA_ACTIVE))

#endif /* __DMA_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
