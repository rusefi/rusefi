/*******************************************************************************
* \file             interrupts_fm4_type_b.c
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

// [andreika]: added OSAL_XXX() macros for ChibiOS compatibility

#include "hal.h"
#include "interrupts_fm4.h"

#ifdef __INTERRUPTS_FM4_TYPE_B_C__

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

    // 0: IRQ003SEL, 1: IRQ004SEL, 2: IRQ005SEL, 3: IRQ006SEL, 4: IRQ007SEL, 
    // 5: IRQ008SEL, 6: IRQ009SEL, 7: IRQ010SEL, 8: EXTIF, 9: UPLL, 
    // 10: I2SPLL, 11: CANFD0, 12: CANFD1, 13: CANFD, 14: CANFD, 15: DSTC, 
    // 16: SD0, 17: SD1, 18: SD0, 19: FLASH, 

    u32Caller = u32Caller;
    while (1u)
    { }
}

/******************************************************************************/
/********************************** FM4: CSV **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CSV)
/**
 ******************************************************************************
 ** \brief FM4: CSV IRQ handler (IRQ#0) Type b
 ******************************************************************************/
    #if (1u == IRQ_CSV_AVAILABLE)
    OSAL_IRQ_HANDLER(CSV_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        Csv_IrqHandler();
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: SWWDG *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_SWWDG)
/**
 ******************************************************************************
 ** \brief FM4: SWWDG IRQ handler (IRQ#1) Type b
 ******************************************************************************/
    #if (1u == IRQ_SWDT_AVAILABLE)
    OSAL_IRQ_HANDLER(SWDT_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        SwwdgIrqHandler();
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: LVD **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_LVD)
/**
 ******************************************************************************
 ** \brief FM4: LVD IRQ handler (IRQ#2) Type b
 ******************************************************************************/
    #if (1u == IRQ_LVD_AVAILABLE)
    OSAL_IRQ_HANDLER(LVD_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        Lvd_IrqHandler();
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: IRQ003SEL *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_IRQ003SEL)
/**
 ******************************************************************************
 ** \brief FM4: IRQ003SEL IRQ handler (IRQ#3) Type b
 ******************************************************************************/
    #if (1u == IRQ_IRQ003SEL_AVAILABLE)
    OSAL_IRQ_HANDLER(IRQ003SEL_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(0ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: IRQ004SEL *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_IRQ004SEL)
/**
 ******************************************************************************
 ** \brief FM4: IRQ004SEL IRQ handler (IRQ#4) Type b
 ******************************************************************************/
    #if (1u == IRQ_IRQ004SEL_AVAILABLE)
    OSAL_IRQ_HANDLER(IRQ004SEL_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(1ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: IRQ005SEL *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_IRQ005SEL)
/**
 ******************************************************************************
 ** \brief FM4: IRQ005SEL IRQ handler (IRQ#5) Type b
 ******************************************************************************/
    #if (1u == IRQ_IRQ005SEL_AVAILABLE)
    OSAL_IRQ_HANDLER(IRQ005SEL_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(2ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: IRQ006SEL *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_IRQ006SEL)
/**
 ******************************************************************************
 ** \brief FM4: IRQ006SEL IRQ handler (IRQ#6) Type b
 ******************************************************************************/
    #if (1u == IRQ_IRQ006SEL_AVAILABLE)
    OSAL_IRQ_HANDLER(IRQ006SEL_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(3ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: IRQ007SEL *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_IRQ007SEL)
/**
 ******************************************************************************
 ** \brief FM4: IRQ007SEL IRQ handler (IRQ#7) Type b
 ******************************************************************************/
    #if (1u == IRQ_IRQ007SEL_AVAILABLE)
    OSAL_IRQ_HANDLER(IRQ007SEL_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(4ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: IRQ008SEL *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_IRQ008SEL)
/**
 ******************************************************************************
 ** \brief FM4: IRQ008SEL IRQ handler (IRQ#8) Type b
 ******************************************************************************/
    #if (1u == IRQ_IRQ008SEL_AVAILABLE)
    OSAL_IRQ_HANDLER(IRQ008SEL_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(5ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: IRQ009SEL *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_IRQ009SEL)
/**
 ******************************************************************************
 ** \brief FM4: IRQ009SEL IRQ handler (IRQ#9) Type b
 ******************************************************************************/
    #if (1u == IRQ_IRQ009SEL_AVAILABLE)
    OSAL_IRQ_HANDLER(IRQ009SEL_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(6ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: IRQ010SEL *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_IRQ010SEL)
/**
 ******************************************************************************
 ** \brief FM4: IRQ010SEL IRQ handler (IRQ#10) Type b
 ******************************************************************************/
    #if (1u == IRQ_IRQ010SEL_AVAILABLE)
    OSAL_IRQ_HANDLER(IRQ010SEL_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(7ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT0 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0)
/**
 ******************************************************************************
 ** \brief FM4: EXINT0 IRQ handler (IRQ#11) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT0_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT0))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT0);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT0)

            Exint_IrqHandler(0u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
        OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT1 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1)
/**
 ******************************************************************************
 ** \brief FM4: EXINT1 IRQ handler (IRQ#12) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT1_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT1_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT1)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT1))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT1)

            Exint_IrqHandler(1u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT2 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2)
/**
 ******************************************************************************
 ** \brief FM4: EXINT2 IRQ handler (IRQ#13) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT2_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT2_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT2)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT2))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT2);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT2)

            Exint_IrqHandler(2u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT3 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3)
/**
 ******************************************************************************
 ** \brief FM4: EXINT3 IRQ handler (IRQ#14) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT3_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT3_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT3)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT3))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT3);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT3)

            Exint_IrqHandler(3u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT4 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4)
/**
 ******************************************************************************
 ** \brief FM4: EXINT4 IRQ handler (IRQ#15) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT4_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT4_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT4)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT4))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT4);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT4)

            Exint_IrqHandler(4u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT5 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5)
/**
 ******************************************************************************
 ** \brief FM4: EXINT5 IRQ handler (IRQ#16) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT5_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT5_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT5)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT5))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT5);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT5)

            Exint_IrqHandler(5u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT6 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6)
/**
 ******************************************************************************
 ** \brief FM4: EXINT6 IRQ handler (IRQ#17) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT6_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT6_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT6)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT6))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT6);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT6)

            Exint_IrqHandler(6u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT7 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
/**
 ******************************************************************************
 ** \brief FM4: EXINT7 IRQ handler (IRQ#18) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT7_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT7_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT7)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT7))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT7);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT7)

            Exint_IrqHandler(7u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: QPRC0 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC0)
/**
 ******************************************************************************
 ** \brief FM4: QPRC0 IRQ handler (IRQ#19) Type b
 ******************************************************************************/
    #if (1u == IRQ_QPRC0_AVAILABLE)
    OSAL_IRQ_HANDLER(QPRC0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_COUNT_INVERSION)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_COUNT_INVERSION))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_COUNT_INVERSION);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_OUT_OF_RANGE)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_OUT_OF_RANGE))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_OUT_OF_RANGE);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_MATCH_RC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH_RC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH_RC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_RC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_RC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_RC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_UFL_OFL_Z)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_UFL_OFL_Z))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_UFL_OFL_Z);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_QPRC0_COUNT_INVERSION) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_OUT_OF_RANGE) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_MATCH_RC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_RC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_UFL_OFL_Z)

            Qprc_IrqHandler((stc_qprcn_t*)&QPRC0, &m_astcQprcInstanceDataLut[QprcInstanceIndexQprc0].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: QPRC1 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC1)
