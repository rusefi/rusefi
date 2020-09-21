/******************************************************************************
* \file             dma.c
*
* \version          1.20
*
* \brief            Direct Memory Access Controller driver
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
#include "dma/dma.h"
   
#if (defined(PDL_PERIPHERAL_DMA_ACTIVE))

const uint8_t cau8DmaReferenceLut[PDL_DMA_INSTANCE_COUNT + 1] = 
{
#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON)
  0,
#endif
#if (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON)
  1,
#endif
#if (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON)
  2,
#endif
#if (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON)
  3,
#endif
#if (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON)
  4,
#endif
#if (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON)
  5,
#endif
#if (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON)
  6,
#endif
#if (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)
  7,
#endif
  255   // End of table
};   

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
// DMACAn => FM3_DMAC_BASE + size of register set + offset of DMACA0
// DMACAn => 0x40060000    + 0x10 * n             + 0x10
#define GET_DMA_ADDRESS(x) (FM_DMAC_BASE + (0x10ul * (x)) + 0x10ul)

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)  
stc_dma_intern_data_t stcDmaInternData;
#endif

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/
typedef union
{
	uint32_t                 u32DMACA;
	stc_dmac_dmaca0_field_t  stcDMACA;
	
}un_dmac_dmaca_t;
  
typedef union
{
	stc_dmac_dmacb0_field_t  stcDMACB;
	uint32_t                 u32DMACB;
	
}un_dmac_dmacb_t;

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)  
/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/
static en_result_t DmaInitNvic(uint8_t u8DmaChannel);
static en_result_t DmaDeInitNvic(uint8_t u8DmaChannel);
static uint8_t DmaGetInternDataIdx(uint8_t u8Channel);
#endif

#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)  
/**
 ******************************************************************************
 ** \brief DMA interrupt handler
 **
 ** \param [in]  u8DmaChannel     Channel number 
 **
 ** \note depending on the DMA stop status the DmaCallback function is called
 **       on successful transfer or DmaErrorCallback function on errornous
 **       transfer. The ErrorCallback has the error code as an (uint8_t)
 **       argument which reflects the 3 bits of the stop status.
 ******************************************************************************/
void DmaIrqHandler(uint8_t u8DmaChannel)
{
  uint32_t u32DmaRegisterBaseAddress;
  uint32_t u32RegisterReadOut;
  uint8_t  u8StopStatus;
  uint8_t u8InstanceIdx;
  
  u32DmaRegisterBaseAddress = GET_DMA_ADDRESS(u8DmaChannel);
  
  // Readout DMACB and update
  u32RegisterReadOut = *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACB_OFFSET));
  
  u8StopStatus = (uint8_t)((u32RegisterReadOut & 0x00070000ul)>> 16u);

  u8InstanceIdx = DmaGetInternDataIdx(u8DmaChannel); // Get DMA instance index
  if(255u == u8InstanceIdx) // Instance not found.
  {
    return;
  }
  
  // Clear interrupt cause
  u32RegisterReadOut &= 0xFFF8FFFFu;     
  *(uint32_t*)(uint32_t)((u32DmaRegisterBaseAddress + DMA_DMACB_OFFSET)) 
    = u32RegisterReadOut;  
  
  if (0x5u == u8StopStatus)  // Successful DMA transfer
  {
    stcDmaInternData.apfnDmaIrqCompletionCb[u8InstanceIdx](); 
  }
  else                      // Errornous DMA transfer
  {
    stcDmaInternData.apfnDmaErrorIrqCb[u8InstanceIdx](u8StopStatus);
  }
}

/**
 ******************************************************************************
 ** \brief Init DMA interrupt IRQ
 **
 ** \param [in]  u8DmaChannel     Channel number 
 **
 ** \retval Ok                    init successful
 **
 ******************************************************************************/
