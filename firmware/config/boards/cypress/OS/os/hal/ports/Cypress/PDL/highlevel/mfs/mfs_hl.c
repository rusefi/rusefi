/*******************************************************************************
* Copyright (C) 2013 Spansion LLC. All Rights Reserved. 
*
* This software is owned and published by: 
* Spansion LLC, 915 DeGuigne Dr. Sunnyvale, CA  94088-3453 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with Spansion 
* components. This software is licensed by Spansion to be adapted only 
* for use in systems utilizing Spansion components. Spansion shall not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  Spansion is providing this software "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the software.  
*
* SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS), 
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING, 
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED 
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED 
* WARRANTY OF NONINFRINGEMENT.  
* SPANSION SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, 
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT 
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, 
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR 
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, 
* SAVINGS OR PROFITS, 
* EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED 
* FROM, THE SOFTWARE.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Disclaimer and Copyright notice must be 
* included with each copy of this software, whether used in part or whole, 
* at all times.  
*/
/******************************************************************************/
/** \file mfs_hl.c
 **
 ** MFS High Level functions
 **
 ** History:
 **   - 2013-03-26  1.0  NT   First version.
 **   - 2014-05-26  1.1  EZ   Return Error when receiving errors occurs in 
 **                           Mfs_Hl_Read().
 **                           Add a function to get the pointer of TX or RX 
 **                           buffer.
 **   - 2014-11-07  1.2  EZ   Update the data transmission sequence in 
 **                           Mfs_Hl_Csio_SynchronousTrans() 
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mfs_hl.h"

#if (defined(PDL_PERIPHERAL_MFS_ACTIVE))

#if (PDL_MFS_USE_HL == PDL_ON)
/**
 ******************************************************************************
 ** \addtogroup MfshlGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
/* for BGR */
#define MFS_HL_RELOADVAL_MIN4UART   (4u)
#define MFS_HL_RELOADVAL_MIN4CSIO   (3u)
#define MFS_HL_RELOADVAL_MIN4LIN    (3u)
#define MFS_HL_BGR_MASK             (0x7FFFu)

/* Number definition */
#define MFS_HL_NUM_1               (1u)
#define MFS_HL_NUM_2               (2u)
#define MFS_HL_NUM_3               (3u)
#define MFS_HL_NUM_4               (4u)
#define MFS_HL_NUM_5               (5u)
#define MFS_HL_NUM_6               (6u)
#define MFS_HL_NUM_7               (7u)
#define MFS_HL_NUM_8               (8u)
#define MFS_HL_NUM_9               (9u)
#define MFS_HL_NUM_10              (10u)

/* Mask data */
#define MFS_HL_MASK_0              (0x01u)
#define MFS_HL_MASK_1              (0x02u)
#define MFS_HL_MASK_2              (0x04u)
#define MFS_HL_MASK_3              (0x08u)
#define MFS_HL_MASK_4              (0x10u)
#define MFS_HL_MASK_5              (0x20u)
#define MFS_HL_MASK_6              (0x40u)
#define MFS_HL_MASK_7              (0x80u)
#define MFS_HL_MASK_0_2            (0x07u)
#define MFS_HL_MASK_0_3            (0x0Fu)
#define MFS_HL_MASK_0_6            (0x7Fu)
#define MFS_HL_MASK_4_7            (0xF0u)

#define MFS_HL_MASK_0_7            (0x000000FFu)
#define MFS_HL_MASK_8_15           (0x0000FF00u)
#define MFS_HL_MASK_16_23          (0x00FF0000u)
#define MFS_HL_MASK_24_31          (0xFF000000u)
#define MFS_HL_MASK_0_15           (0x0000FFFFu)
#define MFS_HL_MASK_16_31          (0xFFFF0000u)

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/// Look-up table for all enabled MFS_HL instances and their internal data
static stc_mfs_hl_instance_data_t m_astcMfsHlInstanceDataLut[MfsInstanceIndexMax] =
{
#if (PDL_PERIPHERAL_ENABLE_MFS0 == PDL_ON)
    {
        &MFS0,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS1 == PDL_ON)
    {
        &MFS1,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS2 == PDL_ON)
    {
        &MFS2,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS3 == PDL_ON)
    {
        &MFS3,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS4 == PDL_ON)
    {
        &MFS4,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS5 == PDL_ON)
    {
        &MFS5,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS6 == PDL_ON)
    {
        &MFS6,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS7 == PDL_ON)
    {
        &MFS7,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS8 == PDL_ON)
    {
        &MFS8,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS9 == PDL_ON)
    {
        &MFS9,  /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS10 == PDL_ON)
    {
        &MFS10, /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS11 == PDL_ON)
    {
        &MFS11, /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS12 == PDL_ON)
    {
        &MFS12, /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS13 == PDL_ON)
    {
        &MFS13, /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS14 == PDL_ON)
    {
        &MFS14, /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS15 == PDL_ON)
    {
        &MFS15, /* pstcInstance */
        {NULL}  /* stcInternData (not initialized yet) */
    },
#endif
};

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static stc_mfs_hl_intern_data_t* MfsHlGetInternDataPtr(volatile stc_mfsn_t* pstcMfs);

static uint16_t MfsHlGetMin(uint16_t u16Num1,
                            uint16_t u16Num2
                           );

static void MfsHlWriteBuf(volatile stc_mfsn_t*      pstcMfs,
                          stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                         );

static void MfsHlIrqHandlerTx(volatile stc_mfsn_t* pstcMfs, 
                              void*                pvHandle
                             );

static void MfsHlReadBuf(volatile stc_mfsn_t*      pstcMfs,
                         stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                        );

static void MfsHlIrqHandlerRx(volatile stc_mfsn_t* pstcMfs, 
                              void*                pvHandle
                             );

static void MfsHlIrqHandlerSts(volatile stc_mfsn_t* pstcMfs, 
                               void*                pvHandle
                              );

static void MfsHlFifoUseInit(volatile stc_mfsn_t* pstcMfs,
                             uint16_t             u16BufFillLvl
                            );

static void MfsHlI2cDataTxMaster(volatile stc_mfsn_t*      pstcI2c,
                                 stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                                );

static void MfsHlI2cDataRxMaster(volatile stc_mfsn_t*      pstcI2c,
                                 stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                                );

static void MfsHlI2cPreStartSlave(volatile stc_mfsn_t*      pstcI2c,
                                  stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                                 );

static void MfsHlI2cDataRxSlave(volatile stc_mfsn_t*      pstcI2c,
                                stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                               );

static void MfsHlI2cDataTxSlave(volatile stc_mfsn_t*      pstcI2c,
                                stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                               );

static void MfsHlI2cIrqHandlerTx(volatile stc_mfsn_t* pstcMfs,
                                 void*                pvHandle
                                );

static void MfsHlI2cIrqHandlerRx(volatile stc_mfsn_t* pstcMfs,
                                 void*                pvHandle
                                );

static void MfsHlI2cIrqHandlerSts(volatile stc_mfsn_t* pstcMfs, 
                                  void*                pvHandle
                                 );

static en_result_t MfsHlI2cStartSlave(volatile stc_mfsn_t*      pstcI2c,
                                      stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                                     );

static en_result_t MfsHlI2cWaitIntState(volatile stc_mfs_hl_intern_data_t* pstcMfsHlInternData);

static en_result_t MfsHlI2cChkTxRxComplete(stc_mfs_hl_intern_data_t* pstcMfsHlInternData,
                                           uint32_t                  u32MaxCnt);

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain MFS_HL instance.
 **
 ** \param pstcMfs Pointer to MFS_HL instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_mfs_hl_intern_data_t* MfsHlGetInternDataPtr(volatile stc_mfsn_t* pstcMfs)
{
    stc_mfs_hl_intern_data_t* pstcMfsInternDataPtr = NULL;
    uint32_t                  u32Instance;

    if (NULL != pstcMfs)
    {
        for (u32Instance = 0u; u32Instance < (uint32_t)MfsInstanceIndexMax; u32Instance++)
        {
            if (pstcMfs == m_astcMfsHlInstanceDataLut[u32Instance].pstcInstance)
            {
                pstcMfsInternDataPtr = &m_astcMfsHlInstanceDataLut[u32Instance].stcInternData;
                break;
            }
        }
    }

    return (pstcMfsInternDataPtr);
} /* MfsHlGetInternDataPtr */

/**
 ******************************************************************************
 ** \brief Get minimum number.
 **
 ** \param [in]   u16Num1   The number to compare to u16Num2
 ** \param [in]   u16Num2   The number to compare to u16Num1
 **
 ** \return The smaller number as the result of comparing u16Num1 with u16Num2
 **
 ******************************************************************************/
static uint16_t MfsHlGetMin(uint16_t u16Num1,
                            uint16_t u16Num2
                           )
{
    uint16_t u16Min;

    /* Return the smaller number as the result of comparing u16Num1 with u16Num2 */
    u16Min = u16Num2;
    if (u16Num1 < u16Num2)
    {
        u16Min = u16Num1;
    }

    return (u16Min);
} /* MfsHlGetMin */

/**
 ******************************************************************************
 ** \brief Write the contents of the buffer to Transmit Data Register.
 **
 ** The data from the internal Ring Buffer is put into the Fifo until there's
 ** no more space left or all data has been transferred to the Fifo.
 ** If the referenced MFS does not have a FIFO single data is put.
 **
 ** \param [in]       pstcMfs              Pointer to MFS instance register area
 ** \param [in,out]   pstcMfsHlInternData  Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlWriteBuf(volatile stc_mfsn_t*      pstcMfs,
                          stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                         )
{
    stc_mfs_hl_buffer_t* pstcBuffer;
    uint32_t             u32Data;
    uint_fast16_t        fu16Loop;
    uint_fast16_t        fu16Cnt;

    pstcBuffer = &pstcMfsHlInternData->stcTxBuffer;
    if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
    {
        /* Get # of Data to put into HW FIFO, whichever space is less */
        /* Temporary var only used to define the order of volatile accesses to */
        /* Get rid of the compiler warning */
        fu16Loop = (uint_fast16_t)MfsHlGetMin( pstcBuffer->u16FillCount,
                                               (uint16_t)(MFS_FIFO_MAX_VAL - (pstcMfs->FBYTE1))
                                             );
    }
    else
    {
        fu16Loop = MFS_HL_NUM_1;
    }

    while (0u != fu16Loop)
    {
        u32Data = 0u;
        if (MfsEightBits >= pstcMfsHlInternData->u8DataWidth)
        {
            fu16Cnt = MFS_HL_NUM_1;
        }
        else if (MfsSixteenBits >= pstcMfsHlInternData->u8DataWidth)
        {
            fu16Cnt = MFS_HL_NUM_2;
        }
        else if (MfsTwentyFourBits >= pstcMfsHlInternData->u8DataWidth)
        {
            fu16Cnt = MFS_HL_NUM_3;
        }
        else
        {
            fu16Cnt = MFS_HL_NUM_4;
        }
        while ((0u != fu16Cnt) && (0u != fu16Loop))
        {
            u32Data <<= MFS_HL_NUM_8;
            u32Data |= (uint32_t)(pstcBuffer->pu8Buffer[pstcBuffer->u16OutIndex]) & MFS_HL_MASK_0_7;
            /* update tail */
            pstcBuffer->u16OutIndex = ((pstcBuffer->u16OutIndex + (uint16_t)MFS_HL_NUM_1) % (pstcBuffer->u16BufferSize));
            pstcBuffer->u16FillCount-- ;
            pstcBuffer->u16TxCount-- ;
            pstcBuffer->u16TxCounter++ ;
            fu16Loop-- ;
            fu16Cnt--;
        }
        if (MfsSixteenBits >= pstcMfsHlInternData->u8DataWidth)
        {
            /* Put data from Buffer into Transmit Data Register */
            pstcMfs->TDR = (uint16_t)u32Data;
        }
        else
        {
            /* Put data from Buffer into Transmit Data Register (upper 16 bits width) */
            pstcMfs->TDR32 = u32Data;
        }
    } /* while ( fu16Loop != 0 ) */
} /* MfsHlWriteBuf */

/**
 ******************************************************************************
 ** \brief MFS_HL transmit interrupt service routine.
 **
 ** This Function is called on each Transmit Interrupt set by the MFS.
 ** This highly depends on the Configuration of the INT Sources.
 ** Interrupts might occur on TX Fifo empty, last bit has been shifted out, ...
 ** The data from the internal Ring Buffer is put into the Fifo until there's
 ** no more space left or all data has been transferred to the Fifo.
 ** If the referenced MFS does not have a FIFO single data is put.
 **
 ** In LIN mode this handler also checks for LIN Break detection. If it is
 ** detected, possible reception errors are cleared, the reception register
 ** (RDR) is flushed and (if not NULL) a dedicated callback function is called.
 ** Afterwards the Function is left by return w/o any further transmission action.
 ** 
 ******************************************************************************/
static void MfsHlIrqHandlerTx(volatile stc_mfsn_t* pstcMfs, 
                              void*                pvHandle
                             )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    stc_mfs_hl_buffer_t*      pstcBuffer;
    uint8_t                   u8End = FALSE;
    volatile uint16_t         u16DummyData;

    pstcMfsHlInternData = (stc_mfs_hl_intern_data_t *)pvHandle;
    pstcBuffer = &pstcMfsHlInternData->stcTxBuffer;

    /* Wait TX bus idle (for CSIO timer mode) */
    if (MfsHlExecWaitTxBusIdle == pstcMfsHlInternData->u8Exec)
    {
        /* Disable TX Idle Interrupt */
        pstcMfs->SCR_f.TBIE = FALSE;
        /* Disable serial timer */
        pstcMfs->SCR_f.TXE = FALSE;
        pstcMfs->SACSR_f.TMRE = FALSE;
        pstcMfs->SCR_f.TXE = TRUE;
        if (NULL != pstcMfsHlInternData->pfnTransmitCbFunction)
        {
            /* Call transmit callback function. */
            pstcMfsHlInternData->pfnTransmitCbFunction(pstcBuffer->u16TxCounter);
        }
        /* Transmit is standby */
        pstcMfsHlInternData->u8Exec = MfsHlExecStby;
    }
    else
    {
        /* Write the contents of the buffer to Transmit Data Register */
        MfsHlWriteBuf(pstcMfs, pstcMfsHlInternData);
        if (0u == pstcBuffer->u16FillCount)
        {
            u8End = TRUE;
        }

        if (TRUE == u8End)
        {
            /* If no more bytes to sent: Disable transmission interrupt(s) */
            if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
            {
                /* Disable TX Fifo Interrupt */
                pstcMfs->FCR1_f.FTIE = FALSE;
            }
            else
            {
                /* Disable TX Interrupt */
                pstcMfs->SCR_f.TIE = FALSE; 
            }

            if (0u == pstcBuffer->u16TxCount)
            {
                /* CSIO timer mode */
                if (MfsHlModeCsioNormalMasterTimer == pstcMfsHlInternData->u8MfsMode)
                {
                    /* Wait TX bus idle */
                    pstcMfsHlInternData->u8Exec = MfsHlExecWaitTxBusIdle;
                    /* Enable TX Idle Interrupt */
                    pstcMfs->SCR_f.TBIE = TRUE;
                }
                else
                {
                    if (NULL != pstcMfsHlInternData->pfnTransmitCbFunction)
                    {
                        /* Call transmit callback function. */
                        pstcMfsHlInternData->pfnTransmitCbFunction(pstcBuffer->u16TxCounter);
                    }
                    /* Transmit is standby */
                    pstcMfsHlInternData->u8Exec = MfsHlExecStby;
                }
            }
        }
    }

} /* MfsHlIrqHandlerTx */

/**
 ******************************************************************************
 ** \brief Store received data into buffer.
 **
 ** It stores the data from the FIFO into the internal Ring Buffer
 ** If the referenced MFS does not have a FIFO single data is read.
 **
 ** \param [in]       pstcMfs              Pointer to MFS instance register area
 ** \param [in,out]   pstcMfsHlInternData  Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlReadBuf(volatile stc_mfsn_t*      pstcMfs,
                         stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                        )
{
    stc_mfs_hl_buffer_t* pstcRxBuffer;
    volatile uint32_t    u32Data;
    uint8_t              u8Cnt;
    uint8_t              u8Shift;

    /* Pointer to the receive buffer   */
    pstcRxBuffer = &pstcMfsHlInternData->stcRxBuffer;

    if (MfsEightBits >= pstcMfsHlInternData->u8DataWidth)
    {
        u8Cnt = MFS_HL_NUM_1;
    }
    else if (MfsSixteenBits >= pstcMfsHlInternData->u8DataWidth)
    {
        u8Cnt = MFS_HL_NUM_2;
    }
    else if (MfsTwentyFourBits >= pstcMfsHlInternData->u8DataWidth)
    {
        u8Cnt = MFS_HL_NUM_3;
    }
    else
    {
        u8Cnt = MFS_HL_NUM_4;
    }
    u8Shift = 0u;
    if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
    {
        /* Now read all available bytes from hardware FIFO (if available) to buffer */
        while (0u != pstcMfs->FBYTE2)
        {
            if (MfsSixteenBits >= pstcMfsHlInternData->u8DataWidth)
            {
                u32Data = (uint32_t)pstcMfs->RDR;
            }
            else
            {
                u32Data = pstcMfs->RDR32;
            }
            /* If there is empty space in RX buffer */
            if (pstcRxBuffer->u16FillCount < pstcRxBuffer->u16BufferSize)
            {
                while ((pstcRxBuffer->u16FillCount < pstcRxBuffer->u16BufferSize) && (0u != u8Cnt))
                {
                    u8Cnt--;
                    u32Data = u32Data >> (u8Shift * MFS_HL_NUM_8);
                    u8Shift++;
                    /* Read data from RX FIFO, store in RX buffer */
                    pstcRxBuffer->pu8Buffer[pstcRxBuffer->u16InIndex] = (uint8_t)u32Data;
                    /* Increment in index */
                    pstcRxBuffer->u16InIndex++;
                    if (pstcRxBuffer->u16InIndex == pstcRxBuffer->u16BufferSize)
                    {
                        pstcRxBuffer->u16InIndex = 0u;
                    }
                    /* Count bytes in RX-FIFO */
                    pstcRxBuffer->u16FillCount++;
                }
            }
            else
            {
                /* We still need to do a dummy read to clear pending interrupts */
                pstcRxBuffer->bOverflow = TRUE;
            }
        }
    }
    else
    {
        if (MfsSixteenBits >= pstcMfsHlInternData->u8DataWidth)
        {
            u32Data = (uint32_t)pstcMfs->RDR;
        }
        else
        {
            u32Data = pstcMfs->RDR32;
        }
        /* If there is empty space in RX buffer */
        if (pstcRxBuffer->u16FillCount < pstcRxBuffer->u16BufferSize)
        {
            while ((pstcRxBuffer->u16FillCount < pstcRxBuffer->u16BufferSize) && (0u != u8Cnt))
            {
                u8Cnt--;
                u32Data = u32Data >> (u8Shift * MFS_HL_NUM_8);
                u8Shift++;
                /* Read data from RX FIFO, store in RX buffer */
                pstcRxBuffer->pu8Buffer[pstcRxBuffer->u16InIndex] = (uint8_t)u32Data;
                /* Increment in index */
                pstcRxBuffer->u16InIndex++;
                if (pstcRxBuffer->u16InIndex == pstcRxBuffer->u16BufferSize)
                {
                    pstcRxBuffer->u16InIndex = 0u;
                }
                /* Count bytes in RX-FIFO */
                pstcRxBuffer->u16FillCount++;
            }
        }
        else
        {
            /* We still need to do a dummy read to clear pending interrupts */
            pstcRxBuffer->bOverflow = TRUE;
        }
    }
} /* MfsHlReadBuf */

