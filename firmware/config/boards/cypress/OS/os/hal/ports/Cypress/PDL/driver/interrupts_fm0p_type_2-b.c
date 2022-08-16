/*******************************************************************************
* \file             interrupts_fm0p_type_2-b.c
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

#include "interrupts_fm0p.h"

#ifdef __INTERRUPTS_FM0P_TYPE_2_B_C__

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
    // 5: RCINTSEL5, 6: RCINTSEL6, 7: RCINTSEL7, 8: DSTC, 9: LCD, 10: FLASH, 

    u32Caller = u32Caller;
    while (1u)
    { }
}

/******************************************************************************/
/********************************* FM0P: CSV **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CSV)
/**
 ******************************************************************************
 ** \brief FM0P: CSV IRQ handler (IRQ#0) Type 2-b
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
/******************************** FM0P: SWWDG *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_SWWDG)
/**
 ******************************************************************************
 ** \brief FM0P: SWWDG IRQ handler (IRQ#1) Type 2-b
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
/********************************* FM0P: LVD **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_LVD)
/**
 ******************************************************************************
 ** \brief FM0P: LVD IRQ handler (IRQ#2) Type 2-b
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
/****************************** FM0P: RCINTSEL0 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL0)
/**
 ******************************************************************************
 ** \brief FM0P: RCINTSEL0 IRQ handler (IRQ#3) Type 2-b
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
/****************************** FM0P: RCINTSEL1 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL1)
/**
 ******************************************************************************
 ** \brief FM0P: RCINTSEL1 IRQ handler (IRQ#4) Type 2-b
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
/****************************** FM0P: RCINTSEL2 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL2)
/**
 ******************************************************************************
 ** \brief FM0P: RCINTSEL2 IRQ handler (IRQ#5) Type 2-b
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
/****************************** FM0P: RCINTSEL3 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL3)
/**
 ******************************************************************************
 ** \brief FM0P: RCINTSEL3 IRQ handler (IRQ#6) Type 2-b
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
/****************************** FM0P: RCINTSEL4 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL4)
/**
 ******************************************************************************
 ** \brief FM0P: RCINTSEL4 IRQ handler (IRQ#7) Type 2-b
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
/****************************** FM0P: RCINTSEL5 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL5)
/**
 ******************************************************************************
 ** \brief FM0P: RCINTSEL5 IRQ handler (IRQ#8) Type 2-b
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
/****************************** FM0P: RCINTSEL6 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL6)
/**
 ******************************************************************************
 ** \brief FM0P: RCINTSEL6 IRQ handler (IRQ#9) Type 2-b
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
/****************************** FM0P: RCINTSEL7 *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RCINTSEL7)
/**
 ******************************************************************************
 ** \brief FM0P: RCINTSEL7 IRQ handler (IRQ#10) Type 2-b
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
/******************************* FM0P: MFT0_WFG *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_WFG)
/**
 ******************************************************************************
 ** \brief FM0P: MFT0_WFG IRQ handler (IRQ#11) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_WFG_DTIF_AVAILABLE)
    void MFT0_WFG_DTIF_IRQHandler(void)
    {
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_WFG10)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_WFG10))
        {
            Dstc_MftWfgIrqHandler(DSTC_IRQ_NUMBER_MFT0_WFG10);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_WFG32)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_WFG32))
        {
            Dstc_MftWfgIrqHandler(DSTC_IRQ_NUMBER_MFT0_WFG32);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_WFG54)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_WFG54))
        {
            Dstc_MftWfgIrqHandler(DSTC_IRQ_NUMBER_MFT0_WFG54);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT0_WFG10) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_WFG32) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_WFG54)

            Mft_Wfg_IrqHandler((volatile stc_mftn_wfg_t*)&MFT0_WFG, &m_astcMftWfgInstanceDataLut[WfgInstanceIndexWfg0].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/**** FM0P: EXINT0, EXINT1, EXINT2, EXINT3, EXINT4, EXINT5, EXINT6, EXINT7 ****/
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
 ** \brief FM0P: EXINT0, EXINT1, EXINT2, EXINT3, EXINT4, EXINT5, EXINT6, EXINT7 IRQ handler (IRQ#12) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_EXINT0_7_AVAILABLE)
    void EXINT0_7_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ12MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT0))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT0);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT1)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT1))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT2)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT2))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT2);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT3)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT3))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT3);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT4)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT4))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT4);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT5)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT5))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT5);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT6)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT6))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT6);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT7)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT7))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT7);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXTINT0) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT1) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT2) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT3) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT4) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT5) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT6) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT7)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Exint_IrqHandler(0u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            Exint_IrqHandler(1u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            Exint_IrqHandler(2u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Exint_IrqHandler(3u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            Exint_IrqHandler(4u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5)
        if (0ul != (u32IrqMon & 0x00000020ul))
        {
            Exint_IrqHandler(5u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6)
        if (0ul != (u32IrqMon & 0x00000040ul))
        {
            Exint_IrqHandler(6u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
        if (0ul != (u32IrqMon & 0x00000080ul))
        {
            Exint_IrqHandler(7u);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/* FM0P: EXINT8, EXINT9, EXINT10, EXINT11, EXINT12, EXINT13, EXINT14, EXINT15, EXINT16, EXINT17, EXINT18, EXINT19, EXINT20, EXINT21, EXINT22, EXINT23 */
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
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23)
/**
 ******************************************************************************
 ** \brief FM0P: EXINT8, EXINT9, EXINT10, EXINT11, EXINT12, EXINT13, EXINT14, EXINT15, EXINT16, EXINT17, EXINT18, EXINT19, EXINT20, EXINT21, EXINT22, EXINT23 IRQ handler (IRQ#13) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_EXINT8_23_AVAILABLE)
    void EXINT8_23_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ13MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT8)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT8))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT8);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT9)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT9))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT9);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT10)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT10))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT10);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXTINT11)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXTINT11))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXTINT11);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT12)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT12))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT12);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXTINT8) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT9) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT10) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXTINT11) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT12)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Exint_IrqHandler(8u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            Exint_IrqHandler(9u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            Exint_IrqHandler(10u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Exint_IrqHandler(11u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            Exint_IrqHandler(12u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13)
        if (0ul != (u32IrqMon & 0x00000020ul))
        {
            Exint_IrqHandler(13u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14)
        if (0ul != (u32IrqMon & 0x00000040ul))
        {
            Exint_IrqHandler(14u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15)
        if (0ul != (u32IrqMon & 0x00000080ul))
        {
            Exint_IrqHandler(15u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16)
        if (0ul != (u32IrqMon & 0x00000100ul))
        {
            Exint_IrqHandler(16u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17)
        if (0ul != (u32IrqMon & 0x00000200ul))
        {
            Exint_IrqHandler(17u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18)
        if (0ul != (u32IrqMon & 0x00000400ul))
        {
            Exint_IrqHandler(18u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19)
        if (0ul != (u32IrqMon & 0x00000800ul))
        {
            Exint_IrqHandler(19u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20)
        if (0ul != (u32IrqMon & 0x00001000ul))
        {
            Exint_IrqHandler(20u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21)
        if (0ul != (u32IrqMon & 0x00002000ul))
        {
            Exint_IrqHandler(21u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22)
        if (0ul != (u32IrqMon & 0x00004000ul))
        {
            Exint_IrqHandler(22u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23)
        if (0ul != (u32IrqMon & 0x00008000ul))
        {
            Exint_IrqHandler(23u);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: DT0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DT0)
/**
 ******************************************************************************
 ** \brief FM0P: DT0 IRQ handler (IRQ#14) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_DT_AVAILABLE)
    void DT_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ14MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_DT0)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            DtIrqHandler(0u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_DT0)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            DtIrqHandler(1u);
        }
        #endif
        
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: MFS0 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0)
/**
 ******************************************************************************
 ** \brief FM0P: MFS0 IRQ handler (IRQ#15) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFS0_RX_TX_AVAILABLE)
    void MFS0_RX_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ15MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS0_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS0_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS0_RX);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFS0_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS0_TX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS0_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS0_RX) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFS0_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: MFS1 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1)
/**
 ******************************************************************************
 ** \brief FM0P: MFS1 IRQ handler (IRQ#16) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFS1_RX_TX_AVAILABLE)
    void MFS1_RX_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ16MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS1_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS1_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS1_RX);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFS1_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS1_TX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS1_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS1_RX) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFS1_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: MFS2 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2)
/**
 ******************************************************************************
 ** \brief FM0P: MFS2 IRQ handler (IRQ#17) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFS2_RX_TX_AVAILABLE)
    void MFS2_RX_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ17MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS2_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS2_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS2_RX);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFS2_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS2_TX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS2_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS2_RX) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFS2_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: MFS3 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3)
/**
 ******************************************************************************
 ** \brief FM0P: MFS3 IRQ handler (IRQ#18) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFS3_RX_TX_AVAILABLE)
    void MFS3_RX_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ18MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS3_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS3_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS3_RX);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFS3_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS3_TX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS3_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS3_RX) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFS3_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: MFS4 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4)
/**
 ******************************************************************************
 ** \brief FM0P: MFS4 IRQ handler (IRQ#19) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFS4_RX_AVAILABLE)
    void MFS4_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ19MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS4_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS4_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS4_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS4_RX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: MFS4 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4)
/**
 ******************************************************************************
 ** \brief FM0P: MFS4 IRQ handler (IRQ#20) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFS4_TX_AVAILABLE)
    void MFS4_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ20MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS4_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS4_TX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS4_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS4_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: MFS5 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5)
/**
 ******************************************************************************
 ** \brief FM0P: MFS5 IRQ handler (IRQ#21) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFS5_RX_AVAILABLE)
    void MFS5_RX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ21MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS5_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS5_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS5_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS5_RX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: MFS5 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5)
/**
 ******************************************************************************
 ** \brief FM0P: MFS5 IRQ handler (IRQ#22) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFS5_TX_AVAILABLE)
    void MFS5_TX_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ22MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS5_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS5_TX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS5_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS5_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/************************* FM0P: PPG, RC0, ICC0, DSTC *************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_PPG) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_RC0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_ICC0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_DSTC)
/**
 ******************************************************************************
 ** \brief FM0P: PPG, RC0, ICC0, DSTC IRQ handler (IRQ#23) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_PPG00_02_04_DSTC_SMCIF0_HDMICEC0_AVAILABLE)
    void PPG00_02_04_DSTC_SMCIF0_HDMICEC0_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ23MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_PPG4)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG4))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG4);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_PPG4)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_PPG)
        if (0ul != (u32IrqMon & 0x00000007ul))
        {
            Ppg_IrqHandler();
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_RC0)
        if (0ul != (u32IrqMon & 0x00000100ul))
        {
            RcIrqHandler((stc_rcn_t*) &RC0, &(m_astcRcInstanceDataLut[RcInstanceIndexRc0].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_ICC0)
        if (0ul != (u32IrqMon & 0x00000200ul))
        {
            Icc_IrqHandler((stc_iccn_t*) &ICC0, &(m_astcIccInstanceDataLut[IccInstanceIndexIcc0].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_DSTC)
        if (0ul != (u32IrqMon & 0x00000200ul))
        {
            PDL_DummyHandler(8ul);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************** FM0P: ICC1, RC1, RTC0, WC0, CLK ***********************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ICC1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_RC1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_RTC0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_WC0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CLK)
/**
 ******************************************************************************
 ** \brief FM0P: ICC1, RC1, RTC0, WC0, CLK IRQ handler (IRQ#24) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_TIM_SMCIF1_HDMICEC1_AVAILABLE)
    void TIM_SMCIF1_HDMICEC1_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ24MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_WC0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_WC0))
        {
            Dstc_WcIrqHandler();
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_WC0)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_ICC1)
        if (0ul != (u32IrqMon & 0x00000400ul))
        {
            Icc_IrqHandler((stc_iccn_t*) &ICC1, &(m_astcIccInstanceDataLut[IccInstanceIndexIcc1].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_RC1)
        if (0ul != (u32IrqMon & 0x00000200ul))
        {
            RcIrqHandler((stc_rcn_t*) &RC1, &(m_astcRcInstanceDataLut[RcInstanceIndexRc1].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_RTC0)
        if (0ul != (u32IrqMon & 0x00000020ul))
        {
            Rtc_IrqHandler((stc_rtcn_t*)&RTC0, &(m_astcRtcInstanceDataLut[RtcInstanceIndexRtc0].stcInternData));
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_WC0)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            Wc_IrqHandler((stc_wcn_t*)&WC0);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CLK)
        if (0ul != (u32IrqMon & 0x0000000Ful))
        {
            Clk_IrqHandler();
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM0P: ADC0 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC0)
/**
 ******************************************************************************
 ** \brief FM0P: ADC0 IRQ handler (IRQ#25) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_ADC0_AVAILABLE)
    void ADC0_IRQHandler(void)
    {
        #if (PDL_ON == PDL_DSTC_ENABLE_ADC0_PRIO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_ADC0_PRIO))
        {
            Dstc_AdcIrqHandler(DSTC_IRQ_NUMBER_ADC0_PRIO, DSTC_IRQ_NUMBER_ADC0_SCAN);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_ADC0_PRIO)

            AdcIrqHandler((volatile stc_adcn_t*)&ADC0, &(m_astcAdcInstanceDataLut[AdcInstanceIndexAdc0].stcInternData));

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/************************ FM0P: USB0_HOST, USB0_DEVICE ************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_HOST) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_DEVICE)
/**
 ******************************************************************************
 ** \brief FM0P: USB0_HOST, USB0_DEVICE IRQ handler (IRQ#26) Type 2-b
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
/********************************* FM0P: LCD **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_LCD)
/**
 ******************************************************************************
 ** \brief FM0P: LCD IRQ handler (IRQ#27) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_LCDC_AVAILABLE)
    void LCDC_IRQHandler(void)
    {
        PDL_DummyHandler(9ul);
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************* FM0P: MFT0_FRT, MFT0_ICU, MFT0_OCU *********************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_FRT) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_ICU) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_OCU)
/**
 ******************************************************************************
 ** \brief FM0P: MFT0_FRT, MFT0_ICU, MFT0_OCU IRQ handler (IRQ#28) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_FRT_ICU_OCU_AVAILABLE)
    void MFT0_FRT_ICU_OCU_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ28MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_FRT0_PEAK)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_FRT0_PEAK))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT0_FRT0_PEAK);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_FRT1_PEAK)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_FRT1_PEAK))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT0_FRT1_PEAK);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_FRT2_PEAK)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_FRT2_PEAK))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT0_FRT2_PEAK);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_FRT0_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_FRT0_ZERO))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT0_FRT0_ZERO);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_FRT1_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_FRT1_ZERO))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT0_FRT1_ZERO);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_FRT2_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_FRT2_ZERO))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT0_FRT2_ZERO);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_ICU0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_ICU0))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_ICU0);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_ICU1)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_ICU1))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_ICU1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_ICU2)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_ICU2))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_ICU2);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_ICU3)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_ICU3))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_ICU3);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_OCU0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_OCU0))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_OCU0);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_OCU1)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_OCU1))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_OCU1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_OCU2)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_OCU2))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_OCU2);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_OCU3)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_OCU3))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_OCU3);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_OCU4)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_OCU4))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_OCU4);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_OCU5)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_OCU5))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT0_OCU5);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT0_PEAK) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT1_PEAK) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT2_PEAK) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT0_ZERO) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT1_ZERO) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT2_ZERO) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_ICU0) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_ICU1) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_ICU2) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_ICU3) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU0) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU1) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU2) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU3) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU4) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU5)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_FRT)
        if (0ul != (u32IrqMon & 0x0000003Ful))
        {
            Mft_Frt_IrqHandler((volatile stc_mftn_frt_t*)&MFT0_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt0].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_ICU)
        if (0ul != (u32IrqMon & 0x000003C0ul))
        {
            Mft_Icu_IrqHandler((stc_mftn_icu_t*)&MFT0_ICU, &m_astcMftIcuInstanceDataLut[IcuInstanceIndexIcu0].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_OCU)
        if (0ul != (u32IrqMon & 0x0000FC00ul))
        {
            Mft_Ocu_IrqHandler((stc_mftn_ocu_t*)&MFT0_OCU, &m_astcMftOcuInstanceDataLut[OcuInstanceIndexOcu0].stcInternData);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/***************************** FM0P: USB0_DEVICE ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_DEVICE)
/**
 ******************************************************************************
 ** \brief FM0P: USB0_DEVICE IRQ handler (IRQ#29) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_USB0_F_AVAILABLE)
    void USB0_F_IRQHandler(void)
    {
        #if (PDL_ON == PDL_DSTC_ENABLE_EP1_DRQ)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EP1_DRQ))
        {
            Dstc_Usb0_F_IrqHandler(DSTC_IRQ_NUMBER_EP1_DRQ);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EP2_DRQ)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EP2_DRQ))
        {
            Dstc_Usb0_F_IrqHandler(DSTC_IRQ_NUMBER_EP2_DRQ);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EP3_DRQ)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EP3_DRQ))
        {
            Dstc_Usb0_F_IrqHandler(DSTC_IRQ_NUMBER_EP3_DRQ);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EP4_DRQ)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EP4_DRQ))
        {
            Dstc_Usb0_F_IrqHandler(DSTC_IRQ_NUMBER_EP4_DRQ);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EP5_DRQ)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EP5_DRQ))
        {
            Dstc_Usb0_F_IrqHandler(DSTC_IRQ_NUMBER_EP5_DRQ);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EP1_DRQ) && \
            (PDL_ON != PDL_DSTC_ENABLE_EP2_DRQ) && \
            (PDL_ON != PDL_DSTC_ENABLE_EP3_DRQ) && \
            (PDL_ON != PDL_DSTC_ENABLE_EP4_DRQ) && \
            (PDL_ON != PDL_DSTC_ENABLE_EP5_DRQ)

            Usb_IrqHandler((stc_usb_t*)&USB0);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/************ FM0P: BT0, BT1, BT2, BT3, BT4, BT5, BT6, BT7, FLASH *************/
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
 ** \brief FM0P: BT0, BT1, BT2, BT3, BT4, BT5, BT6, BT7, FLASH IRQ handler (IRQ#31) Type 2-b
 ******************************************************************************/
    #if (1u == IRQ_BT0_7_FLASHIF_AVAILABLE)
    void BT0_7_FLASHIF_IRQHandler(void)
    {
        uint32_t u32IrqMon = FM_INTREQ->IRQ31MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_BT0_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT0_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT0_IRQ0, DSTC_IRQ_NUMBER_BT0_IRQ1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_BT1_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT1_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT1_IRQ0, DSTC_IRQ_NUMBER_BT1_IRQ1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_BT2_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT2_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT2_IRQ0, DSTC_IRQ_NUMBER_BT2_IRQ1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_BT3_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT3_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT3_IRQ0, DSTC_IRQ_NUMBER_BT3_IRQ1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_BT4_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT4_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT4_IRQ0, DSTC_IRQ_NUMBER_BT6_IRQ0);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT0_IRQ0) && \
            (PDL_ON != PDL_DSTC_ENABLE_BT1_IRQ0) && \
            (PDL_ON != PDL_DSTC_ENABLE_BT2_IRQ0) && \
            (PDL_ON != PDL_DSTC_ENABLE_BT3_IRQ0) && \
            (PDL_ON != PDL_DSTC_ENABLE_BT4_IRQ0)

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
            PDL_DummyHandler(12ul);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************************** FM0P: NMI, HWWDG ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_NMI) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_HWWDG)
/**
 ******************************************************************************
 ** \brief FM0P: NMI, HWWDG IRQ handler (EXC#2) Type 2-b
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


#endif // #ifdef __INTERRUPTS_FM0P_TYPE_2-B_C__

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