static en_result_t DmaInitNvic(uint8_t u8DmaChannel)
{
  switch(u8DmaChannel)
  {
    case 0u:
      NVIC_ClearPendingIRQ(DMAC0_IRQn);
      NVIC_EnableIRQ(DMAC0_IRQn);
      NVIC_SetPriority(DMAC0_IRQn, PDL_IRQ_LEVEL_DMA0);    
      break;
    case 1u:
      NVIC_ClearPendingIRQ(DMAC1_IRQn);
      NVIC_EnableIRQ(DMAC1_IRQn);
      NVIC_SetPriority(DMAC1_IRQn, PDL_IRQ_LEVEL_DMA1);    
      break;
#if (PDL_MCU_CORE == PDL_FM3_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)
    case 2u:
      NVIC_ClearPendingIRQ(DMAC2_IRQn);
      NVIC_EnableIRQ(DMAC2_IRQn);
      NVIC_SetPriority(DMAC2_IRQn, PDL_IRQ_LEVEL_DMA2);    
      break;
    case 3u:
      NVIC_ClearPendingIRQ(DMAC3_IRQn);
      NVIC_EnableIRQ(DMAC3_IRQn);
      NVIC_SetPriority(DMAC3_IRQn, PDL_IRQ_LEVEL_DMA3);    
      break;    
    case 4u:
      NVIC_ClearPendingIRQ(DMAC4_IRQn);
      NVIC_EnableIRQ(DMAC4_IRQn);
      NVIC_SetPriority(DMAC4_IRQn, PDL_IRQ_LEVEL_DMA4);    
      break;
    case 5u:
      NVIC_ClearPendingIRQ(DMAC5_IRQn);
      NVIC_EnableIRQ(DMAC5_IRQn);
      NVIC_SetPriority(DMAC5_IRQn, PDL_IRQ_LEVEL_DMA5);    
      break;    
    case 6u:
      NVIC_ClearPendingIRQ(DMAC6_IRQn);
      NVIC_EnableIRQ(DMAC6_IRQn);
      NVIC_SetPriority(DMAC6_IRQn, PDL_IRQ_LEVEL_DMA6);    
      break;    
    case 7u:
      NVIC_ClearPendingIRQ(DMAC7_IRQn);
      NVIC_EnableIRQ(DMAC7_IRQn);
      NVIC_SetPriority(DMAC7_IRQn, PDL_IRQ_LEVEL_DMA7);    
      break; 
#endif      
    default:
      break;
  }
  return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Init DMA interrupt
 **
 ** \param [in]  u8DmaChannel     Channel number 
 **
 ** \retval Ok                    de-init successful
 **
 ******************************************************************************/
static en_result_t DmaDeInitNvic(uint8_t u8DmaChannel)
{
  switch(u8DmaChannel)
  {
    case 0u:
      NVIC_ClearPendingIRQ(DMAC0_IRQn);
      NVIC_EnableIRQ(DMAC0_IRQn);
      NVIC_SetPriority(DMAC0_IRQn, PDL_IRQ_LEVEL_DMA0);    
      break;
    case 1u:
      NVIC_ClearPendingIRQ(DMAC1_IRQn);
      NVIC_EnableIRQ(DMAC1_IRQn);
      NVIC_SetPriority(DMAC1_IRQn, PDL_IRQ_LEVEL_DMA1);    
      break;
#if (PDL_MCU_CORE == PDL_FM3_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)
    case 2u:
      NVIC_ClearPendingIRQ(DMAC2_IRQn);
      NVIC_EnableIRQ(DMAC2_IRQn);
      NVIC_SetPriority(DMAC2_IRQn, PDL_IRQ_LEVEL_DMA2);    
      break;
    case 3u:
      NVIC_ClearPendingIRQ(DMAC3_IRQn);
      NVIC_EnableIRQ(DMAC3_IRQn);
      NVIC_SetPriority(DMAC3_IRQn, PDL_IRQ_LEVEL_DMA3);    
      break;    
    case 4u:
      NVIC_ClearPendingIRQ(DMAC4_IRQn);
      NVIC_EnableIRQ(DMAC4_IRQn);
      NVIC_SetPriority(DMAC4_IRQn, PDL_IRQ_LEVEL_DMA4);    
      break;
    case 5u:
      NVIC_ClearPendingIRQ(DMAC5_IRQn);
      NVIC_EnableIRQ(DMAC5_IRQn);
      NVIC_SetPriority(DMAC5_IRQn, PDL_IRQ_LEVEL_DMA5);    
      break;    
    case 6u:
      NVIC_ClearPendingIRQ(DMAC6_IRQn);
      NVIC_EnableIRQ(DMAC6_IRQn);
      NVIC_SetPriority(DMAC6_IRQn, PDL_IRQ_LEVEL_DMA6);    
      break;    
    case 7u:
      NVIC_ClearPendingIRQ(DMAC7_IRQn);
      NVIC_EnableIRQ(DMAC7_IRQn);
      NVIC_SetPriority(DMAC7_IRQn, PDL_IRQ_LEVEL_DMA7);    
      break; 
#endif      
    default:
      break;
  }
	
  return Ok;
}

/**
 ******************************************************************************
 ** \brief Return the internal reference index of an DMA channel
 **
 ** \param u8Channel   DMA channel number
 **
 ** \return uint8_t    Reference index (255 if not found or u8Channel > PDL_DMA_CHANNELS)
 **
 ******************************************************************************/
static uint8_t DmaGetInternDataIdx(uint8_t u8Channel)
{
  uint8_t u8Index;
  
  if ((u8Channel + 1u) > PDL_DMA_CHANNELS)
  {
     return 255u;
  }
  
  for (u8Index = 0u; u8Index < PDL_DMA_INSTANCE_COUNT; u8Index++)
  {
    if (u8Channel == cau8DmaReferenceLut[u8Index])
    {
      return u8Index;
    }
  }
        
  return 255;  // Should never happen ...
}

/**
 ******************************************************************************
 ** \brief Enable interrupt of a DMA channel
 **
 ** Enable interrupt and set interrupt callback function of a DMA channel
 **
 ** \param [in]  u8DmaChannel    DMA channel index
 ** \param [in]  pstcIrqSel      DMA interrupt selection structure
 **
 ** \retval Ok                    Interrupt enabled normally
 ** \retval ErrorInvalidParameter u8DmaChannel > DMA_MAX_CH_INDEX 
 **                               Other invalid configuration
 **
 ******************************************************************************/
en_result_t Dma_EnableIrq(uint8_t u8DmaChannel, 
                          stc_dma_irq_sel_t* pstcIrqSel)
{
    uint32_t u32DmaRegisterBaseAddress; 
    un_dmac_dmacb_t unDmacDmacb;
    
    if((u8DmaChannel + 1u) > PDL_DMA_CHANNELS)
    {
        return ErrorInvalidParameter;
    }
    
    u32DmaRegisterBaseAddress = GET_DMA_ADDRESS(u8DmaChannel);
    
    // Readout original DMACB
    unDmacDmacb.u32DMACB = *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACB_OFFSET)) ;
    
    // Update EI and CI bit
    if(pstcIrqSel->bCompleteIrq == TRUE)
    {
        unDmacDmacb.stcDMACB.CI = 1u;
    }
    
    if(pstcIrqSel->bErrorIrq == TRUE)
    {
        unDmacDmacb.stcDMACB.EI = 1u;
    }
    
    // Write back to DMACB
    *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACB_OFFSET)) = unDmacDmacb.u32DMACB;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable interrupt of a DMA channel
 **
 ** \param [in]  u8DmaChannel    DMA channel index
 ** \param [in]  pstcIntSel      DMA interrupt selection structure
 **
 ** \retval Ok                    Interrupt disabled normally
 ** \retval ErrorInvalidParameter u8DmaChannel > DMA_MAX_CH_INDEX 
 **                               Other invalid configuration
 **
 ******************************************************************************/
