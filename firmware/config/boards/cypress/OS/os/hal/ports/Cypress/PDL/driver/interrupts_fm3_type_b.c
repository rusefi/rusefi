/*******************************************************************************
* \file             interrupts_fm3_type_b.c
* 
* \version          1.0
*                       
* \brief            Low-level interrupt processing layer to handle the hardware
*                   interrupts.
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

#include "interrupts_fm3.h"

#ifdef __INTERRUPTS_FM3_TYPE_B_C__

/******************************************************************************/
/******************************* Dummy Handler ********************************/
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Dummy Handler for not implemented peripheral handlers. During
 **        debugging a breakpoint may be set here. The argument #u32Caller
 **        contains the caller index from the handlers above.
 ******************************************************************************/

void PDL_DummyHandler(uint32_t u32Caller)
{

    // 0: RCINTSEL0, 1: RCINTSEL1, 2: RCINTSEL2, 3: RCINTSEL3, 4: RCINTSEL4, 
    // 5: RCINTSEL5, 6: RCINTSEL6, 7: RCINTSEL7, 8: FLASH, 

    u32Caller = u32Caller;
    while (1u)
    { }
}

/******************************************************************************/
/********************************** FM3: CSV **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CSV)
/**
 ******************************************************************************
 ** \brief FM3: CSV IRQ handler (IRQ#0) Type b
 ******************************************************************************/
    #if (1u == IRQ_CSV_AVAILABLE)
    void CSV_IRQHandler(void)
    {
        Csv_IrqHandler();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: SWWDG *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_SWWDG)
/**
 ******************************************************************************
 ** \brief FM3: SWWDG IRQ handler (IRQ#1) Type b
 ******************************************************************************/
    #if (1u == IRQ_SWDT_AVAILABLE)
    void SWDT_IRQHandler(void)
    {
        SwwdgIrqHandler();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM3: LVD **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_LVD)
/**
 ******************************************************************************
 ** \brief FM3: LVD IRQ handler (IRQ#2) Type b
 ******************************************************************************/
    #if (1u == IRQ_LVD_AVAILABLE)
    void LVD_IRQHandler(void)
    {
        Lvd_IrqHandler();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM3: RCINTSEL0 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL0)
/**
 ******************************************************************************
 ** \brief FM3: RCINTSEL0 IRQ handler (IRQ#3) Type b
 ******************************************************************************/
    #if (1u == IRQ_RCINTSEL0_AVAILABLE)
    void RCINTSEL0_IRQHandler(void)
    {
        PDL_DummyHandler(0ul);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM3: RCINTSEL1 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL1)
/**
 ******************************************************************************
 ** \brief FM3: RCINTSEL1 IRQ handler (IRQ#4) Type b
 ******************************************************************************/
    #if (1u == IRQ_RCINTSEL1_AVAILABLE)
    void RCINTSEL1_IRQHandler(void)
    {
        PDL_DummyHandler(1ul);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM3: RCINTSEL2 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL2)
/**
 ******************************************************************************
 ** \brief FM3: RCINTSEL2 IRQ handler (IRQ#5) Type b
 ******************************************************************************/
    #if (1u == IRQ_RCINTSEL2_AVAILABLE)
    void RCINTSEL2_IRQHandler(void)
    {
        PDL_DummyHandler(2ul);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM3: RCINTSEL3 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL3)
/**
 ******************************************************************************
 ** \brief FM3: RCINTSEL3 IRQ handler (IRQ#6) Type b
 ******************************************************************************/
    #if (1u == IRQ_RCINTSEL3_AVAILABLE)
    void RCINTSEL3_IRQHandler(void)
    {
        PDL_DummyHandler(3ul);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM3: RCINTSEL4 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL4)
/**
 ******************************************************************************
 ** \brief FM3: RCINTSEL4 IRQ handler (IRQ#7) Type b
 ******************************************************************************/
    #if (1u == IRQ_RCINTSEL4_AVAILABLE)
    void RCINTSEL4_IRQHandler(void)
    {
        PDL_DummyHandler(4ul);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM3: RCINTSEL5 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL5)
/**
 ******************************************************************************
 ** \brief FM3: RCINTSEL5 IRQ handler (IRQ#8) Type b
 ******************************************************************************/
    #if (1u == IRQ_RCINTSEL5_AVAILABLE)
    void RCINTSEL5_IRQHandler(void)
    {
        PDL_DummyHandler(5ul);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM3: RCINTSEL6 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL6)