/**
 ******************************************************************************
 ** \brief MFS_HL receive interrupt service routine.
 **
 ** This Function is called on each Receive Interrupt set by the MFS. It stores
 ** the data from the FIFO into the internal Ring Buffer an triggers a callback
 ** function if the specific Fill Level of the internal Ring Buffer is reached.
 ** If the referenced MFS does not have a FIFO single data is read.
 **
 ** In LIN mode this ISR also handles the LIN Break detection.
 **
 ******************************************************************************/
static void MfsHlIrqHandlerRx(volatile stc_mfsn_t* pstcMfs, 
                              void*                pvHandle
                             )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    stc_mfs_hl_buffer_t*      pstcRxBuffer;
    volatile uint8_t          u8Ssr;

    pstcMfsHlInternData = (stc_mfs_hl_intern_data_t *)pvHandle;

    u8Ssr = pstcMfs->SSR;
    /* Error is different each mode */
    switch (pstcMfsHlInternData->u8MfsMode & MFS_HL_MODE_GROUP_CHK)
    {
        /* UART */
        case MFS_HL_MODE_GROUP_UART:
            /* Check Overrun/Framing/Parity error */
            u8Ssr &= MFS_UART_SSR_ERR;
            break;
        /* CSIO */
        case MFS_HL_MODE_GROUP_CSIO:
            /* Check Overrun error */
            u8Ssr &= MFS_CSIO_SSR_ERR;
            break;
        /* LIN */
        case MFS_HL_MODE_GROUP_LIN:
            /* Check Overrun/Framing error */
            u8Ssr &= MFS_LIN_SSR_ERR;
            break;
        default:
            u8Ssr = 0u;
            break;
    }
    /* Error */
    if(0u != u8Ssr)
    {
        /* Clear possible reception errors */
        pstcMfs->SSR_f.REC = TRUE;
    }

    /* Store Received Data Register into buffer */
    MfsHlReadBuf(pstcMfs, pstcMfsHlInternData);
    /* Check if receive callback function is valid. */
    if (NULL != pstcMfsHlInternData->pfnReceiveCbFunction)
    {
        /* Pointer to the receive buffer */
        pstcRxBuffer = &pstcMfsHlInternData->stcRxBuffer;
        if (pstcRxBuffer->u16FillCount >= pstcMfsHlInternData->u16RxCallbackBufFillLevel)
        {
            /* Call receive callback function. */
            pstcMfsHlInternData->pfnReceiveCbFunction(pstcRxBuffer->u16FillCount);
        }
    }
} /* MfsHlIrqHandlerRx */

/**
 ******************************************************************************
 ** \brief MFS_HL status interrupt service routine.
 **
 **
 ******************************************************************************/
static void MfsHlIrqHandlerSts(volatile stc_mfsn_t* pstcMfs, 
                               void*                pvHandle
                              )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    volatile uint16_t         u16DummyData;

    pstcMfsHlInternData = (stc_mfs_hl_intern_data_t *)pvHandle;

    /* LIN mode */
    if ((pstcMfs->SMR & MFS_SMR_MODEBITS_MASK) == MFS_SMR_LIN_MODE)
    {
        /* LIN break detected? */
        if (pstcMfs->SSR_f.LBD == 1)
        {
            u16DummyData = pstcMfs->RDR;
            pstcMfs->SSR_f.LBD = FALSE; /* Clear LIN break detection */
            pstcMfs->SSR_f.REC = TRUE;  /* Clear possible reception errors */
            
            /* Break Callback if specified */
            if (NULL != pstcMfsHlInternData->pfnLinBreakCbFunction)
            {
                pstcMfsHlInternData->pfnLinBreakCbFunction();
            }
        }
    }

} /* MfsHlIrqHandlerSts */

/**
 ******************************************************************************
 ** \brief MFS_HL Initialize FIFO to use
 **
 ** \param [in] pstcMfs              Pointer to MFS instance register area
 ** \param [in] u16BufFillLvl        Filled count to interrupt
 **
 ******************************************************************************/
static void MfsHlFifoUseInit(volatile stc_mfsn_t* pstcMfs,
                             uint16_t             u16BufFillLvl
                            )
{
    stc_mfs_fcr0_field_t stcFCR0 = { 0 }; /* Preset to zero */
    stc_mfs_fcr1_field_t stcFCR1 = { 0 };

    /*
     *  Local Fifo Control Register 0 variable
     */

    stcFCR0.FLD  = FALSE;    /* No reload (for transmission)   */
    stcFCR0.FSET = FALSE;    /* FIFO2 read pointer not saved    */
    stcFCR0.FCL2 = TRUE;     /* Reset FIFO2 */
    stcFCR0.FCL1 = TRUE;     /* Reset FIFO1 */
    stcFCR0.FE2  = TRUE;     /* Enable FIFO2 */
    stcFCR0.FE1  = TRUE;     /* Enable FIFO1 */

    /*
     *  Local Fifo Control Register 1 variable
     */

    /* No FIFO test (also not used for I2C) */
    stcFCR1.FTST0 = FALSE;
    stcFCR1.FTST1 = FALSE;

    stcFCR1.FLSTE = FALSE;   /* disable data lost detection */
    stcFCR1.FRIIE = FALSE;   /* disable FIFO idle detection */
    stcFCR1.FDRQ  = FALSE;   /* no request for transmission FIFO data */
    stcFCR1.FTIE  = FALSE;   /* disable transmission FIFO interrupt */
    stcFCR1.FSEL  = FALSE;   /* FIFO1: transmission FIFO, FIFO2: reception FIFO */

    /* Setup hardware */
    pstcMfs->FCR0_f  = stcFCR0;
    pstcMfs->FCR1_f  = stcFCR1;
    pstcMfs->FBYTE2  = (uint8_t)MfsHlGetMin(u16BufFillLvl, MFS_FIFO_MAX_VAL);
} /* MfsHlFifoUseInit */

/**
 ******************************************************************************
 ** \brief MFS_HL Operation to send data for I2C master
 **
 ** \param [in] pstcI2c              Pointer to I2C (MFS) instance register area
 ** \param [in] pstcMfsHlInternData  Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlI2cDataTxMaster(volatile stc_mfsn_t*      pstcI2c,
                                 stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                                )
{
    stc_mfs_hl_buffer_t*     pstcBuffer;
    uint16_t                 u16Num;
    uint16_t                 u16Cnt;
    stc_mfs_smr_field_t      stcSMR;
    stc_mfs_fcr1_field_t     stcFCR1 = { 0 };
    stc_mfs_ssr_field_t      stcSSR; 
    stc_mfs_i2c_ibsr_field_t stcIBSR;
    stc_mfs_i2c_ibcr_field_t stcIBCR;
    stc_mfs_i2c_ismk_field_t stcISMK;

    stcIBSR = pstcI2c->IBSR_f;
    stcIBCR = pstcI2c->IBCR_f;

    /* Get ptr to internal transmit Buffer */
    pstcBuffer = &pstcMfsHlInternData->stcTxBuffer;

    /* stop condition */
    if (TRUE == stcIBSR.SPC)
    {
        /* Clear stop condition interrupt */
        stcIBSR.SPC = FALSE;
        pstcI2c->IBSR_f = stcIBSR;

        /* stop condition interrupt disable */
        stcIBCR.CNDE = FALSE;
        stcIBCR.ACT_SCC = FALSE;
        pstcI2c->IBCR_f = stcIBCR;

        /* Clear IBSR:RACK */
        stcISMK = pstcI2c->ISMK_f;
        stcISMK.EN       = FALSE;
        pstcI2c->ISMK_f = stcISMK;

        /* Restart */
        stcISMK.EN       = TRUE;
        pstcI2c->ISMK_f = stcISMK;

        if (NULL != pstcMfsHlInternData->pfnTransmitCbFunction)
        {
            /* Call transmit callback function. */
            pstcMfsHlInternData->pfnTransmitCbFunction(pstcBuffer->u16OutIndex);
        }
        /* Transmit is standby */
        pstcMfsHlInternData->u8Exec = MfsHlExecStby;
    }
    else if (((pstcBuffer->u16BufferSize == pstcBuffer->u16OutIndex)
    &&        (TRUE == stcIBCR.INT))
    ||        (TRUE == stcIBSR.RACK))
    {
        /* Disable master mode */
        stcIBCR.MSS  = FALSE;
        stcIBCR.ACT_SCC = FALSE;
        /* Disable interrupt */
        stcIBCR.INTE = FALSE;
        /* Clear interrupt */
        stcIBCR.INT  = FALSE;
        /* Enable stop condition interrupt */
        stcIBCR.CNDE = TRUE;
        pstcI2c->IBCR_f = stcIBCR;
    }
    else
    {
        stcSSR = pstcI2c->SSR_f;
        if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
        {
            stcFCR1 = pstcI2c->FCR1_f;
        }
        if ((TRUE == stcSSR.TDRE) || (TRUE == stcFCR1.FDRQ))
        {
            /* FIFO is used */
            if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
            {
                u16Num = MfsHlGetMin(((pstcBuffer->u16BufferSize) - (pstcBuffer->u16OutIndex)),
                                      (uint16_t)(MFS_FIFO_MAX_VAL - (pstcI2c->FBYTE1))
                                    );
                for (u16Cnt = 0u; u16Cnt < u16Num; u16Cnt++)
                {
                    pstcI2c->TDR = pstcBuffer->pu8Buffer[pstcBuffer->u16OutIndex];
                    pstcBuffer->u16OutIndex += MFS_HL_NUM_1;
                }
                /* clear FIFO tx interrupt */
                stcFCR1 = pstcI2c->FCR1_f;
                stcFCR1.FDRQ = FALSE;
                if (pstcBuffer->u16BufferSize == pstcBuffer->u16OutIndex)
                {
                    /* tx FIFO interrupt disable */
                    stcFCR1.FTIE = FALSE;
                }
                pstcI2c->FCR1_f = stcFCR1;
            }
            else
            {
                /* tx data to register */
                pstcI2c->TDR = pstcBuffer->pu8Buffer[pstcBuffer->u16OutIndex];
                pstcBuffer->u16OutIndex += MFS_HL_NUM_1;
                if (pstcBuffer->u16BufferSize == pstcBuffer->u16OutIndex)
                {
                    /* tx interrupt disable */
                    stcSMR = pstcI2c->SMR_f;
                    stcSMR.TIE = FALSE;
                    pstcI2c->SMR_f = stcSMR;
                }
            }
        }

        /* clear interrupt */
        stcIBCR.ACT_SCC = FALSE;
        stcIBCR.INT = FALSE;
        pstcI2c->IBCR_f = stcIBCR;
    }
} /* MfsHlI2cDataTxMaster */

/**
 ******************************************************************************
 ** \brief MFS_HL Operation to receive data for I2C master
 **
 ** \param [in] pstcI2c              Pointer to I2C (MFS) instance register area
 ** \param [in] pstcMfsHlInternData  Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlI2cDataRxMaster(volatile stc_mfsn_t*      pstcI2c,
                                 stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                                )
{
    stc_mfs_hl_buffer_t*     pstcBuffer;
    stc_mfs_ssr_field_t      stcSSR; 
    stc_mfs_i2c_ibsr_field_t stcIBSR;
    stc_mfs_i2c_ibcr_field_t stcIBCR;
    stc_mfs_i2c_ismk_field_t stcISMK;

    stcIBSR = pstcI2c->IBSR_f;
    stcIBCR = pstcI2c->IBCR_f;
    stcSSR  = pstcI2c->SSR_f;

    /* Get ptr to internal transmit Buffer */
    pstcBuffer = &pstcMfsHlInternData->stcRxBuffer;

    /* stop condition */
    if (TRUE == stcIBSR.SPC)
    {
        /* Clear stop condition interrupt */
        stcIBSR.SPC = FALSE;
        pstcI2c->IBSR_f = stcIBSR;

        /* Disable stop condition interrupt */
        stcIBCR.CNDE = FALSE;
        /* Disable interrupt */
        stcIBCR.INTE = FALSE;

        /* Clear IBSR:RACK */
        stcISMK = pstcI2c->ISMK_f;
        stcISMK.EN       = FALSE;
        pstcI2c->ISMK_f = stcISMK;

        /* Restart */
        stcISMK.EN       = TRUE;
        pstcI2c->ISMK_f = stcISMK;

        if (NULL != pstcMfsHlInternData->pfnReceiveCbFunction)
        {
            /* Call receive callback function. */
            pstcMfsHlInternData->pfnReceiveCbFunction(pstcBuffer->u16InIndex);
        }
        /* Receiving is standby */
        pstcMfsHlInternData->u8Exec = MfsHlExecStby;
    }
    /* Received data after second byte */
    else if ((TRUE == stcSSR.RDRF) && (FALSE == stcIBSR.FBT))
    {
        /* Continue until specified data length is received */
        while (pstcBuffer->u16InIndex < pstcBuffer->u16BufferSize)
        {
            stcSSR = pstcI2c->SSR_f;
            if (TRUE == stcSSR.RDRF)
            {
                pstcBuffer->pu8Buffer[pstcBuffer->u16InIndex] = (uint8_t)pstcI2c->RDR;
                pstcBuffer->u16InIndex += MFS_HL_NUM_1;
            }
            else
            {
                /* No data */
                break;
            }
        }
        /* Complete to receive */
        if (pstcBuffer->u16InIndex == pstcBuffer->u16BufferSize)
        {
            /* Stop condition */
            stcIBCR.MSS  = FALSE;
            /* NACK */
            stcIBCR.ACKE = FALSE;
            /* stop condition interrupt enable */
            stcIBCR.CNDE = TRUE;

            /* If restart condition was detected */
            if (TRUE == stcIBSR.RSC)
            {
                /* Clear restart condition */
                stcIBSR.RSC = FALSE;
                pstcI2c->IBSR_f = stcIBSR;
            }
        }
    }
    /* Overrun error */
    else if (TRUE == stcSSR.ORE)
    {
        /* Clear RX error */
        stcSSR.REC = TRUE;
        pstcI2c->SSR_f = stcSSR;
    }
    else
    {
        /* There is no process that should be executed. */
    }

    /* Clear interrupt */
    stcIBCR.ACT_SCC = FALSE;
    stcIBCR.INT = FALSE;
    pstcI2c->IBCR_f = stcIBCR;
} /* MfsHlI2cDataRxMaster */

/**
 ******************************************************************************
 ** \brief MFS_HL Start operation for I2C slave
 **
 ** \param [in] pstcI2c              Pointer to I2C (MFS) instance register area
 ** \param [in] pstcMfsHlInternData  Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlI2cPreStartSlave(volatile stc_mfsn_t*      pstcI2c,
                                  stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                                 )
{
    stc_mfs_smr_field_t      stcSMR;
    stc_mfs_i2c_ibcr_field_t stcIBCR;
    stc_mfs_i2c_ibsr_field_t stcIBSR;
    stc_mfs_i2c_ismk_field_t stcISMK;
    uint8_t                  u8Data;

    stcIBCR = pstcI2c->IBCR_f;
    stcIBSR = pstcI2c->IBSR_f;
    /* Chk Slave Address Received */
    if ((FALSE == stcIBCR.MSS) && (TRUE == stcIBCR.ACT_SCC) && (TRUE == stcIBSR.FBT))
    {
        stcSMR = pstcI2c->SMR_f;
        /* Check direction */
        /* TX */
        if (TRUE == stcIBSR.TRX)
        {
            /* Callback for the I2C slave starting(TX) and get 1st data to send */
            if (NULL != pstcMfsHlInternData->pfnI2cSlvStCbFunction)
            {
                u8Data = pstcMfsHlInternData->pfnI2cSlvStCbFunction(MfsI2cWrite);
            }
            else
            {
                u8Data = 0x00u;
            }
            /* Send 1st data */
            pstcI2c->TDR = (uint16_t)u8Data;
            /* Disable TX interrupt */
            stcSMR.TIE = FALSE;
            stcIBCR.WSEL = FALSE;
        }
        /* RX */
        else
        {
            stcIBCR.ACKE = TRUE;
            /* Disable RX interrupt */
            stcSMR.RIE = FALSE;
            stcIBCR.WSEL = TRUE;
            /* Callback for the I2C slave starting(RX) */
            if (NULL != pstcMfsHlInternData->pfnI2cSlvStCbFunction)
            {
                pstcMfsHlInternData->pfnI2cSlvStCbFunction(MfsI2cRead);
            }
        }
        pstcI2c->SMR_f = stcSMR;
    }

    /* Clear interrupt */
    stcIBCR.ACT_SCC = FALSE;
    stcIBCR.INT = FALSE;
    pstcI2c->IBCR_f = stcIBCR;
} /* MfsHlI2cPreStartSlave */

/**
 ******************************************************************************
 ** \brief MFS_HL Operation to receive data for I2C slave
 **
 ** \param [in] pstcI2c              Pointer to I2C (MFS) instance register area
 ** \param [in] pstcMfsHlInternData  Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlI2cDataRxSlave(volatile stc_mfsn_t*      pstcI2c,
                                stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                               )
{
    stc_mfs_hl_buffer_t*     pstcBuffer;
    stc_mfs_ssr_field_t      stcSSR;
    stc_mfs_i2c_ibcr_field_t stcIBCR;
    stc_mfs_i2c_ibsr_field_t stcIBSR;
    stc_mfs_i2c_ismk_field_t stcISMK;

    stcIBSR = pstcI2c->IBSR_f;
    stcSSR  = pstcI2c->SSR_f;

    /* Get ptr to internal transmit Buffer */
    pstcBuffer = &pstcMfsHlInternData->stcRxBuffer;

    /* Stop condition */
    if (TRUE == stcIBSR.SPC)
    {
        /* Clear stop condition interrupt */
        stcIBSR.SPC = FALSE;
        pstcI2c->IBSR_f = stcIBSR;

        /* Clear IBSR:RACK */
        stcISMK = pstcI2c->ISMK_f;
        stcISMK.EN       = FALSE;
        pstcI2c->ISMK_f = stcISMK;

        /* Restart */
        stcISMK.EN       = TRUE;
        pstcI2c->ISMK_f = stcISMK;

        if (NULL != pstcMfsHlInternData->pfnReceiveCbFunction)
        {
            /* Call receive callback function. */
            pstcMfsHlInternData->pfnReceiveCbFunction(pstcBuffer->u16InIndex);
        }
        /* Receiving is standby */
        pstcMfsHlInternData->u8Exec = MfsHlExecStby;
    }
    /* Received NACK */
    else if (TRUE == stcIBSR.RACK)
    {
        /* Clear interrupt */
        stcIBCR         = pstcI2c->IBCR_f;
        stcIBCR.ACT_SCC = FALSE;
        stcIBCR.INT     = FALSE;
        pstcI2c->IBCR_f = stcIBCR;
    }
    /* Received data */
    else
    {
        stcIBCR = pstcI2c->IBCR_f;

        if ((TRUE == stcSSR.RDRF) && (FALSE == stcIBSR.FBT))
        {
            /* Continue until specified data length is received */
            while (pstcBuffer->u16InIndex < pstcBuffer->u16BufferSize)
            {
                stcSSR = pstcI2c->SSR_f;
                if (TRUE == stcSSR.RDRF)
                {
                    pstcBuffer->pu8Buffer[pstcBuffer->u16InIndex] = (uint8_t)pstcI2c->RDR;
                    pstcBuffer->u16InIndex += MFS_HL_NUM_1;
                }
                else
                {
                    /* No data */
                    break;
                }
            }
            /* Complete to receive */
            if (pstcBuffer->u16InIndex == pstcBuffer->u16BufferSize)
            {
                /* Send NACK */
                stcIBCR.ACKE = FALSE;
            }
            else
            {
                /* Send ACK */
                stcIBCR.ACKE = TRUE;
            }
        }
        /* Clear interrupt */
        stcIBCR.ACT_SCC  = FALSE;
        stcIBCR.INT      = FALSE;
        pstcI2c->IBCR_f = stcIBCR;
    }
    /* Overrun error */
    if (TRUE == stcSSR.ORE)
    {
        /* Clear RX error */
        stcSSR.REC = TRUE;
        pstcI2c->SSR_f = stcSSR;
    }
} /* MfsHlI2cDataRxSlave */