en_result_t Dma_DisableIrq(uint8_t u8DmaChannel, 
                           stc_dma_irq_sel_t* pstcIntSel )
{
    uint32_t u32DmaRegisterBaseAddress;
    un_dmac_dmacb_t unDmacDmacb;
    
    if((u8DmaChannel + 1u) > PDL_DMA_CHANNELS)
    {
        return ErrorInvalidParameter;
    }
    
    u32DmaRegisterBaseAddress = GET_DMA_ADDRESS(u8DmaChannel);
    
    // Readout original DMACB
    unDmacDmacb.u32DMACB = *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACB_OFFSET)) ;
    
    // Update EI and CI bit
    if(pstcIntSel->bCompleteIrq == TRUE)
    {
        unDmacDmacb.stcDMACB.CI = 0u;
    }
    
    if(pstcIntSel->bErrorIrq == TRUE)
    {
        unDmacDmacb.stcDMACB.EI = 0u;
    }
    
    // Write back to DMACB
    *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACB_OFFSET)) = unDmacDmacb.u32DMACB;
    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief Initializes a DMA channel
 **
 ** Sets up an DMA channel without starting immediate DMA transfer.
 ** Set_Dma_Channel() is used for starting DMA transfer.
 **
 ** \param [in]  u8DmaChannel     DMA channel number
 ** \param [in]  pstcConfig       DMA module configuration 
 **
 ** \retval Ok                    init successful
 ** \retval ErrorInvalidParameter pstcAdc == NULL or other invalid configuration
 ** \retval OperationInProgress   DMA channel already in use
 **
 ******************************************************************************/
