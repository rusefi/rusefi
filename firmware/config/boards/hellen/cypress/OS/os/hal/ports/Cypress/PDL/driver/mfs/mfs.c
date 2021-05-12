/*******************************************************************************
* \file             mfs.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the MFS 
*                   driver.
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
#include "mfs/mfs.h"

#if (defined(PDL_PERIPHERAL_MFS_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
#define UartInstanceToIndex(Instance) ((uint32_t)Instance - (uint32_t)&UART0)/0x100u
#define CsioInstanceToIndex(Instance) ((uint32_t)Instance - (uint32_t)&CSIO0)/0x100u
#define I2cInstanceToIndex(Instance)  ((uint32_t)Instance - (uint32_t)&I2C0)/0x100u
#define LinInstanceToIndex(Instance)  ((uint32_t)Instance - (uint32_t)&LIN0)/0x100u


/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/// Look-up table for all enabled MFS instances and their internal data
stc_mfs_instance_data_t m_astcMfsInstanceDataLut[MFS_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_MFS0 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS0_UART_BASE)
            &UART0, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS0_CSIO_BASE)
            &CSIO0,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS0_I2C_BASE)
            &I2C0,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS0_LIN_BASE)
            &LIN0
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet) 
        }  
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS1 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS1_UART_BASE)
            &UART1, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS1_CSIO_BASE)
            &CSIO1,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS1_I2C_BASE)
            &I2C1,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS1_LIN_BASE)
            &LIN1
          #endif
        }, /* pstcInstance */
        {
            MfsInitMode,           //  MFS initial mode 
            {{0u}},                // stcInternData (not initialized yet) 
        }  
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS2 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS2_UART_BASE)
            &UART2, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS2_CSIO_BASE)
            &CSIO2,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS2_I2C_BASE)
            &I2C2,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS2_LIN_BASE)
            &LIN2
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet) 
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS3 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS3_UART_BASE)
            &UART3, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS3_CSIO_BASE)
            &CSIO3,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS3_I2C_BASE)
            &I2C3,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS3_LIN_BASE)
            &LIN3
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet)
            
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS4 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS4_UART_BASE)
            &UART4, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS4_CSIO_BASE)
            &CSIO4,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS4_I2C_BASE)
            &I2C4,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS4_LIN_BASE)
            &LIN4
          #endif
        }, // pstcInstance
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet) 
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS5 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS5_UART_BASE)
            &UART5, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS5_CSIO_BASE)
            &CSIO5,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS5_I2C_BASE)
            &I2C5,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS5_LIN_BASE)
            &LIN5
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet)
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS6 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS6_UART_BASE)
            &UART6, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS6_CSIO_BASE)
            &CSIO6,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS6_I2C_BASE)
            &I2C6,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS6_LIN_BASE)
            &LIN6
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,                // MFS initial mode
            {{0u}},                     // stcInternData (not initialized yet)
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS7 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS7_UART_BASE)
            &UART7, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS7_CSIO_BASE)
            &CSIO7,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS7_I2C_BASE)
            &I2C7,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS7_LIN_BASE)
            &LIN7
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet) 
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS8 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS8_UART_BASE)
            &UART8, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS8_CSIO_BASE)
            &CSIO8,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS8_I2C_BASE)
            &I2C8,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS8_LIN_BASE)
            &LIN8
          #endif
        }, // pstcInstance
        {
            MfsInitMode,            // MFS initial mode
            {{0u}},                 // stcInternData (not initialized yet) 
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS9 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS9_UART_BASE)
            &UART9, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS9_CSIO_BASE)
            &CSIO9,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS9_I2C_BASE)
            &I2C9,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS9_LIN_BASE)
            &LIN9
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode
            {{0u}},                 // stcInternData (not initialized yet) 
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS10 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS10_UART_BASE)
            &UART10, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS10_CSIO_BASE)
            &CSIO10,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS10_I2C_BASE)
            &I2C10,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS10_LIN_BASE)
            &LIN10
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet) 
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS11 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS11_UART_BASE)
            &UART11, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS11_CSIO_BASE)
            &CSIO11,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS11_I2C_BASE)
            &I2C11,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS11_LIN_BASE)
            &LIN11
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet) 
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS12 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS12_UART_BASE) 
            &UART12, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS12_CSIO_BASE)
            &CSIO12,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS12_I2C_BASE)
            &I2C12,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS12_LIN_BASE)
            &LIN12
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet)
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS13 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS13_UART_BASE)
            &UART13, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS13_CSIO_BASE)
            &CSIO13,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS13_I2C_BASE)
            &I2C13,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS13_LIN_BASE)
            &LIN13
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet)
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS14 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS14_UART_BASE) 
            &UART14, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS14_CSIO_BASE)
            &CSIO14,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS14_I2C_BASE)
            &I2C14,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS14_LIN_BASE)
            &LIN14
          #endif
        }, // pstcInstance 
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet) 
        } 
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS15 == PDL_ON)
    {
        {
          #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS15_UART_BASE)
            &UART15, 
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS15_CSIO_BASE)
            &CSIO15,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS15_I2C_BASE)
            &I2C15,
          #endif
          #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS15_LIN_BASE)
            &LIN15
          #endif
        }, // pstcInstance
        {
            MfsInitMode,            // MFS initial mode 
            {{0u}},                 // stcInternData (not initialized yet)
        } 
    },
#endif
};

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
#if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON)  
static stc_mfs_intern_data_t* MfsUartGetInternDataPtr(volatile stc_mfsn_uart_t* pstcMfs);
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON)
static stc_mfs_intern_data_t* MfsCsioGetInternDataPtr(volatile stc_mfsn_csio_t* pstcMfs);
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON)
static stc_mfs_intern_data_t* MfsI2cGetInternDataPtr(volatile stc_mfsn_i2c_t* pstcMfs);
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON)
static stc_mfs_intern_data_t* MfsLinGetInternDataPtr(volatile stc_mfsn_lin_t* pstcMfs);
#endif
/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON) 
/**
 ******************************************************************************
 ** \brief Device dependent initialization of transmit interrupts
 **        according CMSIS with level defined in pdl.h
 **
 ** \param u8Ch   MFS channel
 ** \param bRxTx  FALSE: RX IRQ, TRUE: TX IRQ     
 **
 ******************************************************************************/
static void MfsInitNvic(uint8_t u8Ch, boolean_t bRxTx)
{
    IRQn_Type enIrqIndex; 
    
#if (PDL_MCU_CORE == PDL_FM0P_CORE)    
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    uint8_t au8PrioLevel[8] = 
    {
        PDL_IRQ_LEVEL_MFS0,
        PDL_IRQ_LEVEL_MFS1,
        0u,
        PDL_IRQ_LEVEL_MFS3,
        PDL_IRQ_LEVEL_MFS4,
        0u,
        PDL_IRQ_LEVEL_MFS6_I2CS0,
        PDL_IRQ_LEVEL_MFS7,
    };
    
    enIrqIndex = (IRQn_Type)(MFS0_RX_TX_IRQn + u8Ch);

    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex, au8PrioLevel[u8Ch]);
    NVIC_EnableIRQ(enIrqIndex);
    
#else    
    uint8_t au8PrioLevel[16] = 
    {
        PDL_IRQ_LEVEL_MFS0_RX,  PDL_IRQ_LEVEL_MFS0_TX,
        PDL_IRQ_LEVEL_MFS1_RX,  PDL_IRQ_LEVEL_MFS1_TX,
        PDL_IRQ_LEVEL_MFS2_RX,  PDL_IRQ_LEVEL_MFS2_TX,
        PDL_IRQ_LEVEL_MFS3_RX,  PDL_IRQ_LEVEL_MFS3_TX,
        PDL_IRQ_LEVEL_MFS4_RX,  PDL_IRQ_LEVEL_MFS4_TX,
        PDL_IRQ_LEVEL_MFS5_RX,  PDL_IRQ_LEVEL_MFS5_TX,
        PDL_IRQ_LEVEL_MFS6_RX_DMA0, PDL_IRQ_LEVEL_MFS6_TX_DMA1,
        PDL_IRQ_LEVEL_MFS7_RX_DMA2, PDL_IRQ_LEVEL_MFS7_TX_DMA3
    };

    if(FALSE == bRxTx)
    {
        enIrqIndex = (IRQn_Type)(MFS0_RX_IRQn + u8Ch*2u);

        NVIC_ClearPendingIRQ(enIrqIndex);
        NVIC_SetPriority(enIrqIndex, au8PrioLevel[u8Ch*2u]);
        NVIC_EnableIRQ(enIrqIndex);
    }
    else
    {
        enIrqIndex = (IRQn_Type)(MFS0_TX_IRQn + u8Ch*2u );

        NVIC_ClearPendingIRQ(enIrqIndex);
        NVIC_SetPriority(enIrqIndex, au8PrioLevel[u8Ch*2u+1]);
        NVIC_EnableIRQ(enIrqIndex);
    }
#endif                         
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
    uint8_t au8PrioLevel[16] = 
    {
        PDL_IRQ_LEVEL_MFS0_8_RX,  PDL_IRQ_LEVEL_MFS0_8_TX,
        PDL_IRQ_LEVEL_MFS1_9_RX,  PDL_IRQ_LEVEL_MFS1_9_TX,
        PDL_IRQ_LEVEL_MFS2_10_RX, PDL_IRQ_LEVEL_MFS2_10_TX,
        PDL_IRQ_LEVEL_MFS3_11_RX, PDL_IRQ_LEVEL_MFS3_11_TX,
        PDL_IRQ_LEVEL_MFS4_12_RX, PDL_IRQ_LEVEL_MFS4_12_TX,
        PDL_IRQ_LEVEL_MFS5_13_RX, PDL_IRQ_LEVEL_MFS5_13_TX,
        PDL_IRQ_LEVEL_MFS6_14_RX, PDL_IRQ_LEVEL_MFS6_14_TX,
        PDL_IRQ_LEVEL_MFS7_15_RX, PDL_IRQ_LEVEL_MFS7_15_TX,
    };
      
    if(8 <= u8Ch)
    {
      u8Ch = u8Ch - 8;
    }

  #if defined(IRQ_MFS0_RX_AVAILABLE)
    if(FALSE == bRxTx)
    {
        enIrqIndex = (IRQn_Type)(MFS0_RX_IRQn + u8Ch*2u );

        NVIC_ClearPendingIRQ(enIrqIndex);
        NVIC_SetPriority(enIrqIndex, au8PrioLevel[u8Ch*2u]);
        NVIC_EnableIRQ(enIrqIndex);
    }
    else
    {
        enIrqIndex = (IRQn_Type)(MFS0_TX_IRQn + u8Ch*2u );

        NVIC_ClearPendingIRQ(enIrqIndex);
        NVIC_SetPriority(enIrqIndex, au8PrioLevel[u8Ch*2u+1]);
        NVIC_EnableIRQ(enIrqIndex);
    } 
  #else
    if(FALSE == bRxTx)
    {
        enIrqIndex = (IRQn_Type)(MFS0_8_RX_IRQn + u8Ch*2u );

        NVIC_ClearPendingIRQ(enIrqIndex);
        NVIC_SetPriority(enIrqIndex, au8PrioLevel[u8Ch*2u]);
        NVIC_EnableIRQ(enIrqIndex);
    }
    else
    {
        enIrqIndex = (IRQn_Type)(MFS0_8_TX_IRQn + u8Ch*2u );

        NVIC_ClearPendingIRQ(enIrqIndex);
        NVIC_SetPriority(enIrqIndex, au8PrioLevel[u8Ch*2u+1]);
        NVIC_EnableIRQ(enIrqIndex);
    } 
  #endif
#elif (PDL_MCU_CORE == PDL_FM4_CORE)
    uint8_t au8PrioLevel0[16] = 
    {
        PDL_IRQ_LEVEL_MFS0_RX,  PDL_IRQ_LEVEL_MFS0_TX,
        PDL_IRQ_LEVEL_MFS1_RX,  PDL_IRQ_LEVEL_MFS1_TX,
        PDL_IRQ_LEVEL_MFS2_RX,  PDL_IRQ_LEVEL_MFS2_TX,
        PDL_IRQ_LEVEL_MFS3_RX,  PDL_IRQ_LEVEL_MFS3_TX,
        PDL_IRQ_LEVEL_MFS4_RX,  PDL_IRQ_LEVEL_MFS4_TX,
        PDL_IRQ_LEVEL_MFS5_RX,  PDL_IRQ_LEVEL_MFS5_TX,
        PDL_IRQ_LEVEL_MFS6_RX,  PDL_IRQ_LEVEL_MFS6_TX,
        PDL_IRQ_LEVEL_MFS7_RX,  PDL_IRQ_LEVEL_MFS7_TX,

    }; 
#if (PDL_MCU_TYPE == PDL_FM4_TYPE3)
    uint8_t au8PrioLevel1[8] = 
    {
        PDL_IRQ_LEVEL_MFS8_RX,  PDL_IRQ_LEVEL_MFS8_TX,
        PDL_IRQ_LEVEL_MFS9_RX,  PDL_IRQ_LEVEL_MFS9_TX,
        PDL_IRQ_LEVEL_MFS10_RX,  PDL_IRQ_LEVEL_MFS10_TX,
        PDL_IRQ_LEVEL_MFS11_RX,  PDL_IRQ_LEVEL_MFS11_TX,
    };
    
    uint8_t au8PrioLevel2[8] = 
    {
        PDL_IRQ_LEVEL_MFS12_RX,  PDL_IRQ_LEVEL_MFS12_TX,
        PDL_IRQ_LEVEL_MFS13_RX,  PDL_IRQ_LEVEL_MFS13_TX,
        PDL_IRQ_LEVEL_MFS14_RX,  PDL_IRQ_LEVEL_MFS14_TX,
        PDL_IRQ_LEVEL_MFS15_RX,  PDL_IRQ_LEVEL_MFS15_TX,
    };
#elif (PDL_MCU_TYPE == PDL_FM4_TYPE5)
    uint8_t au8PrioLevel1[4] = 
    {
        PDL_IRQ_LEVEL_MFS8_RX,  PDL_IRQ_LEVEL_MFS8_TX,
        PDL_IRQ_LEVEL_MFS9_RX,  PDL_IRQ_LEVEL_MFS9_TX,
    };
#endif
    if(FALSE == bRxTx)
    {
        enIrqIndex = (IRQn_Type)(MFS0_RX_IRQn + (2u * u8Ch));
        if(u8Ch < 8u)
        {
            NVIC_SetPriority(enIrqIndex, au8PrioLevel0[u8Ch*2u]);
        }
    #if ((PDL_MCU_TYPE == PDL_FM4_TYPE3) || (PDL_MCU_TYPE == PDL_FM4_TYPE5))
        else if((u8Ch >= 8u) && (u8Ch < 12u))
        {
            enIrqIndex = (IRQn_Type)(MFS8_RX_IRQn + 2u * (u8Ch - 8u));
            NVIC_SetPriority(enIrqIndex, au8PrioLevel1[2u * (u8Ch - 8u)]);
        }
    #endif    
    #if (PDL_MCU_TYPE == PDL_FM4_TYPE3)     
        else if((u8Ch >= 12u) && (u8Ch < 16u))
        {
            enIrqIndex = (IRQn_Type)(MFS12_RX_IRQn + 2u * (u8Ch - 12u));
            NVIC_SetPriority(enIrqIndex, au8PrioLevel2[2u * (u8Ch - 12u)]);
        }
        
    #endif 
        else
        {
            return;
        }

        NVIC_ClearPendingIRQ(enIrqIndex);
        NVIC_EnableIRQ(enIrqIndex);
    }
    else
    {
        enIrqIndex = (IRQn_Type)(MFS0_TX_IRQn + 2u * u8Ch);
        if(u8Ch < 8u)
        {
            NVIC_SetPriority(enIrqIndex, au8PrioLevel0[(2u * u8Ch) + 1u]);
        }
    #if (PDL_MCU_TYPE == PDL_FM4_TYPE3) || (PDL_MCU_TYPE == PDL_FM4_TYPE5)
        else if((u8Ch >= 8u) && (u8Ch < 12u))
        {
            enIrqIndex = (IRQn_Type)(MFS8_TX_IRQn + 2u * (u8Ch - 8u));
            NVIC_SetPriority(enIrqIndex, au8PrioLevel1[2u * (u8Ch - 8u) + 1u]);
        }
    #endif    
    #if (PDL_MCU_TYPE == PDL_FM4_TYPE3) 
        else if((u8Ch >= 12u) && (u8Ch < 16u))
        {
            enIrqIndex = (IRQn_Type)(MFS12_TX_IRQn + 2u * (u8Ch - 12u));
            NVIC_SetPriority(enIrqIndex, au8PrioLevel2[2u * (u8Ch - 12u) + 1u]);
        }
    #endif
        NVIC_ClearPendingIRQ(enIrqIndex);
        NVIC_EnableIRQ(enIrqIndex);
    }                              
#endif                         
        
} /* Mfs_InitTxIrq */

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ** \param u8Ch   MFS channel
 ** \param bRxTx  FALSE: RX IRQ, TRUE: TX IRQ     
 **
 ******************************************************************************/
static void MfsDeInitNvic(uint8_t u8Ch, boolean_t bRxTx)
{
    IRQn_Type enIrqIndex; 
  
#if (PDL_MCU_CORE == PDL_FM3_CORE) 
  
    if(8 <= u8Ch)
    {
      u8Ch = u8Ch - 8;
    }

  #if defined(IRQ_MFS0_RX_AVAILABLE)  
    if(FALSE == bRxTx)
    {
        enIrqIndex = (IRQn_Type)(MFS0_RX_IRQn + 2 * u8Ch);
    }
    else
    {
        enIrqIndex = (IRQn_Type)(MFS0_TX_IRQn + 2 * u8Ch);

    }
  #else
    if(FALSE == bRxTx)
    {
        enIrqIndex = (IRQn_Type)(MFS0_8_RX_IRQn + 2 * u8Ch);
    }
    else
    {
        enIrqIndex = (IRQn_Type)(MFS0_8_TX_IRQn + 2 * u8Ch);

    }
  #endif
    
    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_DisableIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex, PDL_DEFAULT_INTERRUPT_LEVEL);