/**
 ******************************************************************************
 ** \brief MFS_HL Operation to send data for I2C slave
 **
 ** \param [in] pstcI2c              Pointer to I2C (MFS) instance register area
 ** \param [in] pstcMfsHlInternData  Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlI2cDataTxSlave(volatile stc_mfsn_t*      pstcI2c,
                                stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                               )
{
    stc_mfs_hl_buffer_t*     pstcBuffer;
    stc_mfs_smr_field_t      stcSMR;
    stc_mfs_fcr1_field_t     stcFCR1 = { 0 };
    stc_mfs_ssr_field_t      stcSSR;
    stc_mfs_i2c_ibcr_field_t stcIBCR;
    stc_mfs_i2c_ibsr_field_t stcIBSR;
    stc_mfs_i2c_ismk_field_t stcISMK;
    uint16_t                 u16Num;
    uint16_t                 u16Cnt;

    stcIBSR = pstcI2c->IBSR_f;

    /* Get ptr to internal transmit Buffer */
    pstcBuffer = &pstcMfsHlInternData->stcTxBuffer;

    /* end of data or */
    if ((pstcBuffer->u16BufferSize == pstcBuffer->u16OutIndex)
    /* Stop condition */
    ||  (TRUE == stcIBSR.SPC))
    {
        /* FIFO is used */
        if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
        {
            /* Disable TX FIFO interrupt */
            stcFCR1          = pstcI2c->FCR1_f;
            stcFCR1.FTIE     = FALSE;
            pstcI2c->FCR1_f = stcFCR1;
        }
        /* FIFO is not used */
        else
        {
            /* Disable TX interrupt */
            stcSMR = pstcI2c->SMR_f;
            stcSMR.TIE = FALSE;
            pstcI2c->SMR_f = stcSMR;
        }
        /* Stop condition */
        if (TRUE == stcIBSR.SPC)
        {
            /* Clear IBSR:RACK */
            stcISMK = pstcI2c->ISMK_f;
            stcISMK.EN       = FALSE;
            pstcI2c->ISMK_f = stcISMK;

            /* Restart */
            stcISMK.EN       = TRUE;
            pstcI2c->ISMK_f = stcISMK;

            /* Clear stop condition interrupt */
            stcIBSR.SPC = FALSE;
            pstcI2c->IBSR_f = stcIBSR;

            if (NULL != pstcMfsHlInternData->pfnTransmitCbFunction)
            {
                /* Call transmit callback function. */
                pstcMfsHlInternData->pfnTransmitCbFunction(pstcBuffer->u16OutIndex);
            }
            /* Transmit is standby */
            pstcMfsHlInternData->u8Exec = MfsHlExecStby;
        }

        /* Clear interrupt */
        stcIBCR          = pstcI2c->IBCR_f;
        stcIBCR.ACT_SCC  = FALSE;
        stcIBCR.INT      = FALSE;
        pstcI2c->IBCR_f = stcIBCR;

        /* Clear RSC interrupt */
        stcIBSR.RSC = FALSE;
        pstcI2c->IBSR_f = stcIBSR;
    }
    else
    {
        stcSSR = pstcI2c->SSR_f;
        /* FIFO is used */
        if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
        {
            stcFCR1 = pstcI2c->FCR1_f;
        }
        if ((TRUE == stcSSR.TDRE) || (TRUE == stcFCR1.FDRQ))
        {
            /* FIFO is used */
            if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
            {
                u16Num = MfsHlGetMin(((pstcBuffer->u16BufferSize) - (pstcBuffer->u16OutIndex)),
                                      (uint16_t)(MFS_FIFO_MAX_VAL - (pstcI2c->FBYTE1))
                                    );
                for (u16Cnt = 0u; u16Cnt < u16Num; u16Cnt++)
                {
                    /* Set data to TX FIFO */
                    pstcI2c->TDR = pstcBuffer->pu8Buffer[pstcBuffer->u16OutIndex];
                    pstcBuffer->u16OutIndex++;
                }
                /* clear FIFO tx interrupt */
                stcFCR1 = pstcI2c->FCR1_f;
                stcFCR1.FDRQ = FALSE;
                pstcI2c->FCR1_f = stcFCR1;
            }
            /* FIFO is not used */
            else
            {
                /* Set data to TX FIFO */
                pstcI2c->TDR = pstcBuffer->pu8Buffer[pstcBuffer->u16OutIndex];
                pstcBuffer->u16OutIndex++;
            }
        }
        /* Clear interrupt */
        stcIBCR          = pstcI2c->IBCR_f;
        stcIBCR.ACT_SCC  = FALSE;
        stcIBCR.INT      = FALSE;
        pstcI2c->IBCR_f = stcIBCR;
        /* Received NACK */
        if (TRUE == stcIBSR.RACK)
        {
            /* Clear RSC interrupt */
            stcIBSR.RSC = FALSE;
            pstcI2c->IBSR_f = stcIBSR;
        }
    }
} /* MfsHlI2cDataTxSlave */

/**
 ******************************************************************************
 ** \brief MFS_HL transmit interrupt service routine for I2C.
 **
 ** This Function is called on Transmit Interrupt set by the I2C.
 **
 ** \param [in]  pstcMfs       Pointer to I2C (MFS) instance register area
 ** \param [in]  pvHandle      Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlI2cIrqHandlerTx(volatile stc_mfsn_t* pstcMfs,
                                 void*                pvHandle
                                )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;

    pstcMfsHlInternData = (stc_mfs_hl_intern_data_t *)pvHandle;

    /* I2C Master mode */
    if (MfsHlModeI2cMaster == pstcMfsHlInternData->u8MfsMode)
    {
        MfsHlI2cDataTxMaster(pstcMfs, pstcMfsHlInternData);
    }
    /* I2C Slave mode */
    else
    {
        MfsHlI2cDataTxSlave(pstcMfs, pstcMfsHlInternData);
    }
} /* MfsHlI2cIrqHandlerTx */

/**
 ******************************************************************************
 ** \brief MFS_HL receive interrupt service routine for I2C.
 **
 ** This Function is called on Reception Interrupt set by the I2C.
 **
 ** \param [in]  pstcMfs       Pointer to I2C (MFS) instance register area
 ** \param [in]  pvHandle      Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlI2cIrqHandlerRx(volatile stc_mfsn_t* pstcMfs,
                                 void*                pvHandle
                                )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;

    pstcMfsHlInternData = (stc_mfs_hl_intern_data_t *)pvHandle;

    /* I2C Master mode */
    if (MfsHlModeI2cMaster == pstcMfsHlInternData->u8MfsMode)
    {
        MfsHlI2cDataRxMaster(pstcMfs, pstcMfsHlInternData);
    }
    /* I2C Slave mode */
    else
    {
        /* If status is standby... */
        if (MfsHlExecStby == pstcMfsHlInternData->u8Exec)
        {
            MfsHlI2cPreStartSlave(pstcMfs, pstcMfsHlInternData);
        }
        else
        {
            MfsHlI2cDataRxSlave(pstcMfs, pstcMfsHlInternData);
        }
    }
} /* MfsHlI2cIrqHandlerRx */

/**
 ******************************************************************************
 ** \brief MFS_HL status interrupt service routine for I2C.
 **
 ** This Function is called on Status Interrupt set by the I2C.
 **
 ** \param [in]  pstcMfs       Pointer to I2C (MFS) instance register area
 ** \param [in]  pvHandle      Pointer to internal data
 **
 ******************************************************************************/
static void MfsHlI2cIrqHandlerSts(volatile stc_mfsn_t* pstcMfs, 
                                  void*                pvHandle
                                 )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;

    pstcMfsHlInternData = (stc_mfs_hl_intern_data_t *)pvHandle;

    /* I2C Master mode */
    if (MfsHlModeI2cMaster == pstcMfsHlInternData->u8MfsMode)
    {
        /* Tramsmission */
        if (MfsHlExecTransmitting == pstcMfsHlInternData->u8Exec)
        {
            MfsHlI2cDataTxMaster(pstcMfs, pstcMfsHlInternData);
        }
        /* Reception */
        else
        {
            MfsHlI2cDataRxMaster(pstcMfs, pstcMfsHlInternData);
        }
    }
    /* I2C Slave mode */
    else
    {
        switch (pstcMfsHlInternData->u8Exec)
        {
            /* Standby */
            case MfsHlExecStby:
                MfsHlI2cPreStartSlave(pstcMfs, pstcMfsHlInternData);
                break;
            /* Receiving (after slave address was received) */
            case MfsHlExecReceiving:
                MfsHlI2cDataRxSlave(pstcMfs, pstcMfsHlInternData);
                break;
            default:
                MfsHlI2cDataTxSlave(pstcMfs, pstcMfsHlInternData);
                break;
        }
    }
} /* MfsHlI2cIrqHandlerSts */

/**
 ******************************************************************************
 ** \brief Initialize an MFS_HL module as UART mode.
 ** This Function initialises the MFS according the UART setup in the passed
 ** configuration structure.
 ** Several checking are done before that and an error is returned if invalid
 ** Modes are requested.
 **
 ** \param [in]  pstcUart      Pointer to UART (MFS) instance register area
 ** \param [in]  pstcConfig    MFS UART configuration 
 **
 ** \retval Ok                    Initializiation of MFS_HL module successfully 
 **                               done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - pstcUart == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **             - one or more values in pstcConfig out of range
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Uart_Init(volatile stc_mfsn_t*      pstcUart,
                             stc_mfs_hl_uart_config_t* pstcConfig
                            )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    uint32_t                  u32ReloadValue;

    /* Preset local register variables to zero */
    stc_mfs_smr_field_t  stcSMR  = { 0 };
    stc_mfs_scr_field_t  stcSCR  = { 0 };
    stc_mfs_escr_field_t stcESCR = { 0 };

    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcUart);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    ||  (NULL == pstcConfig)
       )
    {
        return (ErrorInvalidParameter);
    }

    /* RX Callback Level must be set more than zero */
    if (1u > pstcConfig->u16RxCbBufFillLvl)
    {
        return (ErrorInvalidParameter);
    }

    /* Get reload value */
    u32ReloadValue = Mfs_GetReloadValue(pstcConfig->u32DataRate);
    /* For asynchronous communication, the reload value must be greater than */
    /* or equal to 4 because five times over-sampling is performed internally */
    if (MFS_HL_NUM_4 > u32ReloadValue)
    {
        return (ErrorInvalidParameter);
    }

    /* Check buffer for UART */
    if (((NULL == pstcConfig->pu8TxBuf)
    ||   (0u == pstcConfig->u16TxBufSize))
    ||  ((NULL == pstcConfig->pu8RxBuf)
    ||   (0u == pstcConfig->u16RxBufSize))
       )
    {
        return (ErrorInvalidParameter);
    }

    /* First of all set MFS to Asynchronous mode 0 */
    pstcUart->SMR = 0u;

    /* Then we disable TX and RX for safe operation */
    pstcUart->SCR = 0u;

    /* Clear MFS by setting the Software Reset bit */
    pstcUart->SCR_f.UPCL = TRUE;

    /* Check the configration of FIFO usage */
    switch (pstcConfig->u8FifoUsage)
    {
        case MfsHlUseNoFifo:
        case MfsHlUseFifo:
            /* Set FIFO usage */
            pstcMfsHlInternData->u8FifoUsage = pstcConfig->u8FifoUsage;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* ************ Register Settings ************ */
    /*
     * Local Serial Mode Register variable
     */

    /* Operation Mode Selection */
    switch (pstcConfig->u8UartMode)
    {
        /* Normal mode */
        case MfsUartNormal:
            stcSMR.MD = MFS_MD_UART_NORNAL;
            break;
        /* Multi-Processor mode */
        case MfsUartMulti:
            stcSMR.MD = MFS_MD_UART_MULTI;
            break;
        default:
            return (ErrorInvalidParameter); /* should never see the daylight */
    }

    /* Bit direction */
    if (TRUE == pstcConfig->bBitDirection)
    {
        stcSMR.BDS = TRUE;
    }
    else
    {
        stcSMR.BDS = FALSE;
    }

    /* now setup hardware with correct mode first and then go on with */
    /*   bit settings */
    pstcUart->SMR_f = stcSMR;

    /* Stop bit length 1 */
    switch (pstcConfig->u8StopBit)
    {
        case MfsOneStopBit:
        case MfsThreeStopBits:
            stcSMR.SBL = FALSE;
            break;
        case MfsTwoStopBits:
        case MfsFourStopBits:
            stcSMR.SBL = TRUE;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Enable serial output */
    stcSMR.SOE = TRUE;

    /* now setup hardware (asynchronous) */
    pstcUart->SMR_f = stcSMR;

    /*
     * Local Serial/I2C Control Register variable
     */
    /* now setup hardware */
    pstcUart->SCR_f = stcSCR;

    /*
     * Local Extended Serial Control Register variable
     */
    /* Setup Parity 1 (Only in Normal mode available) */
    if (MfsUartNormal == pstcConfig->u8UartMode)
    {
        switch (pstcConfig->u8Parity)
        {
            case MfsParityNone:
                stcESCR.P   = FALSE; /* Parity type selection (0-even/1-odd) */
                stcESCR.PEN = FALSE; /* Parity disable */
                break;
            case MfsParityEven:
                stcESCR.P   = FALSE; /* Parity type selection (0-even/1-odd) */
                stcESCR.PEN = TRUE;  /* Parity Enable */
                break;
            case MfsParityOdd:
                stcESCR.P   = TRUE;  /* Parity type selection (0-even/1-odd) */
                stcESCR.PEN = TRUE;  /* Parity Enable */
                break;
            default:
                return (ErrorInvalidParameter); /* should never see the daylight */
        }
    } /* if (pstcConfig->enUartMode == MfsAsynchronous0) */

    /* Stop bits */
    switch (pstcConfig->u8StopBit)
    {
        case MfsOneStopBit:
        case MfsTwoStopBits:
            stcESCR.ESBL = FALSE;
            break;
        case MfsThreeStopBits:
        case MfsFourStopBits:
            stcESCR.ESBL = TRUE;
            break;
        default:
            return (ErrorInvalidParameter);
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

    /* NRZ / inverted NRZI */
    if (TRUE == pstcConfig->bSignalSystem)
    {
        stcESCR.INV = TRUE;
    }
    else
    {
        stcESCR.INV = FALSE;
    }

    /* Caracter Length */
    switch (pstcConfig->u8CharLength)
    {
        case MfsFiveBits:
            stcESCR.L = MFS_DATA_LEN_5;
            break;
        case MfsSixBits:
            stcESCR.L = MFS_DATA_LEN_6;
            break;
        case MfsSevenBits:
            stcESCR.L = MFS_DATA_LEN_7;
            break;   
        case MfsEightBits:
            stcESCR.L = MFS_DATA_LEN_8;
            break;
        case MfsNineBits:
            stcESCR.L = MFS_DATA_LEN_9;
            break;
        default:
            return (ErrorInvalidParameter); /* should never see the daylight */
    }

    /* now setup hardware */
    pstcUart->ESCR_f = stcESCR;

    /*
     * Local Serial Status Register variable (USART)
     */
    /* Clear possible reception errors */
    pstcUart->SSR_f.REC = TRUE;

    /*
     *  Local Fifo Control Register 0 variable
     */
    if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
    {
        /* Initialize FIFO to use */
        MfsHlFifoUseInit(pstcUart, pstcConfig->u16RxCbBufFillLvl);
    }

    /* Save RX Callback Buffer Fill Level (needed during servicing RX INT) */
    pstcMfsHlInternData->u16RxCallbackBufFillLevel = pstcConfig->u16RxCbBufFillLvl;

    /* Save character length */
    pstcMfsHlInternData->u8DataWidth = pstcConfig->u8CharLength;

    /* Set baud rate generation reload register */
    pstcUart->BGR = (uint16_t)(u32ReloadValue & MFS_HL_BGR_MASK);

    /* Set callback functions. */
    pstcMfsHlInternData->pfnTransmitCbFunction = pstcConfig->pfnTxCb;
    pstcMfsHlInternData->pfnReceiveCbFunction  = pstcConfig->pfnRxCb;

    /* Initialise TX ring buffer */
    pstcMfsHlInternData->stcTxBuffer.pu8Buffer     = pstcConfig->pu8TxBuf;
    pstcMfsHlInternData->stcTxBuffer.u16BufferSize = pstcConfig->u16TxBufSize;
    pstcMfsHlInternData->stcTxBuffer.u16InIndex    = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcTxBuffer.u16OutIndex   = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcTxBuffer.u16FillCount  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.u16TxCounter  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.u16TxCount    = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.bOverflow     = 0u; /*  Reset Flag */

    /* Initialise RX ring buffer */
    pstcMfsHlInternData->stcRxBuffer.pu8Buffer     = pstcConfig->pu8RxBuf;
    pstcMfsHlInternData->stcRxBuffer.u16BufferSize = pstcConfig->u16RxBufSize;
    pstcMfsHlInternData->stcRxBuffer.u16InIndex    = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcRxBuffer.u16OutIndex   = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcRxBuffer.u16FillCount  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcRxBuffer.bOverflow     = 0u; /*  Reset Flag */

    /* MFS is standby */
    pstcMfsHlInternData->u8Exec = MfsHlExecStby;

    /* Operation Mode Selection */
    if (MfsUartNormal == pstcConfig->u8UartMode)
    {
        /* Normal mode */
        pstcMfsHlInternData->u8MfsMode = MfsHlModeUartNormal;
    }
    else
    {
        /* Multi-Processor mode */
        pstcMfsHlInternData->u8MfsMode = MfsHlModeUartMulti;
    }

    /* Register interrupt handler and internal handle */
    (void)Mfs_SetTxIntCallBack(pstcUart, MfsHlIrqHandlerTx);
    (void)Mfs_SetRxIntCallBack(pstcUart, MfsHlIrqHandlerRx);
    (void)Mfs_SetUpperLayerHandle(pstcUart, pstcMfsHlInternData);

    /* Enable receiver, transmitter */
    pstcUart->SCR_f.TXE = TRUE;
    pstcUart->SCR_f.RXE = TRUE;

    /* Use interruption */
    if (NULL != pstcConfig->pfnRxCb)
    {
        /* Enable reception interrupt */
        pstcUart->SCR_f.RIE = TRUE;
    }

    /* Init transmission interrupt */
    (void)Mfs_InitTxIrq(pstcUart);
    /* Init reception interrupt */
    (void)Mfs_InitRxIrq(pstcUart);

    return (Ok);
} /* Mfs_Hl_Uart_Init */

/**
 ******************************************************************************
 ** \brief UART deinitialisation of a MFS_HL module.
 **
 ** This function de-initializes the MFS_HL module activating as UART.
 ** This function uses for mode change or other changed settings.
 **  
 ** \param [in]   pstcUart      Pointer to UART (MFS) instance register area
 ** 
 ** \retval Ok                    Deinitialisation of MFS_HL module successfully
 **                               done
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - pstcUart == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** 
 ******************************************************************************/
en_result_t Mfs_Hl_Uart_DeInit(volatile stc_mfsn_t* pstcUart)
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcUart);
    /* ... and check */
    if (NULL == pstcMfsHlInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        /* First of all disable receiver, transmitter and deinit interrupts */
        pstcUart->SCR_f.TXE = FALSE;
        pstcUart->SCR_f.RXE = FALSE;

        (void)Mfs_DeInitIrq(pstcUart);     /* returns always en_result_t Ok */

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
        pstcUart->FCR0 = 0u;
        pstcUart->FCR1 = 0u;

        /* Clear MFS by setting the Software Reset bit */
        pstcUart->SCR_f.UPCL = TRUE;

        /* Unregist interrupt handler and internal handle */
        (void)Mfs_SetTxIntCallBack(pstcUart, NULL);
        (void)Mfs_SetRxIntCallBack(pstcUart, NULL);
        (void)Mfs_SetStsIntCallBack(pstcUart, NULL);
        (void)Mfs_SetUpperLayerHandle(pstcUart, NULL);

        /* Reset Fifo buffer */
        pstcMfsHlInternData->stcTxBuffer.pu8Buffer  = NULL;
        pstcMfsHlInternData->stcRxBuffer.pu8Buffer  = NULL;

        /* Reset callback functions. */
        pstcMfsHlInternData->pfnTransmitCbFunction = NULL;
        pstcMfsHlInternData->pfnReceiveCbFunction  = NULL;

        /* Reset LIN Break Callback used for Mfs_Lin_DeInit() */
        pstcMfsHlInternData->pfnLinBreakCbFunction = NULL;
        pstcMfsHlInternData->u8Exec = MfsHlExecStby;
        pstcMfsHlInternData->u8MfsMode = MfsHlModeStby;

        enResult = Ok;
    }

    return (enResult);
} /* Mfs_Hl_Uart_DeInit */