en_result_t Dma_InitChannel(uint8_t u8DmaChannel, const stc_dma_config_t* pstcConfig)                   
{  
  un_dmac_dmaca_t unDmacDmaca;
  un_dmac_dmacb_t unDmacDmacb;
  uint32_t        u32DmaRegisterBaseAddress;
  uint32_t u32DrqselBit;

#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)   
  uint8_t u8InstanceIdx;
  u8InstanceIdx = DmaGetInternDataIdx(u8DmaChannel); // Get DMA instance index
  
  if(0xFFu == u8InstanceIdx)  // instance not fould
  {
    return ErrorInvalidParameter;
  }
#endif  

  // Check for NULL pointer
  if (NULL == pstcConfig)
  {
    return ErrorInvalidParameter ;
  }
    
  unDmacDmaca.u32DMACA = 0u;
  unDmacDmacb.u32DMACB = 0u;

  u32DmaRegisterBaseAddress = GET_DMA_ADDRESS(u8DmaChannel);
  
  // Setup DMACA ...
  //
  // Block Count
  unDmacDmaca.stcDMACA.BC = 0x0Fu & (uint32_t)(pstcConfig->u8BlockCount - 1u);
  
  // Transfer Count
  unDmacDmaca.stcDMACA.TC = 0xFFFFu & (uint32_t)(pstcConfig->u16TransferCount - 1u);
  
  // Interrupt Request Channel
  if (Software == pstcConfig->enDmaIdrq)
  {
    unDmacDmaca.stcDMACA.IS = 0u;
  }
  else
  {
    unDmacDmaca.stcDMACA.IS = 0x20u | (0x1Fu &  (uint32_t)(pstcConfig->enDmaIdrq));
  }
  
  // ... and update hardware
  *(uint32_t*)(uint32_t)((u32DmaRegisterBaseAddress + DMA_DMACA_OFFSET)) = unDmacDmaca.u32DMACA ;
  
  // Setup DMACB ...
  //
  // First Transfer mode
  switch (pstcConfig->enTransferMode)
  {
    case DmaBlockTransfer:
      unDmacDmacb.stcDMACB.MS = 0u;
      break;
    case DmaBurstTransfer:
      unDmacDmacb.stcDMACB.MS = 1u;
      break;
    case DmaDemandTransfer:
      unDmacDmacb.stcDMACB.MS = 2u;
      break;
    default :
      return ErrorInvalidParameter;
  }
  
  // Transfer width
  switch (pstcConfig->enTransferWidth)
  {
    case Dma8Bit:
      unDmacDmacb.stcDMACB.TW = 0u;
      break;
    case Dma16Bit:
      unDmacDmacb.stcDMACB.TW = 1u;
      break;
    case Dma32Bit:
      unDmacDmacb.stcDMACB.TW = 2u;
      break;
    default :
      return ErrorInvalidParameter;
  }
 
  // Fixed source
  unDmacDmacb.stcDMACB.FS = (uint8_t)((pstcConfig->bFixedSource == TRUE) ? 1u : 0u);
  
  // Fixed destination
  unDmacDmacb.stcDMACB.FD = (uint8_t)((pstcConfig->bFixedDestination == TRUE) ? 1u : 0u);
  
  // Reload Count (BC/TC reload)
  unDmacDmacb.stcDMACB.RC = (uint8_t)((pstcConfig->bReloadCount == TRUE) ? 1u : 0u);
  
  // Reload Source
  unDmacDmacb.stcDMACB.RS = (uint8_t)((pstcConfig->bReloadSource == TRUE) ? 1u : 0u);
  
  // Reload Destination
  unDmacDmacb.stcDMACB.RD = (uint8_t)((pstcConfig->bReloadDestination == TRUE) ? 1u : 0u);
  
  // Enable bit mask
  unDmacDmacb.stcDMACB.EM = (uint8_t)((pstcConfig->bEnableBitMask == TRUE) ? 1u : 0u);

#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)    
  // Set interrupt
  if(NULL != pstcConfig->pstcIrqEn)
  {
    if(TRUE == pstcConfig->pstcIrqEn->bCompleteIrq)
    {
      unDmacDmacb.stcDMACB.CI = 1;
    }
     
    if(TRUE == pstcConfig->pstcIrqEn->bErrorIrq)
    {
      unDmacDmacb.stcDMACB.EI = 1;
    }
  }
#endif  
  
  // ... and update hardware
  *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACB_OFFSET))
    = unDmacDmacb.u32DMACB ;
  
  // Setup source address
  *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMASA_OFFSET))
    = pstcConfig->u32SourceAddress;
  
  // Setup destination address
  *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMADA_OFFSET))
    = pstcConfig->u32DestinationAddress;
  
  // Switch resource interrupt to DMA (except software DMA)
  if (pstcConfig->enDmaIdrq != Software)
  {
    u32DrqselBit = 1ul << (uint32_t)(pstcConfig->enDmaIdrq);
    FM_INTREQ->DRQSEL |= u32DrqselBit;
  }

#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)    
  // Set interrupt callback functions 
  if(NULL != pstcConfig->pstcIrqCb)
  {
    stcDmaInternData.apfnDmaIrqCompletionCb[u8InstanceIdx] = pstcConfig->pstcIrqCb->pfnDmaCompletionIrqCb;
    stcDmaInternData.apfnDmaErrorIrqCb[u8InstanceIdx] = pstcConfig->pstcIrqCb->pfnDmaErrorIrqCb;
  }
  
  // Set NVIC
  if(TRUE == pstcConfig->bTouchNvic)
  {
    DmaInitNvic(u8DmaChannel);
  }
#endif
  
  return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initializes a DMA channel
 **
 ** Clears an DMA channel.
 **
 ** \param [in]  u8DmaChannel     DMA channel number
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    init successful
 ** \retval ErrorInvalidParameter pstcAdc == NULL or other invalid configuration
 **
 ******************************************************************************/
en_result_t Dma_DeInitChannel(uint8_t u8DmaChannel, boolean_t bTouchNvic)  
{
    uint32_t u32DmaRegisterBaseAddress;
    u32DmaRegisterBaseAddress = GET_DMA_ADDRESS(u8DmaChannel);

    // clear all registers of DMA channel
    *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACA_OFFSET)) = 0u;
    *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACB_OFFSET)) = 0u;
    *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMASA_OFFSET)) = 0u;
    *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMADA_OFFSET)) = 0u;