#elif (PDL_MCU_CORE == PDL_FM0P_CORE) 
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    enIrqIndex = (IRQn_Type)(MFS0_RX_TX_IRQn + u8Ch);
    
    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_DisableIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex, PDL_DEFAULT_INTERRUPT_LEVEL);
#else
    if(FALSE == bRxTx)
    {
        enIrqIndex = (IRQn_Type)(MFS0_RX_IRQn + 2 * u8Ch);
    }
    else
    {
        enIrqIndex = (IRQn_Type)(MFS0_TX_IRQn + 2 * u8Ch);

    }

    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_DisableIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex, PDL_DEFAULT_INTERRUPT_LEVEL);    
#endif    
#elif (PDL_MCU_CORE == PDL_FM4_CORE)
    if(FALSE == bRxTx)
    {
        if(u8Ch < 8u)
        {
            enIrqIndex = (IRQn_Type)(MFS0_RX_IRQn + 2u * u8Ch);
        }
    #if ((PDL_MCU_TYPE == PDL_FM4_TYPE3) || (PDL_MCU_TYPE == PDL_FM4_TYPE5))
        else if((u8Ch >= 8u) && (u8Ch < 12u))
        {
            enIrqIndex = (IRQn_Type)(MFS8_RX_IRQn + 2u * (u8Ch - 8u));
        }
    #endif
    #if (PDL_MCU_TYPE == PDL_FM4_TYPE3)
        else if((u8Ch >= 12u) && (u8Ch < 16u))
        {
            enIrqIndex = (IRQn_Type)(MFS12_RX_IRQn + 2u * (u8Ch - 12u));
        }
    #endif 
        else
        {
            return;
        }
    }
    else
    {
        if(u8Ch < 8u)
        {
            enIrqIndex = (IRQn_Type)(MFS0_TX_IRQn + 2 * u8Ch);
        }
    #if (PDL_MCU_TYPE == PDL_FM4_TYPE3) || (PDL_MCU_TYPE == PDL_FM4_TYPE5) 
        else if((u8Ch >= 8u) && (u8Ch < 12u))
        {
            enIrqIndex = (IRQn_Type)(MFS8_TX_IRQn + 2u * (u8Ch - 8u));
        }
    #endif
    #if (PDL_MCU_TYPE == PDL_FM4_TYPE3) 
        else if((u8Ch >= 12u) && (u8Ch < 16u))
        {
            enIrqIndex = (IRQn_Type)(MFS12_TX_IRQn + 2u * (u8Ch - 12u));
        }
    #endif 
        else
        {
            return;
        }
    }
    
    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_DisableIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex, PDL_DEFAULT_INTERRUPT_LEVEL);
    
#endif 
    
} /* Mfs_DeInitIrq */

#endif

#if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON)    
/******************************************************************************
 * UART driver functions
 ******************************************************************************/  
/**
 ******************************************************************************
 ** \brief Return the internal data for a certain UART instance.
 **
 ** \param pstcUart Pointer to UART instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_mfs_intern_data_t* MfsUartGetInternDataPtr(volatile stc_mfsn_uart_t* pstcUart)
{
    stc_mfs_intern_data_t* pstcInternDataPtr = NULL;
    uint32_t u32Instance;

    if (NULL != pstcUart)
    {
        for (u32Instance = 0u; u32Instance < (uint32_t)MfsInstanceIndexMax; u32Instance++)
        {
            if (pstcUart == m_astcMfsInstanceDataLut[u32Instance].stcInstance.pstcUartInstance)
            {
                pstcInternDataPtr = &m_astcMfsInstanceDataLut[u32Instance].stcInternData;
                break;
            }
        }
            
    }

    return (pstcInternDataPtr);
} /* MfsGetInternDataPtr */


#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)       
/**
 ******************************************************************************
 ** \brief UART receive interrupt service routine.
 ** 
 ** \param pstcUart Pointer to UART instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure       
 ** 
 ** This function is called on Receive Interrupt set by the MFS. 
 **
 ******************************************************************************/
void MfsUartIrqHandlerRx( volatile stc_mfsn_uart_t*   pstcUart,
                          stc_mfs_intern_data_t* pstcMfsInternData)
{
    if ((NULL != pstcUart) && (NULL != pstcMfsInternData))
    {
        if (NULL != pstcMfsInternData->stcUartInternIrqCb.pfnRxIrqCb)
        {
            pstcMfsInternData->stcUartInternIrqCb.pfnRxIrqCb();
        }
    }
} /* MfsIrqHandlerRx */

/**
 ******************************************************************************
 ** \brief MFS transmit interrupt service routine.
 **
 ** \param pstcUart Pointer to UART instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure     
 **
 ** This function is called on Transmit Interrupt set by the MFS. 
 ** 
 ******************************************************************************/
void MfsUartIrqHandlerTx( volatile stc_mfsn_uart_t*   pstcUart, 
                          stc_mfs_intern_data_t* pstcMfsInternData)
{
    if ((NULL != pstcUart) && (NULL != pstcMfsInternData))
    {
        if(1u == pstcUart->SSR_f.TDRE)
        {
            if (NULL != pstcMfsInternData->stcUartInternIrqCb.pfnTxIrqCb)
            {
                pstcMfsInternData->stcUartInternIrqCb.pfnTxIrqCb();
            }
        }
        
        if(1u == pstcUart->SSR_f.TBI)
        {
            if (NULL != pstcMfsInternData->stcUartInternIrqCb.pfnTxIdleCb)
            {
                pstcMfsInternData->stcUartInternIrqCb.pfnTxIdleCb();
            }
        }
        
        if(1u == pstcUart->FCR_f.FDRQ)
        {
            if (NULL != pstcMfsInternData->stcUartInternIrqCb.pfnTxFifoIrqCb)
            {
                pstcMfsInternData->stcUartInternIrqCb.pfnTxFifoIrqCb();
            }
            
            pstcUart->FCR_f.FDRQ = 0u;
        }
    }
} /* MfsIrqHandlerTx */
            
/**
 ******************************************************************************
 ** \brief Enable one interrupt source of UART 
 **
 ** \param [in] pstcUart   Pointer to UART instance   
 ** \param [in] enIrqSel   Pointer to the selection structure of UART interrupt
 ** \arg UartTxIrq         UART TX interrupt
 ** \arg UartRxIrq         UART RX interrupt
 ** \arg UartTxIdleIrq     UART TX idle interrupt
 ** \arg UartTxFifoIrq     UART TX FIFO interrupt
 **
 ** \retval Ok                    Interrupts has been enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 **                               - Other invalid configuration                   
 ** 
 ******************************************************************************/