/**
 ******************************************************************************
 ** \brief Initialize an MFS_HL module as CSIO mode.
 **
 ** This Function initialises the MFS according the CSIO setup in the passed
 ** configuration structure.
 ** Several checking are done before that and an error is returned if invalid
 ** Modes are requested.
 **
 ** \param [in]  pstcCsio      Pointer to CSIO (MFS) instance register area
 ** \param [in]  pstcConfig    MFS CSIO configuration 
 **
 ** \retval Ok                    Initializiation of MFS_HL module successfully 
 **                               done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - pstcCsio == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **             - one or more enumerated values in pstcUart out of enumaration
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Csio_Init(volatile stc_mfsn_t*      pstcCsio,
                             stc_mfs_hl_csio_config_t* pstcConfig
                            )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    uint32_t                  u32ReloadValue;

    /* Preset local register variables to zero */
    stc_mfs_smr_field_t        stcSMR      = { 0 };
    stc_mfs_scr_field_t        stcSCR      = { 0 };
    stc_mfs_escr_field_t       stcESCR     = { 0 };
    stc_mfs_ssr_field_t        stcSSR      = { 0 };
    stc_mfs_csio_sacsr_field_t stcSACSR    = { 0 };
    stc_mfs_csio_scscr_field_t stcSCSCR    = { 0 };
    uint16_t                   u16STMCR    = 0u;
    uint16_t                   u16SCSTR32  = 0u;
    uint8_t                    u8SCSTR0    = 0u;
    uint8_t                    u8SCSTR1    = 0u;
    uint8_t                    u8TBYTE0    = 0u;

    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcCsio);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    ||  (NULL == pstcConfig)
       )
    {
        return (ErrorInvalidParameter);
    }

    /* RX Callback Level must be set somewhat ge zero (except I2C mode) */
    if (1 > pstcConfig->u16RxCbBufFillLvl)
    {
        return (ErrorInvalidParameter);
    }

    /* Check buffer for CSIO */
    if (((NULL == pstcConfig->pu8TxBuf)
    ||   (0u == pstcConfig->u16TxBufSize))
    ||  ((NULL == pstcConfig->pu8RxBuf)
    ||   (0u == pstcConfig->u16RxBufSize))
       )
    {
        return (ErrorInvalidParameter);
    }

    /* Get reload value */
    u32ReloadValue = Mfs_GetReloadValue(pstcConfig->u32DataRate);
    /* For synchronous communication, the reload value must be greater than  */
    /* or equal to 3 because four times over-sampling is performed internally */
    if (MFS_HL_RELOADVAL_MIN4CSIO > u32ReloadValue)
    {
        return (ErrorInvalidParameter);
    }

    /* First of all set MFS to Asynchronous mode 0 */
    pstcCsio->SMR = 0u;

    /* Then we disable TX and RX for safe operation */
    pstcCsio->SCR = 0u;

    /* Clear MFS by setting the Software Reset bit */
    pstcCsio->SCR_f.UPCL = TRUE;

    /* Check the configration of FIFO usage */
    switch (pstcConfig->u8FifoUsage)
    {
        case MfsHlUseNoFifo:
        case MfsHlUseFifo:
            /* Set FIFO usage */
            pstcMfsHlInternData->u8FifoUsage = pstcConfig->u8FifoUsage;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* ************ Register Settings ************ */
    /* Local Serial Mode Register variable */

    /* Operation Mode Selection */
    switch (pstcConfig->u8CsioMode)
    {
        /* Master mode */
        case MfsCsioMaster:
            stcSCR.MS = FALSE;
            break;
        /* Slave mode */
        case MfsCsioSlave:
            stcSCR.MS = TRUE;
            break;
        default:
            return (ErrorInvalidParameter); /* should never see the daylight */
    }

    /* Set mode (CSIO) */
    stcSMR.MD = MFS_MD_CSIO;

    /* Active Mode Selection */
    switch (pstcConfig->u8CsioActMode)
    {
        /* Normal mode */
        case MfsCsioActNormalMode:
            stcSCR.SPI = FALSE;
            break;
        /* SPI mode */
        case MfsCsioActSpiMode:
            stcSCR.SPI = TRUE;
            break;
        default:
            return (ErrorInvalidParameter); /* should never see the daylight */
    }

    /* Bit direction */
    if (TRUE == pstcConfig->bBitDirection)
    {
        stcSMR.BDS = TRUE;
    }
    else
    {
        stcSMR.BDS = FALSE;
    }

    /* now setup hardware with correct mode first and then go on with */
    /*   bit settings */
    pstcCsio->SMR_f = stcSMR;

    /*  Set synchronous clock input/output according master/slave mode */
    if (MfsCsioMaster == pstcConfig->u8CsioMode)
    {
        /* Master:Clock output */
        stcSMR.SCKE = TRUE;
    }
    else
    {
        /* Slave:Clock input */
        stcSMR.SCKE = FALSE;
    }

    /* Serial clock invert */
    if (TRUE == pstcConfig->bSignalSystem)
    {
        /* Mark level "LOW" */
        stcSMR.SCINV = TRUE;
    }
    else
    {
        /* Mark level "HIGH" */
        stcSMR.SCINV = FALSE;
    }

    /* Enable serial output */
    stcSMR.SOE = TRUE;

    /* Wait time insertion */
    switch (pstcConfig->u8SyncWaitTime)
    {
        case MfsSyncWaitZero:
            stcESCR.WT = MFS_WAIT_TIME_0BIT;
            break;
        case MfsSyncWaitOne:
            stcESCR.WT = MFS_WAIT_TIME_1BIT;
            break;
        case MfsSyncWaitTwo:
            stcESCR.WT = MFS_WAIT_TIME_2BITS;
            break;
        case MfsSyncWaitThree:
            stcESCR.WT = MFS_WAIT_TIME_3BITS;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    stcESCR.SOP = FALSE;

    stcESCR.L3 = FALSE;  /* less than 13 bits */
    stcSSR.AWC = FALSE;  /* 16bit access for RDR/TDR (less than or equal to 16 bits) */
    /* Caracter Length */
    switch (pstcConfig->u8CharLength)
    {
        case MfsFiveBits:
            stcESCR.L = MFS_DATA_LEN_5;
            break;
        case MfsSixBits:
            stcESCR.L = MFS_DATA_LEN_6;
            break;
        case MfsSevenBits:
            stcESCR.L = MFS_DATA_LEN_7;
            break;   
        case MfsEightBits:
            stcESCR.L = MFS_DATA_LEN_8;
            break;
        case MfsNineBits:
            stcESCR.L = MFS_DATA_LEN_9;
            break;
        case MfsTenBits:
            stcESCR.L = MFS_DATA_LEN_10;
            break;
        case MfsElevenBits:
            stcESCR.L = MFS_DATA_LEN_11;
            break;
        case MfsTwelveBits:
            stcESCR.L = MFS_DATA_LEN_12;
            break;
        case MfsThirteenBits:
            stcESCR.L  = MFS_DATA_LEN_13;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            break;
        case MfsFourteenBits:
            stcESCR.L  = MFS_DATA_LEN_14;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            break;
        case MfsFifteenBits:
            stcESCR.L  = MFS_DATA_LEN_15;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            break;
        case MfsSixteenBits:
            stcESCR.L = MFS_DATA_LEN_16;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            break;
        case MfsTwentyBits:
            stcESCR.L  = MFS_DATA_LEN_20;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            stcSSR.AWC = TRUE;  /* 32bit access for RDR/TDR (more than 16 bits) */
            break;
        case MfsTwentyFourBits:
            stcESCR.L  = MFS_DATA_LEN_24;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            stcSSR.AWC = TRUE;  /* 32bit access for RDR/TDR (more than 16 bits) */
            break;
        case MfsThirtyTwoBits:
            stcESCR.L  = MFS_DATA_LEN_32;
            stcESCR.L3 = TRUE;  /* More than 12 bits */
            stcSSR.AWC = TRUE;  /* 32bit access for RDR/TDR (more than 16 bits) */
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* now setup hardware (synchronous) */
    pstcCsio->SMR_f = stcSMR;

    /* now setup hardware */
    pstcCsio->SCR_f = stcSCR;

    /* now setup hardware */
    pstcCsio->ESCR_f = stcESCR;
    pstcCsio->SSR_f = stcSSR;

    /* Local Serial Status Register variable (USART) */
    /* Clear possible reception errors */
    pstcCsio->SSR_f.REC = TRUE;

    /* Master mode */
    if (MfsCsioMaster == pstcConfig->u8CsioMode)
    {
        /* Normal mode */
        if (MfsCsioActNormalMode == pstcConfig->u8CsioActMode)
        {
            pstcMfsHlInternData->u8MfsMode = MfsHlModeCsioNormalMaster;
        }
        /* SPI mode */
        else
        {
            pstcMfsHlInternData->u8MfsMode = MfsHlModeCsioSpiMaster;
        }
    }
    /* Slave mode */
    else
    {
        /* Normal mode */
        if (MfsCsioActNormalMode == pstcConfig->u8CsioActMode)
        {
            pstcMfsHlInternData->u8MfsMode = MfsHlModeCsioNormalSlave;
        }
        /* SPI mode */
        else
        {
            pstcMfsHlInternData->u8MfsMode = MfsHlModeCsioSpiSlave;
        }
    }

    /* SPI Chip select configration */
    if (NULL != pstcConfig->pstcMfsSpiCsConfig)
    {
        /* Chip select function sets when mode is master and SPI */
        if ((MfsCsioActSpiMode == pstcConfig->u8CsioActMode)
        /* Chip select is available only ch.6 or 7. */
        && ((pstcCsio == &MFS6) || (pstcCsio == &MFS7))
           )
        {
            /* Enable chip select */
            stcSCSCR.CSEN0 = TRUE;
            if (MfsCsioMaster == pstcConfig->u8CsioMode)
            {
                /* Enable chip select output */
                stcSCSCR.CSOE = TRUE;
                /* Set clock dividor for chip select activation */
                stcSCSCR.CDIV = pstcConfig->pstcMfsSpiCsConfig->u8CsDivision & MFS_HL_MASK_0_2;
                /* Set chip de-select bit */
                u16SCSTR32 = pstcConfig->pstcMfsSpiCsConfig->u16CsDeSelect;
                /* Set chip select setting delay */
                u8SCSTR0 = pstcConfig->pstcMfsSpiCsConfig->u8CsSetDelay;
                /* Set chip select holding delay */
                u8SCSTR1 = pstcConfig->pstcMfsSpiCsConfig->u8CsHoldDelay;
                pstcMfsHlInternData->u8MfsMode = MfsHlModeCsioSpiMasterWithCs;
            }
            else
            {
                /* Set chip select activation level */
                if (FALSE == pstcConfig->pstcMfsSpiCsConfig->bCsLevel)
                {
                    /* In-active High (Active level low) */
                    stcSCSCR.CSLVL = TRUE;
                }
                pstcMfsHlInternData->u8MfsMode = MfsHlModeCsioSpiSlaveWithCs;
            }
            /* now setup hardware */
            pstcCsio->SCSCR_f = stcSCSCR;
            pstcCsio->SCSTR32 = u16SCSTR32;
            pstcCsio->SCSTR0  = u8SCSTR0;
            pstcCsio->SCSTR1  = u8SCSTR1;
        }
    }
    /* Timer mode configration */
    else if (NULL != pstcConfig->pstcMfsTimerConfig)
    {
        /* Serial timer settings (master only) */
        if ((0u != pstcConfig->pstcMfsTimerConfig->u16SerialTimer)
        &&  (MfsCsioMaster == pstcConfig->u8CsioMode))
        {
            /* Set serial timer */
            u16STMCR = pstcConfig->pstcMfsTimerConfig->u16SerialTimer;
            /* Set serial timer dividor */
            stcSACSR.TDIV = pstcConfig->pstcMfsTimerConfig->u8TimerDivision & MFS_HL_MASK_0_3;
            /* Synchronous transfer with serial timer */
            if (TRUE == pstcConfig->pstcMfsTimerConfig->bTimerSyncEnable)
            {
                /* Enable synchronous transfer */
                stcSACSR.TSYNE = TRUE;
                u8TBYTE0 = pstcConfig->pstcMfsTimerConfig->u8TxByte;
            }
            /* now setup hardware */
            pstcCsio->SACSR_f = stcSACSR;
            pstcCsio->STMCR   = u16STMCR;
            pstcCsio->TBYTE0  = u8TBYTE0;
            /* CSIO timer mode */
            pstcMfsHlInternData->u8MfsMode = MfsHlModeCsioNormalMasterTimer;
        }
    }
    else
    {
        /* There is no process that should be executed. */
    }

    /*  Local Fifo Control Register 0 variable */
    if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
    {
        /* Initialize FIFO to use */
        MfsHlFifoUseInit(pstcCsio, pstcConfig->u16RxCbBufFillLvl);
    }

    /* Save RX Callback Buffer Fill Level (needed during servicing RX INT) */
    pstcMfsHlInternData->u16RxCallbackBufFillLevel = pstcConfig->u16RxCbBufFillLvl;

    /* Save character length */
    pstcMfsHlInternData->u8DataWidth = pstcConfig->u8CharLength;

    /* Set baud rate generation reload register */
    pstcCsio->BGR = (uint16_t)(u32ReloadValue & MFS_HL_BGR_MASK);

    /* Set callback functions. */
    pstcMfsHlInternData->pfnTransmitCbFunction = pstcConfig->pfnTxCb;
    pstcMfsHlInternData->pfnReceiveCbFunction  = pstcConfig->pfnRxCb;

    /* Initialise TX ring buffer */
    pstcMfsHlInternData->stcTxBuffer.pu8Buffer     = pstcConfig->pu8TxBuf;
    pstcMfsHlInternData->stcTxBuffer.u16BufferSize = pstcConfig->u16TxBufSize;
    pstcMfsHlInternData->stcTxBuffer.u16InIndex    = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcTxBuffer.u16OutIndex   = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcTxBuffer.u16FillCount  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.u16TxCounter  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.u16TxCount    = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.bOverflow     = 0u; /*  Reset Flag */

    /* Initialise RX ring buffer */
    pstcMfsHlInternData->stcRxBuffer.pu8Buffer     = pstcConfig->pu8RxBuf;
    pstcMfsHlInternData->stcRxBuffer.u16BufferSize = pstcConfig->u16RxBufSize;
    pstcMfsHlInternData->stcRxBuffer.u16InIndex    = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcRxBuffer.u16OutIndex   = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcRxBuffer.u16FillCount  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcRxBuffer.bOverflow     = 0u; /*  Reset Flag */

    /* MFS is standby */
    pstcMfsHlInternData->u8Exec = MfsHlExecStby;

    /* Register interrupt handler and internal handle */
    (void)Mfs_SetTxIntCallBack(pstcCsio, MfsHlIrqHandlerTx);
    (void)Mfs_SetRxIntCallBack(pstcCsio, MfsHlIrqHandlerRx);
    (void)Mfs_SetStsIntCallBack(pstcCsio, MfsHlIrqHandlerSts);
    (void)Mfs_SetUpperLayerHandle(pstcCsio, pstcMfsHlInternData);

    /* Enable receiver, transmitter */
    pstcCsio->SCR_f.TXE = TRUE;
    pstcCsio->SCR_f.RXE = TRUE;

    /* Use interruption */
    if (NULL != pstcConfig->pfnRxCb)
    {
        /* Enable reception interrupt */
        pstcCsio->SCR_f.RIE = TRUE;
    }

    /* Init transmission interrupt */
    (void)Mfs_InitTxIrq(pstcCsio);
    /* Init reception interrupt */
    (void)Mfs_InitRxIrq(pstcCsio);

    return (Ok);
} /* Mfs_Hl_Csio_Init */

/**
 ******************************************************************************
 ** \brief CSIO deinitialisation of a MFS_HL module.
 **
 ** This function de-initializes the MFS_HL module activating as CSIO.
 ** This function uses for mode change or other changed settings.
 **  
 ** \param [in]   pstcCsio      Pointer to CSIO (MFS) instance register area
 ** 
 ** \retval Ok                    Deinitialisation of MFS_HL module successfully
 **                               done
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - pstcCsio == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** 
 ******************************************************************************/
en_result_t Mfs_Hl_Csio_DeInit(volatile stc_mfsn_t* pstcCsio)
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcCsio);
    /* ... and check */
    if (NULL == pstcMfsHlInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        /* First of all disable receiver, transmitter and deinit interrupts */
        pstcCsio->SCR_f.TXE = FALSE;
        pstcCsio->SCR_f.RXE = FALSE;
    
        (void)Mfs_DeInitIrq(pstcCsio);     /* returns always en_result_t Ok */

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
        pstcCsio->SSR     = 0u;
        pstcCsio->ESCR    = 0u;
        pstcCsio->FCR0    = 0u;
        pstcCsio->FCR1    = 0u;
        pstcCsio->SACSR   = 0u;
        pstcCsio->SCSCR   = 0u;
        pstcCsio->STMCR   = 0u;
        pstcCsio->SCSTR10 = 0u;
        pstcCsio->SCSTR32 = 0u;
        pstcCsio->TBYTE0  = 0u;

        /* Clear MFS by setting the Software Reset bit */
        pstcCsio->SCR_f.UPCL = TRUE;

        /* Unregist interrupt handler and internal handle */
        (void)Mfs_SetTxIntCallBack(pstcCsio, NULL);
        (void)Mfs_SetRxIntCallBack(pstcCsio, NULL);
        (void)Mfs_SetStsIntCallBack(pstcCsio, NULL);
        (void)Mfs_SetUpperLayerHandle(pstcCsio, NULL);

        /* Reset Fifo buffer */
        pstcMfsHlInternData->stcTxBuffer.pu8Buffer  = NULL;
        pstcMfsHlInternData->stcRxBuffer.pu8Buffer  = NULL;
    
        /* Reset callback functions. */
        pstcMfsHlInternData->pfnTransmitCbFunction = NULL;
        pstcMfsHlInternData->pfnReceiveCbFunction  = NULL;

        /* Reset LIN Break Callback used for Mfs_Lin_DeInit() */
        pstcMfsHlInternData->pfnLinBreakCbFunction = NULL;
        pstcMfsHlInternData->u8Exec = MfsHlExecStby;
        pstcMfsHlInternData->u8MfsMode = MfsHlModeStby;

        enResult = Ok;
    }

    return (enResult);
} /* Mfs_Hl_Csio_DeInit */