#if (PDL_INTERRUPT_ENABLE_DMA0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA3 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA5 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_DMA6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_DMA7 == PDL_ON)      
    if (TRUE == bTouchNvic)
    {
        DmaDeInitNvic(u8DmaChannel);
    }
#endif

    return Ok;
} // Dma_DeInit_Channel

/**
 ******************************************************************************
 ** \brief Enable, disable, pause, or trigger a DMA channel via configuration
 **
 ** This function enables, disables, pauses or triggers a DMA transfer according
 ** to the settings in the configuration bits for EB (Enable), PB (Pause) and
 ** ST (Software Trigger)
 **
 ** \param [in]  u8DmaChannel     DMA channel number
 ** \param [in]  bEnable          Enable the channel or not
 ** \param [in]  bPause           Pause the channel or not
 ** \param [in]  bSoftwareTrigger Trigger DMA by software or not
 **
 ** \retval Ok                    Setting finished
 **
 ******************************************************************************/
en_result_t Dma_SetChannel (uint8_t u8DmaChannel, 
                            boolean_t bEnable,
                            boolean_t bPause,
                            boolean_t bSoftwareTrigger) 
{
  un_dmac_dmaca_t unDmacDmaca;
  uint32_t u32RegisterReadOut ;
  uint32_t u32DmaRegisterBaseAddress ;

  unDmacDmaca.u32DMACA = 0u;
  unDmacDmaca.stcDMACA.EB = (uint8_t)((bEnable == TRUE) ? 1u : 0u);
  unDmacDmaca.stcDMACA.PB = (uint8_t)((bPause == TRUE) ? 1u : 0u);
  unDmacDmaca.stcDMACA.ST = (uint8_t)((bSoftwareTrigger == TRUE) ? 1u : 0u);
  
  u32DmaRegisterBaseAddress = GET_DMA_ADDRESS(u8DmaChannel);
  
  // Readout DMACA and update
  u32RegisterReadOut = *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACA_OFFSET)) ;
  unDmacDmaca.u32DMACA |= (u32RegisterReadOut & 0x1FFFFFFFu);   // masking EB, PB, ST ...
  *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACA_OFFSET)) = unDmacDmaca.u32DMACA ;
  
  return Ok;
}

/**
 ******************************************************************************
 ** \brief Read stop cause of a DMA channel
 **
 ** \param [in]  u8DmaChannel       DMA channel
 **
 ** \retval DmaStopByAddrOverflow      DMA stops by address overflow
 ** \retval DmaStopByTransferStopRqst  DMA stops by transfer stop request
 ** \retval DmaStopBySrcAccessError    DMA stops by source access error
 ** \retval DmaStopByDstAccessError    DMA stops by destination access error
 ** \retval DmaStopByTransferComplete  DMA stops by transfer completion
 ** \retval DmaStopByTransferPause     DMA stops by transfer pause
 **
 ******************************************************************************/
dma_stop_cause_t Dma_GetStopCause(uint8_t u8DmaChannel)
{
    un_dmac_dmacb_t unDmacDmacb;
    uint32_t u32DmaRegisterBaseAddress;
    
    u32DmaRegisterBaseAddress = GET_DMA_ADDRESS(u8DmaChannel);
   
    // Readout original DMACB
    unDmacDmacb.u32DMACB = *(uint32_t*)((uint32_t)(u32DmaRegisterBaseAddress + DMA_DMACB_OFFSET)) ;
    
    return (dma_stop_cause_t) (unDmacDmacb.stcDMACB.SS);
}

/**
 ******************************************************************************
 ** \brief Enable DMA globally
 **
 ** \retval Ok                    Enable finished
 **
 ******************************************************************************/