en_result_t Mfs_Uart_EnableIrq(volatile stc_mfsn_uart_t* pstcUart, 
                               en_uart_irq_sel_t enIrqSel)
{
    if (NULL == pstcUart)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case UartTxIrq:
            pstcUart->SCR_f.TIE = 1u;
            break;
        case UartRxIrq:
            pstcUart->SCR_f.RIE = 1u;
            break;
        case UartTxIdleIrq:
            pstcUart->SCR_f.TBIE = 1u;
            break;
        case UartTxFifoIrq:
            pstcUart->FCR_f.FTIE = 1u;
            break;
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable one interrupt source of UART 
 **
 ** \param [in] pstcUart   Pointer to UART instance   
 ** \param [in] enIrqSel   Pointer to the selection structure of UART interrupt
 ** \arg UartTxIrq         UART TX interrupt
 ** \arg UartRxIrq         UART RX interrupt
 ** \arg UartTxIdleIrq     UART TX idle interrupt
 ** \arg UartTxFifoIrq     UART TX FIFO interrupt
 **
 ** \retval Ok                    Interrupts has been disabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 **                               - Other invalid configuration                   
 ** 
 ******************************************************************************/
en_result_t Mfs_Uart_DisableIrq(volatile stc_mfsn_uart_t* pstcUart, 
                                en_uart_irq_sel_t enIrqSel)
{
    if (NULL == pstcUart)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case UartTxIrq:
            pstcUart->SCR_f.TIE = 0u;
            break;
        case UartRxIrq:
            pstcUart->SCR_f.RIE = 0u;
            break;
        case UartTxIdleIrq:
            pstcUart->SCR_f.TBIE = 0u;
            break;
        case UartTxFifoIrq:
            pstcUart->FCR_f.FTIE = 0u;
            break;
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

#endif     

/**
 ******************************************************************************
 ** \brief Initialization of a MFS module to activate as UART.
 **
 ** Set registers to active MFS as UART.
 **
 ** \param [in]   pstcUart        Pointer to UART instance register area
 ** \param [in]   pstcConfig      MFS UART configuration
 **
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcUart == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_Uart_Init(volatile stc_mfsn_uart_t*         pstcUart,
                          const stc_mfs_uart_config_t* pstcConfig)
{
    stc_mfs_intern_data_t* pstcMfsInternData;
    uint8_t u8Ch;
    
    /* Preset local register variables to zero */
    stc_mfs_uart_smr_field_t  stcSMR;
    stc_mfs_uart_scr_field_t  stcSCR;
    stc_mfs_uart_escr_field_t stcESCR;
   
    PDL_ZERO_STRUCT(stcSMR);
    PDL_ZERO_STRUCT(stcSCR);
    PDL_ZERO_STRUCT(stcESCR);
    
    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsInternData = MfsUartGetInternDataPtr(pstcUart);
    
    /* Parameter check and get ptr to internal data struct */
    if ((NULL == pstcMfsInternData)
    ||  (NULL == pstcConfig)
       )
    {
        return (ErrorInvalidParameter);
    }

    /* Calculate the channel number */
    u8Ch = UartInstanceToIndex(pstcUart);
    
    if(u8Ch > 15u)
    {
        return (ErrorInvalidParameter);
    }
    
    /* Set the MFS mode in the internal structure */
    pstcMfsInternData->enMode = MfsUartMode;
    
    /* Set UART mode */
    switch (pstcConfig->enMode)
    {
        case UartNormal:
            stcSMR.MD = 0u;   /* Normal mode */
            break;
        case UartMulti:
            stcSMR.MD = 1u;    /* Multi-processor mode */
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Enable SOE */
    stcSMR.SOE = TRUE;
    /* Set Parity */
    switch(pstcConfig->enParity)
    {
        case UartParityNone:
            stcESCR.P   = FALSE; /* Parity type selection (0-even/1-odd) */
            stcESCR.PEN = FALSE; /* Parity disable */
            break;
        case UartParityEven:
            stcESCR.P   = FALSE; /* Parity type selection (0-even/1-odd) */
            stcESCR.PEN = TRUE;  /* Parity Enable */
            break;
        case UartParityOdd:
            stcESCR.P   = TRUE;  /* Parity type selection (0-even/1-odd) */
            stcESCR.PEN = TRUE;  /* Parity Enable */
            break;
        default:
            return (ErrorInvalidParameter);
    }
    /* Set Stop bit length */
    switch (pstcConfig->enStopBit)
    {
        case UartOneStopBit:
            stcSMR.SBL   = FALSE;
            stcESCR.ESBL = FALSE;
            break;
        case UartTwoStopBits:
            stcSMR.SBL   = TRUE;
            stcESCR.ESBL = FALSE;
            break;     
        case UartThreeStopBits:
            stcSMR.SBL   = FALSE;
            stcESCR.ESBL = TRUE;
            break;
        case UartFourStopBits:
            stcSMR.SBL   = TRUE;
            stcESCR.ESBL = TRUE;
            break;
        default:
            return (ErrorInvalidParameter);
    }    
    /* Set Data bit length (5 - 9bits) */
    switch(pstcConfig->enDataLength)
    {
        case UartFiveBits:
            stcESCR.L = UartFiveBits;
            break;
        case UartSixBits:
            stcESCR.L = UartSixBits;
            break;
        case UartSevenBits:
            stcESCR.L = UartSevenBits;
            break;   
        case UartEightBits:
            stcESCR.L = UartEightBits;
            break;
        case UartNineBits:
            stcESCR.L = UartNineBits;
            break;
        default:
            return (ErrorInvalidParameter);
    }
    /* Set Bit direction (LSB/MSB) */    
    switch(pstcConfig->enBitDirection)
    {
        case UartDataLsbFirst:
            stcSMR.BDS = 0;
            break;
        case UartDataMsbFirst:
            stcSMR.BDS = 1;
            break;  
        default:
            return ErrorInvalidParameter;  
    }

    /* HW Flow */
    if (TRUE == pstcConfig->bHwFlow)
    {
        stcESCR.FLWEN = TRUE;
    }
    else
    {
        stcESCR.FLWEN = FALSE;
    }

    /* Set Signal system (NRZ/Int-NRZ) */
    if (TRUE == pstcConfig->bInvertData)
    {
        stcESCR.INV = TRUE;
    }
    else
    {
        stcESCR.INV = FALSE;
    }
    
    
    /* Clear MFS by setting the Software Reset bit */
    pstcUart->SCR_f.UPCL = TRUE;

    /* Set Baudrate */
    (void)Mfs_Uart_SetBaudRate(pstcUart, pstcConfig->u32BaudRate);
    /* Set registers value */
    pstcUart->SMR_f  = stcSMR;
    pstcUart->SCR_f  = stcSCR;
    pstcUart->ESCR_f = stcESCR;
    
    /* Set external clock */
    pstcUart->BGR_f.EXT = ((pstcConfig->bUseExtClk == TRUE) ? 1u : 0u);
    
    /* Configue FIFO */
    if(pstcConfig->pstcFifoConfig != NULL)
    {
        /* Reset FIFO */
        pstcUart->FCR_f.FCL1 = 1;
        pstcUart->FCR_f.FCL2 = 1;
        /* Enable FIFO receive Idle detection */
        pstcUart->FCR_f.FRIIE = 1u;
        /* Selection TX and RX FIFO  */
        switch(pstcConfig->pstcFifoConfig->enFifoSel)
        {
            case MfsTxFifo1RxFifo2:
                pstcUart->FCR_f.FSEL = 0u;
                break;
            case MfsTxFifo2RxFifo1:
                pstcUart->FCR_f.FSEL = 1u;
                break;
            default:
               return (ErrorInvalidParameter);
        }
        /* Set FIFO count */
        pstcUart->FBYTE1 = pstcConfig->pstcFifoConfig->u8ByteCount1;
        pstcUart->FBYTE2 = pstcConfig->pstcFifoConfig->u8ByteCount2;
        /* Enable FIFO  */
        pstcUart->FCR_f.FE1 = 1u;
        pstcUart->FCR_f.FE2 = 1u;
    }

#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)      
    /* Configue interrupts */
    if(NULL != pstcConfig->pstcIrqEn)
    {
        if(TRUE == pstcConfig->pstcIrqEn->bRxIrq)
        {
            
            pstcUart->SCR_f.RIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxIrq)
        {
            
            pstcUart->SCR_f.TIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxIdleIrq)
        {
            
            pstcUart->SCR_f.TBIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxFifoIrq)
        {
            
            pstcUart->FCR_f.FTIE = 1u;
        }
    }
    
    /* Configure callback functions */
    if(NULL != pstcConfig->pstcIrqCb)
    {
        pstcMfsInternData->stcUartInternIrqCb.pfnRxIrqCb = pstcConfig->pstcIrqCb->pfnRxIrqCb;
        pstcMfsInternData->stcUartInternIrqCb.pfnTxIrqCb = pstcConfig->pstcIrqCb->pfnTxIrqCb;
        pstcMfsInternData->stcUartInternIrqCb.pfnTxIdleCb = pstcConfig->pstcIrqCb->pfnTxIdleCb;
        pstcMfsInternData->stcUartInternIrqCb.pfnTxFifoIrqCb = pstcConfig->pstcIrqCb->pfnTxFifoIrqCb;
    }
    
    /* Configure NVIC */
    if(TRUE == pstcConfig->bTouchNvic)
    {
        MfsInitNvic(u8Ch, FALSE);
        MfsInitNvic(u8Ch, TRUE);
    }
#endif
    
    return (Ok);
} /* MFS_Uart_Init  */

/**
 ******************************************************************************
 ** \brief Deinitialisation of a MFS module activating as UART.
 **
 ** All used uart register are reset to their default values.
 **  
 ** \param [in]   pstcUart      Pointer to UART instance register area
 ** \param [in]   bTouchNvic    Touch NVIC or not
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcUart == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_Uart_DeInit(volatile stc_mfsn_uart_t* pstcUart, boolean_t bTouchNvic)
{
    en_result_t            enResult;
    stc_mfs_intern_data_t* pstcMfsInternData;
    uint8_t u8Ch;
    
    /* Calculate the channel number */
    u8Ch = UartInstanceToIndex(pstcUart);
    
    if(u8Ch > 15u)
    {
        return (ErrorInvalidParameter);
    }

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsInternData = MfsUartGetInternDataPtr(pstcUart);
    /* ... and check */
    if (NULL == pstcMfsInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        pstcUart->SCR_f.TXE = FALSE;
        pstcUart->SCR_f.RXE = FALSE;

        /* Baud Rate Generation Reload Reset */
        pstcUart->BGR = 0u;
            
        /* Serial Mode Register clear all bits (valid in any mode) */
        pstcUart->SMR = 0u;
           
        /* Reset Mfs receive and transmit bit to default */
        /* and clear all error flags */
        pstcUart->SCR = 0u;

        /* Clear reception Errors */
        pstcUart->SSR_f.REC = TRUE;
           
        /* Reset all other used register to default value */
        pstcUart->SSR  = 0u;
        pstcUart->ESCR = 0u;
        pstcUart->FCR = 0u;


        /* Clear MFS by setting the Software Reset bit */
        pstcUart->SCR_f.UPCL = TRUE;

#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)       
        /* Disable NVIC */  
        if(TRUE == bTouchNvic)
        {
            MfsDeInitNvic(u8Ch, FALSE);
            MfsDeInitNvic(u8Ch, TRUE);
        }
#endif        
        enResult = Ok;
    }

    return (enResult);
} /* Mfs_Uart_DeInit */

/**
 ******************************************************************************
 ** \brief Set the baudrate of UART
 **
 ** \param [in] pstcUart    Pointer to UART instance   
 ** \param [in] u32BaudRate Baudrate value [bps]
 ** 
 ** \retval Ok                    UART baud rate has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 **                               - u32BaudRate < 4         
 ** \note
 ** The UART baud rate can be initialized in the Mfs_Uart_Init() and be modified
 ** in the funciton.
 ** 
 ******************************************************************************/
en_result_t Mfs_Uart_SetBaudRate(volatile stc_mfsn_uart_t* pstcUart,
                                 uint32_t u32BaudRate)
{
    uint32_t u32Pclk1;
	
    SystemCoreClockUpdate();  
    if ((NULL == pstcUart) || (u32BaudRate < 4u))
    {
        return ErrorInvalidParameter;
    }
    
#if (PDL_MCU_CORE == PDL_FM0P_CORE)    
    u32Pclk1 = SystemCoreClock / (1ul << (FM_CRG->APBC1_PSR & 0x03u)); /* MFS is attached on APB1 bus in FM0+ device */
#else
    u32Pclk1 = SystemCoreClock / (1ul << (FM_CRG->APBC2_PSR & 0x03u)); /* MFS is attached on APB2 bus in FM3, FM4 device */
#endif    
    pstcUart->BGR_f.BGR = (u32Pclk1/u32BaudRate) - 1;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable UART functions
 **
 ** \param [in] pstcUart   Pointer to UART instance   
 ** \param [in] enFunc     UART function types
 ** \arg   UartTx    UART transfer function
 ** \arg   UartRx    UART receive function
 ** 
 ** \retval Ok                    Function has been enabled normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 **                               - Other invalid configuration                   
 ** 
 ******************************************************************************/
en_result_t Mfs_Uart_EnableFunc(volatile stc_mfsn_uart_t* pstcUart, en_uart_func_t enFunc)
{
    if (NULL == pstcUart)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFunc)
    {
        case UartTx:
            pstcUart->SCR_f.TXE = 1u;
            break;
        case UartRx:
            pstcUart->SCR_f.RXE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable UART functions
 **
 ** \param [in] pstcUart   Pointer to UART instance   
 ** \param [in] enFunc     UART function types
 ** \arg   UartTx    UART transfer function
 ** \arg   UartRx    UART receive function
 ** 
 ** \retval Ok                    Function has been disabled normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 **                               - Other invalid configuration                   
 ** 
 ******************************************************************************/
en_result_t Mfs_Uart_DisableFunc(volatile stc_mfsn_uart_t* pstcUart, en_uart_func_t enFunc)
{
    if (NULL == pstcUart)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFunc)
    {
        case UartTx:
            pstcUart->SCR_f.TXE = 0u;
            break;
        case UartRx:
            pstcUart->SCR_f.RXE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get status of UART according to status type
 **
 ** \param [in] pstcUart     Pointer to UART instance   
 ** \param [in] enStatus     UART status type
 ** \arg   UartParityError   UART parity error
 ** \arg   UartFrameError    UART frame error
 ** \arg   UartRxFull        UART receive buffer full
 ** \arg   UartTxEmpty       UART tranfer buffer empty
 ** \arg   UartTxIdle        UART tranfer idle status
 ** \arg   UartTxFifoRequest UART transfer FIFO request
 ** 
 ** \retval FALSE                 If one of following conditions are met:
 **                               - No UART parity error [enStatus = UartParityError]
 **                               - No UART frame error  [enStatus = UartFrameError]     
 **                               - UART receive buffer is not full [enStatus = UartRxFull]
 **                               - UART tranfer buffer is not empty [enStatus = UartTxEmpty]
 **                               - UART tranfer is on-going [enStatus = UartTxIdle]
 **                               - No UART transfer FIFO request [enStatus = UartTxFifoRequest] 
 ** \retval TRUE                 If one of following conditions are met:
 **                               - UART parity error occurs [enStatus = UartParityError]
 **                               - UART frame error occurs  [enStatus = UartFrameError]    
 **                               - UART receive buffer is full [enStatus = UartRxFull]
 **                               - UART tranfer buffer is empty [enStatus = UartTxEmpty]
 **                               - UART tranfer is idle [enStatus = UartTxIdle]
 **                               - UART transfer FIFO request issues [enStatus = UartTxFifoRequest] 
 ** 
 ******************************************************************************/
boolean_t Mfs_Uart_GetStatus(volatile stc_mfsn_uart_t* pstcUart, 
                             en_uart_status_t enStatus)
{
    boolean_t bResult = FALSE;
    
    switch(enStatus)
    {
        case UartParityError:
            bResult = (pstcUart->SSR_f.PE == 1u) ? TRUE : FALSE;
            break;  
        case UartFrameError:
            bResult = (pstcUart->SSR_f.FRE == 1u) ? TRUE : FALSE;
            break;  
        case UartOverrunError:
            bResult = (pstcUart->SSR_f.ORE == 1u) ? TRUE : FALSE;
            break;  
        case UartRxFull:
            bResult = (pstcUart->SSR_f.RDRF == 1u) ? TRUE : FALSE;
            break;  
        case UartTxEmpty:
            bResult = (pstcUart->SSR_f.TDRE == 1u) ? TRUE : FALSE;
            break;  
        case UartTxIdle:
            bResult = (pstcUart->SSR_f.TBI == 1u) ? TRUE : FALSE;
            break;  
        case UartTxFifoRequest:
            bResult = (pstcUart->FCR_f.FDRQ == 1u) ? TRUE : FALSE;
            break;  
        default:
            break;  
    }
    
    return bResult;
}

/**
 ******************************************************************************
 ** \brief Clear status of UART according to status type
 **
 ** \param [in] pstcUart     Pointer to UART instance   
 ** \param [in] enStatus     UART status type
 ** \arg   UartParityError   UART parity error
 ** \arg   UartFrameError    UART frame error
 ** \arg   UartRxFull        UART receive buffer full
 ** \arg   UartTxEmpty       UART tranfer buffer empty
 ** \arg   UartTxIdle        UART tranfer idle status
 ** \arg   UartTxFifoRequest UART transfer FIFO request
 ** 
 ** \retval Ok                    Status has been cleared normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 ** \note  The following status can only be cleared by hardware behavior:
 **        - UartRxFull
 **        - UartTxEmpty
 **        - UartTxIdle
 ** 
 ******************************************************************************/
en_result_t Mfs_Uart_ClrStatus(volatile stc_mfsn_uart_t* pstcUart,
                               en_uart_status_t enStatus)
{
    if (NULL == pstcUart)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enStatus)
    {
        case UartParityError: 
        case UartFrameError:
        case UartOverrunError:
            pstcUart->SSR_f.REC = 1u;
            break;  
        case UartRxFull:
        case UartTxEmpty:
        case UartTxIdle:
            break;  
        case UartTxFifoRequest:
            pstcUart->FCR_f.FDRQ = 0u;
            break;  
        default:
            return ErrorInvalidParameter;  
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Write UART data buffer
 **
 ** \param [in] pstcUart   Pointer to UART instance   
 ** \param [in] u16Data    Send data
 ** 
 ** \retval Ok                    Data has been successfully sent
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Uart_SendData(volatile stc_mfsn_uart_t* pstcUart, uint16_t u16Data)
{
    if (NULL == pstcUart)
    {
        return ErrorInvalidParameter;
    }
    
    pstcUart->TDR = u16Data;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read UART data buffer
 **
 ** \param [in] pstcUart   Pointer to UART instance   
 ** 
 ** \retval Receive data        
 ** 
 ******************************************************************************/
uint16_t Mfs_Uart_ReceiveData(volatile stc_mfsn_uart_t* pstcUart)
{    
    return (pstcUart->RDR);
}

/**
 ******************************************************************************
 ** \brief Reset UART FIFO
 **
 ** \param [in] pstcUart   Pointer to UART instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** 
 ** \retval Ok                    FIFO has been successfully reset
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Uart_ResetFifo (volatile stc_mfsn_uart_t* pstcUart, 
                                en_mfs_fifo_t enFifo)
{
    if (NULL == pstcUart)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFifo)
    {
        case MfsFifo1:
            pstcUart->FCR_f.FCL1 = 1u;
            break;
        case MfsFifo2:
            pstcUart->FCR_f.FCL2 = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set UART FIFO count
 **
 ** \param [in] pstcUart   Pointer to UART instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** \param [in] u8Count    FIFO count
 ** 
 ** \retval Ok                    FIFO count has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Uart_SetFifoCount(volatile stc_mfsn_uart_t* pstcUart,
                                  en_mfs_fifo_t enFifo,
                                  uint8_t u8Count)
{
    if (NULL == pstcUart)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFifo)
    {
        case MfsFifo1:
            pstcUart->FBYTE1 = u8Count;
            break;
        case MfsFifo2:
            pstcUart->FBYTE2 = u8Count;
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get UART FIFO count
 **
 ** \param [in] pstcUart   Pointer to UART instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** 
 ** \retval FIFO count      
 ** 
 ** This function gets the current data count in selected FIFO.
 **
 ** \note 0xFF will be return value if FIFO index is error.
 **
 ******************************************************************************/
uint8_t Mfs_Uart_GetFifoCount(volatile stc_mfsn_uart_t* pstcUart,
                              en_mfs_fifo_t enFifo)
{
    uint8_t u8FifoCnt = 0xFFu;
    
    switch(enFifo)
    {
        case MfsFifo1:
            u8FifoCnt = pstcUart->FBYTE1;
            break;
        case MfsFifo2:
            u8FifoCnt = pstcUart->FBYTE2;
            break;
        default:
           break;
    }
    
    return u8FifoCnt;
}
#endif

#if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON)                             
/******************************************************************************
 * CSIO driver functions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain CSIO instance.
 **
 ** \param pstcCsio Pointer to CSIO instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_mfs_intern_data_t* MfsCsioGetInternDataPtr(volatile stc_mfsn_csio_t* pstcCsio)
{
    stc_mfs_intern_data_t* pstcInternDataPtr = NULL;
    uint32_t u32Instance;

    if (NULL != pstcCsio)
    {
        for (u32Instance = 0u; u32Instance < (uint32_t)MfsInstanceIndexMax; u32Instance++)
        {
            if (pstcCsio == m_astcMfsInstanceDataLut[u32Instance].stcInstance.pstcCsioInstance)
            {
                pstcInternDataPtr = &m_astcMfsInstanceDataLut[u32Instance].stcInternData;
                break;
            }
        }
            
    }

    return (pstcInternDataPtr);
} /* MfsGetInternDataPtr */

#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON) 
/**
 ******************************************************************************
 ** \brief CSIO receive interrupt service routine.
 ** 
 ** \param pstcCsio Pointer to CSIO instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure       
 ** 
 ** This function is called on Receive Interrupt set by the MFS. 
 **
 ******************************************************************************/
void MfsCsioIrqHandlerRx( volatile stc_mfsn_csio_t*   pstcCsio,
                          stc_mfs_intern_data_t* pstcMfsInternData)
{
    if ((NULL != pstcCsio) && (NULL != pstcMfsInternData))
    {
        if (NULL != pstcMfsInternData->stcCsioInternIrqCb.pfnRxIrqCb)
        {
            pstcMfsInternData->stcCsioInternIrqCb.pfnRxIrqCb();
        }
    }
} /* MfsCsioIrqHandlerRx */

/**
 ******************************************************************************
 ** \brief CSIO transfer interrupt service routine.
 ** 
 ** \param pstcCsio Pointer to CSIO instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure       
 ** 
 ** This function is called on transfer Interrupt set by the MFS. 
 **
 ******************************************************************************/
void MfsCsioIrqHandlerTx( volatile stc_mfsn_csio_t*   pstcCsio, 
                          stc_mfs_intern_data_t* pstcMfsInternData)
{
    if ((NULL != pstcCsio) && (NULL != pstcMfsInternData))
    {
        if(1u == pstcCsio->SSR_f.TDRE)
        {
            if (NULL != pstcMfsInternData->stcCsioInternIrqCb.pfnTxIrqCb)
            {
                pstcMfsInternData->stcCsioInternIrqCb.pfnTxIrqCb();
            }
        }
        
        if(1u == pstcCsio->SSR_f.TBI)
        {
            if (NULL != pstcMfsInternData->stcCsioInternIrqCb.pfnTxIdleCb)
            {
                pstcMfsInternData->stcCsioInternIrqCb.pfnTxIdleCb();
            }
        }
        
        if(1u == pstcCsio->FCR_f.FDRQ)
        {
            if (NULL != pstcMfsInternData->stcCsioInternIrqCb.pfnTxFifoIrqCb)
            {
                pstcMfsInternData->stcCsioInternIrqCb.pfnTxFifoIrqCb();
            }
            
            pstcCsio->FCR_f.FDRQ = 0u;
        }
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
        if(1u == pstcCsio->SACSR_f.CSE)
        {
            if (NULL != pstcMfsInternData->stcCsioInternIrqCb.pfnCsErrIrqCb)
            {
                pstcMfsInternData->stcCsioInternIrqCb.pfnCsErrIrqCb();
            }
            
            pstcCsio->SACSR_f.CSE = 0u;
        }
    #endif    
        
    }
} /* MfsIrqHandlerTx */
          
/**
 ******************************************************************************
 ** \brief CSIO status interrupt service routine.
 ** 
 ** \param pstcCsio Pointer to CSIO instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure       
 ** 
 ** This function is called on transfer Interrupt set by the MFS. 
 **
 ******************************************************************************/
void MfsCsioIrqHandlerStatus( volatile stc_mfsn_csio_t*   pstcCsio, 
                              stc_mfs_intern_data_t* pstcMfsInternData)
{
    if ((NULL != pstcCsio) && (NULL != pstcMfsInternData))
    {
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
        if(1u == pstcCsio->SACSR_f.TINT)
        {
            if (NULL != pstcMfsInternData->stcCsioInternIrqCb.pfnSerialTimerIrqCb)
            {
                pstcMfsInternData->stcCsioInternIrqCb.pfnSerialTimerIrqCb();
            }
            
            pstcCsio->SACSR_f.TINT = 0u;
        }
    #endif    
    }
}

/**
 ******************************************************************************
 ** \brief Enable CSIO interrupts
 **
 ** \param [in] pstcCsio   Pointer to CSIO instance   
 ** \param [in] enIrqSel   CSIO interrupt selection
 ** \arg CsioTxIrq         TX interrupt of CSIO
 ** \arg CsioRxIrq         RX interrupt of CSIO
 ** \arg CsioTxIdleIrq     TX idle interrupt of CSIO
 ** \arg CsioTxFifoIrq     TX FIFO interrupt of CSIO
 ** \arg CsioCsErrIrq      Chip selection error interrupt of CSIO
 ** \arg CsioSerialTimerIrq  Seriel timer interrupt of CSIO
 ** 
 ** \retval Ok                    Interrupts has been enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL
 **                               - enIrqSel == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_EnableIrq(volatile stc_mfsn_csio_t* pstcCsio, 
                               en_csio_irq_sel_t enIrqSel)
{
    stc_mfs_intern_data_t* pstcMfsInternData;

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsInternData = MfsCsioGetInternDataPtr(pstcCsio);
  
    if (NULL == pstcMfsInternData)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case CsioTxIrq:
            pstcCsio->SCR_f.TIE = 1u;
            break;
        case CsioRxIrq:
            pstcCsio->SCR_f.RIE = 1u;
            break;
        case CsioTxIdleIrq:
            pstcCsio->SCR_f.TBIE = 1u;
            break;
        case CsioTxFifoIrq:
            pstcCsio->FCR_f.FTIE = 1u;
            break;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
        case CsioCsErrIrq:
            pstcCsio->SACSR_f.CSEIE = 1u;
            break;
        case CsioSerialTimerIrq:
             pstcCsio->SACSR_f.TINTE = 1u;
            break;
    #endif    
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable CSIO interrupts
 **
 ** \param [in] pstcCsio   Pointer to CSIO instance   
 ** \param [in] enIrqSel   CSIO interrupt type
 ** \arg CsioTxIrq         TX interrupt of CSIO
 ** \arg CsioRxIrq         RX interrupt of CSIO
 ** \arg CsioTxIdleIrq     TX idle interrupt of CSIO
 ** \arg CsioTxFifoIrq     TX FIFO interrupt of CSIO
 ** \arg CsioCsErrIrq      Chip selection error interrupt of CSIO
 ** \arg CsioSerialTimerIrq  Seriel timer interrupt of CSIO
 ** 
 ** \retval Ok                    Interrupts has been disabled and callback 
 **                               fucntion pointers are set to NULL 
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL
 **                               - pstcInt == NULL                
 **
 ** The NVIC IRQ is also disabled in this function.
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_DisableIrq(volatile stc_mfsn_csio_t* pstcCsio, 
                                en_csio_irq_sel_t enIrqSel)
{  
    if (NULL == pstcCsio)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case CsioTxIrq:
            pstcCsio->SCR_f.TIE = 0u;
            break;
        case CsioRxIrq:
            pstcCsio->SCR_f.RIE = 0u;
            break;
        case CsioTxIdleIrq:
            pstcCsio->SCR_f.TBIE = 0u;
            break;
        case CsioTxFifoIrq:
            pstcCsio->FCR_f.FTIE = 0u;
            break;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)         
        case CsioCsErrIrq:
            pstcCsio->SACSR_f.CSEIE = 0u;
            break;
        case CsioSerialTimerIrq:
             pstcCsio->SACSR_f.TINTE = 0u;
            break;
    #endif         
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

#endif   

/**
 ******************************************************************************
 ** \brief Initialization of a MFS module to activate as CSIO.
 **
 ** Set registers to active MFS as CSIO.
 **  
 ** \param [in]   pstcCsio        Pointer to CSIO instance register area
 ** \param [in]   pstcConfig      MFS CSIO configuration
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcCsio == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_Csio_Init(volatile stc_mfsn_csio_t*         pstcCsio,
                          const stc_mfs_csio_config_t* pstcConfig)
{
    stc_mfs_intern_data_t* pstcMfsInternData;
    uint8_t u8Ch;
    
    /* Preset local register variables to zero */
    stc_mfs_csio_smr_field_t  stcSMR;
    stc_mfs_csio_scr_field_t  stcSCR;
    stc_mfs_csio_ssr_field_t  stcSSR;
    stc_mfs_csio_escr_field_t stcESCR;
    
    PDL_ZERO_STRUCT(stcSMR);
    PDL_ZERO_STRUCT(stcSCR);
    PDL_ZERO_STRUCT(stcSSR);
    PDL_ZERO_STRUCT(stcESCR);

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsInternData = MfsCsioGetInternDataPtr(pstcCsio);
    
    /* Parameter check and get ptr to internal data struct */
    if ((NULL == pstcMfsInternData)
        ||  (NULL == pstcConfig))
    {
        return (ErrorInvalidParameter);
    }

    /* Calculate the channel number */
    u8Ch = CsioInstanceToIndex(pstcCsio);
    
    if(u8Ch > 15u)
    {
        return (ErrorInvalidParameter);
    }
    
    /* Set MFS mode in the internal structure */
    pstcMfsInternData->enMode = MfsCsioMode;
    
    /* Set CSIO mode */
    stcSMR.MD = 2u;

    /* Set master/slave mode */
    switch(pstcConfig->enMsMode)
    {
        case CsioMaster:
            stcSCR.MS = FALSE;  /* Master mode */
            /* Enable SOE */
            stcSMR.SOE = TRUE;  
            /* Enable SCKE */
            stcSMR.SCKE = TRUE;
            break;
        case CsioSlave:
            stcSCR.MS = TRUE;   /* Slave mode */
            /* Enable SOE */
            stcSMR.SOE = TRUE;  
            /* Enable SCKE */
            stcSMR.SCKE = FALSE; /* Disable clock output in slave mode */
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    

    /* Set normal/SPI mode */
    switch (pstcConfig->enActMode)
    {
        case CsioActNormalMode:
            stcSCR.SPI = FALSE; /* Normal mode */
            break;
        case CsioActSpiMode:
            stcSCR.SPI = TRUE;  /* SPI mode */
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Wait time insertion */
    switch(pstcConfig->enSyncWaitTime)
    {
        case CsioSyncWaitZero:
            stcESCR.WT = 0u;    /* 0bit */
            break;
        case CsioSyncWaitOne:
            stcESCR.WT = 1u;    /* 1bit */
            break;
        case CsioSyncWaitTwo:
            stcESCR.WT = 2u;   /* 2bits */
            break;
        case CsioSyncWaitThree:
            stcESCR.WT = 3u;   /* 3bits */
            break;
        default:
            return (ErrorInvalidParameter);
    }
#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)  
    stcESCR.L3 = 0u;  /* Default:less than 13 bits */
#endif    

    /* Set Data bit length (5-16,20,24,32 bits) */
    switch (pstcConfig->enDataLength)
    {
        case CsioFiveBits:
            stcESCR.L = 1u;
            break;
        case CsioSixBits:
            stcESCR.L = 2u;
            break;
        case CsioSevenBits:
            stcESCR.L = 3u;
            break;   
        case CsioEightBits:
            stcESCR.L = 0u;
            break;
        case CsioNineBits:
            stcESCR.L = 4u;
            break;
    #if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)           
        case CsioTenBits:
            stcESCR.L = 5u;
            break;
        case CsioElevenBits:
            stcESCR.L = 6u;
            break;
        case CsioTwelveBits:
            stcESCR.L = 7u;
            break;
        case CsioThirteenBits:
            stcESCR.L = 0u;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            break;
        case CsioFourteenBits:
            stcESCR.L = 1u;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            break;
        case CsioFifteenBits:
            stcESCR.L = 2u;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            break;
        case CsioSixteenBits:
            stcESCR.L = 3u;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            break;
    #endif        
        default:
            return (ErrorInvalidParameter);
    }
    /* Set Bit direction (LSB/MSB) */    
    switch(pstcConfig->enBitDirection)
    {
        case CsioDataLsbFirst:
            stcSMR.BDS = FALSE;
            break;
        case CsioDataMsbFirst:
            stcSMR.BDS = TRUE;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* Set Signal system (SCK Mark Level) */
    if (TRUE == pstcConfig->bInvertClk)
    {
        stcSMR.SCINV = TRUE;    /* SCK Make Level : Low */
    }
    else
    {
        stcSMR.SCINV = FALSE;   /* SCK Make Level : High */
    }

    stcSSR.REC = TRUE;          /* Clear received error flag */

    /* Set Baudrate */
    (void)Mfs_Csio_SetBaudRate(pstcCsio, pstcConfig->u32BaudRate);
    /* Set registers value */
    pstcCsio->SMR_f  = stcSMR;
    pstcCsio->SCR_f  = stcSCR;
    pstcCsio->ESCR_f = stcESCR;
    pstcCsio->SSR_f  = stcSSR;
    
#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)  
    if(NULL != pstcConfig->pstcSerialTimer)  /* Use serial timer? */
    {
        /* Set serial timer clock */
        switch(pstcConfig->pstcSerialTimer->enClkDiv)
        {
            case CsioTimerNoDiv:
                pstcCsio->SACSR_f.TDIV = 0u;
                break;
            case CsioTimerDiv2:
                pstcCsio->SACSR_f.TDIV = 1u;
                break;
            case CsioTimerDiv4:
                pstcCsio->SACSR_f.TDIV = 2u;
                break;
            case CsioTimerDiv8: 
                pstcCsio->SACSR_f.TDIV = 3u;
                break;
            case CsioTimerDiv16:
                pstcCsio->SACSR_f.TDIV = 4u;
                break;
            case CsioTimerDiv32: 
                pstcCsio->SACSR_f.TDIV = 5u;
                break;
            case CsioTimerDiv64:
                pstcCsio->SACSR_f.TDIV = 6u;
                break;
            case CsioTimerDiv128: 
                pstcCsio->SACSR_f.TDIV = 7u;
                break;
            case CsioTimerDiv256: 
                pstcCsio->SACSR_f.TDIV = 8u;
                break;    
            default:
                return ErrorInvalidParameter;
        }
        
        /* Transfer count */
        pstcCsio->TBYTE0 = pstcConfig->pstcSerialTimer->u8TransferByteCnt;
        
        /* Timer compare value */
        pstcCsio->STMCR = pstcConfig->pstcSerialTimer->u16CompareVal;
        
        /* Enable sync transfer */
        pstcCsio->SACSR_f.TSYNE = 1u;
    }
    
    if(NULL != pstcConfig->pstcCsConfig)  /* Use chip selection function? */
    {
        if(CsioMaster == pstcConfig->enMsMode)
        {
            
          #if (defined(FM4_MFS_TYPE_A))
          #else
            /* Select CS start pin */
            switch(pstcConfig->pstcCsConfig->enCsStartPin)
            {
                case CsPinScs0:
                    pstcCsio->SCSCR_f.SST = 0u;
                    break;
                case CsPinScs1:
                    pstcCsio->SCSCR_f.SST = 1u;
                    break;
                case CsPinScs2:
                    pstcCsio->SCSCR_f.SST = 2u;
                    break;
                case CsPinScs3:
                    pstcCsio->SCSCR_f.SST = 3u;
                    break;
                default:
                    return ErrorInvalidParameter;
            }
            
            /* Select CS end pin */
            switch(pstcConfig->pstcCsConfig->enCsEndPin)
            {
                case CsPinScs0:
                    pstcCsio->SCSCR_f.SED = 0u;
                    break;
                case CsPinScs1:
                    pstcCsio->SCSCR_f.SED = 1u;
                    break;
                case CsPinScs2:
                    pstcCsio->SCSCR_f.SED = 2u;
                    break;
                case CsPinScs3:
                    pstcCsio->SCSCR_f.SED = 3u;
                    break;
                default:
                    return ErrorInvalidParameter;
            }
         
            /* Set transfer count of each SCS pin */
            pstcCsio->TBYTE0 = pstcConfig->pstcCsConfig->u8Scs0TransferByteCnt;
            pstcCsio->TBYTE1 = pstcConfig->pstcCsConfig->u8Scs1TransferByteCnt;
            pstcCsio->TBYTE2 = pstcConfig->pstcCsConfig->u8Scs2TransferByteCnt;
            pstcCsio->TBYTE3 = pstcConfig->pstcCsConfig->u8Scs3TransferByteCnt;
          #endif 
            /* Select CS active level (only for SCS0) */
            switch(pstcConfig->pstcCsConfig->enLevel)
            {
                case CsHighActive:
                    pstcCsio->SCSCR_f.CSLVL = 0u;
                    break;
                case CsLowActive:
                    pstcCsio->SCSCR_f.CSLVL = 1u;
                    break;
                default:
                    return ErrorInvalidParameter;
            }
            
            /* Active hold or not */
            pstcCsio->SCSCR_f.SCAM = pstcConfig->pstcCsConfig->bActiveHold;
            
            /* Set chip selection clock */
            switch(pstcConfig->pstcCsConfig->enClkDiv)
            {
                case CsClkNoDiv:
                    pstcCsio->SCSCR_f.CDIV = 0u;
                    break;
                case CsClkDiv2:
                    pstcCsio->SCSCR_f.CDIV = 1u;
                    break;
                case CsClkDiv4:
                    pstcCsio->SCSCR_f.CDIV = 2u;
                    break;
                case CsClkDiv8:
                    pstcCsio->SCSCR_f.CDIV = 3u;
                    break;
                case CsClkDiv16:
                    pstcCsio->SCSCR_f.CDIV = 4u;
                    break;
                case CsClkDiv32:
                    pstcCsio->SCSCR_f.CDIV = 5u;
                    break;
                case CsClkDiv64:
                    pstcCsio->SCSCR_f.CDIV = 6u;
                    break;
                default:
                    return ErrorInvalidParameter;
            }
            /* Set chip selection timing */
            pstcCsio->SCSTR1 = pstcConfig->pstcCsConfig->u8CsSetupDelayTime;
            pstcCsio->SCSTR0 = pstcConfig->pstcCsConfig->u8CsHoldDelayTime;
            pstcCsio->SCSTR32 = pstcConfig->pstcCsConfig->u16CsDeselectTime;
        }
        
        /* Enable SCS pin */
        pstcCsio->SCSCR_f.CSEN0 = ((pstcConfig->pstcCsConfig->bScs0En == TRUE) ? 1u : 0u);
      #if (defined(FM4_MFS_TYPE_A))
      #else
        pstcCsio->SCSCR_f.CSEN1 = ((pstcConfig->pstcCsConfig->bScs1En == TRUE) ? 1u : 0u);
        pstcCsio->SCSCR_f.CSEN2 = ((pstcConfig->pstcCsConfig->bScs2En == TRUE) ? 1u : 0u);
        pstcCsio->SCSCR_f.CSEN3 = ((pstcConfig->pstcCsConfig->bScs3En == TRUE) ? 1u : 0u);
      #endif
        
        if(CsioMaster == pstcConfig->enMsMode)
        {
            /* Enable CS pins output */
            pstcCsio->SCSCR_f.CSOE = 1u;
        }
        else
        {
            /* Disable CS pins output */
            pstcCsio->SCSCR_f.CSOE = 0u;
        }
    }
#endif
    
    if(NULL != pstcConfig->pstcFifoConfig) /* Use FIFO function? */
    {
        /* Reset FIFO */
        pstcCsio->FCR_f.FCL1 = 0u;
        pstcCsio->FCR_f.FCL2 = 0u;
        /* Enable FIFO receive Idle detection */
        pstcCsio->FCR_f.FRIIE = 1;
        /* Selection TX and RX FIFO  */
        switch(pstcConfig->pstcFifoConfig->enFifoSel)
        {
            case MfsTxFifo1RxFifo2:
                pstcCsio->FCR_f.FSEL = 0u;
                break;
            case MfsTxFifo2RxFifo1:
                pstcCsio->FCR_f.FSEL = 1u;
                break;
            default:
               return (ErrorInvalidParameter);
        }
        /* Set FIFO count */
        pstcCsio->FBYTE1 = pstcConfig->pstcFifoConfig->u8ByteCount1;
        pstcCsio->FBYTE2 = pstcConfig->pstcFifoConfig->u8ByteCount2;
        /* Enable FIFO  */
        pstcCsio->FCR_f.FE1 = 1u;
        pstcCsio->FCR_f.FE2 = 1u;
    }

#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)         
    /* Configure interrupts */
    if(NULL != pstcConfig->pstcIrqEn)
    { 
        if(TRUE == pstcConfig->pstcIrqEn->bRxIrq)
        {
            pstcCsio->SCR_f.RIE = 1u;   
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxIrq)
        {
            pstcCsio->SCR_f.TIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxIdleIrq)
        {
            pstcCsio->SCR_f.TBIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxFifoIrq)
        {
            pstcCsio->FCR_f.FTIE = 1u;
        }
    #if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)      
        if(TRUE == pstcConfig->pstcIrqEn->bCsErrIrq) 
        {
            pstcCsio->SACSR_f.CSEIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bSerialTimerIrq)
        {
            pstcCsio->SACSR_f.TINTE = 1u;
        }
    #endif    
    }
    
    /* Configure interrupt callback functions */
    if(NULL != pstcConfig->pstcIrqCb)
    {
        pstcMfsInternData->stcCsioInternIrqCb.pfnRxIrqCb = pstcConfig->pstcIrqCb->pfnRxIrqCb;
        pstcMfsInternData->stcCsioInternIrqCb.pfnTxIrqCb = pstcConfig->pstcIrqCb->pfnTxIrqCb;
        pstcMfsInternData->stcCsioInternIrqCb.pfnTxIdleCb = pstcConfig->pstcIrqCb->pfnTxIdleCb;
        pstcMfsInternData->stcCsioInternIrqCb.pfnTxFifoIrqCb = pstcConfig->pstcIrqCb->pfnTxFifoIrqCb;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
        pstcMfsInternData->stcCsioInternIrqCb.pfnCsErrIrqCb = pstcConfig->pstcIrqCb->pfnCsErrIrqCb;
        pstcMfsInternData->stcCsioInternIrqCb.pfnSerialTimerIrqCb = pstcConfig->pstcIrqCb->pfnSerialTimerIrqCb;
    #endif    
    }
    
    /* Configure NVIC */    
    if(TRUE == pstcConfig->bTouchNvic)
    {
        MfsInitNvic(u8Ch, FALSE);
        MfsInitNvic(u8Ch, TRUE);
    }
    
#endif    
    return (Ok);
} /* Mfs_Csio_Init */

/**
 ******************************************************************************
 ** \brief Deinitialisation of a MFS module activating as CSIO.
 **
 ** All used CSIO register are reset to their default values.
 **  
 ** \param [in]   pstcCsio        Pointer to CSIO instance register area
 ** \param [in]   bTouchNvic      Touch NVIC or not
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcCsio == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_Csio_DeInit(volatile stc_mfsn_csio_t* pstcCsio, boolean_t bTouchNvic)
{
    en_result_t            enResult;
    stc_mfs_intern_data_t* pstcMfsInternData;
    uint8_t u8Ch;
    
    /* Calculate the channel number */
    u8Ch = CsioInstanceToIndex(pstcCsio);
    
    if(u8Ch > 15u)
    {
        return (ErrorInvalidParameter);
    }

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsInternData = MfsCsioGetInternDataPtr(pstcCsio);
    /* ... and check */
    if (NULL == pstcMfsInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        /* First of all disable receiver, transmitter and deinit interrupts */
        pstcCsio->SCR_f.TXE = FALSE;
        pstcCsio->SCR_f.RXE = FALSE;

        /* Baud Rate Generation Reload Reset */
        pstcCsio->BGR = 0u;
            
        /* Serial Mode Register clear all bits (valid in any mode) */
        pstcCsio->SMR = 0u;
           
        /* Reset Mfs receive and transmit bit to default */
        /* and clear all error flags */
        pstcCsio->SCR = 0u;

        /* Clear reception Errors */
        pstcCsio->SSR_f.REC = TRUE;
           
        /* Reset all other used register to default value */
        pstcCsio->SSR  = 0u;
        pstcCsio->ESCR = 0u;
        pstcCsio->FCR = 0u;

    #if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)    
        pstcCsio->SACSR = 0u;
        pstcCsio->STMCR = 0u;
        pstcCsio->SCSCR = 0u;
        pstcCsio->SCSTR0 = 0;
        pstcCsio->SCSTR1 = 0;
        pstcCsio->SCSTR32 = 0;
        pstcCsio->TBYTE0 = 0u;
      #if (defined(FM4_MFS_TYPE_A))
      #else
        pstcCsio->TBYTE1 = 0u;
        pstcCsio->TBYTE2 = 0u;
        pstcCsio->TBYTE3 = 0u;
      #endif
    #endif    
  
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)      
        /* Disable NVIC */
        if(TRUE == bTouchNvic)
        {
            MfsDeInitNvic(u8Ch, FALSE);
            MfsDeInitNvic(u8Ch, TRUE);
        }
#endif        
        enResult = Ok;
    }

    return (enResult);
} /* Mfs_Csio_DeInit */

/**
 ******************************************************************************
 ** \brief Set the baudrate of CSIO
 **
 ** \param [in] pstcCsio    Pointer to CSIO instance   
 ** \param [in] u32BaudRate Baudrate value [bps]
 ** 
 ** \retval Ok                    CSIO baud rate has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL
 **                               - u32BaudRate < 3         
 ** \note
 ** The CSIO baud rate can be initialized in the Mfs_Csio_Init() and be modified
 ** in the funciton.
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_SetBaudRate(volatile stc_mfsn_csio_t* pstcCsio,
                                 uint32_t u32BaudRate)
{
    uint32_t u32Pclk1;

    SystemCoreClockUpdate();
    if ((NULL == pstcCsio) || (u32BaudRate < 3u))
    {
        return ErrorInvalidParameter;
    }
#if (PDL_MCU_CORE == PDL_FM0P_CORE)        
    u32Pclk1 = SystemCoreClock /(1ul << (FM_CRG->APBC1_PSR & 0x03u)); /* MFS is attached on APB1 bus for FM0+ */
#else
    u32Pclk1 = SystemCoreClock /(1ul << (FM_CRG->APBC2_PSR & 0x03u)); /* MFS is attached on APB2 bus for FM3 and FM4 */
#endif    
    pstcCsio->BGR_f.BGR = (u32Pclk1/u32BaudRate) - 1u;
    
    return Ok;
}

#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE) 
/**
 ******************************************************************************
 ** \brief Set the compare value of CSIO serial timer
 **
 ** \param [in] pstcCsio        Pointer to CSIO instance   
 ** \param [in] u16CompareValue Compare value
 ** 
 ** \retval Ok                    Compare value has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL
 **
 ** \note
 ** The compare value of CSIO serial timer can be initialized in 
 ** the Mfs_Csio_Init() and be modified in this function.
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_SetTimerCompareValue(volatile stc_mfsn_csio_t* pstcCsio,
                                          uint16_t u16CompareValue)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCsio->STMCR = u16CompareValue;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set the transfer byte count of a selected chip selection pin
 **
 ** \param [in] pstcCsio        Pointer to CSIO instance   
 ** \param [in] enCsPin         CS pin index
 ** \param [in] u8ByteCnt       Transfer byte count of a CS pin 
 ** 
 ** \retval Ok                    Transfer byte count has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL
 **
 ** \note
 ** The transfer byte count of chip selection pin can be initialized in 
 ** the Mfs_Csio_Init() and be modified in this funciton.
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_SetCsTransferByteCount(volatile stc_mfsn_csio_t* pstcCsio,
                                            en_cs_pin_sel_t enCsPin,
                                            uint8_t u8ByteCnt)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enCsPin)
    {
        case CsPinScs0:
            pstcCsio->TBYTE0 = u8ByteCnt;
            break;
      #if (defined(FM4_MFS_TYPE_A))
      #else
        case CsPinScs1:
            pstcCsio->TBYTE1 = u8ByteCnt;
            break;
        case CsPinScs2:
            pstcCsio->TBYTE2 = u8ByteCnt;
            break;
        case CsPinScs3:
            pstcCsio->TBYTE3 = u8ByteCnt;
            break;
      #endif
        default:  
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set the hold status of chip selection pin
 **
 ** \param [in] pstcCsio        Pointer to CSIO instance   
 ** \param [in] bHold 
 ** \arg  FALSE    Make the CS pin inactive if specified count bytes are 
 **                transferred
 ** \arg  TRUE     Hold the CS pin status even if specified count bytes are 
 **                transferred
 ** 
 ** \retval Ok                    Hold status of chip selection pin is set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL
 **
 ******************************************************************************/                                  
en_result_t Mfs_Csio_SetCsHoldStatus(volatile stc_mfsn_csio_t* pstcCsio, 
                                     boolean_t bHold)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCsio->SCSCR_f.SCAM = (bHold == TRUE) ? 1u : 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set the transfer byte count of CSIO serial timer
 **
 ** \param [in] pstcCsio        Pointer to CSIO instance   
 ** \param [in] u8ByteCnt       Transfer byte count
 ** 
 ** \retval Ok                    Transfer byte count has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL
 **
 ** \note
 ** The transfer byte count of CSIO serial timer can be initialized in 
 ** the Mfs_Csio_Init() and be modified in this function.
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_SetTimerTransferByteCount(volatile stc_mfsn_csio_t* pstcCsio,
                                               uint8_t u8ByteCnt)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCsio->TBYTE0 = u8ByteCnt;
    
    return Ok;
}
#endif
                                  
/**
 ******************************************************************************
 ** \brief Enable CSIO functions
 **
 ** \param [in] pstcCsio   Pointer to CSIO instance   
 ** \param [in] enFunc     CSIO function types
 ** \arg   CsioTx            CSIO transfer function
 ** \arg   CsioRx            CSIO receive function
 ** \arg   CsioSerialTimer   CSIO serial timer function
 ** \arg   CsioCsErrOccur    CSIO chip selection error detection function 
 ** 
 ** \retval Ok                    Function has been enabled normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL                
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_EnableFunc(volatile stc_mfsn_csio_t* pstcCsio, en_csio_func_t enFunc)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFunc)
    {
        case CsioTx:
            pstcCsio->SCR_f.TXE = 1u;
            break;
        case CsioRx:
            pstcCsio->SCR_f.RXE = 1u;
            break;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)        
        case CsioSerialTimer:
            pstcCsio->SACSR_f.TMRE = 1u;
            break;
        case CsioCsErrOccur:
            pstcCsio->SACSR_f.TBEEN = 1u;
            break;
    #endif        
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable CSIO functions
 **
 ** \param [in] pstcCsio   Pointer to CSIO instance   
 ** \param [in] enFunc     CSIO function types
 ** \arg   CsioTx            CSIO transfer function
 ** \arg   CsioRx            CSIO receive function
 ** \arg   CsioSerialTimer   CSIO transfer function
 ** \arg   CsioCsErrOccur    CSIO receive function 
 ** 
 ** \retval Ok                    Function has been disabled normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL              
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_DisableFunc(volatile stc_mfsn_csio_t* pstcCsio, en_csio_func_t enFunc)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFunc)
    {
        case CsioTx:
            pstcCsio->SCR_f.TXE = 0u;
            break;
        case CsioRx:
            pstcCsio->SCR_f.RXE = 0u;
            break;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)        
        case CsioSerialTimer:
            pstcCsio->SACSR_f.TMRE = 0u;
            break;
        case CsioCsErrOccur:
            pstcCsio->SACSR_f.TBEEN = 0u;
            break;
    #endif        
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get status of CSIO according to status type
 **
 ** \param [in] pstcCsio     Pointer to CSIO instance   
 ** \param [in] enStatus     CSIO status type
 ** \arg   CsioOverrunError  CSIO parity error
 ** \arg   CsioRxFull        CSIO receive buffer full
 ** \arg   CsioTxEmpty       CSIO tranfer buffer empty
 ** \arg   CsioTxIdle        CSIO tranfer idle status
 ** \arg   CsioTxFifoRequest CSIO transfer FIFO request
 ** \arg   CsioCsErrIntFlag  CSIO chip selection error occurrance
 ** \arg   CsioTimerIntFlag  CSIO serial timer interrupt flag
 ** 
 ** \retval FALSE                 If one of following conditions are met:
 **                               - No CSIO overrun error [enStatus = CsioOverrunError]
 **                               - CSIO receive buffer is not full [enStatus = CsioRxFull]
 **                               - CSIO tranfer buffer is not empty [enStatus = CsioTxEmpty]
 **                               - CSIO tranfer is on-going [enStatus = CsioTxIdle]
 **                               - No CSIO transfer FIFO request [enStatus = CsioTxFifoRequest]
 **                               - CSIO chip selection error doesn't occur [enStatus = CsioCsErrIntFlag]
 **                               - CSIO serial timer interrupt doesn't occur [enStatus = CsioTimerIntFlag]
 ** \retval TRUE                 If one of following conditions are met:
 **                               - CSIO overrun error occurs [enStatus = CsioOverrunError]
 **                               - CSIO receive buffer is full [enStatus = CsioRxFull]
 **                               - CSIO tranfer buffer is empty [enStatus = CsioTxEmpty]
 **                               - CSIO tranfer is idle [enStatus = CsioTxIdle]
 **                               - CSIO transfer FIFO request issues [enStatus = CsioTxFifoRequest]
 **                               - CSIO chip selection error occurs  [enStatus = CsioCsErrIntFlag]
 **                               - CSIO serial timer interrupt occurs [enStatus = CsioTimerIntFlag]
 ** 
 ******************************************************************************/
boolean_t Mfs_Csio_GetStatus(volatile stc_mfsn_csio_t* pstcCsio, 
                               en_csio_status_t enStatus)
{
    boolean_t bRet = FALSE;
      
    switch(enStatus)
    {
        case CsioOverrunError:
            bRet = (1u == pstcCsio->SSR_f.ORE) ? TRUE : FALSE;
            break;
        case CsioRxFull:
            bRet = (1u == pstcCsio->SSR_f.RDRF) ? TRUE : FALSE;
            break;
        case CsioTxEmpty:
            bRet = (1u == pstcCsio->SSR_f.TDRE) ? TRUE : FALSE;
            break;
        case CsioTxIdle:
            bRet = (1u == pstcCsio->SSR_f.TBI) ? TRUE : FALSE;
            break;
        case CsioTxFifoRequest:
            bRet = (1u == pstcCsio->FCR_f.FDRQ) ? TRUE : FALSE;
            break;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)        
        case CsioCsErrIntFlag:
            bRet = (1u == pstcCsio->SACSR_f.CSE) ? TRUE : FALSE;
            break;
        case CsioTimerIntFlag:
            bRet = (1u == pstcCsio->SACSR_f.TINT) ? TRUE : FALSE;
            break;
    #endif        
        default:
            break;
    }
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Clear status of CSIO according to status type
 **
 ** \param [in] pstcCsio     Pointer to CSIO instance   
 ** \param [in] enStatus     UART status type
 ** \arg   CsioOverrunError  CSIO overrun error
 ** \arg   CsioRxFull        CSIO receive buffer full
 ** \arg   CsioTxEmpty       CSIO tranfer buffer empty
 ** \arg   CsioTxIdle        CSIO tranfer idle status
 ** \arg   CsioTxFifoRequest CSIO transfer FIFO request
 ** \arg   CsioCsErrIntFlag  CSIO chip selection error occurrance
 ** 
 ** \retval Ok                    Status has been cleared normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 ** \note  The following status can only be cleared by hardware behavior:
 **        - CsioRxFull
 **        - CsioTxEmpty
 **        - CsioTxIdle
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_ClrStatus(volatile stc_mfsn_csio_t* pstcCsio,
                               en_csio_status_t enStatus)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enStatus)
    {
        case CsioOverrunError:
            pstcCsio->SSR_f.REC = 1u;
            break;
        case CsioRxFull:
        case CsioTxEmpty:
        case CsioTxIdle:
            break;
        case CsioTxFifoRequest:
            pstcCsio->FCR_f.FDRQ = 0u;
            break;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)        
        case CsioCsErrIntFlag:
            pstcCsio->SACSR_f.CSE = 0u;
            break;
        case CsioTimerIntFlag:
            pstcCsio->SACSR_f.TINT = 0u;
            break;
    #endif        
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Write CSIO data buffer
 **
 ** \param [in] pstcCsio   Pointer to CSIO instance   
 ** \param [in] u16Data    Send data
 ** \param [in] bSotEn     SOT output enable 
 ** \arg   FALSE  Mask SOT output
 ** \arg   TRUE   Enable SOT output
 ** 
 ** \retval Ok                    Data has been successfully sent
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_SendData(volatile stc_mfsn_csio_t* pstcCsio, 
                              uint16_t u16Data,
                              boolean_t bSotEn)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    (TRUE == bSotEn) ? (pstcCsio->SMR_f.SOE = 1u) : (pstcCsio->SMR_f.SOE = 0u);
    
    pstcCsio->TDR = u16Data;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read CSIO data buffer
 **
 ** \param [in] pstcCsio   Pointer to CSIO instance   
 ** 
 ** \retval Receive data        
 ** 
 ******************************************************************************/
uint16_t Mfs_Csio_ReceiveData(volatile stc_mfsn_csio_t* pstcCsio)
{
    return pstcCsio->RDR;
}

/**
 ******************************************************************************
 ** \brief Reset CSIO FIFO
 **
 ** \param [in] pstcCsio   Pointer to CSIO instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** 
 ** \retval Ok                    FIFO has been successfully reset
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_ResetFifo (volatile stc_mfsn_csio_t* pstcCsio, 
                                en_mfs_fifo_t enFifo)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFifo)
    {
        case MfsFifo1:
            pstcCsio->FCR_f.FCL1 = 1u;
            break;
        case MfsFifo2:
            pstcCsio->FCR_f.FCL2 = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set CSIO FIFO count
 **
 ** \param [in] pstcCsio   Pointer to CSIO instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** \param [in] u8Count    FIFO count
 ** 
 ** \retval Ok                    FIFO count has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcCsio == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Csio_SetFifoCount(volatile stc_mfsn_csio_t* pstcCsio,
                                  en_mfs_fifo_t enFifo,
                                  uint8_t u8Count)
{
    if (NULL == pstcCsio)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFifo)
    {
        case MfsFifo1:
            pstcCsio->FBYTE1 = u8Count;
            break;
        case MfsFifo2:
            pstcCsio->FBYTE2 = u8Count;
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get CSIO FIFO count
 **
 ** \param [in] pstcCsio   Pointer to CSIO instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** 
 ** \retval FIFO count      
 ** 
 ** This function gets the current data count in selected FIFO.
 **
 ** \note 0xFF will be return value if FIFO index is error.
 **
 ******************************************************************************/
uint8_t Mfs_Csio_GetFifoCount(volatile stc_mfsn_csio_t* pstcCsio,
                              en_mfs_fifo_t enFifo)
{
    uint8_t u8FifoCnt = 0xFFu;
    
    switch(enFifo)
    {
        case MfsFifo1:
            u8FifoCnt = pstcCsio->FBYTE1;
            break;
        case MfsFifo2:
            u8FifoCnt = pstcCsio->FBYTE2;
            break;
        default:
           break;
    }
    
    return u8FifoCnt;
}
#endif
      
#if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON)
/******************************************************************************
 * I2C driver functions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain I2C instance.
 **
 ** \param pstcI2c Pointer to I2C instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_mfs_intern_data_t* MfsI2cGetInternDataPtr(volatile stc_mfsn_i2c_t* pstcI2c)
{
    stc_mfs_intern_data_t* pstcInternDataPtr = NULL;
    uint32_t u32Instance;

    if (NULL != pstcI2c)
    {
        for (u32Instance = 0u; u32Instance < (uint32_t)MfsInstanceIndexMax; u32Instance++)
        {
            if (pstcI2c == m_astcMfsInstanceDataLut[u32Instance].stcInstance.pstcI2cInstance)
            {
                pstcInternDataPtr = &m_astcMfsInstanceDataLut[u32Instance].stcInternData;
                break;
            }
        }
            
    }

    return (pstcInternDataPtr);
} /* MfsGetInternDataPtr */


#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)     
/**
 ******************************************************************************
 ** \brief I2C receive interrupt service routine.
 ** 
 ** \param pstcI2c Pointer to I2C instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure       
 ** 
 ** This function is called on Receive Interrupt set by the MFS. 
 **
 ******************************************************************************/
void MfsI2cIrqHandlerRx( volatile stc_mfsn_i2c_t*   pstcI2c,
                         stc_mfs_intern_data_t* pstcMfsInternData)
{
    if ((NULL != pstcI2c) && (NULL != pstcMfsInternData))
    {
        if (NULL != pstcMfsInternData->stcI2cInternIrqCb.pfnRxIrqCb)
        {
            pstcMfsInternData->stcI2cInternIrqCb.pfnRxIrqCb();
        }
    }
} /* MfsIrqHandlerRx */

/**
 ******************************************************************************
 ** \brief I2C transfer interrupt service routine.
 ** 
 ** \param pstcI2c           Pointer to I2C instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure       
 ** 
 ** This function is called on transfer Interrupt set by the MFS. 
 **
 ******************************************************************************/
void MfsI2cIrqHandlerTx( volatile stc_mfsn_i2c_t*   pstcI2c, 
                         stc_mfs_intern_data_t* pstcMfsInternData)
{
    if(1u == pstcI2c->SSR_f.TDRE)
    {
        if (NULL != pstcMfsInternData->stcI2cInternIrqCb.pfnTxIrqCb)
        {
            pstcMfsInternData->stcI2cInternIrqCb.pfnTxIrqCb();
        }
    }
    
    if(1u == pstcI2c->SSR_f.TBI)
    {
        if (NULL != pstcMfsInternData->stcI2cInternIrqCb.pfnTxIdleCb)
        {
            pstcMfsInternData->stcI2cInternIrqCb.pfnTxIdleCb();
        }
    }
    
    if(1u == pstcI2c->FCR_f.FDRQ)
    {
        if (NULL != pstcMfsInternData->stcI2cInternIrqCb.pfnTxFifoIrqCb)
        {
            pstcMfsInternData->stcI2cInternIrqCb.pfnTxFifoIrqCb();
        }
        
        pstcI2c->FCR_f.FDRQ = 0u;
    }
} /* MfsIrqHandlerTx */
          
/**
 ******************************************************************************
 ** \brief I2C status interrupt service routine.
 ** 
 ** \param pstcI2c           Pointer to I2C instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure       
 ** 
 ** This function is called on transfer Interrupt set by the MFS. 
 **
 ******************************************************************************/
void MfsI2cIrqHandlerStatus( volatile stc_mfsn_i2c_t*   pstcI2c, 
                              stc_mfs_intern_data_t* pstcMfsInternData)
{
    if ((NULL != pstcI2c) && (NULL != pstcMfsInternData))
    {
        if(1u == pstcI2c->IBCR_f.INT)
        {
            if (NULL != pstcMfsInternData->stcI2cInternIrqCb.pfnTxRxIrqCb)
            {
                pstcMfsInternData->stcI2cInternIrqCb.pfnTxRxIrqCb();
            }
            
            // pstcI2c->IBCR_f.INT = 0; /* It will be cleared in callback function */
        }
        
        if(1u == pstcI2c->IBSR_f.SPC)
        {
            if (NULL != pstcMfsInternData->stcI2cInternIrqCb.pfnStopDetectIrqCb)
            {
                pstcMfsInternData->stcI2cInternIrqCb.pfnStopDetectIrqCb();
            }
            
            pstcI2c->IBSR_f.SPC = 0u;
        }
    }
}

/**
 ******************************************************************************
 ** \brief Enable I2C interrupts
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** \param [in] enIrqSel   enumeration of I2C interrupt selection
 ** \arg I2cTxIrq  TX interrupt of I2C
 ** \arg I2cRxIrq  RX interrupt of I2C
 ** \arg I2cTxIdleIrq TX idle interrupt of I2C
 ** \arg I2cTxFifoIrq TX FIFO interrupt of I2C
 ** \arg I2cTxRxIrq   TX and RX interrupt of I2C
 ** \arg I2cStopDetectIrq Stop detection interrupt of I2C
 ** 
 ** \retval Ok                    Interrupts has been enabled 
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2c == NULL
 **                               - Other invalid configuration                   
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_EnableIrq(volatile stc_mfsn_i2c_t* pstcI2c, 
                              en_i2c_irq_sel_t enIrqSel)
{
    if (NULL == pstcI2c)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case I2cTxIrq:
            pstcI2c->SMR_f.TIE = 1u;
            break;
        case I2cRxIrq:
            pstcI2c->SMR_f.RIE = 1u;
            break;
        case I2cTxIdleIrq:
            pstcI2c->FCR_f.FTIE = 1u;
            break;
        case I2cTxFifoIrq:
            pstcI2c->SSR_f.TBIE = 1u;
            break;
        case I2cTxRxIrq:
            pstcI2c->IBCR_f.INTE = 1u;
            break;
        case I2cStopDetectIrq:
            pstcI2c->IBCR_f.CNDE = 1u;
            break;  
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable I2C interrupts
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** \param [in] enIrqSel   enumeration of I2C interrupt selection
 ** \arg I2cTxIrq  TX interrupt of I2C
 ** \arg I2cRxIrq  RX interrupt of I2C
 ** \arg I2cTxIdleIrq TX idle interrupt of I2C
 ** \arg I2cTxFifoIrq TX FIFO interrupt of I2C
 ** \arg I2cTxRxIrq   TX and RX interrupt of I2C
 ** \arg I2cStopDetectIrq Stop detection interrupt of I2C
 ** 
 ** \retval Ok                    Interrupts has been disabled 
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2c == NULL
 **                               - Other invalid configuration                   
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_DisableIrq(volatile stc_mfsn_i2c_t* pstcI2c, 
                              en_i2c_irq_sel_t enIrqSel)
{
    if (NULL == pstcI2c)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case I2cTxIrq:
            pstcI2c->SMR_f.TIE = 0u;
            break;
        case I2cRxIrq:
            pstcI2c->SMR_f.RIE = 0u;
            break;
        case I2cTxIdleIrq:
            pstcI2c->FCR_f.FTIE = 0u;
            break;
        case I2cTxFifoIrq:
            pstcI2c->SSR_f.TBIE = 0u;
            break;
        case I2cTxRxIrq:
            pstcI2c->IBCR_f.INTE = 0u;
            break;
        case I2cStopDetectIrq:
            pstcI2c->IBCR_f.CNDE = 0u;
            break;  
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

#endif   

/**
 ******************************************************************************
 ** \brief Initialization of a MFS module to activate as I2C.
 **
 ** Set registers to active MFS as I2C.
 **  
 ** \param [in]   pstcI2c         Pointer to I2C instance register area
 ** \param [in]   pstcConfig      MFS I2C configuration
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcI2c == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_I2c_Init(volatile stc_mfsn_i2c_t*        pstcI2c, 
                         const stc_mfs_i2c_config_t* pstcConfig)
{
    stc_mfs_intern_data_t* pstcMfsInternData;
    uint8_t u8Ch;
    
	/* Preset local register variables to zero */
    stc_mfs_i2c_smr_field_t   stcSMR;
    stc_mfs_i2c_ibcr_field_t  stcIBCR;
    stc_mfs_i2c_isba_field_t  stcISBA;
    stc_mfs_i2c_ismk_field_t  stcISMK;
    
    PDL_ZERO_STRUCT(stcSMR);
    PDL_ZERO_STRUCT(stcIBCR);
    PDL_ZERO_STRUCT(stcISBA);
    PDL_ZERO_STRUCT(stcISMK);
    
    /* Calculate the channel number */
    u8Ch = I2cInstanceToIndex(pstcI2c);
    
    if(u8Ch > 15u)
    {
        return (ErrorInvalidParameter);
    }

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsInternData = MfsI2cGetInternDataPtr(pstcI2c);
    
    /* Parameter check and get ptr to internal data struct */
    if ((NULL == pstcMfsInternData)
    ||  (NULL == pstcConfig)
       )
    {
        return (ErrorInvalidParameter);
    }
    
    /* Set the MFS mode in the internal structure */
    pstcMfsInternData->enMode = MfsI2cMode;

    /* Set I2C mode */
    switch (pstcConfig->enMsMode)
    {
        case I2cMaster:
        case I2cSlave:
            stcSMR.MD = 4u;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Ack enable */
    stcIBCR.ACKE = TRUE;

    /* Slave mode */
    if (I2cSlave == pstcConfig->enMsMode)
    {
        /* Set slave address */
        stcISBA.SA = pstcConfig->u8SlaveAddr;
        /* Enable slave address detection */
        stcISBA.SAEN = TRUE;
        /* Set slave address bit mask */
        stcISMK.SM = pstcConfig->u8SlaveMaskAddr;
    }
    
    /* Enable I2C*/
    stcISMK.EN = TRUE;

    /* now setup hardware with correct mode first and then go on with */
    /*   bit settings */
    pstcI2c->SMR_f = stcSMR;

    /* I2C disable before other registers are set. */
    pstcI2c->ISMK  = 0u;

    /* Use DMA or not */
    if(TRUE == pstcConfig->bDmaEnable)
    {
        pstcI2c->SSR_f.DMA = 1u;
    }
    else
    {
        pstcI2c->SSR_f.DMA = 0u;
    }

    /* Set baud rate generation */
    (void)Mfs_I2c_SetBaudRate(pstcI2c, pstcConfig->u32BaudRate);

    /* Set registers value */
    pstcI2c->IBCR_f  = stcIBCR;
    pstcI2c->ISBA_f  = stcISBA;
    pstcI2c->ISMK_f  = stcISMK;
    
    if(NULL != pstcConfig->pstcFifoConfig) /* Use FIFO function? */
    {
        /* Reset FIFO */
        pstcI2c->FCR_f.FCL1 = 1u;
        pstcI2c->FCR_f.FCL2 = 1u;
        /* Enable FIFO receive Idle detection */
        pstcI2c->FCR_f.FRIIE = 1;
        /* Selection TX and RX FIFO  */
        switch(pstcConfig->pstcFifoConfig->enFifoSel)
        {
            case MfsTxFifo1RxFifo2:
                pstcI2c->FCR_f.FSEL = 0u;
                break;
            case MfsTxFifo2RxFifo1:
                pstcI2c->FCR_f.FSEL = 1u;
                break;
            default:
               return (ErrorInvalidParameter);
        }
        /* Set FIFO count */
        pstcI2c->FBYTE1 = pstcConfig->pstcFifoConfig->u8ByteCount1;
        pstcI2c->FBYTE2 = pstcConfig->pstcFifoConfig->u8ByteCount2;
        /* Enable FIFO  */
        pstcI2c->FCR_f.FE1 = 1;
        pstcI2c->FCR_f.FE2 = 1;
    }

#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)      
    /* Configure interrupt */
    if(NULL != pstcConfig->pstcIrqEn)
    {
        if(TRUE == pstcConfig->pstcIrqEn->bTxIrq)
        {
            pstcI2c->SMR_f.TIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bRxIrq)
        {
            pstcI2c->SMR_f.RIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxFifoIrq)
        {
            pstcI2c->FCR_f.FTIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxIdleIrq)
        {
            pstcI2c->SSR_f.TBIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxRxIrq)
        {
            pstcI2c->IBCR_f.INTE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bStopDetectIrq)
        {
            pstcI2c->IBCR_f.CNDE = 1u;
        }
    }
    
    /* Configure interrupt callback */
    if(NULL != pstcConfig->pstcIrqCb)
    {
        pstcMfsInternData->stcI2cInternIrqCb.pfnTxIrqCb = pstcConfig->pstcIrqCb->pfnTxIrqCb;
        pstcMfsInternData->stcI2cInternIrqCb.pfnRxIrqCb = pstcConfig->pstcIrqCb->pfnRxIrqCb;
        pstcMfsInternData->stcI2cInternIrqCb.pfnTxFifoIrqCb = pstcConfig->pstcIrqCb->pfnTxFifoIrqCb;
        pstcMfsInternData->stcI2cInternIrqCb.pfnTxIdleCb = pstcConfig->pstcIrqCb->pfnTxIdleCb;
        pstcMfsInternData->stcI2cInternIrqCb.pfnTxRxIrqCb = pstcConfig->pstcIrqCb->pfnTxRxIrqCb;
        pstcMfsInternData->stcI2cInternIrqCb.pfnStopDetectIrqCb = pstcConfig->pstcIrqCb->pfnStopDetectIrqCb;
    }
    
    /* Configure NVIC */
    if(TRUE == pstcConfig->bTouchNvic)
    {
        MfsInitNvic(u8Ch, FALSE);
        MfsInitNvic(u8Ch, TRUE);
    }

#endif
    
    return (Ok);
} /* Mfs_I2c_Init */

/**
 ******************************************************************************
 ** \brief Deinitialisation of a MFS module activating as I2C.
 **
 ** All used I2C register are reset to their default values.
 **  
 ** \param [in]   pstcI2c         Pointer to I2C instance register area
 ** \param [in]   bTouchNvic      Touch NVIC or not
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcI2c == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_I2c_DeInit( volatile stc_mfsn_i2c_t* pstcI2c, boolean_t bTouchNvic)
{
    en_result_t            enResult;
    stc_mfs_intern_data_t* pstcMfsInternData;
    uint8_t u8Ch;
    
    /* Calculate the channel number */
    u8Ch = I2cInstanceToIndex(pstcI2c);
    
    if(u8Ch > 15u)
    {
        return (ErrorInvalidParameter);
    }
    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsInternData = MfsI2cGetInternDataPtr(pstcI2c);
    /* ... and check */
    if (NULL == pstcMfsInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        /* Disable I2C */
        pstcI2c->ISMK_f.EN = FALSE;

        /* Serial Mode Register clear all bits (valid in any mode) */
        pstcI2c->SMR = 0u;
           
        /* Reset Mfs receive and transmit bit to default */
        /* and clear all error flags */
        pstcI2c->IBCR = 0u;

        /* Clear reception Errors */
        pstcI2c->SSR_f.REC = TRUE;

        /* Reset all other used register to default value */
        pstcI2c->SSR  = 0u;
        pstcI2c->IBSR = 0u;
        pstcI2c->FCR = 0u;

        
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)          
        if(TRUE == bTouchNvic)
        {
            MfsDeInitNvic(u8Ch, FALSE);
            MfsDeInitNvic(u8Ch, TRUE);
        }
#endif
        enResult = Ok;
    }

    return (enResult);
} /* Mfs_I2c_DeInit */

/**
 ******************************************************************************
 ** \brief Set the baudrate of I2C
 **
 ** \param [in] pstcI2c     Pointer to I2C instance   
 ** \param [in] u32BaudRate Baudrate value [bps]
 ** 
 ** \retval Ok                    I2C baud rate has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2c == NULL
 **                               - u32BaudRate < 3         
 **
 ** The I2C baud rate can be initialized in the Mfs_I2c_Init() and be modified
 ** with this function.
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_SetBaudRate(volatile stc_mfsn_i2c_t* pstcI2c,
                                uint32_t u32BaudRate)
{
    uint32_t u32Pclk1;
    boolean_t bTempBit = FALSE;

    SystemCoreClockUpdate();
    if ((NULL == pstcI2c) || (u32BaudRate > 400000u))
    {
        return ErrorInvalidParameter;
    }
    
    bTempBit = pstcI2c->ISMK_f.EN;
    pstcI2c->ISMK_f.EN = 1u;
#if (PDL_MCU_CORE == PDL_FM0P_CORE)       
    u32Pclk1 = SystemCoreClock /(1ul << (FM_CRG->APBC1_PSR & 0x03u)); // MFS is attached on APB1 bus for FM0+
#else
    u32Pclk1 = SystemCoreClock /(1ul << (FM_CRG->APBC2_PSR & 0x03u)); // MFS is attached on APB2 bus for FM3 and FM4
#endif    
    pstcI2c->BGR_f.BGR = (u32Pclk1/u32BaudRate) - 1u;
    pstcI2c->ISMK_f.EN = bTempBit;
   
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Generate start signal of I2C
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** 
 ** \retval Ok                    Generate start signal successfully
 ** \retval ErrorInvalidParameter pstcI2c == NULL
 ** \retval ErrorTimeout          Timeout when generating a start signal
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_GenerateStart(volatile stc_mfsn_i2c_t*        pstcI2c)
{
    uint32_t u32TimeOut = SystemCoreClock;
      
    if (NULL == pstcI2c)
    {
        return ErrorInvalidParameter;
    }
    
    pstcI2c->IBCR_f.MSS = 1u;
    while(u32TimeOut--)
    {
        if((1u == pstcI2c->IBCR_f.MSS) && (1u == pstcI2c->IBCR_f.ACT_SCC))
        {
            return Ok;
        }
    }
    
    return  ErrorTimeout;
}

/**
 ******************************************************************************
 ** \brief Generate restart signal of I2C
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** 
 ** \retval Ok                    Generate restart signal successfully
 ** \retval ErrorInvalidParameter pstcI2c == NULL
 ** \retval ErrorTimeout          Timeout when generating a restart signal
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_GenerateRestart(volatile stc_mfsn_i2c_t*      pstcI2c)
{
    uint32_t u32TimeOut = SystemCoreClock;
    
    if (NULL == pstcI2c)
    {
        return ErrorInvalidParameter;
    }
    
    pstcI2c->IBCR_f.ACT_SCC = 1u;
    
    while(u32TimeOut--)
    {
        if(1u == pstcI2c->IBSR_f.RSC)
        {
            return Ok;
        }
    }
    
    return ErrorTimeout;
}

/**
 ******************************************************************************
 ** \brief Generate stop signal of I2C
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** 
 ** \retval Ok                    Generate stop signal successfully
 ** \retval ErrorInvalidParameter pstcI2c == NULL
 ** \retval ErrorTimeout          Timeout when generating a stop signal
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_GenerateStop(volatile stc_mfsn_i2c_t*      pstcI2c)
{
    uint32_t u32TimeOut = SystemCoreClock;
    
    if (NULL == pstcI2c)
    {
        return ErrorInvalidParameter;
    }
    
    while(u32TimeOut--)
    {
        pstcI2c->IBCR &= ~0xc1u;
        if((0u == pstcI2c->IBCR_f.MSS) && (0u == pstcI2c->IBCR_f.ACT_SCC))
        {
            return Ok;
        }
    }
    
    return ErrorTimeout;
}

/**
 ******************************************************************************
 ** \brief Write I2C data buffer
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** \param [in] u8Data     Data to be sent
 ** 
 ** \retval Ok                    Data has been successfully sent
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2c == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_SendData(volatile stc_mfsn_i2c_t*      pstcI2c, uint8_t u8Data)
{
    if (NULL == pstcI2c)
    {
        return ErrorInvalidParameter;
    }
    
    pstcI2c->TDR = u8Data;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read I2C data buffer
 **
 ** \param [in] pstcI2c   Pointer to I2C instance   
 ** 
 ** \retval Receive data        
 ** 
 ******************************************************************************/
uint8_t Mfs_I2c_ReceiveData(volatile stc_mfsn_i2c_t*      pstcI2c)
{
    return pstcI2c->RDR;
}

/**
 ******************************************************************************
 ** \brief Configure ACK signal sent to master
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** \param [in] enAck      ACK to be sent
 ** \arg  I2cAck ACK will be sent as response signal
 ** \arg  I2cNAck NACK will be sent as response signal
 ** 
 ** \retval Ok                    ACK signal has been successfully configured
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2c == NULL           
 ** 
 ** This function is only meaningful when I2C is in slave mode
 **
 ******************************************************************************/
en_result_t Mfs_I2c_ConfigAck(volatile stc_mfsn_i2c_t* pstcI2c, en_i2c_ack_t enAck)
{
    stc_mfs_i2c_ibcr_field_t stcIbcr =  pstcI2c->IBCR_f; 
  
    if (NULL == pstcI2c)
    {
        return ErrorInvalidParameter;
    }
    
    (I2cAck == enAck) ?  (stcIbcr.ACKE = 1u) :  (stcIbcr.ACKE = 0u);
    
    stcIbcr.ACT_SCC = 0u; // Set SCC = 0 to avoid generating a restart 
    
    pstcI2c->IBCR_f = stcIbcr;
    
    return Ok;
    
}

/**
 ******************************************************************************
 ** \brief Get the ACK signal from slave
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** 
 ** \retval I2cAck Receive the ACK  from I2C Slave
 ** \retval I2cNAck Receive the NACK from I2C Slave
 ** 
 ** This function applies in the I2C master mode.
 **
 ******************************************************************************/
en_i2c_ack_t   Mfs_I2c_GetAck(volatile stc_mfsn_i2c_t*        pstcI2c)
{
    en_i2c_ack_t enRet;
    
    enRet = (0u == pstcI2c->IBSR_f.RACK) ? I2cAck : I2cNAck;
    
    return enRet; 
}

/**
 ******************************************************************************
 ** \brief Get status of I2C according to status type
 **
 ** \param [in] pstcI2c      Pointer to I2C instance   
 ** \param [in] enStatus     I2C status type
 ** \arg   I2cOverrunError          I2C overrun error
 ** \arg   I2cRxFull                I2C receive buffer full
 ** \arg   I2cTxEmpty               I2C tranfer buffer empty
 ** \arg   I2cTxFifoRequest         I2C transfer FIFO request
 ** \arg   I2cTxIdle                I2C tranfer idle status
 ** \arg   I2cFirstByteDetect       I2C first byte detection
 ** \arg   I2cReservedByteDetect    I2C reserved address detection
 ** \arg   I2cStopDetect            I2C stop condition detection
 ** \arg   I2cBusStatus             I2C bus status 
 ** \arg   I2cBusErr                I2C bus error
 ** \arg   I2cRxTxIrq               I2C transfer and receive interrupt flag
 ** 
 ** \retval FALSE                 If one of following conditions are met:
 **                               - No I2C overrun error [enStatus = I2cOverrunError] 
 **                               - I2C receive buffer is not full [enStatus = I2cRxFull] 
 **                               - I2C tranfer buffer is not empty [enStatus = I2cTxEmpty] 
 **                               - I2C tranfer FIFO request doesn't issue [enStatus = I2cTxFifoRequest] 
 **                               - I2C tranfer is on-going [enStatus = I2cTxIdle] 
 **                               - I2C first byte is not detected [enStatus = I2cFirstByteDetect] 
 **                               - I2C reserved address is not detected [enStatus = I2cReservedByteDetect] 
 **                               - I2C stop condition is not detected [enStatus = I2cStopDetect] 
 **                               - I2C bus is idle [enStatus = I2cBusStatus] 
 **                               - I2C bus error doesn't occur [enStatus = I2cBusErr] 
 **                               - I2C transfer or receive is not completed. [enStatus = I2cRxTxIrq] 
 **
 ** \retval TRUE                 If one of following conditions are met:
 **                               - I2C overrun error occurs  [enStatus = I2cOverrunError] 
 **                               - I2C receive buffer is full [enStatus = I2cRxFull] 
 **                               - I2C tranfer buffer is empty [enStatus = I2cTxEmpty] 
 **                               - I2C tranfer FIFO request issues [enStatus = I2cTxFifoRequest] 
 **                               - I2C tranfer is idle [enStatus = I2cTxIdle] 
 **                               - I2C first byte is detected [enStatus = I2cFirstByteDetect] 
 **                               - I2C reserved address is detected [enStatus = I2cReservedByteDetect]
 **                               - I2C stop condition is detected [enStatus = I2cStopDetect]
 **                               - I2C bus is busy [enStatus = I2cBusStatus] 
 **                               - I2C bus error occurs [enStatus = I2cBusErr] 
 **                               - I2C transfer or receive is completed. [enStatus = I2cRxTxIrq] 
 ** 
 ******************************************************************************/
boolean_t Mfs_I2c_GetStatus(volatile stc_mfsn_i2c_t* pstcI2c, 
                            en_i2c_status_t enStatus)
{
    boolean_t bRet = FALSE;
    
    switch(enStatus)
    {
        case I2cOverrunError:
            bRet = (1u == pstcI2c->SSR_f.ORE ) ? TRUE : FALSE;
            break;  
        case I2cRxFull:
            bRet = (1u == pstcI2c->SSR_f.RDRF) ? TRUE : FALSE;
            break;  
        case I2cTxEmpty:
            bRet = (1u == pstcI2c->SSR_f.TDRE) ? TRUE : FALSE;
            break;  
        case I2cTxIdle:
            bRet = (1u == pstcI2c->SSR_f.TBI) ? TRUE : FALSE;
            break;  
        case I2cTxFifoRequest:
            bRet = (1u == pstcI2c->FCR_f.FDRQ) ? TRUE : FALSE;
            break;  
        case I2cFirstByteDetect:
            bRet = (1u == pstcI2c->IBSR_f.FBT) ? TRUE : FALSE;
            break;  
        case I2cReservedByteDetect:
            bRet = (1u == pstcI2c->IBSR_f.RSA ) ? TRUE : FALSE;
            break;  
        case I2cStopDetect:
            bRet = (1u == pstcI2c->IBSR_f.SPC) ? TRUE : FALSE;
            break;  
        case I2cBusStatus:
            bRet = (1u == pstcI2c->IBSR_f.BB) ? TRUE : FALSE;
            break;  
        case I2cBusErr:
            bRet = (1u == pstcI2c->IBCR_f.BER) ? TRUE : FALSE;
            break; 
        case I2cRxTxIrq:
            bRet = (1u == pstcI2c->IBCR_f.INT) ? TRUE : FALSE;
            break;
        case I2cDevAddrMatch:
            if((0u == pstcI2c->IBCR_f.MSS) && (1u == pstcI2c->IBCR_f.ACT_SCC))
            {
                bRet = TRUE;
            }
            else
            {
                bRet = FALSE;
            }
            break;
        default:
            break;  
    }
    
    return bRet;
}


/**
 ******************************************************************************
 ** \brief Clear status of I2C according to status type
 **
 ** \param [in] pstcI2c      Pointer to I2C instance   
 ** \param [in] enStatus     I2C status type
 ** \arg   I2cOverrunError          I2C overrun error
 ** \arg   I2cRxFull                I2C receive buffer full
 ** \arg   I2cTxEmpty               I2C tranfer buffer empty
 ** \arg   I2cTxFifoRequest         I2C transfer FIFO request
 ** \arg   I2cTxIdle                I2C tranfer idle status
 ** \arg   I2cFirstByteDetect       I2C first byte detection
 ** \arg   I2cReservedByteDetect    I2C reserved address detection
 ** \arg   I2cStopDetect            I2C stop condition detection
 ** \arg   I2cBusStatus             I2C bus status 
 ** \arg   I2cBusErr                I2C bus error
 ** \arg   I2cRxTxIrq               I2C transfer and receive interrupt flag
 ** 
 ** \retval Ok                    Status has been cleared normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2c == NULL
 ** \note  The following status can only be cleared by hardware behavior:
 **        - I2cRxFull
 **        - I2cTxEmpty
 **        - I2cTxIdle
 **        - I2cFirstByteDetect
 **        - I2cReservedByteDetect
 **        - I2cBusStatus
 **        - I2cBusErr
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_ClrStatus(volatile stc_mfsn_i2c_t* pstcI2c,
                               en_i2c_status_t enStatus)
{
    if (NULL == pstcI2c)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enStatus)
    {
        case I2cOverrunError:
            pstcI2c->SSR_f.REC = 1u;
            break;  
        case I2cRxFull:
        case I2cTxEmpty:
        case I2cTxIdle:
        case I2cDevAddrMatch:  
            break;  
        case I2cTxFifoRequest:
            pstcI2c->FCR_f.FDRQ = 0u;
            break;  
        case I2cFirstByteDetect:
            break;  
        case I2cReservedByteDetect:
            break;  
        case I2cStopDetect:
            pstcI2c->IBSR_f.SPC = 0u;
            break;  
        case I2cBusStatus:
            break;  
        case I2cBusErr:
            break;  
        case I2cRxTxIrq:
            pstcI2c->IBCR &= ~0x41u;
            break;
        default:
            return ErrorInvalidParameter;  
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get direction of I2C data in slave mode
 **
 ** \param [in] pstcI2c      Pointer to I2C instance   
 ** 
 ** \retval i2c_master_tx_slave_rx  Master will send data to slave 
 ** \retval i2c_slave_tx_master_rx  Master will receive data from slave
 ** 
 ** This function can be called after receiving the device address from master
 ** in the slave mode.
 **
 ******************************************************************************/
en_i2c_data_dir_t Mfs_I2c_GetDataDir(volatile stc_mfsn_i2c_t* pstcI2c)
{
    en_i2c_data_dir_t enDir;
    
    (0u == pstcI2c->IBSR_f.TRX) ? (enDir = i2c_master_tx_slave_rx) : (enDir = i2c_slave_tx_master_rx);
    
    return enDir;
}

/**
 ******************************************************************************
 ** \brief Reset I2C FIFO
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** 
 ** \retval Ok                    FIFO has been successfully reset
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2c == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_ResetFifo (volatile stc_mfsn_i2c_t* pstcI2c, 
                               en_mfs_fifo_t enFifo)
{
    if (NULL == pstcI2c)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFifo)
    {
        case MfsFifo1:
            pstcI2c->FCR_f.FCL1 = 1u;
            break;
        case MfsFifo2:
            pstcI2c->FCR_f.FCL2 = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set I2C FIFO count
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** \param [in] u8Count    FIFO count
 ** 
 ** \retval Ok                    FIFO count has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2c == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_I2c_SetFifoCount(volatile stc_mfsn_i2c_t* pstcI2c,
                                  en_mfs_fifo_t enFifo,
                                  uint8_t u8Count)
{
    if (NULL == pstcI2c)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFifo)
    {
        case MfsFifo1:
            pstcI2c->FBYTE1 = u8Count;
            break;
        case MfsFifo2:
            pstcI2c->FBYTE2 = u8Count;
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get I2C FIFO count
 **
 ** \param [in] pstcI2c    Pointer to I2C instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** 
 ** \retval FIFO count      
 ** 
 ** This function gets the current data count in selected FIFO.
 **
 ** \note 0xFF will be return value if FIFO index is error.
 **
 ******************************************************************************/
uint8_t Mfs_I2c_GetFifoCount(volatile stc_mfsn_i2c_t* pstcI2c,
                             en_mfs_fifo_t enFifo)
{
    uint8_t u8FifoCnt = 0xFFu;
    
    switch(enFifo)
    {
        case MfsFifo1:
            u8FifoCnt = pstcI2c->FBYTE1;
            break;
        case MfsFifo2:
            u8FifoCnt = pstcI2c->FBYTE2;
            break;
        default:
           break;
    }
    
    return u8FifoCnt;
}
#endif

#if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON)
/******************************************************************************
 * LIN driver functions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain MFS instance.
 **
 ** \param pstcLin Pointer to LIN instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_mfs_intern_data_t* MfsLinGetInternDataPtr(volatile stc_mfsn_lin_t* pstcLin)
{
    stc_mfs_intern_data_t* pstcInternDataPtr = NULL;
    uint32_t u32Instance;

    if (NULL != pstcLin)
    {
        for (u32Instance = 0u; u32Instance < (uint32_t)MfsInstanceIndexMax; u32Instance++)
        {
            if (pstcLin == m_astcMfsInstanceDataLut[u32Instance].stcInstance.pstcLinInstance)
            {
                pstcInternDataPtr = &m_astcMfsInstanceDataLut[u32Instance].stcInternData;
                break;
            }
        }
            
    }

    return (pstcInternDataPtr);
} /* MfsGetInternDataPtr */

#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON)  ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON)  ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)    
/**
 ******************************************************************************
 ** \brief LIN receive interrupt service routine.
 **
 ** \param pstcLin Pointer to LIN instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure     
 **
 ** This function is called on Transmit Interrupt set by the MFS. 
 ** 
 ******************************************************************************/
void MfsLinIrqHandlerRx( volatile stc_mfsn_lin_t*   pstcLin,
                         stc_mfs_intern_data_t* pstcMfsInternData)
{
    if ((NULL != pstcLin) && (NULL != pstcMfsInternData))
    {
        if (NULL != pstcMfsInternData->stcLinInternIrqCb.pfnRxIrqCb)
        {
            pstcMfsInternData->stcLinInternIrqCb.pfnRxIrqCb();
        }
    }
} /* MfsIrqHandlerRx */

/**
 ******************************************************************************
 ** \brief LIN transmit interrupt service routine.
 **
 ** \param pstcLin Pointer to LIN instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure     
 **
 ** This function is called on Transmit Interrupt set by the MFS. 
 ** 
 ******************************************************************************/
void MfsLinIrqHandlerTx( volatile stc_mfsn_lin_t*   pstcLin, 
                         stc_mfs_intern_data_t* pstcMfsInternData)
{
    if ((NULL != pstcLin) && (NULL != pstcMfsInternData))
    {
        if(1u == pstcLin->SSR_f.TDRE)
        {
            if (NULL != pstcMfsInternData->stcLinInternIrqCb.pfnTxIrqCb)
            {
                pstcMfsInternData->stcLinInternIrqCb.pfnTxIrqCb();
            }
        }
        
        if(1u == pstcLin->SSR_f.TBI)
        {
            if (NULL != pstcMfsInternData->stcLinInternIrqCb.pfnTxIdleIrqCb)
            {
                pstcMfsInternData->stcLinInternIrqCb.pfnTxIdleIrqCb();
            }
        }
        
        if(1u == pstcLin->FCR_f.FDRQ)
        {
            if (NULL != pstcMfsInternData->stcLinInternIrqCb.pfnTxFifoIrqCb)
            {
                pstcMfsInternData->stcLinInternIrqCb.pfnTxFifoIrqCb();
            }
            
            pstcLin->FCR_f.FDRQ = 0u;
        }
        
        
    }
} /* MfsIrqHandlerTx */

/**
 ******************************************************************************
 ** \brief LIN status interrupt service routine.
 **
 ** \param pstcLin Pointer to LIN instance   
 ** \param pstcMfsInternData Pointer to MFS internal data structure     
 **
 ** This function is called on Transmit Interrupt set by the MFS. 
 ** 
 ******************************************************************************/
void MfsLinIrqHandlerStatus( volatile stc_mfsn_lin_t*   pstcLin, 
                             stc_mfs_intern_data_t* pstcMfsInternData)
{
    if(1u == pstcLin->SSR_f.LBD)
    {
        pstcLin->SSR_f.LBD = 0u;
        if (NULL != pstcMfsInternData->stcLinInternIrqCb.pfnLinBreakIrqCb)
        {
            pstcMfsInternData->stcLinInternIrqCb.pfnLinBreakIrqCb();
        }
    }
}

/**
 ******************************************************************************
 ** \brief Enable LIN interrupts
 **
 ** \param [in] pstcLin    Pointer to LIN instance   
 ** \param [in] enIrqSel   Enumeration of LIN interrupts
 ** \arg LinTxIrq          TX interrupt of LIN
 ** \arg LinRxIrq          RX interrupt of LIN
 ** \arg LinBreakIrq       LIN break interrupt
 ** \arg LinTxIdleIrq      TX idle interrupt of LIN
 ** \arg LinTxFifoIrq      TX FIFO interrupt of LIN
 ** 
 ** \retval Ok                    Interrupts has been enabled 
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL        
 **
 ******************************************************************************/            
en_result_t Mfs_Lin_EnableIrq(volatile stc_mfsn_lin_t* pstcLin, 
                              en_lin_irq_sel_t enIrqSel)
{
    if (NULL == pstcLin)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case LinTxIrq:
            pstcLin->SCR_f.TIE = 1u;
            break;
        case LinRxIrq:
            pstcLin->SCR_f.RIE = 1u;
            break;
        case LinBreakIrq:
            pstcLin->ESCR_f.LBIE = 1u;
            break;
        case LinTxIdleIrq:
            pstcLin->SCR_f.TBIE = 1u;
            break;
        case LinTxFifoIrq:
            pstcLin->FCR_f.FTIE = 1u;
            break;
        default:
            return (ErrorInvalidParameter);  
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable LIN interrupts
 **
 ** \param [in] pstcLin    Pointer to LIN instance   
 ** \param [in] enIrqSel   Enumeration of LIN interrupts
 ** \arg LinTxIrq          TX interrupt of LIN
 ** \arg LinRxIrq          RX interrupt of LIN
 ** \arg LinBreakIrq       LIN break interrupt
 ** \arg LinTxIdleIrq      TX idle interrupt of LIN
 ** \arg LinTxFifoIrq      TX FIFO interrupt of LIN
 ** 
 ** \retval Ok                    Interrupts has been disabled 
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL        
 **
 ******************************************************************************/            
en_result_t Mfs_Lin_DisableIrq(volatile stc_mfsn_lin_t* pstcLin, 
                               en_lin_irq_sel_t enIrqSel)
{
    if (NULL == pstcLin)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case LinTxIrq:
            pstcLin->SCR_f.TIE = 0u;
            break;
        case LinRxIrq:
            pstcLin->SCR_f.RIE = 0u;
            break;
        case LinBreakIrq:
            pstcLin->ESCR_f.LBIE = 0u;
            break;
        case LinTxIdleIrq:
            pstcLin->SCR_f.TBIE = 0u;
            break;
        case LinTxFifoIrq:
            pstcLin->FCR_f.FTIE = 0u;
            break;
        default:
            return (ErrorInvalidParameter);  
    }
    
    return Ok;
}

#endif     

/**
 ******************************************************************************
 ** \brief Initialization of a MFS module to activate as LIN.
 **
 ** Set registers to active MFS as LIN.
 **  
 ** \param [in]   pstcLin         Pointer to LIN instance register area
 ** \param [in]   pstcConfig      MFS LIN configuration
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcLin == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_Lin_Init( volatile stc_mfsn_lin_t*        pstcLin, 
                          const stc_mfs_lin_config_t* pstcConfig)
{
    stc_mfs_intern_data_t* pstcMfsInternData;
    uint32_t u32DummyCnt = 100u;
    uint8_t u8Ch;
    
	  /* Preset local register variables to zero */
    stc_mfs_lin_smr_field_t  stcSMR;
    stc_mfs_lin_scr_field_t  stcSCR;
    stc_mfs_lin_escr_field_t stcESCR;
	
    PDL_ZERO_STRUCT(stcSMR);
    PDL_ZERO_STRUCT(stcSCR);
    PDL_ZERO_STRUCT(stcESCR);
    
    /* Calculate the channel number */
    u8Ch = LinInstanceToIndex(pstcLin);
    
    if(u8Ch > 15u)
    {
        return (ErrorInvalidParameter);
    }
  
    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsInternData = MfsLinGetInternDataPtr(pstcLin);
    
    /* Parameter check and get ptr to internal data struct */
    if ((NULL == pstcMfsInternData)
    ||  (NULL == pstcConfig)
       )
    {
        return (ErrorInvalidParameter);
    }
    
    /* Set MFS mode in the internal structure */
    pstcMfsInternData->enMode = MfsLinMode;

    /* First of all set MFS to Asynchronous mode 0 */
    pstcLin->SMR = 0u;

    /* Then we disable TX and RX for safe operation */
    pstcLin->SCR = 0u;

    /* Clear MFS by setting the Software Reset bit */
    pstcLin->SCR_f.UPCL = TRUE;

    /* Set LIN mode to HW and preset variable */
    stcSMR.MD = 3u;

    /* Enable serial output */
    stcSMR.SOE = TRUE;

    /* Operation Mode Selection */
    switch (pstcConfig->enMsMode)
    {
        /* Master */
        case LinMasterMode:
            stcSCR.MS = FALSE;
            break;
        /* Slave */
        case LinSlaveMode:
            stcSCR.MS = TRUE;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Set LIN master configuration */
    if (LinMasterMode == pstcConfig->enMsMode)
    {
        /* Stop bits configuration */
        switch (pstcConfig->enStopBits)
        {
            case LinOneStopBit:
                stcSMR.SBL   = FALSE;
                stcESCR.ESBL = FALSE;
                break;
            case LinTwoStopBits:
                stcSMR.SBL   = TRUE;
                stcESCR.ESBL = FALSE;
                break;
            case LinThreeStopBits:
                stcSMR.SBL   = FALSE;
                stcESCR.ESBL = TRUE;
                break;
            case LinFourStopBits:
                stcSMR.SBL   = TRUE;
                stcESCR.ESBL = TRUE;
                break;
            default:
                return (ErrorInvalidParameter);
        }
        /* Break length configuration */
        switch (pstcConfig->enBreakLength)
        {
            case LinBreakLength13:
                stcESCR.LBL = 0u;
                break;
            case LinBreakLength14:
                stcESCR.LBL = 1u;
                break;
            case LinBreakLength15:
                stcESCR.LBL = 2u;
                break;
            case LinBreakLength16:
                stcESCR.LBL = 3u;
                break;
            default:
                return (ErrorInvalidParameter);
        }
        /* Delimiter length configuration */
        switch (pstcConfig->enDelimiterLength)
        {
            case LinDelimiterLength1:
                stcESCR.DEL = 0u;
                break;
            case LinDelimiterLength2:
                stcESCR.DEL = 1u;
                break;
            case LinDelimiterLength3:
                stcESCR.DEL = 2u;
                break;
            case LinDelimiterLength4:
                stcESCR.DEL = 3u;
                break;
            default:
                return (ErrorInvalidParameter);
        }
    }

    /* Set Baudrate */
    (void)Mfs_Lin_SetBaudRate(pstcLin, pstcConfig->u32BaudRate);

    /* Set registers value */
    pstcLin->SMR_f  = stcSMR;
    pstcLin->SCR_f  = stcSCR;
    pstcLin->ESCR_f = stcESCR;
    
    /* Set external clock */
    pstcLin->BGR_f.EXT = ((pstcConfig->bUseExtClk == TRUE) ? 1u : 0u);
    
    while(u32DummyCnt--)
    {
        pstcLin->SSR_f.LBD = 0u;
    }
   
    if(pstcConfig->pstcFifoConfig != NULL)
    {
        /* Reset FIFO */
        pstcLin->FCR_f.FCL1 = 1u;
        pstcLin->FCR_f.FCL2 = 1u;
        /* Enable FIFO receive Idle detection */
        pstcLin->FCR_f.FRIIE = 1u;
        /* Selection TX and RX FIFO  */
        switch(pstcConfig->pstcFifoConfig->enFifoSel)
        {
            case MfsTxFifo1RxFifo2:
                pstcLin->FCR_f.FSEL = 0u;
                break;
            case MfsTxFifo2RxFifo1:
                pstcLin->FCR_f.FSEL = 1u;
                break;
            default:
               return (ErrorInvalidParameter);
        }
        /* Set FIFO count */
        pstcLin->FBYTE1 = pstcConfig->pstcFifoConfig->u8ByteCount1;
        pstcLin->FBYTE2 = pstcConfig->pstcFifoConfig->u8ByteCount2;
        /* Enable FIFO  */
        pstcLin->FCR_f.FE1 = 1u;
        pstcLin->FCR_f.FE2 = 1u;
    }

#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON)  ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON)  ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)     
    /* Configure interrupt */
    if(NULL != pstcConfig->pstcIrqEn)
    {
        if(TRUE == pstcConfig->pstcIrqEn->bTxIrq)
        {
            pstcLin->SCR_f.TIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bRxIrq)
        {
            pstcLin->SCR_f.RIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxIdleIrq)
        {
            pstcLin->SCR_f.TBIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bTxFifoIrq)
        {
            pstcLin->FCR_f.FTIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bLinBreakIrq)
        {
            pstcLin->ESCR_f.LBIE = 1u;
        }
    }
    
    /* Configure interrupt callback */
    if(NULL != pstcConfig->pstcIrqCb)
    {
        pstcMfsInternData->stcLinInternIrqCb.pfnRxIrqCb = pstcConfig->pstcIrqCb->pfnRxIrqCb;
        pstcMfsInternData->stcLinInternIrqCb.pfnTxIrqCb = pstcConfig->pstcIrqCb->pfnTxIrqCb;
        pstcMfsInternData->stcLinInternIrqCb.pfnTxIdleIrqCb = pstcConfig->pstcIrqCb->pfnTxIdleIrqCb;
        pstcMfsInternData->stcLinInternIrqCb.pfnTxFifoIrqCb = pstcConfig->pstcIrqCb->pfnTxFifoIrqCb;
        pstcMfsInternData->stcLinInternIrqCb.pfnLinBreakIrqCb = pstcConfig->pstcIrqCb->pfnLinBreakIrqCb;
    }
    
    /* Configure NVIC */
    if(TRUE == pstcConfig->bTouchNvic)
    {
        MfsInitNvic(u8Ch, FALSE);
        MfsInitNvic(u8Ch, TRUE);
    }
#endif
    
    return (Ok);
} /* Mfs_Lin_Init */

/**
 ******************************************************************************
 ** \brief Deinitialisation of a MFS module activating as LIN.
 **
 ** All used LIN register are reset to their default values.
 **  
 ** \param [in]   pstcLin      Pointer to LIN instance register area
 ** \param [in]   bTouchNvic   Touch NVIC or not
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcLin == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_Lin_DeInit(volatile stc_mfsn_lin_t* pstcLin, boolean_t bTouchNvic)
{
    en_result_t            enResult;
    stc_mfs_intern_data_t* pstcMfsInternData;
    uint8_t u8Ch;
    
    /* Calculate the channel number */
    u8Ch = LinInstanceToIndex(pstcLin);
    
    if(u8Ch > 15u)
    {
        return (ErrorInvalidParameter);
    }

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsInternData = MfsLinGetInternDataPtr(pstcLin);
    /* ... and check */
    if (NULL == pstcMfsInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        pstcLin->SCR_f.TXE = FALSE;
        pstcLin->SCR_f.RXE = FALSE;

        /* Baud Rate Generation Reload Reset */
        pstcLin->BGR = 0u;
            
        /* Serial Mode Register clear all bits (valid in any mode) */
        pstcLin->SMR = 0u;
           
        /* Reset Mfs receive and transmit bit to default */
        /* and clear all error flags */
        pstcLin->SCR = 0u;

        /* Clear reception Errors */
        pstcLin->SSR_f.REC = TRUE;
           
        /* Reset all other used register to default value */
        pstcLin->SSR  = 0u;
        pstcLin->ESCR = 0u;
        pstcLin->FCR = 0u;


        /* Clear MFS by setting the Software Reset bit */
        pstcLin->SCR_f.UPCL = TRUE;

#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON)  ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON)  ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)         
        if(TRUE == bTouchNvic)
        {
            MfsDeInitNvic(u8Ch, FALSE);
            MfsDeInitNvic(u8Ch, TRUE);
        }
#endif        
        enResult = Ok;
    }

    return (enResult);
} /* Mfs_Lin_DeInit */