/**
 ******************************************************************************
 ** \brief Simultaneously transmit and receive data as CSIO master
 **
 ** This function will transmit and receive the same amount of data, based on 
 ** the serial (shift) clock signal (MFS SCK pin) in synchronous master or 
 ** slave mode. The operation mode of the MFS instance must be setup for 
 ** synchronous master or slave mode.
 **
 ** While the data in Mfs_Hl_Csio_SynchronousTrans#pu8TxData is transmitted the
 ** input data is received and stored in Mfs_Hl_Csio_SynchronousTrans#pu8RxData.
 ** The function operates in CSIO (blocking) mode 
 ** only i.e. it will wait until the amount of data defined by
 ** Mfs_Hl_Csio_SynchronousTrans#u16TransferSize 
 ** is transmitted/received. No MFS internal buffers or transfer counters
 ** are used.
 ** The TX/RX callback functions will not be called.
 ** Note, that in synchronous slave mode, the transfer is controlled by 
 ** the external master device, providing the serial shift clock. This may 
 ** cause this function to block very long time.
 **
 ** Because this function uses blocking method, no interrupts are needed and
 ** therfore are not used. Also no FIFO operation is performed.
 **
 ** \note Synchronous (non-blocking) TX/RX operations are provided by using 
 **       the functions Mfs_Hl_Write() and Mfs_Hl_Read() for MFS CSIO master
 **       and slave modes, too. Note, that these functions do not support
 **       full-duplex operation!
 **       This function can use only if character length was set to less or 
 **       equal to eight bits.
 **       Mfs_Hl_Csio_SynchronousTrans#pu8TxData and 
 **       Mfs_Hl_Csio_SynchronousTrans#pu8RxData can be set NULL, but either 
 **       should be set except NULL.
 **       If only Mfs_Hl_Csio_SynchronousTrans#pu8TxData is set NULL, 
 **       dummy data is sent and the received data is stored in the buffer 
 **       shown by Mfs_Hl_Csio_SynchronousTrans#pu8RxData.
 **       If only Mfs_Hl_Csio_SynchronousTrans#pu8RxData is set NULL, the data 
 **       of the buffer shown by Mfs_Hl_Csio_SynchronousTrans#pu8TxData is 
 **       transmitted and the received data is thrown away.
 **
 ** \param [in]     pstcCsio          Pointer to CSIO (MFS) instance register area
 ** \param [in]     pu8TxData         Pointer to transmit data buffer, can be NULL.
 ** \param [in,out] pu8RxData         Pointer to receive data buffer, can be NULL.
 ** \param [in]     u16TransferSize   Transmit size (Must be at least 1)
 ** \param [in]     bCsHolding        Specify to hold chip select
 **
 ** \retval Ok                        Transfer done successfully.
 ** \retval ErrorInvalidParameter     If any of following conditions are met:
 **         - pstcCsio == NULL
 **         - pu8TxData == NULL or pu8RxData == NULL
 **         - u16TransferSize == 0 
 **         - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                      (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** \retval ErrorOperationInProgress  A transmission is still pending or 
 **                                   the RX buffer is not empty.
 ** \retval ErrorInvalidMode          MFS_HL module was not initalised in MFS
 **                                   CSIO mode.
 ** \retval Error                     Character length is over 8 bits.
 **
******************************************************************************/
en_result_t Mfs_Hl_Csio_SynchronousTrans(volatile stc_mfsn_t* pstcCsio,
                                         const uint8_t*       pu8TxData,
                                         uint8_t*             pu8RxData,
                                         uint16_t             u16TransferSize,
                                         boolean_t            bCsHolding
                                        )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData ;
    uint16_t                  u16DataToSendCounter;
    uint16_t                  u16DataReceivedCounter;
    uint8_t                   u8Data;
    volatile uint8_t          u8Reg;

    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcCsio);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    || ((NULL == pu8TxData) && (NULL == pu8RxData))
    /* Check for 0 < transmission length */
    ||  (0u == u16TransferSize)
       )
    {
        return (ErrorInvalidParameter);
    }

    /* check for valid mode (only sync mode allowed) */
    if (MFS_MD_CSIO != pstcCsio->SMR_f.MD)
    {
        return (ErrorInvalidMode);
    }

    /* This function can use when character length is set less or equel to 8 bits. */
    if (MfsEightBits < pstcMfsHlInternData->u8DataWidth)
    {
        return (Error);
    }

    switch (pstcMfsHlInternData->u8MfsMode)
    {
        /* Set tx bytes when chip select is used on master mode */
        case MfsHlModeCsioSpiMasterWithCs:
            if (MFS_CSIO_TBYTE_MAX < u16TransferSize)
            {
                return (ErrorInvalidParameter);
            }
            pstcCsio->SCR_f.TXE = FALSE;
            /* Set size to active chip select */
            pstcCsio->TBYTE0 = (uint8_t)u16TransferSize;
            if (TRUE == bCsHolding)
            {
                /* Hold chip select */
                pstcCsio->SCSCR_f.SCAM = TRUE;
            }
            else
            {
                /* In-active chip select when transmit is end */
                pstcCsio->SCSCR_f.SCAM = FALSE;
            }
            pstcCsio->SCR_f.TXE = TRUE;
            break;

        /* CSIO timer mode */
        case MfsHlModeCsioNormalMasterTimer:
            /* Enable serial timer */
            pstcCsio->SCR_f.TXE = FALSE;
            pstcCsio->SACSR_f.TMRE = TRUE;
            pstcCsio->SCR_f.TXE = TRUE;
            break;

        default:
            break;
    }

    /* Use interruption */
    if (NULL != pstcMfsHlInternData->pfnReceiveCbFunction)
    {
        /* Disable reception interrupt during sending and receiving data */
        pstcCsio->SCR_f.RIE = FALSE;
    }

    u16DataToSendCounter   = 0u;
    u16DataReceivedCounter = 0u;

    while (u16DataReceivedCounter != u16TransferSize)
    {
        /* Wait for TDR empty */
        while(FALSE == pstcCsio->SSR_f.TDRE)
        {
            PDL_WAIT_LOOP_HOOK();
        }
        /* If pu8TxData is NULL, dummy data is sent. */
        u8Data = 0u;
        if (NULL != pu8TxData)
        {
            u8Data = pu8TxData[u16DataToSendCounter];
        }
        /* Send data */
        pstcCsio->TDR = (uint16_t)u8Data;
        u16DataToSendCounter++;

        do
        {
            /* Check reception */
            u8Reg = pstcCsio->SSR;
        /* wait for reception finsihed */
        } while (0u == (u8Reg & MFS_SSR_RDRF));
        /* Check Overrun error */
        if (0u != (u8Reg & MFS_CSIO_SSR_ERR))
        {
            pstcCsio->SSR_f.REC = TRUE;  /* Clear possible reception errors */
        }
        /* If pu8RxData is NULL, dummy data is received. */
        u8Data = (uint8_t)pstcCsio->RDR;
        if (NULL != pu8RxData)
        {
            pu8RxData[u16DataReceivedCounter] = u8Data;
        }
        u16DataReceivedCounter++;
    }

    /* CSIO timer mode */
    if (MfsHlModeCsioNormalMasterTimer == pstcMfsHlInternData->u8MfsMode)
    {
        /* Wait for TX bus idle */
        while (FALSE == pstcCsio->SSR_f.TBI)
        {
            /* Disable serial timer */
            pstcCsio->SCR_f.TXE    = FALSE;
            pstcCsio->SACSR_f.TMRE = FALSE;
            pstcCsio->SCR_f.TXE    = TRUE;
        }
    }

    /* Use interruption */
    if (NULL != pstcMfsHlInternData->pfnReceiveCbFunction)
    {
        /* Enable reception interrupt */
        pstcCsio->SCR_f.RIE = TRUE;
    }

    return (Ok);
} /* Mfs_Hl_Csio_SynchronousTrans */

/**
 ******************************************************************************
 ** \brief Initialize an MFS_HL module as LIN mode.
 **
 ** This Function initialises the MFS according the LIN setup in the passed
 ** configuration structure.
 ** Several checking are done before that and an error is returned if invalid
 ** Modes are requested.
 **
 ** \param [in]  pstcLin       Pointer to LIN (MFS) instance register area
 ** \param [in]  pstcConfig    MFS LIN configuration 
 **
 ** \retval Ok                    Initializiation of MFS_HL module successfully 
 **                               done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - pstcCsio == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **             - one or more enumerated values in pstcUart out of enumaration
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Lin_Init(volatile stc_mfsn_t*     pstcLin,
                            stc_mfs_hl_lin_config_t* pstcConfig
                           )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    uint32_t                  u32ReloadValue;

    /* Preset local register variables to zero */
    stc_mfs_smr_field_t  stcSMR  = { 0 };
    stc_mfs_scr_field_t  stcSCR  = { 0 };
    stc_mfs_escr_field_t stcESCR = { 0 };

    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcLin);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    ||  (NULL == pstcConfig)
       )
    {
        return (ErrorInvalidParameter);
    }

    /* Get reload value */
    u32ReloadValue = Mfs_GetReloadValue(pstcConfig->u32DataRate);
    /* For asynchronous communication, the reload value must be greater than  */
    /* or equal to 3 because five times over-sampling is performed internally */
    if (MFS_HL_RELOADVAL_MIN4LIN > u32ReloadValue)
    {
        return (ErrorInvalidParameter);
    }

    /* Check buffer for LIN */
    if (((NULL == pstcConfig->pu8TxBuf)
     ||  (0u == pstcConfig->u16TxBufSize))
     || ((NULL == pstcConfig->pu8RxBuf)
     ||  (0u == pstcConfig->u16RxBufSize)))
    {
        return (ErrorInvalidParameter);
    }

    /* Check the configration of FIFO usage */
    switch (pstcConfig->u8FifoUsage)
    {
        case MfsHlUseNoFifo:
        case MfsHlUseFifo:
            /* Set FIFO usage */
            pstcMfsHlInternData->u8FifoUsage = pstcConfig->u8FifoUsage;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* First of all set MFS to Asynchronous mode 0 */
    pstcLin->SMR = 0u;

    /* Then we disable TX and RX for safe operation */
    pstcLin->SCR = 0u;

    /* Clear MFS by setting the Software Reset bit */
    pstcLin->SCR_f.UPCL = TRUE;

    /* Set LIN mode to HW and preset variable */
    stcSMR.MD = MFS_MD_LIN;

    /* Serial output */
    stcSMR.SOE = TRUE;

    /* Operation Mode Selection */
    switch (pstcConfig->u8LinMode)
    {
        /* Master */
        case MfsLinMaster:
            stcSCR.MS = FALSE;
            break;
        /* Slave */
        case MfsLinSlave:
            stcSCR.MS = TRUE;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Set LIN master configuration */
    if (MfsLinMaster == pstcConfig->u8LinMode)
    {
        /* Stop bits configration */
        switch (pstcConfig->u8StopBits)
        {
            case MfsLinOneStopBit:
                stcSMR.SBL   = FALSE;
                stcESCR.ESBL = FALSE;
                break;
            case MfsLinTwoStopBits:
                stcSMR.SBL   = TRUE;
                stcESCR.ESBL = FALSE;
                break;
            case MfsLinThreeStopBits:
                stcSMR.SBL   = FALSE;
                stcESCR.ESBL = TRUE;
                break;
            case MfsLinFourStopBits:
                stcSMR.SBL   = TRUE;
                stcESCR.ESBL = TRUE;
                break;
            default:
                return (ErrorInvalidParameter);
        }
        /* Break length configration */
        switch (pstcConfig->u8BreakLength)
        {
            case MfsLinBreakLength13:
                stcESCR.LBL = MFS_LIN_BREAK_13BITS;
                break;
            case MfsLinBreakLength14:
                stcESCR.LBL = MFS_LIN_BREAK_14BITS;
                break;
            case MfsLinBreakLength15:
                stcESCR.LBL = MFS_LIN_BREAK_15BITS;
                break;
            case MfsLinBreakLength16:
                stcESCR.LBL = MFS_LIN_BREAK_16BITS;
                break;
            default:
                return (ErrorInvalidParameter);
        }
        /* Delimiter length configration */
        switch (pstcConfig->u8DelimiterLength)
        {
            case MfsLinDelimiterLength1:
                stcESCR.DEL = MFS_LIN_DELIMITER_1BIT;
                break;
            case MfsLinDelimiterLength2:
                stcESCR.DEL = MFS_LIN_DELIMITER_2BITS;
                break;
            case MfsLinDelimiterLength3:
                stcESCR.DEL = MFS_LIN_DELIMITER_3BITS;
                break;
            case MfsLinDelimiterLength4:
                stcESCR.DEL = MFS_LIN_DELIMITER_4BITS;
                break;
            default:
                return (ErrorInvalidParameter);
        }
    }

    /* Lin break interrupt */
    if (TRUE == pstcConfig->bLinBreakIrqEnable)
    {
        stcESCR.LBIE = TRUE;
    }
    else
    {
        stcESCR.LBIE = FALSE;
    }

    /*  Local Fifo Control Register 0 variable */
    if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
    {
        /* Initialize FIFO to use */
        MfsHlFifoUseInit(pstcLin, MFS_RX_FIFO_MIN_VAL);
    }

    /* Save RX Callback Buffer Fill Level (needed during servicing RX INT) */
    pstcMfsHlInternData->u16RxCallbackBufFillLevel = MFS_RX_FIFO_MIN_VAL;

    /* Save character length (Only 8bits) */
    pstcMfsHlInternData->u8DataWidth = MfsEightBits;

    /* Set baud rate generation reload register */
    pstcLin->BGR = (uint16_t)(u32ReloadValue & MFS_HL_BGR_MASK);

    /* Set callback functions. */
    pstcMfsHlInternData->pfnTransmitCbFunction = pstcConfig->pfnTxCb;
    pstcMfsHlInternData->pfnReceiveCbFunction  = pstcConfig->pfnRxCb;
    pstcMfsHlInternData->pfnLinBreakCbFunction = pstcConfig->pfnLinBrkCb;

    /* Initialise TX ring buffer */
    pstcMfsHlInternData->stcTxBuffer.pu8Buffer     = pstcConfig->pu8TxBuf;
    pstcMfsHlInternData->stcTxBuffer.u16BufferSize = pstcConfig->u16TxBufSize;
    pstcMfsHlInternData->stcTxBuffer.u16InIndex    = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcTxBuffer.u16OutIndex   = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcTxBuffer.u16FillCount  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.u16TxCounter  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.u16TxCount    = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.bOverflow     = 0u; /*  Reset Flag */

    /* Initialise RX ring buffer */
    pstcMfsHlInternData->stcRxBuffer.pu8Buffer     = pstcConfig->pu8RxBuf;
    pstcMfsHlInternData->stcRxBuffer.u16BufferSize = pstcConfig->u16RxBufSize;
    pstcMfsHlInternData->stcRxBuffer.u16InIndex    = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcRxBuffer.u16OutIndex   = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcRxBuffer.u16FillCount  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcRxBuffer.bOverflow     = 0u; /*  Reset Flag */

    /* MFS is standby */
    pstcMfsHlInternData->u8Exec = MfsHlExecStby;

    /* Operation Mode Selection */
    if (MfsLinMaster == pstcConfig->u8LinMode)
    {
        /* Master mode */
        pstcMfsHlInternData->u8MfsMode = MfsHlModeLinMaster;
    }
    else
    {
        /* Slave mode */
        pstcMfsHlInternData->u8MfsMode = MfsHlModeLinSlave;
    }


    /* Register interrupt handler and internal handle */
    (void)Mfs_SetTxIntCallBack(pstcLin, MfsHlIrqHandlerTx);
    (void)Mfs_SetRxIntCallBack(pstcLin, MfsHlIrqHandlerRx);
    (void)Mfs_SetStsIntCallBack(pstcLin, MfsHlIrqHandlerSts);
    (void)Mfs_SetUpperLayerHandle(pstcLin, pstcMfsHlInternData);

    /* Setup hardware */
    pstcLin->SMR_f  = stcSMR;
    pstcLin->SCR_f  = stcSCR;
    pstcLin->ESCR_f = stcESCR;

    /* Enable receiver, transmitter */
    pstcLin->SCR_f.TXE = TRUE;
    pstcLin->SCR_f.RXE = TRUE;

    /* Use interruption */
    if (NULL != pstcConfig->pfnRxCb)
    {
        /* Enable reception interrupt */
        pstcLin->SCR_f.RIE = TRUE;
    }

    /* Init transmission interrupt */
    (void)Mfs_InitTxIrq(pstcLin);
    /* Init reception interrupt */
    (void)Mfs_InitRxIrq(pstcLin);

    return (Ok);
} /* Mfs_Hl_Lin_Init */

/**
 ******************************************************************************
 ** \brief LIN deinitialisation of a MFS_HL module.
 **
 ** This function de-initializes the MFS_HL module activating as LIN.
 ** This function uses for mode change or other changed settings.
 ** 
 ** \param [in]  pstcLin       Pointer to LIN (MFS) instance register area
 **
 ** \retval Ok                     LIN Deinitialisation of MFS_HL module 
 **                                successfully done
 ** \retval ErrorInvalidMode       MFS not in LIN mode
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **             - pstcMfs == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Lin_DeInit(volatile stc_mfsn_t* pstcLin)
{
    en_result_t enResult;

    /* Check correct LIN master mode */
    if (MFS_SMR_LIN_MODE != (pstcLin->SMR & MFS_SMR_MODEBITS_MASK))
    {
        enResult = ErrorInvalidMode;
    }
    else
    {
        enResult = Mfs_Hl_Uart_DeInit(pstcLin);
    }

    return (enResult);
} /* Mfs_Hl_Lin_DeInit */