/**
 ******************************************************************************
 ** \brief FM4: QPRC1 IRQ handler (IRQ#20) Type b
 ******************************************************************************/
    #if (1u == IRQ_QPRC1_AVAILABLE)
    OSAL_IRQ_HANDLER(QPRC1_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_COUNT_INVERSION)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_COUNT_INVERSION))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_COUNT_INVERSION);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_OUT_OF_RANGE)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_OUT_OF_RANGE))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_OUT_OF_RANGE);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_MATCH_RC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH_RC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH_RC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_RC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_RC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_RC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_UFL_OFL_Z)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_UFL_OFL_Z))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_UFL_OFL_Z);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_QPRC0_COUNT_INVERSION) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_OUT_OF_RANGE) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_MATCH_RC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_RC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_UFL_OFL_Z)

            Qprc_IrqHandler((stc_qprcn_t*)&QPRC1, &m_astcQprcInstanceDataLut[QprcInstanceIndexQprc1].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT0_WFG ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_WFG)
/**
 ******************************************************************************
 ** \brief FM4: MFT0_WFG IRQ handler (IRQ#21) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_WFG_DTIF_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT0_WFG_DTIF_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
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

            Mft_Wfg_IrqHandler((stc_mftn_wfg_t*)&MFT0_WFG, &m_astcMftWfgInstanceDataLut[WfgInstanceIndexWfg0].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT1_WFG ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_WFG)
/**
 ******************************************************************************
 ** \brief FM4: MFT1_WFG IRQ handler (IRQ#22) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT1_WFG_DTIF_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT1_WFG_DTIF_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_WFG10)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_WFG10))
        {
            Dstc_MftWfgIrqHandler(DSTC_IRQ_NUMBER_MFT1_WFG10);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_WFG32)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_WFG32))
        {
            Dstc_MftWfgIrqHandler(DSTC_IRQ_NUMBER_MFT1_WFG32);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_WFG54)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_WFG54))
        {
            Dstc_MftWfgIrqHandler(DSTC_IRQ_NUMBER_MFT1_WFG54);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT1_WFG10) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_WFG32) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_WFG54)

            Mft_Wfg_IrqHandler((stc_mftn_wfg_t*)&MFT1_WFG, &m_astcMftWfgInstanceDataLut[WfgInstanceIndexWfg1].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT2_WFG ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_WFG)
/**
 ******************************************************************************
 ** \brief FM4: MFT2_WFG IRQ handler (IRQ#23) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT2_WFG_DTIF_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT2_WFG_DTIF_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_WFG10)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_WFG10))
        {
            Dstc_MftWfgIrqHandler(DSTC_IRQ_NUMBER_MFT2_WFG10);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_WFG32)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_WFG32))
        {
            Dstc_MftWfgIrqHandler(DSTC_IRQ_NUMBER_MFT2_WFG32);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_WFG54)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_WFG54))
        {
            Dstc_MftWfgIrqHandler(DSTC_IRQ_NUMBER_MFT2_WFG54);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT2_WFG10) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_WFG32) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_WFG54)

            Mft_Wfg_IrqHandler((stc_mftn_wfg_t*)&MFT2_WFG, &m_astcMftWfgInstanceDataLut[WfgInstanceIndexWfg2].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT0_FRT ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_FRT)
/**
 ******************************************************************************
 ** \brief FM4: MFT0_FRT IRQ handler (IRQ#24) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_FRT_PEAK_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT0_FRT_PEAK_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
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
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT0_PEAK) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT1_PEAK) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT2_PEAK)

            Mft_Frt_IrqHandler((stc_mftn_frt_t*)&MFT0_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt0].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT0_FRT ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_FRT)
/**
 ******************************************************************************
 ** \brief FM4: MFT0_FRT IRQ handler (IRQ#25) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_FRT_ZERO_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT0_FRT_ZERO_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_FRT0_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_FRT0_ZERO))
        {
            Dstc_MftFrtZeroIrqHandler(DSTC_IRQ_NUMBER_MFT0_FRT0_ZERO);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_FRT1_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_FRT1_ZERO))
        {
            Dstc_MftFrtZeroIrqHandler(DSTC_IRQ_NUMBER_MFT0_FRT1_ZERO);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT0_FRT2_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT0_FRT2_ZERO))
        {
            Dstc_MftFrtZeroIrqHandler(DSTC_IRQ_NUMBER_MFT0_FRT2_ZERO);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT0_ZERO) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT1_ZERO) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_FRT2_ZERO)

            Mft_Frt_IrqHandler((stc_mftn_frt_t*)&MFT0_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt0].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT0_ICU ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_ICU)
/**
 ******************************************************************************
 ** \brief FM4: MFT0_ICU IRQ handler (IRQ#26) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_ICU_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT0_ICU_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
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
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT0_ICU0) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_ICU1) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_ICU2) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_ICU3)

            Mft_Icu_IrqHandler((stc_mftn_icu_t*)&MFT0_ICU, &m_astcMftIcuInstanceDataLut[IcuInstanceIndexIcu0].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT0_OCU ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT0_OCU)
/**
 ******************************************************************************
 ** \brief FM4: MFT0_OCU IRQ handler (IRQ#27) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT0_OCU_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT0_OCU_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
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
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU0) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU1) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU2) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU3) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU4) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT0_OCU5)

            Mft_Ocu_IrqHandler((stc_mftn_ocu_t*)&MFT0_OCU, &m_astcMftOcuInstanceDataLut[OcuInstanceIndexOcu0].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT1_FRT ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_FRT)
/**
 ******************************************************************************
 ** \brief FM4: MFT1_FRT IRQ handler (IRQ#28) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT1_FRT_PEAK_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT1_FRT_PEAK_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_FRT0_PEAK)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_FRT0_PEAK))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT1_FRT0_PEAK);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_FRT1_PEAK)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_FRT1_PEAK))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT1_FRT1_PEAK);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_FRT2_PEAK)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_FRT2_PEAK))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT1_FRT2_PEAK);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT1_FRT0_PEAK) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_FRT1_PEAK) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_FRT2_PEAK)

            Mft_Frt_IrqHandler((stc_mftn_frt_t*)&MFT1_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt1].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT1_FRT ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_FRT)
/**
 ******************************************************************************
 ** \brief FM4: MFT1_FRT IRQ handler (IRQ#29) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT1_FRT_ZERO_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT1_FRT_ZERO_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_FRT0_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_FRT0_ZERO))
        {
            Dstc_MftFrtZeroIrqHandler(DSTC_IRQ_NUMBER_MFT1_FRT0_ZERO);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_FRT1_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_FRT1_ZERO))
        {
            Dstc_MftFrtZeroIrqHandler(DSTC_IRQ_NUMBER_MFT1_FRT1_ZERO);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_FRT2_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_FRT2_ZERO))
        {
            Dstc_MftFrtZeroIrqHandler(DSTC_IRQ_NUMBER_MFT1_FRT2_ZERO);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT1_FRT0_ZERO) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_FRT1_ZERO) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_FRT2_ZERO)

            Mft_Frt_IrqHandler((stc_mftn_frt_t*)&MFT1_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt1].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT1_ICU ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_ICU)
/**
 ******************************************************************************
 ** \brief FM4: MFT1_ICU IRQ handler (IRQ#30) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT1_ICU_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT1_ICU_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_ICU0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_ICU0))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_ICU0);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_ICU1)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_ICU1))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_ICU1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_ICU2)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_ICU2))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_ICU2);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_ICU3)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_ICU3))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_ICU3);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT1_ICU0) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_ICU1) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_ICU2) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_ICU3)

            Mft_Icu_IrqHandler((stc_mftn_icu_t*)&MFT1_ICU, &m_astcMftIcuInstanceDataLut[IcuInstanceIndexIcu1].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT1_OCU ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT1_OCU)
/**
 ******************************************************************************
 ** \brief FM4: MFT1_OCU IRQ handler (IRQ#31) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT1_OCU_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT1_OCU_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_OCU0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_OCU0))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_OCU0);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_OCU1)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_OCU1))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_OCU1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_OCU2)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_OCU2))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_OCU2);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_OCU3)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_OCU3))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_OCU3);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_OCU4)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_OCU4))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_OCU4);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT1_OCU5)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT1_OCU5))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT1_OCU5);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT1_OCU0) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_OCU1) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_OCU2) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_OCU3) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_OCU4) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT1_OCU5)

            Mft_Ocu_IrqHandler((stc_mftn_ocu_t*)&MFT1_OCU, &m_astcMftOcuInstanceDataLut[OcuInstanceIndexOcu1].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT2_FRT ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_FRT)
/**
 ******************************************************************************
 ** \brief FM4: MFT2_FRT IRQ handler (IRQ#32) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT2_FRT_PEAK_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT2_FRT_PEAK_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_FRT2_PEAK)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_FRT2_PEAK))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT2_FRT2_PEAK);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_FRT1_PEAK)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_FRT1_PEAK))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT2_FRT1_PEAK);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_FRT2_PEAK)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_FRT2_PEAK))
        {
            Dstc_MftFrtPeakIrqHandler(DSTC_IRQ_NUMBER_MFT2_FRT2_PEAK);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT2_FRT2_PEAK) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_FRT1_PEAK) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_FRT2_PEAK)

            Mft_Frt_IrqHandler((stc_mftn_frt_t*)&MFT2_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt2].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT2_FRT ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_FRT)
/**
 ******************************************************************************
 ** \brief FM4: MFT2_FRT IRQ handler (IRQ#33) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT2_FRT_ZERO_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT2_FRT_ZERO_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_FRT2_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_FRT2_ZERO))
        {
            Dstc_MftFrtZeroIrqHandler(DSTC_IRQ_NUMBER_MFT2_FRT2_ZERO);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_FRT1_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_FRT1_ZERO))
        {
            Dstc_MftFrtZeroIrqHandler(DSTC_IRQ_NUMBER_MFT2_FRT1_ZERO);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_FRT2_ZERO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_FRT2_ZERO))
        {
            Dstc_MftFrtZeroIrqHandler(DSTC_IRQ_NUMBER_MFT2_FRT2_ZERO);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT2_FRT2_ZERO) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_FRT1_ZERO) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_FRT2_ZERO)

            Mft_Frt_IrqHandler((stc_mftn_frt_t*)&MFT2_FRT, &m_astcMftFrtInstanceDataLut[FrtInstanceIndexFrt2].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT2_ICU ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_ICU)
/**
 ******************************************************************************
 ** \brief FM4: MFT2_ICU IRQ handler (IRQ#34) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT2_ICU_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT2_ICU_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_ICU0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_ICU0))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_ICU0);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_ICU1)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_ICU1))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_ICU1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_ICU2)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_ICU2))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_ICU2);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_ICU3)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_ICU3))
        {
            Dstc_MftIcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_ICU3);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT2_ICU0) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_ICU1) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_ICU2) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_ICU3)

            Mft_Icu_IrqHandler((stc_mftn_icu_t*)&MFT2_ICU, &m_astcMftIcuInstanceDataLut[IcuInstanceIndexIcu2].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************* FM4: MFT2_OCU ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFT2_OCU)
/**
 ******************************************************************************
 ** \brief FM4: MFT2_OCU IRQ handler (IRQ#35) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFT2_OCU_AVAILABLE)
    OSAL_IRQ_HANDLER(MFT2_OCU_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_OCU0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_OCU0))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_OCU0);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_OCU1)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_OCU1))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_OCU1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_OCU2)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_OCU2))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_OCU2);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_OCU3)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_OCU3))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_OCU3);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_OCU4)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_OCU4))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_OCU4);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_MFT2_OCU5)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFT2_OCU5))
        {
            Dstc_MftOcuIrqHandler(DSTC_IRQ_NUMBER_MFT2_OCU5);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFT2_OCU0) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_OCU1) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_OCU2) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_OCU3) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_OCU4) && \
            (PDL_ON != PDL_DSTC_ENABLE_MFT2_OCU5)

            Mft_Ocu_IrqHandler((stc_mftn_ocu_t*)&MFT2_OCU, &m_astcMftOcuInstanceDataLut[OcuInstanceIndexOcu2].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: PPG **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_PPG)
/**
 ******************************************************************************
 ** \brief FM4: PPG IRQ handler (IRQ#36) Type b
 ******************************************************************************/
    #if (1u == IRQ_PPG00_02_04_AVAILABLE)
    OSAL_IRQ_HANDLER(PPG00_02_04_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_PPG0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG0))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG0);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_PPG2)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG2))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG2);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_PPG4)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG4))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG4);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_PPG0) && \
            (PDL_ON != PDL_DSTC_ENABLE_PPG2) && \
            (PDL_ON != PDL_DSTC_ENABLE_PPG4)

            Ppg_IrqHandler();

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: PPG **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_PPG)
/**
 ******************************************************************************
 ** \brief FM4: PPG IRQ handler (IRQ#37) Type b
 ******************************************************************************/
    #if (1u == IRQ_PPG08_10_12_AVAILABLE)
    OSAL_IRQ_HANDLER(PPG08_10_12_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_PPG8)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG8))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG8);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_PPG10)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG10))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG10);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_PPG12)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG12))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG12);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_PPG8) && \
            (PDL_ON != PDL_DSTC_ENABLE_PPG10) && \
            (PDL_ON != PDL_DSTC_ENABLE_PPG12)

            Ppg_IrqHandler();

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: PPG **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_PPG)
/**
 ******************************************************************************
 ** \brief FM4: PPG IRQ handler (IRQ#38) Type b
 ******************************************************************************/
    #if (1u == IRQ_PPG16_18_20_AVAILABLE)
    OSAL_IRQ_HANDLER(PPG16_18_20_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_PPG16)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG16))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG16);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_PPG18)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG18))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG18);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_PPG20)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_PPG20))
        {
            Dstc_PpgIrqHandler(DSTC_IRQ_NUMBER_PPG20);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_PPG16) && \
            (PDL_ON != PDL_DSTC_ENABLE_PPG18) && \
            (PDL_ON != PDL_DSTC_ENABLE_PPG20)

            Ppg_IrqHandler();

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT0)
/**
 ******************************************************************************
 ** \brief FM4: BT0 IRQ handler (IRQ#39) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT0_AVAILABLE)
    OSAL_IRQ_HANDLER(BT0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT0_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT0_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT0_IRQ0, DSTC_IRQ_NUMBER_BT0_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT0_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT0, &m_astcBtInstanceDataLut[BtInstanceIndexBt0].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT1 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT1)
/**
 ******************************************************************************
 ** \brief FM4: BT1 IRQ handler (IRQ#40) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT1_AVAILABLE)
    OSAL_IRQ_HANDLER(BT1_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT1_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT1_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT1_IRQ0, DSTC_IRQ_NUMBER_BT1_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT1_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT1, &m_astcBtInstanceDataLut[BtInstanceIndexBt1].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT2 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT2)
/**
 ******************************************************************************
 ** \brief FM4: BT2 IRQ handler (IRQ#41) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT2_AVAILABLE)
    OSAL_IRQ_HANDLER(BT2_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT2_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT2_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT2_IRQ0, DSTC_IRQ_NUMBER_BT2_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT2_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT2, &m_astcBtInstanceDataLut[BtInstanceIndexBt2].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT3 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT3)
/**
 ******************************************************************************
 ** \brief FM4: BT3 IRQ handler (IRQ#42) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT3_AVAILABLE)
    OSAL_IRQ_HANDLER(BT3_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT3_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT3_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT3_IRQ0, DSTC_IRQ_NUMBER_BT3_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT3_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT3, &m_astcBtInstanceDataLut[BtInstanceIndexBt3].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT4 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT4)
/**
 ******************************************************************************
 ** \brief FM4: BT4 IRQ handler (IRQ#43) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT4_AVAILABLE)
    OSAL_IRQ_HANDLER(BT4_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT4_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT4_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT4_IRQ0, DSTC_IRQ_NUMBER_BT4_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT4_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT4, &m_astcBtInstanceDataLut[BtInstanceIndexBt4].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT5 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT5)
/**
 ******************************************************************************
 ** \brief FM4: BT5 IRQ handler (IRQ#44) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT5_AVAILABLE)
    OSAL_IRQ_HANDLER(BT5_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT5_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT5_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT5_IRQ0, DSTC_IRQ_NUMBER_BT5_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT5_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT5, &m_astcBtInstanceDataLut[BtInstanceIndexBt5].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT6 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT6)
/**
 ******************************************************************************
 ** \brief FM4: BT6 IRQ handler (IRQ#45) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT6_AVAILABLE)
    OSAL_IRQ_HANDLER(BT6_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT6_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT6_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT6_IRQ0, DSTC_IRQ_NUMBER_BT6_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT6_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT6, &m_astcBtInstanceDataLut[BtInstanceIndexBt6].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT7 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT7)
/**
 ******************************************************************************
 ** \brief FM4: BT7 IRQ handler (IRQ#46) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT7_AVAILABLE)
    OSAL_IRQ_HANDLER(BT7_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT7_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT7_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT7_IRQ0, DSTC_IRQ_NUMBER_BT7_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT7_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT7, &m_astcBtInstanceDataLut[BtInstanceIndexBt7].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: DT0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DT0)
/**
 ******************************************************************************
 ** \brief FM4: DT0 IRQ handler (IRQ#47) Type b
 ******************************************************************************/
    #if (1u == IRQ_DT_AVAILABLE)
    OSAL_IRQ_HANDLER(DT_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ047MON;

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
        
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: WC0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_WC0)
/**
 ******************************************************************************
 ** \brief FM4: WC0 IRQ handler (IRQ#48) Type b
 ******************************************************************************/
    #if (1u == IRQ_WC_AVAILABLE)
    OSAL_IRQ_HANDLER(WC_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_WC)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_WC))
        {
            Dstc_WcIrqHandler();
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_WC)

            Wc_IrqHandler((stc_wcn_t*)&WC0);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: EXTIF *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXTIF)