/**
 ******************************************************************************
 ** \brief FM3: RCINTSEL6 IRQ handler (IRQ#9) Type b
 ******************************************************************************/
    #if (1u == IRQ_RCINTSEL6_AVAILABLE)
    void RCINTSEL6_IRQHandler(void)
    {
        PDL_DummyHandler(6ul);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM3: RCINTSEL7 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL7)
/**
 ******************************************************************************
 ** \brief FM3: RCINTSEL7 IRQ handler (IRQ#10) Type b
 ******************************************************************************/
    #if (1u == IRQ_RCINTSEL7_AVAILABLE)
    void RCINTSEL7_IRQHandler(void)
    {
        PDL_DummyHandler(7ul);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/************************** FM3: MFT0_WFG_DTIF, MFS8 **************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_WFG_DTIF) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8)
/**
 ******************************************************************************
 ** \brief FM3: MFT0_WFG_DTIF, MFS8 IRQ handler (IRQ#11) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_WFG_DTIF_MFS8_RX_TX_AVAILABLE)
    void MFT0_WFG_DTIF_MFS8_RX_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ11MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_WFG_DTIF)
        if (0ul != (u32IrqMon & 0x0000000Ful))
        {
            Mft_Wfg_IrqHandler((volatile stc_mftn_wfg_t*)&MFT0_WFG, &m_astcMftWfgInstanceDataLut[WfgInstanceIndexWfg0].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8 )
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS8_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS8_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS8_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS8_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8 )
        if (0ul != (u32IrqMon & 0x00000020ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS8_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS8_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS8_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS8_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8 )
        if (0ul != (u32IrqMon & 0x00000040ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS8_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS8_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS8_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN8, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs8].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/**** FM3: EXINT0, EXINT1, EXINT2, EXINT3, EXINT4, EXINT5, EXINT6, EXINT7 *****/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
/**
 ******************************************************************************
 ** \brief FM3: EXINT0, EXINT1, EXINT2, EXINT3, EXINT4, EXINT5, EXINT6, EXINT7 IRQ handler (IRQ#12) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT0_7_AVAILABLE)
    void EXINT0_7_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ12MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint0);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint1);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint2);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint3);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint4);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5)
        if (0ul != (u32IrqMon & 0x00000020ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint5);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6)
        if (0ul != (u32IrqMon & 0x00000040ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint6);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
        if (0ul != (u32IrqMon & 0x00000080ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint7);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/* FM3: EXINT8, EXINT9, EXINT10, EXINT11, EXINT12, EXINT13, EXINT14, EXINT15, EXINT16, EXINT17, EXINT18, EXINT19, EXINT20, EXINT21, EXINT22, EXINT23, EXINT24, EXINT25, EXINT26, EXINT27, EXINT28, EXINT30, EXINT31 */
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31)
/**
 ******************************************************************************
 ** \brief FM3: EXINT8, EXINT9, EXINT10, EXINT11, EXINT12, EXINT13, EXINT14, EXINT15, EXINT16, EXINT17, EXINT18, EXINT19, EXINT20, EXINT21, EXINT22, EXINT23, EXINT24, EXINT25, EXINT26, EXINT27, EXINT28, EXINT30, EXINT31 IRQ handler (IRQ#13) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT8_31_AVAILABLE)
    void EXINT8_31_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ13MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint8);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint9);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint10);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint11);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint12);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13)
        if (0ul != (u32IrqMon & 0x00000020ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint13);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14)
        if (0ul != (u32IrqMon & 0x00000040ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint14);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15)
        if (0ul != (u32IrqMon & 0x00000080ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint15);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16)
        if (0ul != (u32IrqMon & 0x00000100ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint16);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17)
        if (0ul != (u32IrqMon & 0x00000200ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint17);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18)
        if (0ul != (u32IrqMon & 0x00000400ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint18);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19)
        if (0ul != (u32IrqMon & 0x00000800ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint19);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20)
        if (0ul != (u32IrqMon & 0x00001000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint20);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21)
        if (0ul != (u32IrqMon & 0x00002000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint21);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22)
        if (0ul != (u32IrqMon & 0x00004000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint22);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23)
        if (0ul != (u32IrqMon & 0x00008000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint23);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24)
        if (0ul != (u32IrqMon & 0x00010000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint24);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25)
        if (0ul != (u32IrqMon & 0x00020000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint25);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26)
        if (0ul != (u32IrqMon & 0x00040000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint26);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27)
        if (0ul != (u32IrqMon & 0x00080000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint27);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28)
        if (0ul != (u32IrqMon & 0x00100000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint28);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28)
        if (0ul != (u32IrqMon & 0x00020000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint29);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30)
        if (0ul != (u32IrqMon & 0x00040000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint30);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31)
        if (0ul != (u32IrqMon & 0x00080000ul))
        {
            Exint_IrqHandler(ExintInstanceIndexExint31);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************************** FM3: DT0, QPRC0 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DT0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC0)
/**
 ******************************************************************************
 ** \brief FM3: DT0, QPRC0 IRQ handler (IRQ#14) Type b
 ******************************************************************************/
    #if (1u == IRQ_DT_QPRC_AVAILABLE)
    void DT_QPRC_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ14MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_DT0)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            DtIrqHandler(DtChannel0);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_DT0)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            DtIrqHandler(DtChannel1);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC0)
        if (0ul != (u32IrqMon & 0x000000FCul))
        {
            Qprc_IrqHandler((volatile stc_qprcn_t*)&QPRC0, &m_astcQprcInstanceDataLut[QprcInstanceIndexQprc0].stcInternData);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: MFS0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0)