/**
 ******************************************************************************
 ** \brief Set LIN Break with an MFS_HL module in LIN Master Mode
 **
 ** \note MFS_HL module must be initialized to LIN Master mode
 **
 ** This Function sets a LIN break and break delimiter length with the
 ** configuration by the previous initialization.
 ** 
 ** \param [in]  pstcLin       Pointer to LIN (MFS) instance register area
 **
 ** \retval Ok                        LIN Break is (being) generated
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcLin == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** \retval ErrorInvalidMode          MFS not in LIN master mode
 ** \retval ErrorOperationInProgress  MFS not ready for generating break
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Lin_SetBreak(volatile stc_mfsn_t* pstcLin)
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcLin);
    /* ... and check */
    if (NULL == pstcMfsHlInternData)
    {
        enResult = ErrorInvalidParameter;
    }

    /* Check correct LIN master mode */
    else if ((MFS_SMR_LIN_MODE != (pstcLin->SMR & MFS_SMR_MODEBITS_MASK))
    ||       (TRUE == pstcLin->SCR_f.MS) )
    {
        enResult = ErrorInvalidMode;
    }

    /* Check if transmission bus is free (no transmission ongoing) */
    else if (FALSE == pstcLin->SSR_f.TBI)
    {
        enResult = ErrorOperationInProgress;
    }

    else
    {
        /* Finally generate LIN break with configured length and delimiter */
        pstcLin->SCR_f.LBR = TRUE;
        enResult = Ok;
    }

    return (enResult);
} /* Mfs_Hl_Lin_SetBreak */

/**
 ******************************************************************************
 ** \brief Set new baud divisor (NOT Baud Rate!) in LIN Slave Mode
 **
 ** \pre MFS_HL module must be initialized to LIN Slave mode
 **
 ** This Function sets a new (calculated) baud divisor, if the MFS is in LIN
 ** Slave mode.
 **
 ** \note This function should only be called:
 **       - After a complete LIN frame was received and before next LIN Break
 **       - Shortly after the second ICU interrupt within the LIN Synch Field
 **         and before the next start bit of the LIN Header byte!
 ** 
 ** \param [in]  pstcLin         Pointer to LIN (MFS) instance register area
 ** \param [in]  u16BaudDivisor  New (calculated) Baud Divisor
 **
 ** \retval Ok                        Baud rate was set correctly
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcLin == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** \retval ErrorInvalidMode          MFS not in LIN slave mode
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Lin_SetNewBaudDivisor(volatile stc_mfsn_t* pstcLin,
                                         uint16_t             u16BaudDivisor
                                        )
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcLin);
    /* ... and check */
    if (NULL == pstcMfsHlInternData)
    {
        enResult = ErrorInvalidParameter;
    }

    /* Check correct LIN slave mode */
    else if ((MFS_SMR_LIN_MODE != (pstcLin->SMR & MFS_SMR_MODEBITS_MASK))
    ||       (FALSE == pstcLin->SCR_f.MS) )
    {
        enResult = ErrorInvalidMode;
    }

    else
    {
        /* Update Baud Rate register */
        pstcLin->BGR = u16BaudDivisor;
        enResult = Ok;
    }

    return (enResult);
} /* Mfs_Hl_Lin_SetNewBaudDivisor */

/**
 ******************************************************************************
 ** \brief Transfers n bytes of recent receive buffer to user buffer
 **
 ** \param [in]     pstcLin         Pointer to MFS instance register area
 ** \param [in,out] pu8Data         Pointer to data buffer
 ** \param [in]     u16ReadCount    Number of bytes to be transfered
 **
 ** \retval Ok                      Process is success
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **             - pstcLin == NULL
 **             - pu8Data == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Lin_TransferRxBuffer(volatile stc_mfsn_t* pstcLin,
                                        uint8_t*             pu8Data,
                                        uint16_t             u16ReadCount)
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    uint16_t                  u16Count;
    int32_t                   i32BufferCount;

    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcLin);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    ||  (NULL == pu8Data)
       )
    {
        enResult = ErrorInvalidParameter;
    }

    else
    {
        enResult = Ok;
        /* Check if nothing to do */
        if (0u != u16ReadCount)
        {
            /* Calculate internal buffer index */
            i32BufferCount = (int32_t)(pstcMfsHlInternData->stcRxBuffer.u16InIndex) - (int32_t)(u16ReadCount);
            
            /* Adjust ring buffer index? */
            if (0 > i32BufferCount)
            {
                i32BufferCount += (int32_t)pstcMfsHlInternData->stcRxBuffer.u16BufferSize;
            }

            for (u16Count = 0u; u16Count < u16ReadCount; u16Count++)
            {
                pu8Data[u16Count] = pstcMfsHlInternData->stcRxBuffer.pu8Buffer[i32BufferCount];
                i32BufferCount++;

                if (i32BufferCount > (int32_t)pstcMfsHlInternData->stcRxBuffer.u16BufferSize)
                {
                     i32BufferCount = 0;
                }
            }
        }
    }

    return (enResult);
} /* Mfs_Hl_Lin_TransferRxBuffer */

/**
 ******************************************************************************
 ** \brief Disable reception interrupt for LIN mode
 **
 ** \retval Ok                      Baud rate was set correctly
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **             - pstcLin == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Lin_DisableRxInterrupt(volatile stc_mfsn_t* pstcLin)
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcLin);
    /* ... and check */
    if (NULL == pstcMfsHlInternData)
    {
        enResult = ErrorInvalidParameter;
    }

    else
    {
        pstcLin->SCR_f.RIE = FALSE;
        enResult = Ok;
    }

    return (enResult);
} /* Mfs_Hl_Lin_DisableRxInterrupt */

/**
 ******************************************************************************
 ** \brief Initialize an MFS_HL module as I2C mode.
 ** This Function initialises the MFS according the I2C setup in the
 ** passed Config Struct. Several Checkings are done before that and an error
 ** is returned if invalid Modes are requested.
 **
 ** \param [in]  pstcI2c       Pointer to I2C (MFS) instance register area
 ** \param [in]  pstcConfig    MFS I2C configuration 
 **
 ** \retval Ok                    Initializiation of MFS_HL module successfully 
 **                               done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - pstcI2c == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 **             - one or more enumerated values in pstcUart out of enumaration
 ** \retval ErrorUninitialized    If bus clock is over specification, NFCR can't
 **                                 set properly. So this function returned this
 **                                 error code.
 **
 ******************************************************************************/
en_result_t Mfs_Hl_I2c_Init(volatile stc_mfsn_t*     pstcI2c,
                            stc_mfs_hl_i2c_config_t* pstcConfig
                           )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    uint32_t                  u32ReloadValue;
    uint32_t                  u32BusClock;

    /* Preset local register variables to zero */
    stc_mfs_smr_field_t       stcSMR   = { 0 };
    stc_mfs_i2c_ibcr_field_t  stcIBCR  = { 0 };
    stc_mfs_i2c_isba_field_t  stcISBA  = { 0 };
    stc_mfs_i2c_ismk_field_t  stcISMK  = { 0 };
    uint8_t                   u8NFCR   = 0;

    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcI2c);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    ||  (NULL == pstcConfig)
       )
    {
        return (ErrorInvalidParameter);
    }

    /* Get reload value */
    u32ReloadValue = Mfs_I2c_GetReloadValue(pstcConfig->u32DataRate);
    /* For asynchronous communication, the reload value must be greater than 0 */
    /* because reload counter is stop */
    if (0u == u32ReloadValue)
    {
        return (ErrorInvalidParameter);
    }

    /* Check the configration of FIFO usage */
    switch (pstcConfig->u8FifoUsage)
    {
        case MfsHlUseNoFifo:
        case MfsHlUseFifo:
            /* Set FIFO usage */
            pstcMfsHlInternData->u8FifoUsage = pstcConfig->u8FifoUsage;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* ************ Register Settings ************ */
    /* Local Serial Mode Register variable */

    /* Operation Mode Selection */
    switch (pstcConfig->u8I2cMode)
    {
        /* I2C master mode */
        case MfsI2cMaster:
        /* I2C slave mode */
        case MfsI2cSlave:
            stcSMR.MD = MFS_MD_I2C;
            break;
        default:
            return (ErrorInvalidParameter); /* should never see the daylight */
    }

    /* Noize filter settings according to the APB2 bus clock */
    u32BusClock = Mfs_GetBusClock();
    if (MFS_CLOCK_40M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess40M;
    }
    else if (MFS_CLOCK_60M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess60M;
    }
    else if (MFS_CLOCK_80M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess80M;
    }
    else if (MFS_CLOCK_100M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess100M;
    }
    else if (MFS_CLOCK_120M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess120M;
    }
    else if (MFS_CLOCK_140M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess140M;
    }
    else if (MFS_CLOCK_160M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess160M;
    }
    else if (MFS_CLOCK_180M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess180M;
    }
    else if (MFS_CLOCK_200M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess200M;
    }
    else if (MFS_CLOCK_220M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess220M;
    }
    else if (MFS_CLOCK_240M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess240M;
    }
    else if (MFS_CLOCK_260M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess260M;
    }
    else if (MFS_CLOCK_280M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess280M;
    }
    else if (MFS_CLOCK_300M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess300M;
    }
    else if (MFS_CLOCK_320M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess320M;
    }
    else if (MFS_CLOCK_340M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess340M;
    }
    else if (MFS_CLOCK_360M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess360M;
    }
    else if (MFS_CLOCK_380M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess380M;
    }
    else if (MFS_CLOCK_400M > u32BusClock)
    {
        u8NFCR = MfsI2cNoizeFilterLess400M;
    }
    else
    {
        return (ErrorUninitialized);
    }

    /* Fast mode selection */
    if ((pstcI2c == MFS_I2C_FAST_MODE_PLUS_CH_A)
    ||  (pstcI2c == MFS_I2C_FAST_MODE_PLUS_CH_B)
       )
    {
        switch (pstcConfig->u8FastMode)
        {
            /* Standard-mode */
            case MfsI2cDisableFastModePlus:
                if (pstcI2c == MFS_I2C_FAST_MODE_PLUS_CH_A)
                {
                    FM4_GPIO->EPFR16_f.SFMPAC = FALSE;
                }
                else
                {
                    FM4_GPIO->EPFR16_f.SFMPBC = FALSE;
                }
                break;
            /* Fast-mode Plus */
            case MfsI2cEnableFastModePlus:
                if (pstcI2c == MFS_I2C_FAST_MODE_PLUS_CH_A)
                {
                    FM4_GPIO->EPFR16_f.SFMPAC = TRUE;
                }
                else
                {
                    FM4_GPIO->EPFR16_f.SFMPBC = TRUE;
                }
                break;
            default:
                return (ErrorInvalidParameter); /* should never see the daylight */
        }
    }

    /* Ack enable */
    stcIBCR.ACKE = TRUE;

    /* Slave mode */
    if (MfsI2cSlave == pstcConfig->u8I2cMode)
    {
        /* Enable stop condition interrupt */
        stcIBCR.CNDE = TRUE;
        /* Enable interrupt */
        stcIBCR.INTE = TRUE;
        /* Set slave address */
        stcISBA.SA = pstcConfig->u8SlvAddr;
        /* Enable slave address detection */
        stcISBA.SAEN = TRUE;
    }

    /* Enable I2C, Enable bit comparing */
    stcISMK.SM = MFS_HL_MASK_0_6;
    stcISMK.EN = TRUE;

    /* now setup hardware with correct mode first and then go on with */
    /*   bit settings */
    pstcI2c->SMR_f = stcSMR;

    /* I2C disable before other registers are set. */
    pstcI2c->ISMK  = 0u;

    /*  Local Fifo Control Register 0 variable */
    if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
    {
        /* Initialize FIFO to use */
        MfsHlFifoUseInit(pstcI2c, MFS_FIFO_MAX_VAL);
    }

    /* Save RX Callback Buffer Fill Level (needed during servicing RX INT) */
    pstcMfsHlInternData->u16RxCallbackBufFillLevel = 0u;

    /* Save character length */
    pstcMfsHlInternData->u8DataWidth = MfsEightBits;

    /* Clear callback functions. */
    pstcMfsHlInternData->pfnTransmitCbFunction = pstcConfig->pfnTxCb;
    pstcMfsHlInternData->pfnReceiveCbFunction  = pstcConfig->pfnRxCb;

    /* Initialise TX ring buffer */
    pstcMfsHlInternData->stcTxBuffer.pu8Buffer     = NULL;
    pstcMfsHlInternData->stcTxBuffer.u16BufferSize = 0u;
    pstcMfsHlInternData->stcTxBuffer.u16InIndex    = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcTxBuffer.u16OutIndex   = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcTxBuffer.u16FillCount  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.u16TxCounter  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.u16TxCount    = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcTxBuffer.bOverflow     = 0u; /*  Reset Flag */

    /* Initialise RX ring buffer */
    pstcMfsHlInternData->stcRxBuffer.pu8Buffer     = NULL;
    pstcMfsHlInternData->stcRxBuffer.u16BufferSize = 0u;
    pstcMfsHlInternData->stcRxBuffer.u16InIndex    = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcRxBuffer.u16OutIndex   = 0u; /*  Reset Index */
    pstcMfsHlInternData->stcRxBuffer.u16FillCount  = 0u; /*  Reset Counter */
    pstcMfsHlInternData->stcRxBuffer.bOverflow     = 0u; /*  Reset Flag */

    /* MFS is standby */
    pstcMfsHlInternData->u8Exec = MfsHlExecStby;

    /* Operation Mode Selection */
    if (MfsI2cMaster == pstcConfig->u8I2cMode)
    {
        /* Master mode */
        pstcMfsHlInternData->u8MfsMode = MfsHlModeI2cMaster;
        /* Clear the starting slave callback function. */
        pstcMfsHlInternData->pfnI2cSlvStCbFunction = NULL;
    }
    else
    {
        /* Slave mode */
        pstcMfsHlInternData->u8MfsMode = MfsHlModeI2cSlave;
        /* Set the starting slave callback function. */
        pstcMfsHlInternData->pfnI2cSlvStCbFunction = pstcConfig->pfnI2cSlvStCb;
    }

    /* Register interrupt handler and internal handle */
    (void)Mfs_SetTxIntCallBack(pstcI2c, MfsHlI2cIrqHandlerTx);
    (void)Mfs_SetRxIntCallBack(pstcI2c, MfsHlI2cIrqHandlerRx);
    (void)Mfs_SetStsIntCallBack(pstcI2c, MfsHlI2cIrqHandlerSts);
    (void)Mfs_SetUpperLayerHandle(pstcI2c, pstcMfsHlInternData);

    /* Set baud rate generation reload register */
    pstcI2c->BGR = (uint16_t)(u32ReloadValue & MFS_HL_BGR_MASK);

    pstcI2c->NFCR = u8NFCR;

    pstcI2c->IBCR_f  = stcIBCR;
    pstcI2c->ISBA_f  = stcISBA;
    pstcI2c->ISMK_f  = stcISMK;

    /* Init transmission interrupt */
    (void)Mfs_InitTxIrq(pstcI2c);
    /* Init reception interrupt */
    (void)Mfs_InitRxIrq(pstcI2c);

    return (Ok);
} /* Mfs_Hl_I2c_Init */

/**
 ******************************************************************************
 ** \brief I2C deinitialisation of a MFS_HL module.
 **
 ** All used mfs register are reset to their default values.
 **  
 ** \param [in]   pstcI2c         Pointer to I2C (MFS) instance register area
 ** 
 ** \retval Ok                    Deinitialisation of MFS_HL module successfully
 **                               done
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - pstcI2c == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** 
 ******************************************************************************/
en_result_t Mfs_Hl_I2c_DeInit(volatile stc_mfsn_t* pstcI2c)
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcI2c);
    /* ... and check */
    if (NULL == pstcMfsHlInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        /* Disable I2C */
        pstcI2c->ISMK_f.EN = FALSE;

        (void)Mfs_DeInitIrq(pstcI2c);     /* returns always en_result_t Ok */

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
        pstcI2c->FCR0 = 0u;
        pstcI2c->FCR1 = 0u;
        pstcI2c->NFCR = 0u;

        /* Unregist interrupt handler and internal handle */
        (void)Mfs_SetTxIntCallBack(pstcI2c, NULL);
        (void)Mfs_SetRxIntCallBack(pstcI2c, NULL);
        (void)Mfs_SetStsIntCallBack(pstcI2c, NULL);
        (void)Mfs_SetUpperLayerHandle(pstcI2c, NULL);

        /* Reset Fifo buffer */
        pstcMfsHlInternData->stcTxBuffer.pu8Buffer  = NULL;
        pstcMfsHlInternData->stcRxBuffer.pu8Buffer  = NULL;
    
        /* Reset callback functions. */
        pstcMfsHlInternData->pfnTransmitCbFunction = NULL;
        pstcMfsHlInternData->pfnReceiveCbFunction  = NULL;
        pstcMfsHlInternData->pfnI2cSlvStCbFunction = NULL;

        /* Reset LIN Break Callback used for Mfs_Lin_DeInit() */
        pstcMfsHlInternData->pfnLinBreakCbFunction = NULL;
        pstcMfsHlInternData->u8Exec = MfsHlExecStby;
        pstcMfsHlInternData->u8MfsMode = MfsHlModeStby;

        enResult = Ok;
    }

    return (enResult);
} /* Mfs_Hl_I2c_DeInit */

/**
 ******************************************************************************
 ** \brief Start operation as I2C slave
 **
 ** \param [in] pstcI2c              Pointer to I2C (MFS) instance register area
 ** \param [in] pstcMfsHlInternData  Pointer to internal data
 **
 ** \retval Ok                       Start slave process successfully done
 ** \retval ErrorInvalidMode         Slave isn't active
 ** 
 ******************************************************************************/
static en_result_t MfsHlI2cStartSlave(volatile stc_mfsn_t*      pstcI2c,
                                      stc_mfs_hl_intern_data_t* pstcMfsHlInternData
                                     )
{
    en_result_t               enResult;
    stc_mfs_smr_field_t       stcSMR;
    stc_mfs_i2c_ibcr_field_t  stcIBCR;
    stc_mfs_i2c_ibsr_field_t  stcIBSR;
    stc_mfs_fcr1_field_t      stcFCR1;

    stcIBCR = pstcI2c->IBCR_f;
    /* Slave is active */
    if ((TRUE == stcIBCR.ACT_SCC) && (FALSE == stcIBCR.MSS))
    {
        /* Set ACK */
        stcIBCR.ACKE     = TRUE;
        stcIBCR.ACT_SCC  = FALSE;
        pstcI2c->IBCR_f = stcIBCR;

        stcIBSR = pstcI2c->IBSR_f;

        /* Direction is TX */
        if (TRUE == stcIBSR.TRX)
        {
            /* FIFO is used */
            if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
            {
                /* tx FIFO interrupt enable */
                stcFCR1 = pstcI2c->FCR1_f;
                stcFCR1.FTIE = TRUE;
                pstcI2c->FCR1_f = stcFCR1;
            }
            /* FIFO is not used */
            else
            {
                /* tx interrupt enable */
                stcSMR = pstcI2c->SMR_f;
                stcSMR.TIE = TRUE;
                pstcI2c->SMR_f = stcSMR;
            }
        }
        /* Direction is RX */
        else
        {
            /* rx interrupt enable */
            stcSMR = pstcI2c->SMR_f;
            stcSMR.RIE = TRUE;
            pstcI2c->SMR_f = stcSMR;
        }
        enResult = Ok;
    }
    else
    {
        pstcMfsHlInternData->u8Exec = MfsHlExecStby;
        enResult = ErrorInvalidMode;
    }

    return (enResult);
} /* MfsHlI2cStartSlave */