/**
 ******************************************************************************
 ** \brief FM4: EXTIF IRQ handler (IRQ#49) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXTBUS_ERR_AVAILABLE)
    OSAL_IRQ_HANDLER(EXTBUS_ERR_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(8ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: RTC0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_RTC0)
/**
 ******************************************************************************
 ** \brief FM4: RTC0 IRQ handler (IRQ#50) Type b
 ******************************************************************************/
    #if (1u == IRQ_RTC_AVAILABLE)
    OSAL_IRQ_HANDLER(RTC_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        Rtc_IrqHandler((stc_rtcn_t*)&RTC0, &(m_astcRtcInstanceDataLut[RtcInstanceIndexRtc0].stcInternData));
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT8 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8)
/**
 ******************************************************************************
 ** \brief FM4: EXINT8 IRQ handler (IRQ#51) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT8_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT8_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT8)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT8))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT8);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT8)

            Exint_IrqHandler(8u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT9 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9)
/**
 ******************************************************************************
 ** \brief FM4: EXINT9 IRQ handler (IRQ#52) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT9_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT9_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT9)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT9))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT9);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT9)

            Exint_IrqHandler(9u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT10 ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10)
/**
 ******************************************************************************
 ** \brief FM4: EXINT10 IRQ handler (IRQ#53) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT10_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT10_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT10)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT10))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT10);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT10)

            Exint_IrqHandler(10u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT11 ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11)
/**
 ******************************************************************************
 ** \brief FM4: EXINT11 IRQ handler (IRQ#54) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT11_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT11_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT11)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT11))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT11);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT11)

            Exint_IrqHandler(11u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT12 ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12)
/**
 ******************************************************************************
 ** \brief FM4: EXINT12 IRQ handler (IRQ#55) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT12_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT12_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT12)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT12))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT12);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT12)

            Exint_IrqHandler(12u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT13 ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13)
/**
 ******************************************************************************
 ** \brief FM4: EXINT13 IRQ handler (IRQ#56) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT13_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT13_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT13)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT13))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT13);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT13)

            Exint_IrqHandler(13u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT14 ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14)
/**
 ******************************************************************************
 ** \brief FM4: EXINT14 IRQ handler (IRQ#57) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT14_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT14_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT14)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT14))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT14);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT14)

            Exint_IrqHandler(14u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: EXINT15 ********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15)
/**
 ******************************************************************************
 ** \brief FM4: EXINT15 IRQ handler (IRQ#58) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT15_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT15_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT15)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT15))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT15);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT15)

            Exint_IrqHandler(15u);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/*************************** FM4: CLK, UPLL, I2SPLL ***************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CLK) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_UPLL) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_I2SPLL)
/**
 ******************************************************************************
 ** \brief FM4: CLK, UPLL, I2SPLL IRQ handler (IRQ#59) Type b
 ******************************************************************************/
    #if (1u == IRQ_TIM_AVAILABLE)
    OSAL_IRQ_HANDLER(TIM_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ059MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CLK)
        if (0ul != (u32IrqMon & 0x00000007ul))
        {
            Clk_IrqHandler();
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_UPLL)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            PDL_DummyHandler(9ul);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_I2SPLL)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            PDL_DummyHandler(10ul);
        }
        #endif
        
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0)
/**
 ******************************************************************************
 ** \brief FM4: MFS0 IRQ handler (IRQ#60) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS0_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS0_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ060MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS0_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS0_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS0_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS0_RX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS0)
/**
 ******************************************************************************
 ** \brief FM4: MFS0 IRQ handler (IRQ#61) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS0_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS0_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ061MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS0_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS0_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS0_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS0_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS1 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1)
/**
 ******************************************************************************
 ** \brief FM4: MFS1 IRQ handler (IRQ#62) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS1_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS1_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ062MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS1_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS1_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS1_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS1_RX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS1 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS1)
/**
 ******************************************************************************
 ** \brief FM4: MFS1 IRQ handler (IRQ#63) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS1_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS1_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ063MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS1_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS1_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS1_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS1_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS2 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2)
/**
 ******************************************************************************
 ** \brief FM4: MFS2 IRQ handler (IRQ#64) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS2_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS2_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ064MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS2_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS2_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS2_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS2_RX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS2 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS2)
/**
 ******************************************************************************
 ** \brief FM4: MFS2 IRQ handler (IRQ#65) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS2_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS2_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ065MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS2_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS2_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS2_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS2_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS3 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3)
/**
 ******************************************************************************
 ** \brief FM4: MFS3 IRQ handler (IRQ#66) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS3_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS3_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ066MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS3_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS3_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS3_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS3_RX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS3 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS3)
/**
 ******************************************************************************
 ** \brief FM4: MFS3 IRQ handler (IRQ#67) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS3_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS3_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ067MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS3_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS3_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS3_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS3_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS4 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4)
/**
 ******************************************************************************
 ** \brief FM4: MFS4 IRQ handler (IRQ#68) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS4_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS4_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ068MON;

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
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS4 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS4)
/**
 ******************************************************************************
 ** \brief FM4: MFS4 IRQ handler (IRQ#69) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS4_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS4_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ069MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS4_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS4_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS4_TX);
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
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS5 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5)
/**
 ******************************************************************************
 ** \brief FM4: MFS5 IRQ handler (IRQ#70) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS5_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS5_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ070MON;

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
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS5 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS5)
/**
 ******************************************************************************
 ** \brief FM4: MFS5 IRQ handler (IRQ#71) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS5_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS5_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ071MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS5_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS5_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS5_TX);
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
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS6 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6)
/**
 ******************************************************************************
 ** \brief FM4: MFS6 IRQ handler (IRQ#72) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS6_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS6_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ072MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS6_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS6_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS6_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS6_RX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS6 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS6)
/**
 ******************************************************************************
 ** \brief FM4: MFS6 IRQ handler (IRQ#73) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS6_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS6_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ073MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS6_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS6_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS6_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS6_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS7 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7)
/**
 ******************************************************************************
 ** \brief FM4: MFS7 IRQ handler (IRQ#74) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS7_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS7_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ074MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS7_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS7_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS7_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS7_RX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS7 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS7)
/**
 ******************************************************************************
 ** \brief FM4: MFS7 IRQ handler (IRQ#75) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS7_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS7_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ075MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS7_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS7_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS7_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS7_TX)

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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: ADC0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC0)
/**
 ******************************************************************************
 ** \brief FM4: ADC0 IRQ handler (IRQ#76) Type b
 ******************************************************************************/
    #if (1u == IRQ_ADC0_AVAILABLE)
    OSAL_IRQ_HANDLER(ADC0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_ADC0_PRIO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_ADC0_PRIO))
        {
            Dstc_AdcIrqHandler(DSTC_IRQ_NUMBER_ADC0_PRIO, DSTC_IRQ_NUMBER_ADC0_SCAN);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_ADC0_PRIO)

            AdcIrqHandler((stc_adcn_t*)&ADC0, &(m_astcAdcInstanceDataLut[AdcInstanceIndexAdc0].stcInternData));

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: ADC1 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC1)
/**
 ******************************************************************************
 ** \brief FM4: ADC1 IRQ handler (IRQ#77) Type b
 ******************************************************************************/
    #if (1u == IRQ_ADC1_AVAILABLE)
    OSAL_IRQ_HANDLER(ADC1_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_ADC1_PRIO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_ADC1_PRIO))
        {
            Dstc_AdcIrqHandler(DSTC_IRQ_NUMBER_ADC1_PRIO, DSTC_IRQ_NUMBER_ADC1_SCAN);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_ADC1_PRIO)

            AdcIrqHandler((stc_adcn_t*)&ADC1, &(m_astcAdcInstanceDataLut[AdcInstanceIndexAdc1].stcInternData));

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************************** FM4: USB0_DEVICE ******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_DEVICE)
/**
 ******************************************************************************
 ** \brief FM4: USB0_DEVICE IRQ handler (IRQ#78) Type b
 ******************************************************************************/
    #if (1u == IRQ_USB0_F_AVAILABLE)
    OSAL_IRQ_HANDLER(USB0_F_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
    	// [andreika]: fix for newer MCUHEADER
        Usb_IrqHandlerF((stc_usbn_t*)&USB0);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/************************ FM4: USB0_HOST, USB0_DEVICE *************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_HOST) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_USB0_DEVICE)
/**
 ******************************************************************************
 ** \brief FM4: USB0_HOST, USB0_DEVICE IRQ handler (IRQ#79) Type b
 ******************************************************************************/
    #if (1u == IRQ_USB0_H_F_AVAILABLE)
    OSAL_IRQ_HANDLER(USB0_H_F_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
    	// [andreika]: fix for newer MCUHEADER
        Usb_IrqHandlerH((stc_usbn_t*)&USB0);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: CAN0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CAN0)
/**
 ******************************************************************************
 ** \brief FM4: CAN0 IRQ handler (IRQ#80) Type b
 ******************************************************************************/
    #if (1u == IRQ_CAN0_AVAILABLE)
    OSAL_IRQ_HANDLER(CAN0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        CanIrqHandler((stc_cann_t*) &CAN0, &(m_astcCanInstanceDataLut[CanInstanceIndexCan0].stcInternData));
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************** FM4: CAN1, CANFD0, CANFD1, CANFD **********************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_CAN1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CANFD0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CANFD1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CANFD)

/**
 ******************************************************************************
 ** \brief FM4: CAN1, CANFD0, CANFD1, CANFD IRQ handler (IRQ#81) Type b
 ******************************************************************************/
    #if (1u == IRQ_CAN1_CANFD0_AVAILABLE)
    OSAL_IRQ_HANDLER(CAN1_CANFD0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ081MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CAN1)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            CanIrqHandler((stc_cann_t*) &CAN1, &(m_astcCanInstanceDataLut[CanInstanceIndexCan1].stcInternData));
        }
        #endif