/**
 ******************************************************************************
 ** \brief Set the baudrate of LIN
 **
 ** \param [in] pstcLin     Pointer to LIN instance   
 ** \param [in] u32BaudRate Baudrate value [bps]
 ** 
 ** \retval Ok                    LIN baud rate has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL
 **                               - u32BaudRate < 3         
 ** \note
 ** The LIN baud rate can be initialized in the Mfs_Lin_Init() and be modified
 ** with the function.
 ** 
 ******************************************************************************/
en_result_t Mfs_Lin_SetBaudRate(volatile stc_mfsn_lin_t* pstcLin,
                                uint32_t u32BaudRate)
{
    uint32_t u32Pclk1;

    SystemCoreClockUpdate();
    if ((NULL == pstcLin) || (u32BaudRate < 3u))
    {
        return ErrorInvalidParameter;
    }
#if (PDL_MCU_CORE == PDL_FM0P_CORE)       
    u32Pclk1 = SystemCoreClock /(1ul << (FM_CRG->APBC1_PSR & 0x03u)); /* MFS is attached on APB1 bus for FM0+ */
#else
    u32Pclk1 = SystemCoreClock /(1ul << (FM_CRG->APBC2_PSR & 0x03u)); /* MFS is attached on APB2 bus for FM3 and FM4 */
#endif    
    pstcLin->BGR_f.BGR = (u32Pclk1/u32BaudRate) - 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Generate LIN break filed 
 **
 ** \param [in] pstcLin     Pointer to LIN instance   
 ** 
 ** \retval Ok                    Break filed has been successfully generated
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL  
 ** 
 ******************************************************************************/
en_result_t Mfs_Lin_GenerateBreakField(volatile stc_mfsn_lin_t* pstcLin)
{
    if (NULL == pstcLin)
    {
        return ErrorInvalidParameter;
    }
    
    pstcLin->SCR_f.LBR = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable LIN functions
 **
 ** \param [in] pstcLin    Pointer to LIN instance   
 ** \param [in] enFunc     LIN function types
 ** \arg   LinTx    LIN transfer function
 ** \arg   LinRx    LIN receive function
 ** 
 ** \retval Ok                    Function has been enabled normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL                
 ** 
 ******************************************************************************/
en_result_t Mfs_Lin_EnableFunc(volatile stc_mfsn_lin_t* pstcLin, en_lin_func_t enFunc)
{
    if (NULL == pstcLin)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFunc)
    {
        case LinTx:
            pstcLin->SCR_f.TXE = 1u;
            break;
        case LinRx:
            pstcLin->SCR_f.RXE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable LIN functions
 **
 ** \param [in] pstcLin    Pointer to LIN instance   
 ** \param [in] enFunc     LIN function types
 ** \arg   LinTx    LIN transfer function
 ** \arg   LinRx    LIN receive function
 ** 
 ** \retval Ok                    Function has been disabled normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL                  
 ** 
 ******************************************************************************/
en_result_t Mfs_Lin_DisableFunc(volatile stc_mfsn_lin_t* pstcLin, en_lin_func_t enFunc)
{
    if (NULL == pstcLin)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFunc)
    {
        case LinTx:
            pstcLin->SCR_f.TXE = 0u;
            break;
        case LinRx:
            pstcLin->SCR_f.RXE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get status of LIN according to status type
 **
 ** \param [in] pstcLin      Pointer to LIN instance   
 ** \param [in] enStatus     LIN status type
 ** \arg   LinParityError   LIN parity error
 ** \arg   LinFrameError    LIN frame error
 ** \arg   LinRxFull        LIN receive buffer full
 ** \arg   LinTxEmpty       LIN tranfer buffer empty
 ** \arg   LinTxIdle        LIN tranfer idle status
 ** \arg   LinBreakFlag     LIN break field detection flag
 ** \arg   LinTxFifoRequest LIN transfer FIFO request
 ** 
 ** \retval FALSE                 If one of following conditions are met:
 **                               - No LIN parity error [enStatus = LinParityError]
 **                               - No LIN frame error  [enStatus = LinFrameError]    
 **                               - LIN receive buffer is not full [enStatus = LinRxFull]
 **                               - LIN tranfer buffer is not empty [enStatus = LinTxEmpty]
 **                               - LIN tranfer is on-going [enStatus = LinTxIdle]
 **                               - LIN break field is not detected [enStatus = LinBreakFlag]
 **                               - No LIN transfer FIFO request [enStatus = LinTxFifoRequest] 
 ** \retval TRUE                 If one of following conditions are met:
 **                               - LIN parity error occurs [enStatus = LinParityError]
 **                               - LIN frame error occurs  [enStatus = LinFrameError]   
 **                               - LIN receive buffer is full [enStatus = LinRxFull]
 **                               - LIN tranfer buffer is empty [enStatus = LinTxEmpty]
 **                               - LIN tranfer is idle [enStatus = LinTxIdle]
 **                               - LIN break field is detected [enStatus = LinBreakFlag]
 **                               - LIN transfer FIFO request issues [enStatus = LinTxFifoRequest] 
 ** 
 ******************************************************************************/
boolean_t Mfs_Lin_GetStatus(volatile stc_mfsn_lin_t* pstcLin, 
                            en_lin_status_t enStatus)
{
    boolean_t bResult = FALSE;
    
    switch(enStatus)
    {
        case LinFrameError:
            bResult = (1u == pstcLin->SSR_f.FRE) ? TRUE : FALSE;
            break;  
        case LinOverrunError:
            bResult = (1u == pstcLin->SSR_f.ORE) ? TRUE : FALSE;
            break;  
        case LinRxFull:
            bResult = (1u == pstcLin->SSR_f.RDRF) ? TRUE : FALSE;
            break;  
        case LinTxEmpty:
            bResult = (1u == pstcLin->SSR_f.TDRE) ? TRUE : FALSE;
            break;  
        case LinTxIdle:
            bResult = (1u == pstcLin->SSR_f.TBI) ? TRUE : FALSE;
            break;  
        case LinBreakFlag:
            bResult = (1u == pstcLin->SSR_f.LBD) ? TRUE : FALSE;
            break;
        case LinTxFifoRequest:
            bResult = (1u == pstcLin->FCR_f.FDRQ) ? TRUE : FALSE;
            break;  
        default:
            break;  
    }
    
    return bResult;
}

/**
 ******************************************************************************
 ** \brief Clear status of LIN according to status type
 **
 ** \param [in] pstcLin      Pointer to LIN instance   
 ** \param [in] enStatus     LIN status type
 ** \arg   LinParityError   LIN parity error
 ** \arg   LinFrameError    LIN frame error
 ** \arg   LinRxFull        LIN receive buffer full
 ** \arg   LinTxEmpty       LIN tranfer buffer empty
 ** \arg   LinTxIdle        LIN tranfer idle status
 ** \arg   LinBreakFlag     LIN break field detection flag
 ** \arg   LinTxFifoRequest LIN transfer FIFO request
 ** 
 ** \retval Ok                    Status has been cleared normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL
 ** \note  The following status can only be cleared by hardware behavior:
 **        - LinRxFull
 **        - LinTxEmpty
 **        - LinTxIdle
 ** 
 ******************************************************************************/
en_result_t Mfs_Lin_ClrStatus(volatile stc_mfsn_lin_t* pstcLin,
                              en_lin_status_t enStatus)
{
    if (NULL == pstcLin)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enStatus)
    {
        case LinFrameError:
        case LinOverrunError:
            pstcLin->SSR_f.REC = 1u;
            break;  
        case LinRxFull:
        case LinTxEmpty:
        case LinTxIdle:
            break;  
        case LinBreakFlag:
            pstcLin->SSR_f.LBD = 0u;
            break;
        case LinTxFifoRequest:
            pstcLin->FCR_f.FDRQ = 0u;
            break;  
        default:
            return ErrorInvalidParameter;  
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Write LIN data buffer
 **
 ** \param [in] pstcLin   Pointer to LIN instance   
 ** \param [in] Data      Data to be sent
 ** 
 ** \retval Ok                    Data has been successfully sent
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Lin_SendData(volatile stc_mfsn_lin_t* pstcLin, uint8_t Data)
{
    if (NULL == pstcLin)
    {
        return ErrorInvalidParameter;
    }
    
    pstcLin->TDR = Data;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read LIN data buffer
 **
 ** \param [in] pstcLin   Pointer to LIN instance   
 ** 
 ** \retval Receive data        
 ** 
 ******************************************************************************/
uint8_t Mfs_Lin_ReceiveData(volatile stc_mfsn_lin_t* pstcLin)
{    
    return (pstcLin->RDR);
}

/**
 ******************************************************************************
 ** \brief Reset LIN FIFO
 **
 ** \param [in] pstcLin    Pointer to LIN instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** 
 ** \retval Ok                    FIFO has been successfully reset
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Lin_ResetFifo (volatile stc_mfsn_lin_t* pstcLin, 
                               en_mfs_fifo_t enFifo)
{
    if (NULL == pstcLin)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFifo)
    {
        case MfsFifo1:
            pstcLin->FCR_f.FCL1 = 1u;
            break;
        case MfsFifo2:
            pstcLin->FCR_f.FCL2 = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set LIN FIFO count
 **
 ** \param [in] pstcLin    Pointer to LIN instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** \param [in] u8Count    FIFO count
 ** 
 ** \retval Ok                    FIFO count has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcLin == NULL           
 ** 
 ******************************************************************************/
en_result_t Mfs_Lin_SetFifoCount(volatile stc_mfsn_lin_t* pstcLin,
                                 en_mfs_fifo_t enFifo,
                                 uint8_t u8Count)
{
    if (NULL == pstcLin)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enFifo)
    {
        case MfsFifo1:
            pstcLin->FBYTE1 = u8Count;
            break;
        case MfsFifo2:
            pstcLin->FBYTE2 = u8Count;
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get LIN FIFO count
 **
 ** \param [in] pstcLin    Pointer to LIN instance   
 ** \param [in] enFifo     FIFO1 or FIFO2
 ** 
 ** \retval FIFO count      
 ** 
 ** This function gets the current data count in selected FIFO.
 **
 ** \note 0xFF will be return value if FIFO index is error.
 **
 ******************************************************************************/
uint8_t Mfs_Lin_GetFifoCount(volatile stc_mfsn_lin_t* pstcLin,
                             en_mfs_fifo_t enFifo)
{
    uint8_t u8FifoCnt = 0xFFu;
    
    switch(enFifo)
    {
        case MfsFifo1:
            u8FifoCnt = pstcLin->FBYTE1;
            break;
        case MfsFifo2:
            u8FifoCnt = pstcLin->FBYTE2;
            break;
        default:
           break;
    }
    
    return u8FifoCnt;
}

#endif                             
                             
#endif /* #if (defined(PDL_PERIPHERAL_MFS_ACTIVE)) */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