/**
 ******************************************************************************
 ** \brief Wait to change I2C interruption state
 **
 ** \param [in] pstcMfsHlInternData  Pointer to internal data
 **
 ** \retval Ok              Interruption state changed within the specified
 **                         period of time.
 ** \retval ErrorTimeout    Interruption state didn't change within the
 **                         specified period of time.
 **
 ******************************************************************************/
static en_result_t MfsHlI2cWaitIntState(volatile stc_mfs_hl_intern_data_t* pstcMfsHlInternData)
{
    en_result_t       enResult = ErrorTimeout;
    volatile uint32_t u32Count;
    uint32_t          u32MaxCnt;

    u32MaxCnt = Mfs_GetBusClock() / MFS_HL_NUM_10;
    u32Count  = 0u;
    while (u32Count < u32MaxCnt)
    {
        /* Wait until tx or rx is completed */
        if (MfsHlExecStby == pstcMfsHlInternData->u8Exec)
        {
            enResult = Ok;
            break;
        }
        u32Count++;
    }
    pstcMfsHlInternData->u8Exec = MfsHlExecStby;

    return (enResult);
} /* MfsHlI2cWaitIntState */

/**
 ******************************************************************************
 ** \brief Check changing I2C interruption state
 **
 ** \param [in] pstcMfsHlInternData  Pointer to internal data
 ** \param [in] u32MaxCnt            Maximum counter to check state changing
 **
 ** \retval Ok                       Interruption state changed within the
 **                                  specified period of count.
 ** \retval ErrorOperationInProgress Interruption state doesn't change and
 **                                  counter isn't overflow yet.
 ** \retval ErrorTimeout             Interruption state didn't change within
 **                                  the specified period of count.
 **
 ******************************************************************************/
static en_result_t MfsHlI2cChkTxRxComplete(stc_mfs_hl_intern_data_t* pstcMfsHlInternData,
                                           uint32_t                  u32MaxCnt)
{
    en_result_t enResult;

    /* If tx or rx is completed, return OK */
    if (MfsHlExecStby == pstcMfsHlInternData->u8Exec)
    {
        enResult = Ok;
    }
    else
    {
        enResult = ErrorOperationInProgress;
        /* If tx or rx is proceeding, polling counter counts */
        pstcMfsHlInternData->u32I2cProcCnt++;
        if (u32MaxCnt <= pstcMfsHlInternData->u32I2cProcCnt)
        {
            enResult = ErrorTimeout;
            pstcMfsHlInternData->u8Exec = MfsHlExecStby;
        }
    }

    return (enResult);
}

/**
 ******************************************************************************
 ** \brief Write data to MFS_HL module for I2C
 **
 ** The data provided by Mfs_Hl_I2c_Write#pu8Data is used directly and
 ** the transmission (via TX interrupt) is started, if not ongoing already. 
 **
 ** If Mfs_Hl_I2c_Write#bBlocking is specified FALSE, the function will return
 ** immediately after datas are transfered to the remote device.
 **
 ** If Mfs_Hl_I2c_Write#bBlocking is specified TRUE, the function wait until
 ** all data is transferred to the MFS hardware FIFO. If the referenced MFS 
 ** does not have a FIFO single data is written.
 **
 ** Note: Don't access to the buffer which specified by pu8Data until transmission
 ** processing completes.
 **
 ** \param [in]     pstcI2c       Pointer to I2C (MFS) instance register area
 ** \param [in]     u8SlaveAddr   Slave address for master mode
 ** \param [in]     pu8Data       Pointer to data buffer for transmission
 ** \param [in,out] pu16WriteCnt  Pointrer to variable for number of bytes to
 **                               send(must be at least 1).
 **                               And pointrer to variable for number of bytes
 **                               been sent.
 ** \param [in]     bBlocking     If TRUE, synchronously wait until all data is
 **                               transferred into the hardware (Fifo) buffer.
 **                               If FALSE, return immediately.
 **
 ** \retval Ok                        Write data successfully done or started.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcI2c == NULL
 **             - pu8Data == NULL
 **             - pu16WriteCnt == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** \retval ErrorOperationInProgress  If the following condition is met:
 **             - A transmission or reception is still ongoing while another
 **               I2C operation should be started.  
 ** \retval ErrorTimeout              Interruption state didn't change within
 **                                   the specified period of time.
 **
 ******************************************************************************/
en_result_t Mfs_Hl_I2c_Write(volatile stc_mfsn_t* pstcI2c,
                             uint8_t              u8SlaveAddr,
                             uint8_t*             pu8Data,
                             uint16_t*            pu16WriteCnt,
                             boolean_t            bBlocking
                            )
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    stc_mfs_hl_buffer_t*      pstcBuffer;
    stc_mfs_smr_field_t       stcSMR;
    stc_mfs_i2c_ibcr_field_t  stcIBCR;
    stc_mfs_fcr1_field_t      stcFCR1;

    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcI2c);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    ||  (NULL == pu8Data)
    ||  (NULL == pu16WriteCnt)
       )
    {
        enResult = ErrorInvalidParameter;
    }

    /* Check the execution */
    else if (MfsHlExecStby != pstcMfsHlInternData->u8Exec)
    {
        enResult = ErrorOperationInProgress;
    }

    else
    {
        enResult = Ok;
        /* Check if nothing to do */
        if (0u != *pu16WriteCnt)
        {
            /* Get ptr to internal transmit Buffer */
            pstcBuffer = &pstcMfsHlInternData->stcTxBuffer;

            /* Preset buffer */
            pstcBuffer->pu8Buffer = pu8Data;
            pstcBuffer->u16BufferSize = *pu16WriteCnt;
            pstcBuffer->u16OutIndex = 0u;

            /* Change state */
            pstcMfsHlInternData->u8Exec = MfsHlExecTransmitting;

            /* Master mode */
            if (MfsHlModeI2cMaster == pstcMfsHlInternData->u8MfsMode)
            {
                /* tx */
                /* write slave address,  bit0 = 0 (tx) */
                u8SlaveAddr <<= MFS_HL_NUM_1;
                u8SlaveAddr |= (uint8_t)(MfsI2cWrite);
                pstcI2c->TDR = (uint16_t)(u8SlaveAddr);

                stcIBCR = pstcI2c->IBCR_f;
                /* Set master mode */
                stcIBCR.MSS  = TRUE;
                /* Enable ACK */
                stcIBCR.ACKE = TRUE;
                /* Enable interrupt */
                stcIBCR.INTE = TRUE;
                stcIBCR.ACT_SCC = FALSE;
                /* wait select */
                stcIBCR.WSEL = FALSE;
                pstcI2c->IBCR_f = stcIBCR;

                /* FIFO is used */
                if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
                {
                    /* tx FIFO interrupt enable : interruption occur */
                    stcFCR1 = pstcI2c->FCR1_f;
                    stcFCR1.FTIE = TRUE;
                    pstcI2c->FCR1_f = stcFCR1;
                }
                /* FIFO is not used */
                else
                {
                    /* tx interrupt enable : interruption occur */
                    stcSMR = pstcI2c->SMR_f;
                    stcSMR.TIE = TRUE;
                    pstcI2c->SMR_f = stcSMR;
                }
                enResult = Ok;
            }
            /* Slave mode */
            else
            {
                enResult = MfsHlI2cStartSlave(pstcI2c, pstcMfsHlInternData);
            }

            if (Ok == enResult)
            {
                /* Specified non-blocking call */
                if (FALSE == bBlocking)
                {
                    /* Polling counter for check status is clear */
                    pstcMfsHlInternData->u32I2cProcCnt = 0;
                }
                /* Specified blocking call */
                else
                {
                    /* Wait until TX is completed or error occur */
                    enResult = MfsHlI2cWaitIntState(pstcMfsHlInternData);

                    if (0u == pstcBuffer->u16OutIndex)
                    {
                        enResult = ErrorTimeout;
                    }
                    *pu16WriteCnt = pstcBuffer->u16OutIndex;
                }
            }
        }
    }

    return (enResult);
} /* Mfs_Hl_I2c_Write */

/**
 ******************************************************************************
 ** \brief Read received data from I2C
 **
 ** The received data is copied from Receive Data Register into the provided 
 ** data buffer Mfs_Hl_I2c_Read#pu8Data.
 ** The slave address (Mfs_Hl_I2c_Read#u8SlaveAddr) is used on master mode.
 ** The size is defined by Mfs_Hl_I2c_Read#pu16ReadCnt.
 ** If non-blocking is specified (Mfs_Hl_I2c_Read#bBlocking is FALSE), this 
 ** function will return immediately after available datas from remote device
 ** are received into the provided buffer. (Mfs_Hl_I2c_Read#pu8Data)
 **
 ** Anyway, Receive interrupt is used in any case.
 **
 ** Note: Don't access to the buffer which specified by pu8Data until reception
 ** processing completes.
 **
 ** \param [in]       pstcI2c         Pointer to I2C (MFS) instance register area.
 ** \param [in]       u8SlaveAddr     Slave Address.
 ** \param [in,out]   pu8Data         Buffer to store received characters.
 ** \param [in,out]   pu16ReadCnt     Pointrer to variable for number of bytes 
 **                                   been read.
 ** \param [in]       bBlocking       If TRUE, synchronously wait until pu16ReadCnt
 **                                   bytes have been received.
 **                                   If FALSE, return immediately.
 **
 ** \retval Ok                        Read data successfully done or started.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcI2c == NULL
 **             - pu8Data == NULL
 **             - pu16ReadCnt == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** \retval ErrorOperationInProgress  If the following condition is met:
 **             - A transmission or reception is still ongoing while another
 **               I2C operation should be started.  
 ** \retval ErrorTimeout              Interruption state didn't change within
 **                                   the specified period of time.
 **
 ******************************************************************************/
en_result_t Mfs_Hl_I2c_Read(volatile stc_mfsn_t* pstcI2c,
                            uint8_t              u8SlaveAddr,
                            uint8_t*             pu8Data,
                            uint16_t*            pu16ReadCnt,
                            boolean_t            bBlocking
                           )
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    stc_mfs_hl_buffer_t*      pstcBuffer;
    stc_mfs_i2c_ibcr_field_t  stcIBCR;
    uint16_t                  u16Count;
    uint16_t                  u16Cnt;

    /* Check for valid pointers, and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcI2c);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    ||  (NULL == pu8Data)
    ||  (NULL == pu16ReadCnt)
       )
    {
        enResult = ErrorInvalidParameter;
    }

    /* Check the execution */
    else if (MfsHlExecStby != pstcMfsHlInternData->u8Exec)
    {
        enResult = ErrorOperationInProgress;
    }

    else
    {
        /* Get ptr to internal reception Buffer */
        pstcBuffer = &pstcMfsHlInternData->stcRxBuffer;

        pstcBuffer->pu8Buffer = pu8Data;
        pstcBuffer->u16BufferSize = *pu16ReadCnt;
        pstcBuffer->u16InIndex = 0u;

        /* Change state */
        pstcMfsHlInternData->u8Exec = MfsHlExecReceiving;

        /* Master mode */
        if (MfsHlModeI2cMaster == pstcMfsHlInternData->u8MfsMode)
        {
            /* FIFO is used */
            if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
            {
                                                                      /* -1 is for SlaveAddr */
                u16Count = MfsHlGetMin((pstcBuffer->u16BufferSize),
                                       (MFS_FIFO_MAX_VAL - MFS_HL_NUM_1)
                                      );
                /* Set threshold value for RX FIFO */
                pstcI2c->FBYTE2 = (uint8_t)u16Count;
            }
            /* FIFO is not used */
            else
            {
                u16Count = 0u;
            }
            /* rx */
            /* write slave address,  bit0 = 1 (rx) */
            u8SlaveAddr <<= MFS_HL_NUM_1;
            u8SlaveAddr |= (uint8_t)(MfsI2cRead);
            pstcI2c->TDR = (uint16_t)(u8SlaveAddr);

            /* Write dummy data (0x00) for FIFO */
            for (u16Cnt = 0u; u16Cnt < u16Count; u16Cnt++)
            {
                pstcI2c->TDR = 0x00u;
            }

            stcIBCR = pstcI2c->IBCR_f;
            /* Enable ACK */
            stcIBCR.ACKE = TRUE;
            /* wait select */
            stcIBCR.WSEL = TRUE;
            /* interrup enable */
            stcIBCR.INTE = TRUE;
            /* clear interrupt */
            stcIBCR.ACT_SCC = FALSE;
            stcIBCR.INT     = FALSE;
            /* Set master mode */
            stcIBCR.MSS  = TRUE;
            pstcI2c->IBCR_f = stcIBCR;

            enResult = Ok;
        }
        /* Slave mode */
        else
        {
            enResult = MfsHlI2cStartSlave(pstcI2c, pstcMfsHlInternData);
        }

        if (Ok == enResult)
        {
            /* Specified non-blocking call */
            if (FALSE == bBlocking)
            {
                /* Polling counter for check status is clear */
                pstcMfsHlInternData->u32I2cProcCnt = 0;
            }
            /* Specified blocking call */
            else
            {
                /* Wait until TX is completed or error occur */
                enResult = MfsHlI2cWaitIntState(pstcMfsHlInternData);

                if ((Ok != enResult)
                ||  (0u == pstcBuffer->u16InIndex))
                {
                    *pu16ReadCnt = 0u;
                    enResult = ErrorTimeout;
                }
                else
                {
                    *pu16ReadCnt = pstcBuffer->u16InIndex;
                }
            }
        }
    }

    return (enResult);
} /* Mfs_Hl_I2c_Read */

/**
 ******************************************************************************
 ** \brief Check to complete I2C transmission
 **
 ** If Mfs_Hl_I2c_Write() is used by non-blocking, please call this function
 ** at the application periodically because execution status doesn't change to
 ** standby if stop condition isn't detected.
 **
 ** \param [in]       pstcI2c         Pointer to I2C (MFS) instance register area.
 ** \param [in]       u32MaxCnt       Maximum period to check state changing
 **
 ** \retval Ok                        Write data successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcI2c == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS))
 ** \retval ErrorInvalidMode          A reception is still ongoing.  
 ** \retval ErrorOperationInProgress  A transmission is still ongoing.
 ** \retval ErrorTimeout              A transmission didn't complete within
 **                                   the specified period.
 **
 ******************************************************************************/
en_result_t Mfs_Hl_I2c_WaitTxComplete(volatile stc_mfsn_t* pstcI2c,
                                      uint32_t             u32MaxCnt
                                     )
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    stc_mfs_hl_buffer_t*      pstcBuffer;

    /* Check for valid pointers, and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcI2c);
    /* ... and check */
    if (NULL == pstcMfsHlInternData)
    {
        enResult = ErrorInvalidParameter;
    }

    /* Check the execution */
    else if ((MfsHlExecTransmitting != pstcMfsHlInternData->u8Exec)
    &&  (MfsHlExecStby != pstcMfsHlInternData->u8Exec))
    {
        enResult = ErrorInvalidMode;
    }

    else
    {
        /* Get ptr to internal transmit Buffer */
        pstcBuffer = &pstcMfsHlInternData->stcTxBuffer;

        enResult = Ok;
        if (MfsHlExecTransmitting == pstcMfsHlInternData->u8Exec)
        {
            /* Check tx or rx is completed or error occur */
            enResult = MfsHlI2cChkTxRxComplete(pstcMfsHlInternData, u32MaxCnt);
            if (ErrorOperationInProgress != enResult)
            {
                if (0u == pstcBuffer->u16OutIndex)
                {
                    enResult = ErrorTimeout;
                }
            }
        }
    }

    return (enResult);
} /* Mfs_Hl_I2c_WaitTxComplete */

/**
 ******************************************************************************
 ** \brief Check to complete I2C reception
 **
 ** If Mfs_Hl_I2c_Read() is used by non-blocking, please call this function
 ** at the application periodically because execution status doesn't change to
 ** standby if stop condition isn't detected.
 **
 ** \param [in]       pstcI2c         Pointer to I2C (MFS) instance register area.
 ** \param [in]       u32MaxCnt       Maximum period to check state changing
 **
 ** \retval Ok                        Write data successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcI2c == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS))
 ** \retval ErrorInvalidMode          A transmission is still ongoing.  
 ** \retval ErrorOperationInProgress  A reception is still ongoing.
 ** \retval ErrorTimeout              A reception didn't complete within
 **                                   the specified period.
 **
 ******************************************************************************/
en_result_t Mfs_Hl_I2c_WaitRxComplete(volatile stc_mfsn_t* pstcI2c,
                                      uint32_t             u32MaxCnt
                                     )
{
    en_result_t               enResult;
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    stc_mfs_hl_buffer_t*      pstcBuffer;

    /* Check for valid pointers, and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcI2c);
    /* ... and check */
    if (NULL == pstcMfsHlInternData)
    {
        enResult = ErrorInvalidParameter;
    }

    /* Check the execution */
    else if ((MfsHlExecReceiving != pstcMfsHlInternData->u8Exec)
    &&  (MfsHlExecStby != pstcMfsHlInternData->u8Exec))
    {
        enResult = ErrorInvalidMode;
    }

    else
    {
        enResult = Ok;
        if (MfsHlExecReceiving == pstcMfsHlInternData->u8Exec)
        {
            /* Check tx or rx is completed or error occur */
            enResult = MfsHlI2cChkTxRxComplete(pstcMfsHlInternData, u32MaxCnt);
            if (ErrorOperationInProgress != enResult)
            {
                /* Get ptr to internal reception Buffer */
                pstcBuffer = &pstcMfsHlInternData->stcRxBuffer;

                if ((Ok != enResult)
                ||  (0u == pstcBuffer->u16InIndex))
                {
                    enResult = ErrorTimeout;
                }
            }
        }
    }

    return (enResult);
} /* Mfs_Hl_I2c_WaitRxComplete */