// [andreika]: disable error interrupts for now; are they buggy?
#if 0        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CANFD0)
        // CAN-FD double bit error interrupt
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            // [andreika]: fix for newer MCUHEADER
            CanfdIrqHandler((stc_canfdn_t*)&CANFD0);
            //PDL_DummyHandler(11ul);
        }
        // CAN-FD single bit error interrupt
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            // [andreika]: fix for newer MCUHEADER
            CanfdIrqHandler((stc_canfdn_t*)&CANFD0);
            //PDL_DummyHandler(12ul);
        }
        #endif
#endif

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CANFD0)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            // [andreika]: fix for newer MCUHEADER
            CanfdIrqHandler((stc_canfdn_t*)&CANFD0);
            //PDL_DummyHandler(13ul);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CANFD1)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            // [andreika]: fix for newer MCUHEADER
            CanfdIrqHandler((stc_canfdn_t*)&CANFD1);
            //PDL_DummyHandler(14ul);
        }
        #endif
        OSAL_IRQ_EPILOGUE();
    }
    #elif (1u == IRQ_CAN1_AVAILABLE)
    OSAL_IRQ_HANDLER(CAN1_IRQHandler)
    {
        OSAL_IRQ_PROLOGUE();
        CanIrqHandler((stc_cann_t*) &CAN1, &(m_astcCanInstanceDataLut[CanInstanceIndexCan1].stcInternData));
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/******************************** FM4: ETHER0 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ETHER0)
/**
 ******************************************************************************
 ** \brief FM4: ETHER0 IRQ handler (IRQ#82) Type b
 ******************************************************************************/
    #if (1u == IRQ_ETHER0_AVAILABLE)
    OSAL_IRQ_HANDLER(ETHER0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        EmacIrqHandler((FM_ETHERNET_MAC_TypeDef*)&EMAC0, &(m_astcEmacInstanceDataLut[EmacInstanceIndexEmac0].stcInternData));
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: DMA0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA0)
/**
 ******************************************************************************
 ** \brief FM4: DMA0 IRQ handler (IRQ#83) Type b
 ******************************************************************************/
    #if (1u == IRQ_DMAC0_AVAILABLE)
    OSAL_IRQ_HANDLER(DMAC0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        DmaIrqHandler(0u);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: DMA1 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA1)
/**
 ******************************************************************************
 ** \brief FM4: DMA1 IRQ handler (IRQ#84) Type b
 ******************************************************************************/
    #if (1u == IRQ_DMAC1_AVAILABLE)
    OSAL_IRQ_HANDLER(DMAC1_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        DmaIrqHandler(1u);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: DMA2 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA2)
/**
 ******************************************************************************
 ** \brief FM4: DMA2 IRQ handler (IRQ#85) Type b
 ******************************************************************************/
    #if (1u == IRQ_DMAC2_AVAILABLE)
    OSAL_IRQ_HANDLER(DMAC2_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        DmaIrqHandler(2u);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: DMA3 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA3)
/**
 ******************************************************************************
 ** \brief FM4: DMA3 IRQ handler (IRQ#86) Type b
 ******************************************************************************/
    #if (1u == IRQ_DMAC3_AVAILABLE)
    OSAL_IRQ_HANDLER(DMAC3_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        DmaIrqHandler(3u);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: DMA4 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA4)
/**
 ******************************************************************************
 ** \brief FM4: DMA4 IRQ handler (IRQ#87) Type b
 ******************************************************************************/
    #if (1u == IRQ_DMAC4_AVAILABLE)
    OSAL_IRQ_HANDLER(DMAC4_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        DmaIrqHandler(4u);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: DMA5 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA5)
/**
 ******************************************************************************
 ** \brief FM4: DMA5 IRQ handler (IRQ#88) Type b
 ******************************************************************************/
    #if (1u == IRQ_DMAC5_AVAILABLE)
    OSAL_IRQ_HANDLER(DMAC5_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        DmaIrqHandler(5u);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: DMA6 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA6)
/**
 ******************************************************************************
 ** \brief FM4: DMA6 IRQ handler (IRQ#89) Type b
 ******************************************************************************/
    #if (1u == IRQ_DMAC6_AVAILABLE)
    OSAL_IRQ_HANDLER(DMAC6_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        DmaIrqHandler(6u);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: DMA7 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DMA7)
/**
 ******************************************************************************
 ** \brief FM4: DMA7 IRQ handler (IRQ#90) Type b
 ******************************************************************************/
    #if (1u == IRQ_DMAC7_AVAILABLE)
    OSAL_IRQ_HANDLER(DMAC7_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        DmaIrqHandler(7u);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: DSTC **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DSTC)
/**
 ******************************************************************************
 ** \brief FM4: DSTC IRQ handler (IRQ#91) Type b
 ******************************************************************************/
    #if (1u == IRQ_DSTC_AVAILABLE)
    OSAL_IRQ_HANDLER(DSTC_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(15ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************** FM4: EXINT16, EXINT17, EXINT18, EXINT19 *******************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19)
/**
 ******************************************************************************
 ** \brief FM4: EXINT16, EXINT17, EXINT18, EXINT19 IRQ handler (IRQ#92) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT16_19_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT16_19_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ092MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT16)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT16))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT16);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT17)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT17))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT17);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT18)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT18))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT18);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT19)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT19))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT19);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT16) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT17) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT18) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT19)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Exint_IrqHandler(16u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            Exint_IrqHandler(17u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            Exint_IrqHandler(18u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Exint_IrqHandler(19u);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************** FM4: EXINT20, EXINT21, EXINT22, EXINT23 *******************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23)
/**
 ******************************************************************************
 ** \brief FM4: EXINT20, EXINT21, EXINT22, EXINT23 IRQ handler (IRQ#93) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT20_23_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT20_23_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ093MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT20)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT20))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT20);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT21)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT21))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT21);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT22)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT22))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT22);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT23)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT23))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT23);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT20) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT21) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT22) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT23)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Exint_IrqHandler(20u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            Exint_IrqHandler(21u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            Exint_IrqHandler(22u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Exint_IrqHandler(24u);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************** FM4: EXINT24, EXINT25, EXINT26, EXINT27 *******************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27)
/**
 ******************************************************************************
 ** \brief FM4: EXINT24, EXINT25, EXINT26, EXINT27 IRQ handler (IRQ#94) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT24_27_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT24_27_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ094MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT24)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT24))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT24);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT25)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT25))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT25);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT26)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT26))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT26);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT27)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT27))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT27);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT24) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT25) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT26) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT27)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Exint_IrqHandler(24u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            Exint_IrqHandler(25u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            Exint_IrqHandler(26u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Exint_IrqHandler(27u);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************** FM4: EXINT28, EXINT29, EXINT30, EXINT31 *******************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT29) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31)
/**
 ******************************************************************************
 ** \brief FM4: EXINT28, EXINT29, EXINT30, EXINT31 IRQ handler (IRQ#95) Type b
 ******************************************************************************/
    #if (1u == IRQ_EXINT28_31_AVAILABLE)
    OSAL_IRQ_HANDLER(EXINT28_31_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ095MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT28)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT28))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT28);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT29)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT29))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT29);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT30)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT30))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT30);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_EXINT31)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_EXINT31))
        {
            Dstc_ExintIrqHandler(DSTC_IRQ_NUMBER_EXINT31);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_EXINT28) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT29) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT30) && \
            (PDL_ON != PDL_DSTC_ENABLE_EXINT31)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Exint_IrqHandler(28u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT29)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            Exint_IrqHandler(29u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            Exint_IrqHandler(30u);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Exint_IrqHandler(31u);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: QPRC2 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC2)