en_result_t Dma_Enable(void)
{
    FM_DMAC->DMACR = 0x80000000ul; // Set DE Bit (DMA enable all channels)

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable DMA globally
 **
 ** \retval Ok                    Disable finished
 **
 ******************************************************************************/
en_result_t Dma_Disable(void)
{
    FM_DMAC->DMACR = 0u; // Clear everything

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Pause DMA globally
 **
 ** \retval Ok                    Pause finished
 **
 ******************************************************************************/
en_result_t Dma_Pause(void) 
{
    FM_DMAC->DMACR_f.DH = 0x0Fu;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Resume DMA globally
 **
 ** \retval Ok                    Resume from pause status finished
 **
 ******************************************************************************/
en_result_t Dma_Resume(void) 
{
    FM_DMAC->DMACR_f.DH = 0x0000u;
    
    return Ok;
}

#if (PDL_MCU_TYPE == PDL_FM3_TYPE2)
/**
 ******************************************************************************
 ** \brief Select a source for the IRQ10 input of DMA
 **
 ** Re-select the DMA IRQ10 source
 **
 ** \param [in]  enDmaIrq10Sel    The sources which can be selected, see 
 **                               #en_dma_irq10_sel_t for the details.
 **
 ** \retval Ok                    Selected normally
 ** \retval ErrorInvalidParameter enDmaIrq10Sel is out of range
 **
 ** \note This function is only avaible for FM3 TYPE2 product
 **
 ******************************************************************************/
en_result_t Dma_SetIrq10Selector(en_dma_irq10_sel_t enDmaIrq10Sel)
{
    switch (enDmaIrq10Sel)
    {
        case DmaIrq10Bt4Irq0:
            FM_INTREQ->DQESEL_f.ESEL10 = 0u;
            break;
        case DmaIrq10Usb1Ep1:
            FM_INTREQ->DRQSEL1_f.USB1EP1 = 1u;
            FM_INTREQ->DQESEL_f.ESEL10 = 1u;
            break;
        case DmaIrq10Usb1Ep2:
            FM_INTREQ->DRQSEL1_f.USB1EP2 = 1u;
            FM_INTREQ->DQESEL_f.ESEL10 = 2u;
            break;
        case DmaIrq10Usb1Ep3:
            FM_INTREQ->DRQSEL1_f.USB1EP3 = 1u;
            FM_INTREQ->DQESEL_f.ESEL10 = 3u;
            break;
        case DmaIrq10Usb1Ep4:
            FM_INTREQ->DRQSEL1_f.USB1EP4 = 1u;
            FM_INTREQ->DQESEL_f.ESEL10 = 4u;
            break;
        case DmaIrq10Usb1Ep5:  
            FM_INTREQ->DRQSEL1_f.USB1EP5 = 1u;
            FM_INTREQ->DQESEL_f.ESEL10 = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Select a source for the IRQ11 input of DMA
 **
 ** Re-select the DMA IRQ11 source
 **
 ** \param [in]  enDmaIrq11Sel    The sources which can be selected, see 
 **                               #en_dma_irq11_sel_t for the details.
 **
 ** \retval Ok                    Selected normally
 ** \retval ErrorInvalidParameter enDmaIrq11Sel is out of range
 **
 ** \note This function is only avaible for FM3 TYPE2 product
 **
 ******************************************************************************/
en_result_t Dma_SetIrq11Selector(en_dma_irq11_sel_t enDmaIrq11Sel)
{
    switch (enDmaIrq11Sel)
    {
        case DmaIrq11Bt6Irq0:
            FM_INTREQ->DQESEL_f.ESEL11 = 0u;
            break;
        case DmaIrq11Usb1Ep1:
            FM_INTREQ->DRQSEL1_f.USB1EP1 = 1u;
            FM_INTREQ->DQESEL_f.ESEL11 = 1u;
            break;
        case DmaIrq11Usb1Ep2:
            FM_INTREQ->DRQSEL1_f.USB1EP2 = 1u;
            FM_INTREQ->DQESEL_f.ESEL11 = 2u;
            break;
        case DmaIrq11Usb1Ep3:
            FM_INTREQ->DRQSEL1_f.USB1EP3 = 1u;
            FM_INTREQ->DQESEL_f.ESEL11 = 3u;
            break;
        case DmaIrq11Usb1Ep4:
            FM_INTREQ->DRQSEL1_f.USB1EP4 = 1u;
            FM_INTREQ->DQESEL_f.ESEL11 = 4u;
            break;
        case DmaIrq11Usb1Ep5:  
            FM_INTREQ->DRQSEL1_f.USB1EP5 = 1u;
            FM_INTREQ->DQESEL_f.ESEL11 = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Select a source for the IRQ24 input of DMA
 **
 ** Re-select the DMA IRQ24 source
 **
 ** \param [in]  enDmaIrq24Sel    The sources which can be selected, see 
 **                               #en_dma_irq24_sel_t for the details.
 **
 ** \retval Ok                    Selected normally
 ** \retval ErrorInvalidParameter enDmaIrq24Sel is out of range
 **
 ** \note This function is only avaible for FM3 TYPE2 product
 **
 ******************************************************************************/
en_result_t Dma_SetIrq24Selector(en_dma_irq24_sel_t enDmaIrq24Sel)
{
    switch (enDmaIrq24Sel)
    {
        case DmaIrq24Mfs6Rx:
            FM_INTREQ->DQESEL_f.ESEL24 = 0u;
            break;
        case DmaIrq24Usb1Ep1:
            FM_INTREQ->DRQSEL1_f.USB1EP1 = 1u;
            FM_INTREQ->DQESEL_f.ESEL24 = 1u;
            break;
        case DmaIrq24Usb1Ep2:
            FM_INTREQ->DRQSEL1_f.USB1EP2 = 1u;
            FM_INTREQ->DQESEL_f.ESEL24 = 2u;
            break;
        case DmaIrq24Usb1Ep3:
            FM_INTREQ->DRQSEL1_f.USB1EP3 = 1u;
            FM_INTREQ->DQESEL_f.ESEL24 = 3u;
            break;
        case DmaIrq24Usb1Ep4:
            FM_INTREQ->DRQSEL1_f.USB1EP4 = 1u;
            FM_INTREQ->DQESEL_f.ESEL24 = 4u;
            break;
        case DmaIrq24Usb1Ep5:  
            FM_INTREQ->DRQSEL1_f.USB1EP5 = 1u;
            FM_INTREQ->DQESEL_f.ESEL24 = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Select a source for the IRQ25 input of DMA
 **
 ** Re-select the DMA IRQ25 source
 **
 ** \param [in]  enDmaIrq25Sel    The sources which can be selected, see 
 **                               #en_dma_irq25_sel_t for the details.
 **
 ** \retval Ok                    Selected normally
 ** \retval ErrorInvalidParameter enDmaIrq25Sel is out of range
 **
 ** \note This function is only avaible for FM3 TYPE2 product
 **
 ******************************************************************************/
en_result_t Dma_SetIrq25Selector(en_dma_irq25_sel_t enDmaIrq25Sel)
{
    switch (enDmaIrq25Sel)
    {
        case DmaIrq25Mfs6Tx:
            FM_INTREQ->DQESEL_f.ESEL25 = 0u;
            break;
        case DmaIrq24Usb1Ep1:
            FM_INTREQ->DRQSEL1_f.USB1EP1 = 1u;
            FM_INTREQ->DQESEL_f.ESEL25 = 1u;
            break;
        case DmaIrq24Usb1Ep2:
            FM_INTREQ->DRQSEL1_f.USB1EP2 = 1u;
            FM_INTREQ->DQESEL_f.ESEL25 = 2u;
            break;
        case DmaIrq24Usb1Ep3:
            FM_INTREQ->DRQSEL1_f.USB1EP3 = 1u;
            FM_INTREQ->DQESEL_f.ESEL25 = 3u;
            break;
        case DmaIrq24Usb1Ep4:
            FM_INTREQ->DRQSEL1_f.USB1EP4 = 1u;
            FM_INTREQ->DQESEL_f.ESEL25 = 4u;
            break;
        case DmaIrq24Usb1Ep5:  
            FM_INTREQ->DRQSEL1_f.USB1EP5 = 1u;
            FM_INTREQ->DQESEL_f.ESEL25 = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Select a source for the IRQ26 input of DMA
 **
 ** Re-select the DMA IRQ26 source
 **
 ** \param [in]  enDmaIrq26Sel    The sources which can be selected, see 
 **                               #en_dma_irq26_sel_t for the details.
 **
 ** \retval Ok                    Selected normally
 ** \retval ErrorInvalidParameter enDmaIrq26Sel is out of range
 **
 ** \note This function is only avaible for FM3 TYPE2 product
 **
 ******************************************************************************/
en_result_t Dma_SetIrq26Selector(en_dma_irq26_sel_t enDmaIrq26Sel)
{
    switch (enDmaIrq26Sel)
    {
        case DmaIrq26Mfs7Rx:
            FM_INTREQ->DQESEL_f.ESEL26 = 0u;
            break;
        case DmaIrq26Usb1Ep1:
            FM_INTREQ->DRQSEL1_f.USB1EP1 = 1u;
            FM_INTREQ->DQESEL_f.ESEL26 = 1u;
            break;
        case DmaIrq26Usb1Ep2:
            FM_INTREQ->DRQSEL1_f.USB1EP2 = 1u;
            FM_INTREQ->DQESEL_f.ESEL26 = 2u;
            break;
        case DmaIrq26Usb1Ep3:
            FM_INTREQ->DRQSEL1_f.USB1EP3 = 1u;
            FM_INTREQ->DQESEL_f.ESEL26 = 3u;
            break;
        case DmaIrq26Usb1Ep4:
            FM_INTREQ->DRQSEL1_f.USB1EP4 = 1u;
            FM_INTREQ->DQESEL_f.ESEL26 = 4u;
            break;
        case DmaIrq26Usb1Ep5:  
            FM_INTREQ->DRQSEL1_f.USB1EP5 = 1u;
            FM_INTREQ->DQESEL_f.ESEL26 = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Select a source for the IRQ27 input of DMA
 **
 ** Re-select the DMA IRQ27 source
 **
 ** \param [in]  enDmaIrq27Sel    The sources which can be selected, see 
 **                               #en_dma_irq27_sel_t for the details.
 **
 ** \retval Ok                    Selected normally
 ** \retval ErrorInvalidParameter enDmaIrq27Sel is out of range
 **
 ** \note This function is only avaible for FM3 TYPE2 product
 **
 ******************************************************************************/
en_result_t Dma_SetIrq27Selector(en_dma_irq27_sel_t enDmaIrq27Sel)
{
    switch (enDmaIrq27Sel)
    {
        case DmaIrq27Mfs7Tx:
            FM_INTREQ->DQESEL_f.ESEL27 = 0u;
            break;
        case DmaIrq27Usb1Ep1:
            FM_INTREQ->DRQSEL1_f.USB1EP1 = 1u;
            FM_INTREQ->DQESEL_f.ESEL27 = 1u;
            break;
        case DmaIrq27Usb1Ep2:
            FM_INTREQ->DRQSEL1_f.USB1EP2 = 1u;
            FM_INTREQ->DQESEL_f.ESEL27 = 2u;
            break;
        case DmaIrq27Usb1Ep3:
            FM_INTREQ->DRQSEL1_f.USB1EP3 = 1u;
            FM_INTREQ->DQESEL_f.ESEL27 = 3u;
            break;
        case DmaIrq27Usb1Ep4:
            FM_INTREQ->DRQSEL1_f.USB1EP4 = 1u;
            FM_INTREQ->DQESEL_f.ESEL27 = 4u;
            break;
        case DmaIrq27Usb1Ep5:  
            FM_INTREQ->DRQSEL1_f.USB1EP5 = 1u;
            FM_INTREQ->DQESEL_f.ESEL27 = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Select a source for the IRQ30 input of DMA
 **
 ** Re-select the DMA IRQ30 source
 **
 ** \param [in]  enDmaIrq30Sel    The sources which can be selected, see 
 **                               #en_dma_irq30_sel_t for the details.
 **
 ** \retval Ok                    Selected normally
 ** \retval ErrorInvalidParameter enDmaIrq30Sel is out of range
 **
 ** \note This function is only avaible for FM3 TYPE2 product
 **
 ******************************************************************************/
en_result_t Dma_SetIrq30Selector(en_dma_irq30_sel_t enDmaIrq30Sel)
{
    switch (enDmaIrq30Sel)
    {
        case DmaIrq30ExtIrq2:
            FM_INTREQ->DQESEL_f.ESEL30 = 0u;
            break;
        case DmaIrq30Usb1Ep1:
            FM_INTREQ->DRQSEL1_f.USB1EP1 = 1u;
            FM_INTREQ->DQESEL_f.ESEL30 = 1u;
            break;
        case DmaIrq30Usb1Ep2:
            FM_INTREQ->DRQSEL1_f.USB1EP2 = 1u;
            FM_INTREQ->DQESEL_f.ESEL30 = 2u;
            break;
        case DmaIrq30Usb1Ep3:
            FM_INTREQ->DRQSEL1_f.USB1EP3 = 1u;
            FM_INTREQ->DQESEL_f.ESEL30 = 3u;
            break;
        case DmaIrq30Usb1Ep4:
            FM_INTREQ->DRQSEL1_f.USB1EP4 = 1u;
            FM_INTREQ->DQESEL_f.ESEL30 = 4u;
            break;
        case DmaIrq30Usb1Ep5:  
            FM_INTREQ->DRQSEL1_f.USB1EP5 = 1u;
            FM_INTREQ->DQESEL_f.ESEL30 = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Select a source for the IRQ31 input of DMA
 **
 ** Re-select the DMA IRQ31 source
 **
 ** \param [in]  enDmaIrq31Sel    The sources which can be selected, see 
 **                               #en_dma_irq31_sel_t for the details.
 **
 ** \retval Ok                    Selected normally
 ** \retval ErrorInvalidParameter enDmaIrq31Sel is out of range
 **
 ** \note This function is only avaible for FM3 TYPE2 product
 **
 ******************************************************************************/
en_result_t Dma_SetIrq31Selector(en_dma_irq31_sel_t enDmaIrq31Sel)
{
    switch (enDmaIrq31Sel)
    {
        case DmaIrq31ExtIrq3:
            FM_INTREQ->DQESEL_f.ESEL31 = 0u;
            break;
        case DmaIrq31Usb1Ep1:
            FM_INTREQ->DRQSEL1_f.USB1EP1 = 1u;
            FM_INTREQ->DQESEL_f.ESEL31 = 1u;
            break;
        case DmaIrq31Usb1Ep2:
            FM_INTREQ->DRQSEL1_f.USB1EP2 = 1u;
            FM_INTREQ->DQESEL_f.ESEL31 = 2u;
            break;
        case DmaIrq31Usb1Ep3:
            FM_INTREQ->DRQSEL1_f.USB1EP3 = 1u;
            FM_INTREQ->DQESEL_f.ESEL31 = 3u;
            break;
        case DmaIrq31Usb1Ep4:
            FM_INTREQ->DRQSEL1_f.USB1EP4 = 1u;
            FM_INTREQ->DQESEL_f.ESEL31 = 4u;
            break;
        case DmaIrq31Usb1Ep5:  
            FM_INTREQ->DRQSEL1_f.USB1EP5 = 1u;
            FM_INTREQ->DQESEL_f.ESEL31 = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

#endif

#endif // #if (defined(PDL_PERIPHERAL_DMA_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

