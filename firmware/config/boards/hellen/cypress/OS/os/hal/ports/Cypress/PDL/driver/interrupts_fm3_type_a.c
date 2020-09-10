/*******************************************************************************
* \file             interrupts_fm3_type_a.c
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

#ifdef __INTERRUPTS_FM3_TYPE_A_C__

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

    // 0: FLASH, 

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
 ** \brief FM3: CSV IRQ handler (IRQ#0) Type a
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
 ** \brief FM3: SWWDG IRQ handler (IRQ#1) Type a
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
 ** \brief FM3: LVD IRQ handler (IRQ#2) Type a
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
/********************* FM3: MFT0_WFG, MFT1_WFG, MFT2_WFG **********************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_WFG) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_WFG) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_WFG)
/**
 ******************************************************************************
 ** \brief FM3: MFT0_WFG, MFT1_WFG, MFT2_WFG IRQ handler (IRQ#3) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFT0_2_WFG_DTIF_AVAILABLE)
    void MFT0_2_WFG_DTIF_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ03MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_WFG)
        if (0ul != (u32IrqMon & 0x0000000Ful))
        {
            Mft_Wfg_IrqHandler((volatile stc_mftn_wfg_t*)&MFT0_WFG, &m_astcMftWfgInstanceDataLut[WfgInstanceIndexWfg0].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_WFG)
        if (0ul != (u32IrqMon & 0x000000F0ul))
        {
            Mft_Wfg_IrqHandler((volatile stc_mftn_wfg_t*)&MFT1_WFG, &m_astcMftWfgInstanceDataLut[WfgInstanceIndexWfg1].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_WFG)
        if (0ul != (u32IrqMon & 0x00000F00ul))
        {
            Mft_Wfg_IrqHandler((volatile stc_mftn_wfg_t*)&MFT2_WFG, &m_astcMftWfgInstanceDataLut[WfgInstanceIndexWfg2].stcInternData);
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
 ** \brief FM3: EXINT0, EXINT1, EXINT2, EXINT3, EXINT4, EXINT5, EXINT6, EXINT7 IRQ handler (IRQ#4) Type a
 ******************************************************************************/
    #if (1u == IRQ_EXINT0_7_AVAILABLE)
    void EXINT0_7_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ04MON;

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
 ** \brief FM3: EXINT8, EXINT9, EXINT10, EXINT11, EXINT12, EXINT13, EXINT14, EXINT15, EXINT16, EXINT17, EXINT18, EXINT19, EXINT20, EXINT21, EXINT22, EXINT23, EXINT24, EXINT25, EXINT26, EXINT27, EXINT28, EXINT30, EXINT31 IRQ handler (IRQ#5) Type a
 ******************************************************************************/
    #if (1u == IRQ_EXINT8_31_AVAILABLE)
    void EXINT8_31_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ05MON;

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
/*********************** FM3: DT0, QPRC0, QPRC1, QPRC2 ************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DT0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC2)
/**
 ******************************************************************************
 ** \brief FM3: DT0, QPRC0, QPRC1, QPRC2 IRQ handler (IRQ#6) Type a
 ******************************************************************************/
    #if (1u == IRQ_DT_QPRC0_2_AVAILABLE)
    void DT_QPRC0_2_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ06MON;

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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC1)
        if (0ul != (u32IrqMon & 0x00003F00ul))
        {
            Qprc_IrqHandler((volatile stc_qprcn_t*)&QPRC1, &m_astcQprcInstanceDataLut[QprcInstanceIndexQprc1].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC2)
        if (0ul != (u32IrqMon & 0x000FC000ul))
        {
            Qprc_IrqHandler((volatile stc_qprcn_t*)&QPRC2, &m_astcQprcInstanceDataLut[QprcInstanceIndexQprc2].stcInternData);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************************** FM3: MFS0, MFS8 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8)
/**
 ******************************************************************************
 ** \brief FM3: MFS0, MFS8 IRQ handler (IRQ#7) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS0_8_RX_AVAILABLE)
    void MFS0_8_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ07MON;

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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8 )
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        
    }
    #elif (1u == IRQ_MFS0_RX_AVAILABLE)
    void MFS0_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ07MON;

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
    
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************************** FM3: MFS0, MFS8 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8)
/**
 ******************************************************************************
 ** \brief FM3: MFS0, MFS8 IRQ handler (IRQ#8) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS0_8_TX_AVAILABLE)
    void MFS0_8_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ08MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8 )
        if (0ul != (u32IrqMon & 0x00000004ul))
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
        if (0ul != (u32IrqMon & 0x00000008ul))
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
    #elif (1u == IRQ_MFS0_TX_AVAILABLE)
    void MFS0_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ08MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
/****************************** FM3: MFS1, MFS9 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9)
/**
 ******************************************************************************
 ** \brief FM3: MFS1, MFS9 IRQ handler (IRQ#9) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS1_9_RX_AVAILABLE)
    void MFS1_9_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ09MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS1_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS1_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS1_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS1_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS9_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS9_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS9_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS9_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS1_RX_AVAILABLE)
    void MFS1_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ09MON;

    #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1 )
    if (0ul != (u32IrqMon & 0x00000001ul))
    {
        switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData.enMode)
        {
            case MfsUartMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS1_UART_BASE)
                MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                #endif
                break;
            case MfsCsioMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS1_CSIO_BASE)
                MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                #endif
                break;
            case MfsI2cMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS1_I2C_BASE)
                MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
                #endif
                break;
            case MfsLinMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS1_LIN_BASE)
                MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN1, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs1].stcInternData));
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
/****************************** FM3: MFS1, MFS9 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9)
/**
 ******************************************************************************
 ** \brief FM3: MFS1, MFS9 IRQ handler (IRQ#10) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS1_9_TX_AVAILABLE)
    void MFS1_9_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ10MON;

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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS9_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS9_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS9_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS9_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9 )
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS9_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS9_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS9_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN9, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs9].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS1_TX_AVAILABLE)
    void MFS1_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ10MON;

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
/****************************** FM3: MFS2, MFS10 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10)
/**
 ******************************************************************************
 ** \brief FM3: MFS2, MFS10 IRQ handler (IRQ#11) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS2_10_RX_AVAILABLE)
    void MFS2_10_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ11MON;

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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS10_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS10_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS10_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS10_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS2_RX_AVAILABLE)
    void MFS2_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ11MON;

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
    
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************************** FM3: MFS2, MFS10 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10)
/**
 ******************************************************************************
 ** \brief FM3: MFS2, MFS10 IRQ handler (IRQ#12) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS2_10_TX_AVAILABLE)
    void MFS2_10_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ12MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS10_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS10_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS10_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS10_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10 )
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS10_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS10_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS10_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN10, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs10].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS2_TX_AVAILABLE)
    void MFS2_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ12MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
/****************************** FM3: MFS3, MFS11 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11)
/**
 ******************************************************************************
 ** \brief FM3: MFS3, MFS11 IRQ handler (IRQ#13) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS3_11_RX_AVAILABLE)
    void MFS3_11_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ13MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS3_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS3_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS3_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS3_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS11_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS11_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS11_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS11_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS3_RX_AVAILABLE)
    void MFS3_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ13MON;

    #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3 )
    if (0ul != (u32IrqMon & 0x00000001ul))
    {
        switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData.enMode)
        {
            case MfsUartMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS3_UART_BASE)
                MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                #endif
                break;
            case MfsCsioMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS3_CSIO_BASE)
                MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                #endif
                break;
            case MfsI2cMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS3_I2C_BASE)
                MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
                #endif
                break;
            case MfsLinMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS3_LIN_BASE)
                MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN3, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs3].stcInternData));
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
/****************************** FM3: MFS3, MFS11 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11)
/**
 ******************************************************************************
 ** \brief FM3: MFS3, MFS11 IRQ handler (IRQ#14) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS3_11_TX_AVAILABLE)
    void MFS3_11_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ14MON;

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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS11_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS11_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS11_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS11_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11 )
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS11_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS11_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS11_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN11, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs11].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS3_TX_AVAILABLE)
    void MFS3_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ14MON;

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
/****************************** FM3: MFS4, MFS12 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12)
/**
 ******************************************************************************
 ** \brief FM3: MFS4, MFS12 IRQ handler (IRQ#15) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS4_12_RX_AVAILABLE)
    void MFS4_12_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ15MON;

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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS12_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS12_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS12_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS12_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS4_RX_AVAILABLE)
    void MFS4_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ15MON;

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
/****************************** FM3: MFS4, MFS12 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12)
/**
 ******************************************************************************
 ** \brief FM3: MFS4, MFS12 IRQ handler (IRQ#16) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS4_12_TX_AVAILABLE)
    void MFS4_12_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ16MON;

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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS12_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS12_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS12_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS12_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12 )
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS12_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS12_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS12_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN12, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs12].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS4_TX_AVAILABLE)
    void MFS4_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ16MON;

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
/****************************** FM3: MFS5, MFS13 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13)
/**
 ******************************************************************************
 ** \brief FM3: MFS5, MFS13 IRQ handler (IRQ#17) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS5_13_RX_AVAILABLE)
    void MFS5_13_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ17MON;

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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS13_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS13_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS13_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS13_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS5_RX_AVAILABLE)
    void MFS5_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ17MON;

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
/****************************** FM3: MFS5, MFS13 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13)
/**
 ******************************************************************************
 ** \brief FM3: MFS5, MFS13 IRQ handler (IRQ#18) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS5_13_TX_AVAILABLE)
    void MFS5_13_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ18MON;

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
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS13_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS13_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS13_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS13_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13 )
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS13_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS13_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS13_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN13, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs13].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS5_TX_AVAILABLE)
    void MFS5_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ18MON;

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
/****************************** FM3: MFS6, MFS14 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14)
/**
 ******************************************************************************
 ** \brief FM3: MFS6, MFS14 IRQ handler (IRQ#19) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS6_14_RX_AVAILABLE)
    void MFS6_14_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ19MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS6_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS6_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS6_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS6_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS14_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS14_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS14_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS14_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS6_RX_AVAILABLE)
    void MFS6_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ19MON;

    #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6 )
    if (0ul != (u32IrqMon & 0x00000001ul))
    {
        switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData.enMode)
        {
            case MfsUartMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS6_UART_BASE)
                MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                #endif
                break;
            case MfsCsioMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS6_CSIO_BASE)
                MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                #endif
                break;
            case MfsI2cMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS6_I2C_BASE)
                MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                #endif
                break;
            case MfsLinMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS6_LIN_BASE)
                MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
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
/****************************** FM3: MFS6, MFS14 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14)
/**
 ******************************************************************************
 ** \brief FM3: MFS6, MFS14 IRQ handler (IRQ#20) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS6_14_TX_AVAILABLE)
    void MFS6_14_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ20MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS6_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS6_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS6_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS6_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS6_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS6_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS6_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS14_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS14_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS14_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS14_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14 )
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS14_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS14_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS14_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN14, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs14].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS6_TX_AVAILABLE)
    void MFS6_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ20MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS6_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS6_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS6_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS6_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS6_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS6_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS6_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN6, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs6].stcInternData));
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
/****************************** FM3: MFS7, MFS15 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15)
/**
 ******************************************************************************
 ** \brief FM3: MFS7, MFS15 IRQ handler (IRQ#21) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS7_15_RX_AVAILABLE)
    void MFS7_15_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ21MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS7_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS7_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS7_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS7_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS15_UART_BASE)
                    MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS15_CSIO_BASE)
                    MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS15_I2C_BASE)
                    MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS15_LIN_BASE)
                    MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS7_RX_AVAILABLE)
    void MFS7_RX_IRQHandler(void)
    {
    uint32_t u32IrqMon = FM_INTREQ->IRQ21MON;

    #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7 )
    if (0ul != (u32IrqMon & 0x00000001ul))
    {
        switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData.enMode)
        {
            case MfsUartMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS7_UART_BASE)
                MfsUartIrqHandlerRx((stc_mfsn_uart_t*)&UART7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                #endif
                break;
            case MfsCsioMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS7_CSIO_BASE)
                MfsCsioIrqHandlerRx((stc_mfsn_csio_t*)&CSIO7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                #endif
                break;
            case MfsI2cMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS7_I2C_BASE)
                MfsI2cIrqHandlerRx((stc_mfsn_i2c_t*)&I2C7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                #endif
                break;
            case MfsLinMode:
                #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS7_LIN_BASE)
                MfsLinIrqHandlerRx((stc_mfsn_lin_t*)&LIN7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
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
/****************************** FM3: MFS7, MFS15 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15)
/**
 ******************************************************************************
 ** \brief FM3: MFS7, MFS15 IRQ handler (IRQ#22) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFS7_15_TX_AVAILABLE)
    void MFS7_15_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ22MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS7_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS7_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS7_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS7_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS7_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS7_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS7_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15 )
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS15_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS15_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS15_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS15_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15 )
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS15_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS15_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS15_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN15, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs15].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
    }
    #elif (1u == IRQ_MFS7_TX_AVAILABLE)
    void MFS7_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ22MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7 )
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData.enMode)
            {
                case MfsUartMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) && defined(FM_MFS7_UART_BASE)
                    MfsUartIrqHandlerTx((stc_mfsn_uart_t*)&UART7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS7_CSIO_BASE)
                    MfsCsioIrqHandlerTx((stc_mfsn_csio_t*)&CSIO7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS7_I2C_BASE)
                    MfsI2cIrqHandlerTx((stc_mfsn_i2c_t*)&I2C7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS7_LIN_BASE)
                    MfsLinIrqHandlerTx((stc_mfsn_lin_t*)&LIN7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                default:
                    break;
            }
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7 )
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            switch(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData.enMode)
            {
                case MfsUartMode:
                    // should never happen
                    while (1u)
                    { }
                case MfsCsioMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON) && defined(FM_MFS7_CSIO_BASE)
                    MfsCsioIrqHandlerStatus((stc_mfsn_csio_t*)&CSIO7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsI2cMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) && defined(FM_MFS7_I2C_BASE)
                    MfsI2cIrqHandlerStatus((stc_mfsn_i2c_t*)&I2C7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
                    #endif
                    break;
                case MfsLinMode:
                    #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON) && defined(FM_MFS7_LIN_BASE)
                    MfsLinIrqHandlerStatus((stc_mfsn_lin_t*)&LIN7, &(m_astcMfsInstanceDataLut[MfsInstanceIndexMfs7].stcInternData));
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
/****************************** FM3: PPG00_02_20 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_PPG00_02_20)
/**
 ******************************************************************************
 ** \brief FM3: PPG00_02_20 IRQ handler (IRQ#23) Type a
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
 ** \brief FM3: RTC, WC, CLK IRQ handler (IRQ#24) Type a
 ******************************************************************************/
    #if (1u == IRQ_TIM_WC_RTC_AVAILABLE)
    void TIM_WC_RTC_IRQHandler(void)
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
 ** \brief FM3: ADC0 IRQ handler (IRQ#25) Type a
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
 ** \brief FM3: ADC1 IRQ handler (IRQ#26) Type a
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
 ** \brief FM3: ADC2, LCD IRQ handler (IRQ#27) Type a
 ******************************************************************************/
    #if (1u == IRQ_ADC2_LCD_AVAILABLE)
    void ADC2_LCD_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ27MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC2)
        if (0ul != (u32IrqMon & 0x0000000Ful))
        {
            AdcIrqHandler((volatile stc_adcn_t*)&ADC2, &(m_astcAdcInstanceDataLut[AdcInstanceIndexAdc2].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_LCD)
        if (0ul != (u32IrqMon & 0x00000010ul))
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
 ** \brief FM3: MFT0_FRT, MFT1_FRT, MFT2_FRT IRQ handler (IRQ#28) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFT0_2_FRT_AVAILABLE)
    void MFT0_2_FRT_IRQHandler(void)
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
 ** \brief FM3: MFT0_ICU, MFT1_ICU, MFT2_ICU IRQ handler (IRQ#29) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFT0_2_ICU_AVAILABLE)
    void MFT0_2_ICU_IRQHandler(void)
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
 ** \brief FM3: MFT0_OCU, MFT1_OCU, MFT2_OCU IRQ handler (IRQ#30) Type a
 ******************************************************************************/
    #if (1u == IRQ_MFT0_2_OCU_AVAILABLE)
    void MFT0_2_OCU_IRQHandler(void)
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
/**************** FM3: BT0, BT1, BT2, BT3, BT4, BT5, BT6, BT7 *****************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT2) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT3) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT4) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT5) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT6) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT7)
/**
 ******************************************************************************
 ** \brief FM3: BT0, BT1, BT2, BT3, BT4, BT5, BT6, BT7 IRQ handler (IRQ#31) Type a
 ******************************************************************************/
    #if (1u == IRQ_BT0_7_AVAILABLE)
    void BT0_7_IRQHandler(void)
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
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/***************************** FM3: ETHER0, CAN0 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ETHER0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CAN0)
/**
 ******************************************************************************
 ** \brief FM3: ETHER0, CAN0 IRQ handler (IRQ#32) Type a
 ******************************************************************************/
    #if (1u == IRQ_ETHER0_CAN0_AVAILABLE)
    void ETHER0_CAN0_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ32MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_ETHER0)
        if (0ul != (u32IrqMon & 0x0000000Eul))
        {
            EmacIrqHandler((FM_ETHERNET_MAC_TypeDef*)&EMAC0, &(m_astcEmacInstanceDataLut[EmacInstanceIndexEmac0].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CAN0)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Can_IrqHandler(0u);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/***************************** FM3: ETHER1, CAN1 ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ETHER1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CAN1)
/**
 ******************************************************************************
 ** \brief FM3: ETHER1, CAN1 IRQ handler (IRQ#33) Type a
 ******************************************************************************/
    #if (1u == IRQ_ETHER1_CAN1_AVAILABLE)
    void ETHER1_CAN1_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ33MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_ETHER1)
        if (0ul != (u32IrqMon & 0x0000000Eul))
        {
            EmacIrqHandler((FM_ETHERNET_MAC_TypeDef*)&EMAC1, &(m_astcEmacInstanceDataLut[EmacInstanceIndexEmac1].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CAN1)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Can_IrqHandler(1u);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************************** FM3: USB0_DEVICE ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_DEVICE)
/**
 ******************************************************************************
 ** \brief FM3: USB0_DEVICE IRQ handler (IRQ#34) Type a
 ******************************************************************************/
    #if (1u == IRQ_USB0_F_AVAILABLE)
    void USB0_F_IRQHandler(void)
    {
        Usb_IrqHandler((stc_usb_t*)&USB0);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/************************ FM3: USB0_HOST, USB0_DEVICE *************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_HOST) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_DEVICE)
/**
 ******************************************************************************
 ** \brief FM3: USB0_HOST, USB0_DEVICE IRQ handler (IRQ#35) Type a
 ******************************************************************************/
    #if (1u == IRQ_USB0_H_F_AVAILABLE)
    void USB0_H_F_IRQHandler(void)
    {
        Usb_IrqHandler((stc_usb_t*)&USB0);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/*************************** FM3: USB1_DEVICE, CEC0 ***************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_DEVICE) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CEC0)
/**
 ******************************************************************************
 ** \brief FM3: USB1_DEVICE, CEC0 IRQ handler (IRQ#36) Type a
 ******************************************************************************/
    #if (1u == IRQ_USB1_F_HDMICEC0_AVAILABLE)
    void USB1_F_HDMICEC0_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ36MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_DEVICE)
        if (0ul != (u32IrqMon & 0x0000001Ful))
        {
            Usb_IrqHandler((stc_usb_t*)&USB1);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CEC0)
        if (0ul != (u32IrqMon & 0x000000020ul))
        {
            RcIrqHandler((stc_rcn_t*) &RC0, &(m_astcRcInstanceDataLut[RcInstanceIndexRc0].stcInternData));
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************* FM3: USB1_HOST, USB1_DEVICE, CEC1 **********************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_HOST) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_DEVICE) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CEC1)
/**
 ******************************************************************************
 ** \brief FM3: USB1_HOST, USB1_DEVICE, CEC1 IRQ handler (IRQ#37) Type a
 ******************************************************************************/
    #if (1u == IRQ_USB1_H_F_HDMICEC1_AVAILABLE)
    void USB1_H_F_HDMICEC1_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ37MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_HOST) ||(PDL_ON == PDL_INTERRUPT_ENABLE_USB1_DEVICE)
        if (0ul != (u32IrqMon & 0x0000003Ful))
        {
            Usb_IrqHandler((stc_usb_t*)&USB1);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CEC1)
        if (0ul != (u32IrqMon & 0x000000040ul))
        {
            RcIrqHandler((stc_rcn_t*) &RC1, &(m_astcRcInstanceDataLut[RcInstanceIndexRc1].stcInternData));
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: DMA0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA0)
/**
 ******************************************************************************
 ** \brief FM3: DMA0 IRQ handler (IRQ#38) Type a
 ******************************************************************************/
    #if (1u == IRQ_DMAC0_AVAILABLE)
    void DMAC0_IRQHandler(void)
    {
        DmaIrqHandler(0u);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: DMA1 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA1)
/**
 ******************************************************************************
 ** \brief FM3: DMA1 IRQ handler (IRQ#39) Type a
 ******************************************************************************/
    #if (1u == IRQ_DMAC1_AVAILABLE)
    void DMAC1_IRQHandler(void)
    {
        DmaIrqHandler(1u);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: DMA2 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA2)
/**
 ******************************************************************************
 ** \brief FM3: DMA2 IRQ handler (IRQ#40) Type a
 ******************************************************************************/
    #if (1u == IRQ_DMAC2_AVAILABLE)
    void DMAC2_IRQHandler(void)
    {
        DmaIrqHandler(2u);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: DMA3 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA3)
/**
 ******************************************************************************
 ** \brief FM3: DMA3 IRQ handler (IRQ#41) Type a
 ******************************************************************************/
    #if (1u == IRQ_DMAC3_AVAILABLE)
    void DMAC3_IRQHandler(void)
    {
        DmaIrqHandler(3u);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: DMA4 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA4)
/**
 ******************************************************************************
 ** \brief FM3: DMA4 IRQ handler (IRQ#42) Type a
 ******************************************************************************/
    #if (1u == IRQ_DMAC4_AVAILABLE)
    void DMAC4_IRQHandler(void)
    {
        DmaIrqHandler(4u);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: DMA5 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA5)
/**
 ******************************************************************************
 ** \brief FM3: DMA5 IRQ handler (IRQ#43) Type a
 ******************************************************************************/
    #if (1u == IRQ_DMAC5_AVAILABLE)
    void DMAC5_IRQHandler(void)
    {
        DmaIrqHandler(5u);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: DMA6 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA6)
/**
 ******************************************************************************
 ** \brief FM3: DMA6 IRQ handler (IRQ#44) Type a
 ******************************************************************************/
    #if (1u == IRQ_DMAC6_AVAILABLE)
    void DMAC6_IRQHandler(void)
    {
        DmaIrqHandler(6u);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: DMA7 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA7)
/**
 ******************************************************************************
 ** \brief FM3: DMA7 IRQ handler (IRQ#45) Type a
 ******************************************************************************/
    #if (1u == IRQ_DMAC7_AVAILABLE)
    void DMAC7_IRQHandler(void)
    {
        DmaIrqHandler(7u);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/************* FM3: BT8, BT9, BT10, BT11, BT12, BT13, BT14, BT15 **************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT8) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT9) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT10) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT11) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT12) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT13) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT14) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT15)
/**
 ******************************************************************************
 ** \brief FM3: BT8, BT9, BT10, BT11, BT12, BT13, BT14, BT15 IRQ handler (IRQ#46) Type a
 ******************************************************************************/
    #if (1u == IRQ_BT8_15_AVAILABLE)
    void BT8_15_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ46MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT8)
        if (0ul != (u32IrqMon & 0x00000003ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT8, &m_astcBtInstanceDataLut[BtInstanceIndexBt8].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT9)
        if (0ul != (u32IrqMon & 0x0000000Cul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT9, &m_astcBtInstanceDataLut[BtInstanceIndexBt9].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT10)
        if (0ul != (u32IrqMon & 0x00000030ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT10, &m_astcBtInstanceDataLut[BtInstanceIndexBt10].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT11)
        if (0ul != (u32IrqMon & 0x000000C0ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT11, &m_astcBtInstanceDataLut[BtInstanceIndexBt11].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT12)
        if (0ul != (u32IrqMon & 0x00000300ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT12, &m_astcBtInstanceDataLut[BtInstanceIndexBt12].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT13)
        if (0ul != (u32IrqMon & 0x00000C00ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT13, &m_astcBtInstanceDataLut[BtInstanceIndexBt13].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT14)
        if (0ul != (u32IrqMon & 0x00003000ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT14, &m_astcBtInstanceDataLut[BtInstanceIndexBt14].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT15)
        if (0ul != (u32IrqMon & 0x0000C000ul))
        {
            Bt_IrqHandler((volatile stc_btn_t*)&BT15, &m_astcBtInstanceDataLut[BtInstanceIndexBt15].stcInternData);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM3: FLASH *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_FLASH)
/**
 ******************************************************************************
 ** \brief FM3: FLASH IRQ handler (IRQ#47) Type a
 ******************************************************************************/
    #if (1u == IRQ_FLASHIF_AVAILABLE)
    void FLASHIF_IRQHandler(void)
    {
        PDL_DummyHandler(0ul);
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
 ** \brief FM3: NMI, HWWDG IRQ handler (EXC#2) Type a
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


#endif // #ifdef __INTERRUPTS_FM3_TYPE_A_C__

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