/**
 ******************************************************************************
 ** \brief FM4: QPRC2 IRQ handler (IRQ#96) Type b
 ******************************************************************************/
    #if (1u == IRQ_QPRC2_AVAILABLE)
    OSAL_IRQ_HANDLER(QPRC2_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_COUNT_INVERSION)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_COUNT_INVERSION))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_COUNT_INVERSION);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_OUT_OF_RANGE)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_OUT_OF_RANGE))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_OUT_OF_RANGE);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_MATCH_RC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH_RC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH_RC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_RC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_RC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_RC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_UFL_OFL_Z)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_UFL_OFL_Z))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_UFL_OFL_Z);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_QPRC0_COUNT_INVERSION) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_OUT_OF_RANGE) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_MATCH_RC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_RC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_UFL_OFL_Z)

            Qprc_IrqHandler((stc_qprcn_t*)&QPRC2, &m_astcQprcInstanceDataLut[QprcInstanceIndexQprc2].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: QPRC3 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_QPRC3)
/**
 ******************************************************************************
 ** \brief FM4: QPRC3 IRQ handler (IRQ#97) Type b
 ******************************************************************************/
    #if (1u == IRQ_QPRC3_AVAILABLE)
    OSAL_IRQ_HANDLER(QPRC3_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_COUNT_INVERSION)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_COUNT_INVERSION))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_COUNT_INVERSION);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_OUT_OF_RANGE)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_OUT_OF_RANGE))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_OUT_OF_RANGE);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_MATCH_RC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH_RC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_MATCH_RC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_PC_RC_MATCH)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_PC_RC_MATCH))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_PC_RC_MATCH);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_QPRC0_UFL_OFL_Z)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_QPRC0_UFL_OFL_Z))
        {
            Dstc_QprcIrqHandler(DSTC_IRQ_NUMBER_QPRC0_UFL_OFL_Z);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_QPRC0_COUNT_INVERSION) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_OUT_OF_RANGE) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_MATCH_RC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_PC_RC_MATCH) && \
            (PDL_ON != PDL_DSTC_ENABLE_QPRC0_UFL_OFL_Z)

            Qprc_IrqHandler((stc_qprcn_t*)&QPRC3, &m_astcQprcInstanceDataLut[QprcInstanceIndexQprc3].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT8 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT8)