/**
 ******************************************************************************
 ** \brief FM3: MFS0 IRQ handler (IRQ#15) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS0_RX_TX_AVAILABLE)
    void MFS0_RX_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ15MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS0_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS0_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS0_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS0_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS0_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS0_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS0_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS0_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS0_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS0_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS0_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN0, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs0].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: MFS1 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1)
/**
 ******************************************************************************
 ** \brief FM3: MFS1 IRQ handler (IRQ#16) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS1_RX_TX_AVAILABLE)
    void MFS1_RX_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ16MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS1_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS1_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS1_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS1_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS1_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS1_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS1_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS1_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS1_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS1_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS1_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: MFS2 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2)
/**
 ******************************************************************************
 ** \brief FM3: MFS2 IRQ handler (IRQ#17) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS2_RX_TX_AVAILABLE)
    void MFS2_RX_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ17MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS2_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS2_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS2_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS2_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS2_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS2_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS2_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS2_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS2_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS2_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS2_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN2, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs2].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: MFS3 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3)
/**
 ******************************************************************************
 ** \brief FM3: MFS3 IRQ handler (IRQ#18) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS3_RX_TX_AVAILABLE)
    void MFS3_RX_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ18MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS3_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS3_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS3_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS3_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS3_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS3_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS3_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS3_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS3_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS3_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS3_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: MFS4 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4)
/**
 ******************************************************************************
 ** \brief FM3: MFS4 IRQ handler (IRQ#19) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS4_RX_AVAILABLE)
    void MFS4_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ19MON;

    #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4 )
    if (0ul != (u32IrqMon & 0x00000001ul))
    {
        switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData.enMode)
        {
            case MfsUartMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS4_UART_BASE)
                MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                #endif
                break;
            case MfsCsioMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS4_CSIO_BASE)
                MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                #endif
                break;
            case MfsI2cMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS4_I2C_BASE)
                MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                #endif
                break;
            case MfsLinMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS4_LIN_BASE)
                MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                #endif
                break;
            default:
                break;
        }
    }
    #endif
    
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: MFS4 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4)
/**
 ******************************************************************************
 ** \brief FM3: MFS4 IRQ handler (IRQ#20) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS4_TX_AVAILABLE)
    void MFS4_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ20MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS4_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS4_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS4_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS4_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS4_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS4_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS4_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN4, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs4].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: MFS5 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5)
/**
 ******************************************************************************
 ** \brief FM3: MFS5 IRQ handler (IRQ#21) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS5_RX_AVAILABLE)
    void MFS5_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ21MON;

    #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5 )
    if (0ul != (u32IrqMon & 0x00000001ul))
    {
        switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData.enMode)
        {
            case MfsUartMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS5_UART_BASE)
                MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                #endif
                break;
            case MfsCsioMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS5_CSIO_BASE)
                MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                #endif
                break;
            case MfsI2cMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS5_I2C_BASE)
                MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                #endif
                break;
            case MfsLinMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS5_LIN_BASE)
                MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                #endif
                break;
            default:
                break;
        }
    }
    #endif
    
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: MFS5 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5)
/**
 ******************************************************************************
 ** \brief FM3: MFS5 IRQ handler (IRQ#22) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS5_TX_AVAILABLE)
    void MFS5_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ22MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS5_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS5_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS5_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS5_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS5_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS5_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS5_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN5, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs5].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM3: PPG0_20 ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_PPG0_20)
/**
 ******************************************************************************
 ** \brief FM3: PPG0_20 IRQ handler (IRQ#23) Type b
 ******************************************************************************/
    #if (1u == IRQ_PPG00_02_20_AVAILABLE)
    void PPG00_02_20_IRQHandler(void)
    {
        Ppg_IrqHandler();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/***************************** FM3: RTC, WC, CLK ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RTC) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_WC) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CLK)
/**
 ******************************************************************************
 ** \brief FM3: RTC, WC, CLK IRQ handler (IRQ#24) Type b
 ******************************************************************************/
    #if (1u == IRQ_TIM_AVAILABLE)
    void TIM_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ24MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_RTC)
        if (0ul != (u32IrqMon & 0x00000020ul))
        {
            Rtc_IrqHandler((stc_rtcn_t*)&RTC0, &(m_astcRtcInstanceDataLut[RtcInstanceIndexRtc0].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_WC)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            Wc_IrqHandler(&WC0);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CLK)
        if (0ul != (u32IrqMon & 0x00000007ul))
        {
            Clk_IrqHandler();
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: ADC0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC0)
/**
 ******************************************************************************
 ** \brief FM3: ADC0 IRQ handler (IRQ#25) Type b
 ******************************************************************************/
    #if (1u == IRQ_ADC0_AVAILABLE)
    void ADC0_IRQHandler(void)
    {
        AdcIrqHandler((volatile stc_adcn_t*)&ADC0, &(m_astcAdcInstanceDataLut[AdcInstanceIndexAdc0].stcInternData));
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: ADC1 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC1)
/**
 ******************************************************************************
 ** \brief FM3: ADC1 IRQ handler (IRQ#26) Type b
 ******************************************************************************/
    #if (1u == IRQ_ADC1_AVAILABLE)
    void ADC1_IRQHandler(void)
    {
        AdcIrqHandler((volatile stc_adcn_t*)&ADC1, &(m_astcAdcInstanceDataLut[AdcInstanceIndexAdc1].stcInternData));
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM3: ADC2, LCD *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_LCD)
/**
 ******************************************************************************
 ** \brief FM3: ADC2, LCD IRQ handler (IRQ#27) Type b
 ******************************************************************************/
    #if (1u == IRQ_ADC2_LCDC_AVAILABLE)
    void ADC2_LCDC_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ27MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC2)
        if (0ul != (u32IrqMon & 0x0000001Ful))
        {
            AdcIrqHandler((volatile stc_adcn_t*)&ADC2, &(m_astcAdcInstanceDataLut[AdcInstanceIndexAdc2].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_LCD)
        if (0ul != (u32IrqMon & 0x00000020ul))
        {
            Lcd_IrqHandler();
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************* FM3: MFT0_FRT, MFT1_FRT, MFT2_FRT **********************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_FRT) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_FRT) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_FRT)
/**
 ******************************************************************************
 ** \brief FM3: MFT0_FRT, MFT1_FRT, MFT2_FRT IRQ handler (IRQ#28) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_1_2_FRT_AVAILABLE)
    void MFT0_1_2_FRT_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ28MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_FRT)
        if (0ul != (u32IrqMon & 0x0000003Ful))
        {
            Mft_Frt_IrqHandler((volatile stc_mftn_frt_t*)&MFT0_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt0].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_FRT)
        if (0ul != (u32IrqMon & 0x00000FC0ul))
        {
            Mft_Frt_IrqHandler((volatile stc_mftn_frt_t*)&MFT1_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt1].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_FRT)
        if (0ul != (u32IrqMon & 0x0003F000ul))
        {
            Mft_Frt_IrqHandler((volatile stc_mftn_frt_t*)&MFT2_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt2].stcInternData);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************* FM3: MFT0_ICU, MFT1_ICU, MFT2_ICU **********************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_ICU) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_ICU) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_ICU)
/**
 ******************************************************************************
 ** \brief FM3: MFT0_ICU, MFT1_ICU, MFT2_ICU IRQ handler (IRQ#29) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_1_2_ICU_AVAILABLE)
    void MFT0_1_2_ICU_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ29MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_ICU)
        if (0ul != (u32IrqMon & 0x0000000Ful))
        {
            Mft_Icu_IrqHandler((volatile stc_mftn_icu_t*)&MFT0_ICU, &m_astcMftIcuInstanceDataLut[IcuInstanceIndexIcu0].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_ICU)
        if (0ul != (u32IrqMon & 0x000000F0ul))
        {
            Mft_Icu_IrqHandler((volatile stc_mftn_icu_t*)&MFT1_ICU, &m_astcMftIcuInstanceDataLut[IcuInstanceIndexIcu1].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_ICU)
        if (0ul != (u32IrqMon & 0x00000F00ul))
        {
            Mft_Icu_IrqHandler((volatile stc_mftn_icu_t*)&MFT2_ICU, &m_astcMftIcuInstanceDataLut[IcuInstanceIndexIcu2].stcInternData);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************* FM3: MFT0_OCU, MFT1_OCU, MFT2_OCU **********************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_OCU) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_OCU) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_OCU)
/**
 ******************************************************************************
 ** \brief FM3: MFT0_OCU, MFT1_OCU, MFT2_OCU IRQ handler (IRQ#30) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_1_2_OCU_AVAILABLE)
    void MFT0_1_2_OCU_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ30MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_OCU)
        if (0ul != (u32IrqMon & 0x0000003Ful))
        {
            Mft_Ocu_IrqHandler((volatile stc_mftn_ocu_t*)&MFT0_OCU, &m_astcMftOcuInstanceDataLut[OcuInstanceIndexOcu0].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_OCU)
        if (0ul != (u32IrqMon & 0x00000FC0ul))
        {
            Mft_Ocu_IrqHandler((volatile stc_mftn_ocu_t*)&MFT1_OCU, &m_astcMftOcuInstanceDataLut[OcuInstanceIndexOcu1].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_OCU)
        if (0ul != (u32IrqMon & 0x0003F000ul))
        {
            Mft_Ocu_IrqHandler((volatile stc_mftn_ocu_t*)&MFT2_OCU, &m_astcMftOcuInstanceDataLut[OcuInstanceIndexOcu2].stcInternData);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/************* FM3: BT0, BT1, BT2, BT3, BT4, BT5, BT6, BT7, FLASH *************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT3) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT4) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT5) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT6) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT7) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_FLASH)
/**
 ******************************************************************************
 ** \brief FM3: BT0, BT1, BT2, BT3, BT4, BT5, BT6, BT7, FLASH IRQ handler (IRQ#31) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT0_7_FLASHIF_AVAILABLE)
    void BT0_7_FLASHIF_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ31MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT0)
        if (0ul != (u32IrqMon & 0x00000003ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT0, &m_astcBtInstanceDataLut[BtInstanceIndexBt0].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT1)
        if (0ul != (u32IrqMon & 0x0000000Cul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT1, &m_astcBtInstanceDataLut[BtInstanceIndexBt1].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT2)
        if (0ul != (u32IrqMon & 0x00000030ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT2, &m_astcBtInstanceDataLut[BtInstanceIndexBt2].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT3)
        if (0ul != (u32IrqMon & 0x000000C0ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT3, &m_astcBtInstanceDataLut[BtInstanceIndexBt3].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT4)
        if (0ul != (u32IrqMon & 0x00000300ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT4, &m_astcBtInstanceDataLut[BtInstanceIndexBt4].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT5)
        if (0ul != (u32IrqMon & 0x00000C00ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT5, &m_astcBtInstanceDataLut[BtInstanceIndexBt5].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT6)
        if (0ul != (u32IrqMon & 0x00003000ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT6, &m_astcBtInstanceDataLut[BtInstanceIndexBt6].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT7)
        if (0ul != (u32IrqMon & 0x0000C000ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT7, &m_astcBtInstanceDataLut[BtInstanceIndexBt7].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_FLASH)
        if (0ul != (u32IrqMon & 0x08000000ul))
        {
            PDL_DummyHandler(8ul);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************************** FM3: NMI, HWWDG *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_NMI) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_HWWDG)
/**
 ******************************************************************************
 ** \brief FM3: NMI, HWWDG IRQ handler (EXC#2) Type b
 ******************************************************************************/
    #if (1u == IRQ_NMI_AVAILABLE)
void NMI_Handler(void)
{
    uint32_t u32IrqMon = FM_INTREQ->EXC02MON;

    #if (PDL_ON == PDL_INTERRUPT_ENABLE_NMI)
    if (0ul != (u32IrqMon & 0x00000001ul))
    {
        Exint_Nmi_IrqHandler();
    }
    #endif
    
    #if (PDL_ON == PDL_INTERRUPT_ENABLE_HWWDG)
    if (0ul != (u32IrqMon & 0x00000002ul))
    {
        HwwdgIrqHandler();
    }
    #endif
    
}
#else
    #error No IRQ handler found.
#endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


#endif // #ifdef __INTERRUPTS_FM3_TYPE_B_C__

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