/**
 ******************************************************************************
 ** \brief Write data to MFS_HL module synchronously or asynchronously
 **
 ** This function can use for UART, CSIO or LIN.
 **
 ** The data provided by Mfs_Hl_Write#pu8Data is copied into the internal
 ** TX buffer and the transmission (via TX interrupt) is started, if transmission
 ** is not ongoing already. 
 ** Depending on the Mfs_Hl_Write#bBlocking parameter, the function return behavior
 ** is different.
 **
 ** For an asynchronous (non-blocking) call (Mfs_Hl_Write#bBlocking = FALSE),
 ** the free size of the internal buffer must be sufficient to take all data
 ** (Mfs_Hl_Write#pu8Data) of length Mfs_Hl_Write#u16WriteCnt, otherwise
 ** the function will return ErrorBufferFull. 
 ** After all data is copied into the internal buffer, the function will return 
 ** immediately. The transmission may be pending when the function returns.
 **
 ** For a synchronous (blocking) call (Mfs_Hl_Write#bBlocking = TRUE),
 ** the function will wait until all data is transferred to the MFS hardware FIFO.
 ** The transmission  may be pending when the function returns.
 ** If the referenced MFS does not have a FIFO single data is written.
 **
 ** However, when call-back for transmission is not set, this function executes
 ** only by blocking.
 **
 ** \param [in]  pstcMfs       Pointer to MFS instance register area
 ** \param [in]  pu8Data       Transmit data buffer holding data to transmit
 ** \param [in]  u16WriteCnt   Number of characters to write, must be at least 1
 ** \param [in]  bBlocking     If TRUE, synchronously wait until all data is
 **                            transferred into the hardware Fifo buffer
 **                            If FALSE, put data into internal TX buffer and return 
 **                            immediately
 **                            But this parameter isn't effective when the 
 **                            call-back for transmission isn't set.
 ** \param [in]  bCsHolding    Specify to hold chip select
 **                            This parameter is only effective when mode is
 **                            CSIO/SPI master and chip select function is used.
 **
 ** \retval Ok                        Transmit data successfully done or started.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcMfs == NULL
 **             - pu8Data == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** \retval ErrorOperationInProgress  A transmission or reception is still 
 **                                   ongoing
 ** \retval ErrorBufferFull           Insufficient free size of TX buffer to 
 **                                   take all data (in case of 
 **                                   Mfs_Hl_Write#bBlocking = FALSE only)
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Write(volatile stc_mfsn_t* pstcMfs,
                         uint8_t*             pu8Data,
                         uint16_t             u16WriteCnt,
                         boolean_t            bBlocking,
                         boolean_t            bCsHolding
                        )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    stc_mfs_hl_buffer_t*      pstcBuffer;
    uint_fast16_t             fu16DataSent;
    uint_fast16_t             fu16Idx;
    boolean_t                 bTxInProgress;

    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcMfs);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    ||  (NULL == pu8Data)
       )
    {
        return (ErrorInvalidParameter);
    }

    /* Check if nothing to do */
    if (0u == u16WriteCnt)
    {
        return (Ok);
    }

    /* Check the execution */
    if (MfsHlExecStby != pstcMfsHlInternData->u8Exec)
    {
        return (ErrorOperationInProgress);
    }

    /* Get ptr to internal transmit Buffer */
    pstcBuffer = &pstcMfsHlInternData->stcTxBuffer;

    /* Check if ring buffer can take all bytes (blocking only) */
    if ((FALSE == bBlocking) &&
        (u16WriteCnt > (pstcBuffer->u16BufferSize - pstcBuffer->u16FillCount))
       )
    {
        /* not enough space left if non-blocking mode is requested */
        return (ErrorBufferFull);
    }

    /* Change state */
    pstcMfsHlInternData->u8Exec = MfsHlExecTransmitting;

    /* Set total tx bytes (for CSIO timer mode) */
    pstcBuffer->u16TxCount = u16WriteCnt;
    /* Clear total tx bytes */
    pstcBuffer->u16TxCounter = 0u;

    /* CSIO timer mode */
    if (MfsHlModeCsioNormalMasterTimer == pstcMfsHlInternData->u8MfsMode)
    {
        /* Enable serial timer */
        pstcMfs->SCR_f.TXE = FALSE;
        pstcMfs->SACSR_f.TMRE = TRUE;
        pstcMfs->SCR_f.TXE = TRUE;
    }
    /* Loop until all data has been sent (blocking only) */
    /* If non-blocking mode is requested, it is guaranteed here that the */
    /* provided data will fit into the tx buffer */
    while (0u < u16WriteCnt)
    {
        /* Use interruption */
        if (NULL != pstcMfsHlInternData->pfnTransmitCbFunction)
        {
            /* Check for transmission ongoing */
            bTxInProgress = pstcMfs->SSR_f.TDRE;
            
            /* In case, a transmission is already pending */
            if (TRUE == bTxInProgress)
            {
                if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
                {  
                    /* Disable transmission Fifo interrupt during copy */
                    pstcMfs->FCR1_f.FTIE = FALSE;
                }
                else /* if (pstcMfsInternData->enFifoAvailable == MfsHasFifo) */
                {
                    /* Disable transmission interrupt during copy */
                    pstcMfs->SCR_f.TIE = FALSE;
                }
            }
        }

        /* Copy data to provided destinaton buffer and save # bytes been read */
        /* determine free size in TX buffer */
        fu16DataSent = (uint_fast16_t)MfsHlGetMin(((pstcBuffer->u16BufferSize) - (pstcBuffer->u16FillCount)),
                                                    u16WriteCnt
                                                 );

        if (MfsHlModeCsioSpiMasterWithCs == pstcMfsHlInternData->u8MfsMode)
        {
            if (MFS_CSIO_TBYTE_MAX < fu16DataSent)
            {
                fu16DataSent = MFS_CSIO_TBYTE_MAX;
            }
        }

        /* store bytes in TX buffer */
        for (fu16Idx = 0u; fu16Idx < fu16DataSent; fu16Idx++)
        {
            pstcBuffer->pu8Buffer[pstcBuffer->u16InIndex] = pu8Data[fu16Idx];

            /* Update in index */
            pstcBuffer->u16InIndex++;
            if (pstcBuffer->u16InIndex == pstcBuffer->u16BufferSize)
            {
                pstcBuffer->u16InIndex = 0u; /* wrapped around */
            }
        }

        pstcBuffer->u16FillCount += (uint16_t)fu16DataSent;
        u16WriteCnt              -= (uint16_t)fu16DataSent;

        /* Set tx bytes when chip select is used on master mode */
        if (MfsHlModeCsioSpiMasterWithCs == pstcMfsHlInternData->u8MfsMode)
        {
            pstcMfs->SCR_f.TXE = FALSE;
            /* Set size to active chip select */
            pstcMfs->TBYTE0 = (uint8_t)fu16DataSent;
            if ((0u != u16WriteCnt) || (TRUE == bCsHolding))
            {
                /* Hold chip select */
                pstcMfs->SCSCR_f.SCAM = TRUE;
            }
            else
            {
                /* In-active chip select when transmit is end */
                pstcMfs->SCSCR_f.SCAM = FALSE;
            }
            pstcMfs->SCR_f.TXE = TRUE;
        }
        /* Use interruption */
        if (NULL != pstcMfsHlInternData->pfnTransmitCbFunction)
        {
            if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
            {
                /* Now enable transmission Fifo interrupt to trigger send operation */
                pstcMfs->FCR1_f.FTIE = TRUE;
            }
            else /* if (MfsHlUseNoFifo == pstcMfsHlInternData->u8FifoUsage) */
            {
                /* Now enable transmission interrupt to trigger send operation */
                pstcMfs->SCR_f.TIE = TRUE;
            }
        }
        /* No use interruption */
        else
        {
            /* Wait until all data has been tranferred to the MFS HW FIFO */
            while (0u != pstcBuffer->u16FillCount)
            {
                /* If Transmit Data Register (TDR) is empty */
                if (TRUE == pstcMfs->SSR_f.TDRE)
                {
                    /* Write the contents of the buffer to Transmit Data Register */
                    MfsHlWriteBuf(pstcMfs, pstcMfsHlInternData);
                }
            }
            if (0u == u16WriteCnt)
            {
                /* CSIO timer mode */
                if (MfsHlModeCsioNormalMasterTimer == pstcMfsHlInternData->u8MfsMode)
                {
                    /* Wait for TX bus idle */
                    while (FALSE == pstcMfs->SSR_f.TBI)
                    {
                        /* There is no process that should be executed. */
                        ;
                    }
                    /* Disable serial timer */
                    pstcMfs->SCR_f.TXE    = FALSE;
                    pstcMfs->SACSR_f.TMRE = FALSE;
                    pstcMfs->SCR_f.TXE    = TRUE;
                }
                pstcMfsHlInternData->u8Exec = MfsHlExecStby;
            }
        }
    } /* while (u16WriteCnt) */

    /* Wait until all data has been tranferred to the MFS HW FIFO (when blocking) */
    if (TRUE == bBlocking)
    {
        while (MfsHlExecStby != pstcMfsHlInternData->u8Exec)
        {
            PDL_WAIT_LOOP_HOOK();
        }
    }

    return (Ok);
} /* Mfs_Hl_Write */

/**
 ******************************************************************************
 ** \brief Read received data from MFS_HL module synchronously or asynchronously
 **
 ** This function can use for UART, CSIO or LIN.
 **
 ** The received data is copied from internal RX buffer (filled by RX interrupt)
 ** into the provided data buffer Mfs_Hl_Read#pu8Data. The size is defined
 ** by Mfs_Hl_Read#pu16DataCnt. Depending on the Mfs_Hl_Read#bBlocking
 ** parameter, the function behavior is different.
 **
 ** For an asynchronous (non-blocking) call (Mfs_Hl_Read#bBlocking = FALSE),
 ** the function will return immediately after all currently available characters
 ** (in SW ring buffer and HW FIFO) are copied into the provided buffer 
 ** (Mfs_Hl_Read#pu8Data) or the maximum count (Mfs_Hl_Read#u16ReadCnt)
 ** is reached. The value returned by Mfs_Hl_Read#pu16DataCnt gives the 
 ** count of characters that was read actually.
 ** If the referenced MFS does not have a FIFO single data is read.
 **
 ** For a synchronous (blocking) call (Mfs_Hl_Read#bBlocking == TRUE),
 ** the function will return after the requested count of characters
 ** (Mfs_Hl_Read#pu16DataCnt) is received completely.
 ** This should be used with caution as the full application can get stuck
 ** if no further data is received.
 **
 ** \param [in]       pstcMfs         Pointer to MFS instance register area
 ** \param [in,out]   pu8Data         Buffer to store received data
 ** \param [in,out]   pu16DataCnt     Pointer to variable for number of bytes
 **                                   been read
 ** \param [in]       u16ReadCnt      Maximum number of characters to read 
 **                                   (ensure sufficient size of pu16DataCnt#pu8Data!)
 ** \param [in]       bBlocking       If TRUE, synchronously wait until 
 **                                   pu16DataCnt#u16ReadCnt bytes have been
 **                                   received.
 **                                   If FALSE, read all available and
 **                                   return immediately.
 **
 ** \retval Ok                        Read data successfully done or started.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcMfs == NULL
 **             - pu8Data == NULL
 **             - pu16DataCnt == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_MFS)) 
 ** \retval ErrorOperationInProgress  If the following condition is met:
 **             - An asynchronous transmission is still ongoing while another
 **               asynchronous operation should be started.
 **
 ******************************************************************************/
en_result_t Mfs_Hl_Read(volatile stc_mfsn_t* pstcMfs,
                        uint8_t*             pu8Data,
                        uint16_t*            pu16DataCnt,
                        uint16_t             u16ReadCnt,
                        boolean_t            bBlocking
                       )
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
    stc_mfs_hl_buffer_t*      pstcBuffer;
    uint_fast16_t             fu16Idx;
    uint_fast16_t             fu16Length;
    uint16_t                  u16BytesToReadLeft;
    volatile uint8_t          u8Ssr;
    volatile uint8_t          u8SsrErr;

    if (NULL != pu16DataCnt)
    {
        *pu16DataCnt = 0u;   /* Preset to default */
    }

    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcMfs);
    /* ... and check */
    if ((NULL == pstcMfsHlInternData)
    ||  (NULL == pu8Data)
    ||  (NULL == pu16DataCnt)
       )
    {
        return (ErrorInvalidParameter);
    }

    /* Check the execution */
    if (MfsHlExecStby != pstcMfsHlInternData->u8Exec)
    {
        return (ErrorOperationInProgress);
    }

    /* Save Read Count for later use */
    u16BytesToReadLeft = u16ReadCnt;

    /* Check for nothing to do */
    if (0u == u16ReadCnt)
    {
        return (Ok);
    }

    /* Get ptr to internal receive Buffer */
    pstcBuffer = &pstcMfsHlInternData->stcRxBuffer;

    /* Adjust FIFO, if u16ReadCnt less than FIFO depth */
    if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
    {
        if (MFS_FIFO_MAX_VAL > u16ReadCnt)
        {
            pstcMfs->FBYTE2 = (uint8_t)u16ReadCnt;
        }
        else
        {
            pstcMfs->FBYTE2 = MFS_FIFO_MAX_VAL;
        }
    }

    /* Use interruption */
    if (NULL != pstcMfsHlInternData->pfnReceiveCbFunction)
    {
        if (FALSE == pstcMfs->SCR_f.RIE)
        {
            /* Enable reception interrupt */
            pstcMfs->SCR_f.RIE = TRUE;
        }
    }

    /* Read all available bytes from ring buffer, blocking. */
    while (0u < u16BytesToReadLeft)
    {
        /* Blocking read */
        if (TRUE == bBlocking)
        {
            while (0u == pstcBuffer->u16FillCount)
            {
                /* No use interruption */
                if (NULL == pstcMfsHlInternData->pfnReceiveCbFunction)
                {
                    /* Read status */
                    u8Ssr = u8SsrErr = pstcMfs->SSR;
                    /* Error is different each mode */
                    switch (pstcMfsHlInternData->u8MfsMode & MFS_HL_MODE_GROUP_CHK)
                    {
                        /* UART */
                        case MFS_HL_MODE_GROUP_UART:
                            /* Check Overrun/Framing/Parity error */
                            u8SsrErr &= MFS_UART_SSR_ERR;
                            break;
                        /* CSIO */
                        case MFS_HL_MODE_GROUP_CSIO:
                            /* Check Overrun error */
                            u8SsrErr &= MFS_CSIO_SSR_ERR;
                            break;
                        /* LIN */
                        case MFS_HL_MODE_GROUP_LIN:
                            /* Check Overrun/Framing error */
                            u8SsrErr &= MFS_LIN_SSR_ERR;
                            break;
                        default:
                            u8SsrErr = 0u;
                            break;
                    }
                    /* Error */
                    if(0u != u8SsrErr)
                    {
                        /* Clear possible reception errors */
                        pstcMfs->SSR_f.REC = TRUE;
                        
                        /* If error occurs when receiving data, return error. */
                        return Error;
                    }
                    /* If received data is full... */
                    if (0u != (u8Ssr & MFS_SSR_RDRF))
                    {
                        /* Store Received Data Register into buffer */
                        MfsHlReadBuf(pstcMfs, pstcMfsHlInternData);
                    }
                }
                else
                {
                    /* Wait until at least one byte is available */
                    PDL_WAIT_LOOP_HOOK();
                }
            }
        }
        else
        {
            /* No use interruption */
            if (NULL == pstcMfsHlInternData->pfnReceiveCbFunction)
            {
                /* Read status */
                u8Ssr = u8SsrErr = pstcMfs->SSR;
                /* Error is different each mode */
                switch (pstcMfsHlInternData->u8MfsMode & MFS_HL_MODE_GROUP_CHK)
                {
                    /* UART */
                    case MFS_HL_MODE_GROUP_UART:
                        /* Check Overrun/Framing/Parity error */
                        u8SsrErr &= MFS_UART_SSR_ERR;
                        break;
                    /* CSIO */
                    case MFS_HL_MODE_GROUP_CSIO:
                        /* Check Overrun error */
                        u8SsrErr &= MFS_CSIO_SSR_ERR;
                        break;
                    /* LIN */
                    case MFS_HL_MODE_GROUP_LIN:
                        /* Check Overrun/Framing error */
                        u8SsrErr &= MFS_LIN_SSR_ERR;
                        break;
                    default:
                        u8SsrErr = 0u;
                        break;
                }
                /* Error */
                if(0u != u8SsrErr)
                {
                    /* Clear possible reception errors */
                    pstcMfs->SSR_f.REC = TRUE;
                    /* If error occurs when receiving data, return error. */
                    return Error;
                }
                /* If received data is full... */
                if (0u != (u8Ssr & MFS_SSR_RDRF))
                {
                    /* Store Received Data Register into buffer */
                    MfsHlReadBuf(pstcMfs, pstcMfsHlInternData);
                }
            }
            if (0u == pstcBuffer->u16FillCount)
            {
                return (Ok);
            }
        }

        /* Disable reception interrupt */
        pstcMfs->SCR_f.RIE = FALSE;

        /* Copy data to destinaton buffer and save no. of bytes been read */
        /* get number of bytes to read */
        fu16Length = (uint_fast16_t)MfsHlGetMin(pstcBuffer->u16FillCount, u16BytesToReadLeft);

        /* if there are any bytes left to read */
        if (0u != fu16Length)
        {
            /* read bytes out of RX buffer */
            for (fu16Idx = *pu16DataCnt; fu16Idx < (fu16Length + *pu16DataCnt); fu16Idx++)
            {
                pu8Data[fu16Idx] = pstcBuffer->pu8Buffer[pstcBuffer->u16OutIndex];
                /* Update out index */
                pstcBuffer->u16OutIndex++;
                if (pstcBuffer->u16OutIndex == pstcBuffer->u16BufferSize)
                {
                    pstcBuffer->u16OutIndex = 0u;
                }
            }
            pstcBuffer->u16FillCount -= (uint16_t)fu16Length; /* Update fill counter */
        }

        *pu16DataCnt       += (uint16_t)fu16Length; /* Provide no. of read to the caller */
        u16BytesToReadLeft -= (uint16_t)fu16Length; /* Some data processed */

        /* Adjust FIFO, if bytes to read left less than FIFO depth */
        if (MfsHlUseFifo == pstcMfsHlInternData->u8FifoUsage)
        {
            if (MFS_FIFO_MAX_VAL > u16BytesToReadLeft)
            {
                if (0u == u16BytesToReadLeft)
                {
                    pstcMfs->FBYTE2 = MFS_RX_FIFO_MIN_VAL;
                }
                else
                {
                    pstcMfs->FBYTE2 = (uint8_t)u16BytesToReadLeft;
                }
            }
        }
        /* Use interruption */
        if (NULL != pstcMfsHlInternData->pfnReceiveCbFunction)
        {
            /* Reenable reception interrupt */
            pstcMfs->SCR_f.RIE = TRUE;
        }
    }

    return (Ok);
} /* Mfs_Hl_Read */


/**
 ******************************************************************************
 ** \brief Get the internal TX or RX buffer pointer
 **
 ** The buffer information can be gotten with this pointer.
 **
 ** \param [in]       pstcMfs         Pointer to MFS instance register area.
 ** \param [in]       enBufIndex      Buffer index
 ** \param [out]      ppstcBuffer     Pointer to the pointer of RX or TX buffer
 **
 ** \retval Ok                        Write data successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcMfs == NULL
 **             - pstcMfsHlInternData == NULL (invalid or disabled MFS unit
 **                                           (PDL_PERIPHERAL_ENABLE_MFS))
 **             - (enBuf != MfsHlRxBuffer) && (enBuf != MfsHlTxBuffer) 
 **
 ******************************************************************************/
en_result_t Mfs_Hl_GetBufferPointer(volatile stc_mfsn_t* pstcMfs,
                                    en_mfs_hl_buffer_t   enBufIndex,
                                    stc_mfs_hl_buffer_t** ppstcBuffer)
{
    stc_mfs_hl_intern_data_t* pstcMfsHlInternData;
  
    /* Check for valid pointers and get pointer to internal data struct ... */
    pstcMfsHlInternData = MfsHlGetInternDataPtr(pstcMfs);
    /* ... and check */
    if (NULL == pstcMfsHlInternData)
    {
        return (ErrorInvalidParameter);
    }
    
    switch(enBufIndex)
    {
        case MfsHlRxBuffer:
            *ppstcBuffer = &pstcMfsHlInternData->stcRxBuffer;
            break;
        case MfsHlTxBuffer:  
            *ppstcBuffer = &pstcMfsHlInternData->stcTxBuffer;
            break;
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}
                                               
//@} // MfshlGroup

#endif /* #if (PDL_MFS_USE_HL == PDL_ON) */

#endif /* #if (defined(PDL_PERIPHERAL_MFS_ACTIVE)) */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