/**
 ******************************************************************************
 ** \brief FM4: BT8 IRQ handler (IRQ#98) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT8_AVAILABLE)
    OSAL_IRQ_HANDLER(BT8_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT8_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT8_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT8_IRQ0, DSTC_IRQ_NUMBER_BT8_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT8_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT8, &m_astcBtInstanceDataLut[BtInstanceIndexBt8].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: BT9 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT9)
/**
 ******************************************************************************
 ** \brief FM4: BT9 IRQ handler (IRQ#99) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT9_AVAILABLE)
    OSAL_IRQ_HANDLER(BT9_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT9_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT9_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT9_IRQ0, DSTC_IRQ_NUMBER_BT9_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT9_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT9, &m_astcBtInstanceDataLut[BtInstanceIndexBt9].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: BT10 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT10)
/**
 ******************************************************************************
 ** \brief FM4: BT10 IRQ handler (IRQ#100) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT10_AVAILABLE)
    OSAL_IRQ_HANDLER(BT10_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT10_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT10_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT10_IRQ0, DSTC_IRQ_NUMBER_BT10_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT10_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT10, &m_astcBtInstanceDataLut[BtInstanceIndexBt10].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: BT11 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT11)
/**
 ******************************************************************************
 ** \brief FM4: BT11 IRQ handler (IRQ#101) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT11_AVAILABLE)
    OSAL_IRQ_HANDLER(BT11_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_BT11_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT11_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT11_IRQ0, DSTC_IRQ_NUMBER_BT11_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT11_IRQ0)

            Bt_IrqHandler((stc_btn_t*)&BT11, &m_astcBtInstanceDataLut[BtInstanceIndexBt11].stcInternData);

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/************************ FM4: BT12, BT13, BT14, BT15 *************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_BT12) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT13) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT14) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_BT15)
/**
 ******************************************************************************
 ** \brief FM4: BT12, BT13, BT14, BT15 IRQ handler (IRQ#102) Type b
 ******************************************************************************/
    #if (1u == IRQ_BT12_15_AVAILABLE)
    OSAL_IRQ_HANDLER(BT12_15_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ102MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_BT12_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT12_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT12_IRQ0, DSTC_IRQ_NUMBER_BT12_IRQ1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_BT13_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT13_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT13_IRQ0, DSTC_IRQ_NUMBER_BT13_IRQ1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_BT14_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT14_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT14_IRQ0, DSTC_IRQ_NUMBER_BT14_IRQ1);
        }
        #endif
        
        #if (PDL_ON == PDL_DSTC_ENABLE_BT15_IRQ0)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_BT15_IRQ0))
        {
            Dstc_BtIrqHandler(DSTC_IRQ_NUMBER_BT15_IRQ0, DSTC_IRQ_NUMBER_BT15_IRQ1);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_BT12_IRQ0) && \
            (PDL_ON != PDL_DSTC_ENABLE_BT13_IRQ0) && \
            (PDL_ON != PDL_DSTC_ENABLE_BT14_IRQ0) && \
            (PDL_ON != PDL_DSTC_ENABLE_BT15_IRQ0)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT12)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            Bt_IrqHandler((stc_btn_t*)&BT12, &m_astcBtInstanceDataLut[BtInstanceIndexBt12].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT13)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            Bt_IrqHandler((stc_btn_t*)&BT13, &m_astcBtInstanceDataLut[BtInstanceIndexBt13].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT14)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            Bt_IrqHandler((stc_btn_t*)&BT14, &m_astcBtInstanceDataLut[BtInstanceIndexBt14].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_BT15)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Bt_IrqHandler((stc_btn_t*)&BT15, &m_astcBtInstanceDataLut[BtInstanceIndexBt15].stcInternData);
        }
        #endif
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS8 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8)
/**
 ******************************************************************************
 ** \brief FM4: MFS8 IRQ handler (IRQ#103) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS8_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS8_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ103MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS8_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS8_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS8_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS8_RX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS8 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8)
/**
 ******************************************************************************
 ** \brief FM4: MFS8 IRQ handler (IRQ#104) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS8_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS8_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ104MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS8_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS8_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS8_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS8_TX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS8 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS9 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9)
/**
 ******************************************************************************
 ** \brief FM4: MFS9 IRQ handler (IRQ#105) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS9_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS9_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ105MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS9_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS9_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS9_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS9_RX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS9 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9)
/**
 ******************************************************************************
 ** \brief FM4: MFS9 IRQ handler (IRQ#106) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS9_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS9_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ106MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS9_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS9_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS9_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS9_TX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS9 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS10 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10)
/**
 ******************************************************************************
 ** \brief FM4: MFS10 IRQ handler (IRQ#107) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS10_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS10_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ107MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS10_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS10_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS10_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS10_RX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS10 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10)
/**
 ******************************************************************************
 ** \brief FM4: MFS10 IRQ handler (IRQ#108) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS10_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS10_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ108MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS10_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS10_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS10_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS10_TX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS10 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS11 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11)
/**
 ******************************************************************************
 ** \brief FM4: MFS11 IRQ handler (IRQ#109) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS11_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS11_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ109MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS11_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS11_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS11_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS11_RX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS11 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11)
/**
 ******************************************************************************
 ** \brief FM4: MFS11 IRQ handler (IRQ#110) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS11_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS11_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ110MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS11_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS11_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS11_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS11_TX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS11 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: ADC2 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_ADC2)
/**
 ******************************************************************************
 ** \brief FM4: ADC2 IRQ handler (IRQ#111) Type b
 ******************************************************************************/
    #if (1u == IRQ_ADC2_AVAILABLE)
    OSAL_IRQ_HANDLER(ADC2_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        #if (PDL_ON == PDL_DSTC_ENABLE_ADC2_PRIO)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_ADC2_PRIO))
        {
            Dstc_AdcIrqHandler(DSTC_IRQ_NUMBER_ADC2_PRIO, DSTC_IRQ_NUMBER_ADC2_SCAN);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_ADC2_PRIO)

            AdcIrqHandler((stc_adcn_t*)&ADC2, &(m_astcAdcInstanceDataLut[AdcInstanceIndexAdc2].stcInternData));

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/**************************** FM4: DSTC_HWINT_NEW *****************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_DSTC_HWINT_NEW)
/**
 ******************************************************************************
 ** \brief FM4: DSTC_HWINT_NEW IRQ handler (IRQ#112) Type b
 ******************************************************************************/
    #if (1u == IRQ_DSTC_HWINT_AVAILABLE)
    OSAL_IRQ_HANDLER(DSTC_HWINT_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        Dstc_IrqHandler();
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/*************************** FM4: USB1_DEVICE, CEC0 ***************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_DEVICE) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CEC0)
/**
 ******************************************************************************
 ** \brief FM4: USB1_DEVICE, CEC0 IRQ handler (IRQ#113) Type b
 ******************************************************************************/
    #if (1u == IRQ_USB1_F_HDMICEC0_AVAILABLE)
    OSAL_IRQ_HANDLER(USB1_F_HDMICEC0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ113MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_DEVICE)
        if (0ul != (u32IrqMon & 0x0000001Ful))
        {
            Usb_IrqHandler((stc_usb_t*)&USB1);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CEC0)
        if (0ul != (u32IrqMon & 0x00000020ul))
        {
            RcIrqHandler(0u);
        }
        #endif
        
    }
    #elif (1u == IRQ_USB1_F_AVAILABLE)
    void USB1_F_IRQHandler(void)
    {
        Usb_IrqHandler((stc_usb_t*)&USB1);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************* FM4: USB1_HOST, USB1_DEVICE, CEC1 **********************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_HOST) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_DEVICE) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_CEC1)
/**
 ******************************************************************************
 ** \brief FM4: USB1_HOST, USB1_DEVICE, CEC1 IRQ handler (IRQ#114) Type b
 ******************************************************************************/
    #if (1u == IRQ_USB1_H_F_HDMICEC1_AVAILABLE)
    OSAL_IRQ_HANDLER(USB1_H_F_HDMICEC1_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ114MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_USB1_HOST) ||(PDL_ON == PDL_INTERRUPT_ENABLE_USB1_DEVICE)
        if (0ul != (u32IrqMon & 0x0000003Ful))
        {
            Usb_IrqHandler((stc_usb_t*)&USB1);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_CEC1)
        if (0ul != (u32IrqMon & 0x00000040ul))
        {
            Rc_IrqHandler(1u);
        }
        #endif
        
    }
    #elif (1u == IRQ_USB1_H_F_AVAILABLE)
    void USB1_H_F_IRQHandler(void)
    {
        Usb_IrqHandler((stc_usb_t*)&USB1);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: HSSPI *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_HSSPI)
/**
 ******************************************************************************
 ** \brief FM4: HSSPI IRQ handler (IRQ#115) Type b
 ******************************************************************************/
    #if (1u == IRQ_HSSPI0_AVAILABLE)
    OSAL_IRQ_HANDLER(HSSPI0_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        HsspiHandler();
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/**************** FM4: I2S0, PCRC, I2S1, SD0, SD1, ICC0, ICC01 ****************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_I2S0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_PCRC) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_I2S1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_SD0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_SD1) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_ICC0) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_ICC01)
/**
 ******************************************************************************
 ** \brief FM4: I2S0, PCRC, I2S1, SD0, SD1, ICC0, ICC01 IRQ handler (IRQ#117) Type b
 ******************************************************************************/
    #if (1u == IRQ_PCRC_I2S0_1_AVAILABLE)
    OSAL_IRQ_HANDLER(PCRC_I2S0_1_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ117MON;

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_I2S0)
        if (0ul != (u32IrqMon & 0x00000001ul))
        {
            I2sIrqHandler(&I2S0, &m_astcI2sInstanceDataLut[I2sInstanceIndexI2s0].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_PCRC)
        if (0ul != (u32IrqMon & 0x00000002ul))
        {
            PcrcIrqHandler();
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_I2S1)
        if (0ul != (u32IrqMon & 0x00000004ul))
        {
            I2sIrqHandler(&I2S1, &m_astcI2sInstanceDataLut[I2sInstanceIndexI2s1].stcInternData);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_SD0)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            PDL_DummyHandler(16ul);
        }
        #endif
        
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_SD1)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            PDL_DummyHandler(17ul);
        }
        #endif
        
    }
    #elif (1u == IRQ_ICC0_1_AVAILABLE)
    void ICC0_1_IRQHandler(void)
    {
        #if (PDL_ON == PDL_INTERRUPT_ENABLE_ICC0) || \
            (PDL_ON == PDL_INTERRUPT_ENABLE_ICC01)
        uint32_t u32IrqMon = FM_INTREQ->IRQ117MON;
        #endif

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_ICC0)
        if (0ul != (u32IrqMon & 0x00000008ul))
        {
            Icc_IrqHandler((stc_iccn_t*) &ICC0, &(m_astcIccInstanceDataLut[IccInstanceIndexIcc0].stcInternData));
        }
        #endif

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_ICC01)
        if (0ul != (u32IrqMon & 0x00000010ul))
        {
            Icc_IrqHandler((stc_iccn_t*) &ICC1, &(m_astcIccInstanceDataLut[IccInstanceIndexIcc1].stcInternData));
        }
        #endif
        
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************** FM4: SD0 **********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_SD0)
/**
 ******************************************************************************
 ** \brief FM4: SD0 IRQ handler (IRQ#118) Type b
 ******************************************************************************/
    #if (1u == IRQ_SD_AVAILABLE)
    OSAL_IRQ_HANDLER(SD_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(18ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: FLASH *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_FLASH)
/**
 ******************************************************************************
 ** \brief FM4: FLASH IRQ handler (IRQ#119) Type b
 ******************************************************************************/
    #if (1u == IRQ_FLASHIF_AVAILABLE)
    OSAL_IRQ_HANDLER(FLASHIF_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        PDL_DummyHandler(19ul);
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS12 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12)
/**
 ******************************************************************************
 ** \brief FM4: MFS12 IRQ handler (IRQ#120) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS12_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS12_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ120MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS12_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS12_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS12_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS12_RX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS12 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12)
/**
 ******************************************************************************
 ** \brief FM4: MFS12 IRQ handler (IRQ#121) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS12_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS12_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ121MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS12_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS12_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS12_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS12_TX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS12 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS13 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13)
/**
 ******************************************************************************
 ** \brief FM4: MFS13 IRQ handler (IRQ#122) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS13_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS13_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ122MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS13_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS13_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS13_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS13_RX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS13 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13)
/**
 ******************************************************************************
 ** \brief FM4: MFS13 IRQ handler (IRQ#123) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS13_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS13_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ123MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS13_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS13_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS13_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS13_TX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS13 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS14 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14)
/**
 ******************************************************************************
 ** \brief FM4: MFS14 IRQ handler (IRQ#124) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS14_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS14_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ124MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS14_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS14_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS14_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS14_RX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS14 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14)
/**
 ******************************************************************************
 ** \brief FM4: MFS14 IRQ handler (IRQ#125) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS14_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS14_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ125MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS14_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS14_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS14_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS14_TX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS14 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS15 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15)
/**
 ******************************************************************************
 ** \brief FM4: MFS15 IRQ handler (IRQ#126) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS15_RX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS15_RX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ126MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS15_RX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS15_RX))
        {
            Dstc_MfsRxIrqHandler(DSTC_IRQ_NUMBER_MFS15_RX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS15_RX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/********************************* FM4: MFS15 *********************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15)
/**
 ******************************************************************************
 ** \brief FM4: MFS15 IRQ handler (IRQ#127) Type b
 ******************************************************************************/
    #if (1u == IRQ_MFS15_TX_AVAILABLE)
    OSAL_IRQ_HANDLER(MFS15_TX_IRQHandler)
    {
    	OSAL_IRQ_PROLOGUE();
        uint32_t u32IrqMon = FM_INTREQ->IRQ127MON;

        #if (PDL_ON == PDL_DSTC_ENABLE_MFS15_TX)
        if (TRUE == Dstc_ReadHwintBit(DSTC_IRQ_NUMBER_MFS15_TX))
        {
            Dstc_MfsTxIrqHandler(DSTC_IRQ_NUMBER_MFS15_TX);
        }
        #endif
        

        #if (PDL_ON != PDL_DSTC_ENABLE_MFS15_TX)

        #if (PDL_ON == PDL_INTERRUPT_ENABLE_MFS15 )
        if (0ul != (u32IrqMon & 0x00000001ul))
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
        if (0ul != (u32IrqMon & 0x00000002ul))
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
        

        #endif // #if (PDL_ON != PDL_DSTC_ENABLE_... )
    	OSAL_IRQ_EPILOGUE();
    }
    #else
        #error No IRQ handler found.
    #endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


/******************************************************************************/
/****************************** FM4: NMI, HWWDG *******************************/
/******************************************************************************/
#if (PDL_ON == PDL_INTERRUPT_ENABLE_NMI) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_HWWDG)
/**
 ******************************************************************************
 ** \brief FM4: NMI, HWWDG IRQ handler (EXC#2) Type b
 ******************************************************************************/
    #if (1u == IRQ_NMI_AVAILABLE)
OSAL_IRQ_HANDLER(NMI_Handler)
{
	OSAL_IRQ_PROLOGUE();
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
    OSAL_IRQ_EPILOGUE();
}
#else
    #error No IRQ handler found.
#endif // #if (1u == IRQ_ ... _AVAILABLE)
#endif // #if (PDL_ON == PDL_INTERRUPT_ENABLE_ ... )


#endif // #ifdef __INTERRUPTS_FM4_TYPE_B_C__

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
