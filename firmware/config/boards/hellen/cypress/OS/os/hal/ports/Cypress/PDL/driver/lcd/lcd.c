/*******************************************************************************
* \file             lcd.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the LCD 
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
#include "lcd/lcd.h"

#if (defined(PDL_PERIPHERAL_LCD_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
#define LCD_RAM_MAX_INDEX   (39u)

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_LCD == PDL_ON)
static func_ptr_t m_pfnIrqCb = NULL;
#endif

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

#if (PDL_INTERRUPT_ENABLE_LCD == PDL_ON)
/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts according CMSIS with
 **          level defined in pdl.h
 ******************************************************************************/
static void LcdInitNvic(void)
{    
    NVIC_ClearPendingIRQ(LCDC_IRQn);
    NVIC_EnableIRQ(LCDC_IRQn);
    NVIC_SetPriority(LCDC_IRQn, PDL_IRQ_LEVEL_LCDC);

    return;
}

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS 
 ******************************************************************************/
static void LcdDeInitNvic(void)
{ 
    NVIC_ClearPendingIRQ(LCDC_IRQn);
    NVIC_DisableIRQ(LCDC_IRQn);
    NVIC_SetPriority(LCDC_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    
    return;
}

/**
 ******************************************************************************
 ** \brief Irq of the LCD module
 ******************************************************************************/
void Lcd_IrqHandler(void)
{    
    if(1u == bFM_LCDC_LCDCC2_LCDIF)
    {
        bFM_LCDC_LCDCC2_LCDIF = 0u;
        if(NULL != m_pfnIrqCb)
        {
            m_pfnIrqCb(); 
        }
    }

    return;
}
#endif

/**
 ******************************************************************************
 ** \brief Initialize the LCD module
 **
 ** \param pstcConfig: LCD module configuration 
 **
 ** \retval  Ok                             Initializiation of LCD  successfully  done.
 ** \retval  ErrorInvalidParameter          If one of following condition matches:
 **                                                       - pstcConfig == NULL
 **                                                       - pstcConfig->enDispMode  > Lcd_8Com_1Div8Duty
 **                                                       - pstcConfig->enDivResVal > LcdDivRes10K
 **                                                       - pstcConfig->en8ComBias > Lcd1Div4Bias
 **                                                       - pstcConfig->enInputIoMode > LcdInputIoConnect
 **                                                       - pstcConfig->stcClkConf.u32DivVal > 0x200000u
 **                                                       - pstcConfig->stcClkConf.u32DivVal == 0x00u 
 **                                                       - pstcConfig->stcClkConf.enSrcClk > LcdClkSelPclk
 ** 
 ******************************************************************************/
en_result_t Lcd_Init(const stc_lcd_config_t* pstcConfig)
{
    en_result_t             enResult = ErrorInvalidParameter;
    stc_lcdc_lcdcc1_field_t stcLCDCC1;   
    stc_lcdc_lcdcc2_field_t stcLCDCC2;   
    stc_lcdc_lcdcc3_field_t stcLCDCC3;   
    stc_lcdc_lcdc_blink_field_t stcLCDC_BLINK;   
#if (PDL_MCU_CORE == PDL_FM0P_CORE)
    stc_lcdc_lcdc_booster_field_t stcLCDC_BOOSTER;
#endif
    if (NULL != pstcConfig)
    {
        PDL_ZERO_STRUCT(stcLCDCC1);
        PDL_ZERO_STRUCT(stcLCDCC2);
        PDL_ZERO_STRUCT(stcLCDCC3);        
        PDL_ZERO_STRUCT(stcLCDC_BLINK);        
#if (PDL_MCU_CORE == PDL_FM0P_CORE)
        PDL_ZERO_STRUCT(stcLCDC_BOOSTER);
#endif
        /*LCDC Control Register 1*/
        stcLCDCC1.LCDEN = ((pstcConfig->bTimerMod == TRUE) ? 1u : 0u);
        stcLCDCC1.VSEL  = ((pstcConfig->enDivMode == LcdcIntRes) ? 1u : 0u); //VSEL = 1 internal divider resister     
        switch(pstcConfig->enDispMode)
        {
            case LcdStop:
                stcLCDCC1.MS = 0u;
                break;
            case Lcd_4Com_1Div2Duty:
                stcLCDCC1.MS = 1u;       // 4COM 1/2 bias
                break;
            case Lcd_4Com_1Div3Duty:
                stcLCDCC1.MS = 2u;       // 4COM 1/3 bias 
                break;
            case Lcd_4Com_1Div4Duty:
                stcLCDCC1.MS = 3u;       // 4COM 1/4 bias 
                break;
            case Lcd_8Com_1Div8Duty:
                stcLCDCC1.MS = 4u;       // 8COM 1/8 bias 
                break;
            default:
                return ErrorInvalidParameter;
        }
        // Update hardware
        FM_LCDC->LCDCC1_f = stcLCDCC1;
        
        /*LCDC Control Register 2*/
        switch(pstcConfig->enDivResVal)
        {
            case LcdDivRes100K:
                stcLCDCC2.RSEL = 0u;
                break;
            case LcdDivRes10K:
                stcLCDCC2.RSEL = 1u; 
                break;
            default:
                return ErrorInvalidParameter;
        }

        if(Lcd_8Com_1Div8Duty == pstcConfig->enDispMode)
        {
            switch(pstcConfig->en8ComBias)
            {
                case Lcd1Div3Bias:
                    stcLCDCC2.BLS8 = 0u;
                    break;
                case Lcd1Div4Bias:
                    stcLCDCC2.BLS8 = 1u; 
                    break;
                default:
                    return ErrorInvalidParameter;
            }
        }
        stcLCDCC2.INV  = ((pstcConfig->bEnDispRevs == TRUE) ? 1u : 0u);     
        stcLCDCC2.BK = ((pstcConfig->bEnBlankDisp == TRUE) ? 1u : 0u);   
        stcLCDCC2.LCDIF = 0u; 
        // Update hardware
        FM_LCDC->LCDCC2_f = stcLCDCC2;

        /*LCDC Control Register 3*/                
        // Update hardware        
        FM_LCDC->LCDCC3 = (pstcConfig->stcVvSel.u8Vv << 1u);
        switch(pstcConfig->enInputIoMode)
        {
            case LcdInputIoCutoff:
                FM_LCDC->LCDCC3_f.PICTL = 0u;
                break;
            case LcdInputIoConnect:
                FM_LCDC->LCDCC3_f.PICTL = 1u;  
                break;
            default:
                return ErrorInvalidParameter;
        }
        
        /*LCDC Clock Prescaler Register*/
        if((pstcConfig->stcClkConf.u32DivVal > 0x200000u) || 
            (0u == pstcConfig->stcClkConf.u32DivVal))   
        {
            return ErrorInvalidParameter;
        }
        FM_LCDC->LCDC_PSR = pstcConfig->stcClkConf.u32DivVal;
        switch(pstcConfig->stcClkConf.enSrcClk)
        {
            case LcdClkSelSubClk:
                FM_LCDC->LCDC_PSR_f.CLKSEL = 0u;
                break;
            case LcdClkSelPclk:
                FM_LCDC->LCDC_PSR_f.CLKSEL = 1u;       
                break;
            default:
                return ErrorInvalidParameter;
        }
        
        /*LCDC COM Output Enable Register*/
        // Update hardware
        FM_LCDC->LCDC_COMEN  = pstcConfig->stcComSel.u8ComSel;

        /*LCDC SEG Output Enable Register 1 */
        // Update hardware
        FM_LCDC->LCDC_SEGEN1  = pstcConfig->stcSegSel1.u32SegSel1;
        FM_LCDC->LCDC_SEGEN2  = pstcConfig->stcSegSel2.u32SegSel2;

#if (PDL_MCU_CORE == PDL_FM0P_CORE)
        if(TRUE == pstcConfig->bEnBooster)
        {
            FM_LCDC->LCDC_BOOSTER_f.BTRC = pstcConfig->stcBooster.BTRC;
            FM_LCDC->LCDC_BOOSTER_f.BTRF = pstcConfig->stcBooster.BTRF;
            FM_LCDC->LCDC_BOOSTER_f.CENSEL = pstcConfig->stcBooster.CENSEL;
            FM_LCDC->LCDC_BOOSTER_f.BSTOPT = pstcConfig->stcBooster.BSTOPT;
            FM_LCDC->LCDC_BOOSTER_f.BSTPD = pstcConfig->stcBooster.BSTPD;
        }
#endif

#if (PDL_INTERRUPT_ENABLE_LCD == PDL_ON)
        FM_LCDC->LCDCC2_f.LCDIEN = ((pstcConfig->bIrqEn == TRUE) ? 1u : 0u);
        m_pfnIrqCb = pstcConfig->pfnIrqCb;
        // Initialize NVIC
        if(TRUE == pstcConfig->bTouchNvic)
        {
            LcdInitNvic();
        }
        else
        {
            LcdDeInitNvic();
        }
#endif
        enResult = Ok;
    }

    return (enResult);
}

/**
 ******************************************************************************
 ** \brief De-Initialize LCD
 ******************************************************************************/
void Lcd_DeInit( void)
{
    FM_LCDC->LCDCC1 = 0x00u;    
    FM_LCDC->LCDCC2 = 0x14u;
    FM_LCDC->LCDCC2 = 0x3Eu;
    FM_LCDC->LCDC_PSR = 0x00000000u;
    FM_LCDC->LCDC_COMEN  = 0x00000000u;
    FM_LCDC->LCDC_SEGEN1 = 0x00000000u;
    FM_LCDC->LCDC_SEGEN2 = 0x00000000u;
    FM_LCDC->LCDC_SEGEN2 = 0x00000000u;
    FM_LCDC->LCDC_BLINK  = 0x0000u;
    #if (PDL_MCU_CORE == PDL_FM0P_CORE)  
    FM_LCDC->LCDC_BOOSTER  = 0x0E03u;
    #endif
    
#if (PDL_INTERRUPT_ENABLE_LCD == PDL_ON) 
    m_pfnIrqCb = NULL;
    LcdDeInitNvic();
#endif    
    
    return;
}

/**
 ******************************************************************************
 ** \brief Configure LCD input IO mode
 ** \param enInputIoMode
 ** \arg   LcdInputIoCutoff: Input IO of LCD is cut-off
 ** \arg   LcdInputIoConnect:  Input IO of LCD is not cut-off
 **
 ** \retval  Ok                             LCD input IO mode is set normally.
 ** \retval  ErrorInvalidParameter          If one of following condition matches:
 **                                                       - enInputIoMode > LcdInputIoConnect
 **
 ******************************************************************************/
en_result_t Lcd_ConfInputIoMode(en_lcd_input_io_mode_t enInputIoMode)
{    
    switch(enInputIoMode)
    {
        case LcdInputIoCutoff:
            FM_LCDC->LCDCC3_f.PICTL = 0u;
            break;
        case LcdInputIoConnect:
            FM_LCDC->LCDCC3_f.PICTL = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Config LCD display mode 
 ** 
 ** \param enDispMode  
 ** 
 ******************************************************************************/
void Lcd_SetDispMode(en_lcd_disp_mode_t enDispMode)
{
    switch(enDispMode)
    {
        case LcdStop:
            FM_LCDC->LCDCC1_f.MS = 0u;    // stop display
            break;
        case Lcd_4Com_1Div2Duty:
            FM_LCDC->LCDCC1_f.MS = 1u;    // 4 COM 1/2 bias
            break;
        case Lcd_4Com_1Div3Duty:
            FM_LCDC->LCDCC1_f.MS = 2u;    // 4 COM 1/3 bias 
            break;
        case Lcd_4Com_1Div4Duty:
            FM_LCDC->LCDCC1_f.MS = 3u;    // 4COM 1/4 bias 
            break;
        case Lcd_8Com_1Div8Duty:
            FM_LCDC->LCDCC1_f.MS = 4u;    // 8COM 1/8 bias 
            break;
        default:
            break;
    }

    return;
}

/**
 ******************************************************************************
 ** \brief Blank is displayed
 ******************************************************************************/
void Lcd_EnableBlankDisp(void)
{
    
    FM_LCDC->LCDCC2_f.BK = 1u;       // display blank

    return;
}

/**
 ******************************************************************************
 ** \brief Data stored in LCDRAM  is displayed.
 ******************************************************************************/
void Lcd_DisableBlankDisp(void)
{
    
    FM_LCDC->LCDCC2_f.BK = 0u;       // display according with LCDRAM data 

    return;
}

/**
 ******************************************************************************
 ** \brief Set bitsmap in the LCD RAM .
 ** 
 ** \param u8RAMIndex  LCD RAM index
 ** \param u8Bitsmap   Bit map in a RAM byte 
 ** \param bBit Bit value
 ** 
 ** \retval  Ok                             bitsmap in the LCD RAM is set normally.
 ** \retval  ErrorInvalidParameter          If one of following condition matches:
 **                                                       - u8RAMIndex > LCD_RAM_MAX_INDEX
 **
 ******************************************************************************/
en_result_t Lcd_WriteRAMBits(uint8_t u8RAMIndex, uint8_t u8Bitsmap, 
                             boolean_t bBit)
{    
    if(u8RAMIndex > LCD_RAM_MAX_INDEX)
        return ErrorInvalidParameter;
    
    if(0u == bBit)
    {
        *((volatile uint8_t*)(&(FM_LCDC->LCDRAM00)) + u8RAMIndex)  &= ~u8Bitsmap;
    }
    else
    {
        *((volatile uint8_t*)(&(FM_LCDC->LCDRAM00)) + u8RAMIndex)  |= u8Bitsmap;
    }  

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set 1 bit in the LCD RAM .
 ** 
 ** \param u8RAMIndex  LCD RAM index
 ** \param u8BitIndex  Bit index in a RAM byte 
 ** \param bBit Bit value
 ** 
 ** \retval  Ok                             1 bit in the LCD RAM is set normally.
 ** \retval  ErrorInvalidParameter          If one of following condition matches:
 **                                                      - u8RAMIndex > LCD_RAM_MAX_INDEX
 **
 ******************************************************************************/
en_result_t Lcd_WriteRAMBit(uint8_t u8RAMIndex, uint8_t u8BitIndex, 
                            boolean_t bBit)
{
    if(u8RAMIndex > LCD_RAM_MAX_INDEX)
        return ErrorInvalidParameter;

    if(0 == bBit)
    {        
        *((volatile uint8_t*)(&(FM_LCDC->LCDRAM00)) + u8RAMIndex) &= ~(1ul<<u8BitIndex);
    }
    else
    {
        *((volatile uint8_t*)(&(FM_LCDC->LCDRAM00)) + u8RAMIndex) |= (1ul<<u8BitIndex);
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief write byte to  LCD RAM .
 ** 
 ** \param u8RAMIndex  LCD RAM index
 ** \param u8DataByte  data to be written
 ** 
 ** \retval  Ok                             Byte in the LCD RAM is read normally.
 ** \retval  ErrorInvalidParameter          If one of following condition matches:
 **                                                       - u8RAMIndex > LCD_RAM_MAX_INDEX
 **
 ******************************************************************************/
en_result_t Lcd_WriteRAMByte(uint8_t u8RAMIndex, uint8_t u8DataByte)
{
    if(u8RAMIndex > LCD_RAM_MAX_INDEX)
        return ErrorInvalidParameter;
    
    *((volatile uint8_t*)(&(FM_LCDC->LCDRAM00)) + u8RAMIndex)  = u8DataByte;

    return Ok;
}  

/**
 ******************************************************************************
 ** \brief Read  byte from LCD RAM .
 ** 
 ** \param u8RAMIndex  LCD RAM index
 ** \param pu8DataByte pointer of  buffer
 ** 
 ** \retval  Ok                             Byte in the LCD RAM is read normally.
 ** \retval  ErrorInvalidParameter          If one of following condition matches:
 **                                                       - u8RAMIndex > LCD_RAM_MAX_INDEX
 **
 ******************************************************************************/
en_result_t Lcd_ReadRAMByte(uint8_t u8RAMIndex, uint8_t *pu8DataByte)
{
    if(u8RAMIndex > LCD_RAM_MAX_INDEX)
        return ErrorInvalidParameter;
    
    *pu8DataByte =  *((volatile uint8_t*)(&(FM_LCDC->LCDRAM00)) + u8RAMIndex); 

    return Ok;
}  

/**
 ******************************************************************************
 ** \brief LCD is display all.
 ******************************************************************************/
void Lcd_FillWholeRam(void)
{
    uint8_t i;
    
    for (i = 0; i <= LCD_RAM_MAX_INDEX; i++)
    {
        *((volatile uint8_t*)(&(FM_LCDC->LCDRAM00)) + i) = 0xFFu;
    }
    
    return;
}

/**
 ******************************************************************************
 ** \brief Clear LCD all.
 ******************************************************************************/
void Lcd_ClrWholeRam(void)
{
    uint8_t i;
    
    for (i = 0; i <= LCD_RAM_MAX_INDEX; i++)
    {
        *((volatile uint8_t*)(&(FM_LCDC->LCDRAM00)) + i) = 0x00u;
    }
    
    return;
}

/**
 ******************************************************************************
 ** \brief Select blink interval time
 ** \param enBlinkInterval
 ** \arg   LCD_BLINK_HALF_SEC blink interval is 0.5s
 ** \arg   LCD_BLINK_ONE_SEC  blink interval is 1s
 **
 ** \retval  Ok                             Blink interval time is set normally.
 ** \retval  ErrorInvalidParameter          If one of following condition matches:
 **                                                       - enBlinkInterval > LcdBlinkIntOneSecond
 **
 ******************************************************************************/
en_result_t Lcd_SetBinkInterval(en_lcd_blink_interval_t enBlinkInterval)
{
    switch(enBlinkInterval)
    {
        case LcdBlinkIntHalfSecond:
            FM_LCDC->LCDCC3_f.BLSEL = 0u;
            break;
        case LcdBlinkIntOneSecond:
            FM_LCDC->LCDCC3_f.BLSEL = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Select blink dot
 **
 ** \param enBlinkDot blink dot
 ** \param enBlinkOp  blink operation
 **
 ** \retval  Ok                            Blink dot is set normally.
 ** \retval  ErrorInvalidParameter         If one of following condition matches:
 **                                                   - enBlinkDot > LCDC_Blik8COMS0C0
 **                                                   - LcdBlinkOff != enBlinkOp && LcdBlinkOn != enBlinkOp
 **
 ******************************************************************************/
en_result_t Lcd_SetBlinkDot(en_lcd_blink_dot_t enBlinkDot, 
                                en_lcd_blink_operation_t enBlinkOp)
{
    if((LcdBlinkOff != enBlinkOp)&&(LcdBlinkOn != enBlinkOp))
        return ErrorInvalidParameter; 
  
    if(LCDC_BlinkDotOff == enBlinkDot)
    {
        FM_LCDC->LCDC_BLINK = 0x0000u;               //close all blink function
        return Ok;
    }    

    if((LCDC_Blik4COMS3C3 == enBlinkDot)||(LCDC_Blik8COMS1C7 == enBlinkDot))          //blink 4COM SEG3-SOM3 or 8COM SEG1-COM7
        FM_LCDC->LCDC_BLINK_f.BLD15 = enBlinkOp;
    else if((LCDC_Blik4COMS3C2 == enBlinkDot)||(LCDC_Blik8COMS1C6 == enBlinkDot))     //blink 4COM SEG3-SOM2 or 8COM SEG1-COM6
        FM_LCDC->LCDC_BLINK_f.BLD14 = enBlinkOp;
    else if((LCDC_Blik4COMS3C1 == enBlinkDot)||(LCDC_Blik8COMS1C5 == enBlinkDot))     //blink 4COM SEG3-SOM1 or 8COM SEG1-COM5
        FM_LCDC->LCDC_BLINK_f.BLD13 = enBlinkOp;
    else if((LCDC_Blik4COMS3C0 == enBlinkDot)||(LCDC_Blik8COMS1C4 == enBlinkDot))     //blink 4COM SEG3-SOM0 or 8COM SEG1-COM4
        FM_LCDC->LCDC_BLINK_f.BLD12 = enBlinkOp;
    else if((LCDC_Blik4COMS2C3 == enBlinkDot)||(LCDC_Blik8COMS1C3 == enBlinkDot))     //blink 4COM SEG2-SOM3 or 8COM SEG1-COM3
        FM_LCDC->LCDC_BLINK_f.BLD11 = enBlinkOp;
    else if((LCDC_Blik4COMS2C2 == enBlinkDot)||(LCDC_Blik8COMS1C2 == enBlinkDot))     //blink 4COM SEG2-SOM2 or 8COM SEG1-COM2
        FM_LCDC->LCDC_BLINK_f.BLD10 = enBlinkOp;
    else if((LCDC_Blik4COMS2C1 == enBlinkDot)||(LCDC_Blik8COMS1C1 == enBlinkDot))     //blink 4COM SEG2-SOM1 or 8COM SEG1-COM1
        FM_LCDC->LCDC_BLINK_f.BLD09 = enBlinkOp;
    else if((LCDC_Blik4COMS2C0 == enBlinkDot)||(LCDC_Blik8COMS1C0 == enBlinkDot))     //blink 4COM SEG2-SOM0 or 8COM SEG1-COM0
        FM_LCDC->LCDC_BLINK_f.BLD08 = enBlinkOp;
    else if((LCDC_Blik4COMS1C3 == enBlinkDot)||(LCDC_Blik8COMS0C7 == enBlinkDot))     //blink 4COM SEG1-SOM3 or 8COM SEG0-COM7
        FM_LCDC->LCDC_BLINK_f.BLD07 = enBlinkOp;
    else if((LCDC_Blik4COMS1C2 == enBlinkDot)||(LCDC_Blik8COMS0C6 == enBlinkDot))     //blink 4COM SEG1-SOM2 or 8COM SEG0-COM6
        FM_LCDC->LCDC_BLINK_f.BLD06 = enBlinkOp;
    else if((LCDC_Blik4COMS1C1 == enBlinkDot)||(LCDC_Blik8COMS0C5 == enBlinkDot))     //blink 4COM SEG1-SOM1 or 8COM SEG0-COM5
        FM_LCDC->LCDC_BLINK_f.BLD05 = enBlinkOp;
    else if((LCDC_Blik4COMS1C0 == enBlinkDot)||(LCDC_Blik8COMS0C4 == enBlinkDot))     //blink 4COM SEG1-SOM0 or 8COM SEG0-COM4
        FM_LCDC->LCDC_BLINK_f.BLD04 = enBlinkOp;
    else if((LCDC_Blik4COMS0C3 == enBlinkDot)||(LCDC_Blik8COMS0C3 == enBlinkDot))     //blink 4COM SEG0-SOM3 or 8COM SEG0-COM3
        FM_LCDC->LCDC_BLINK_f.BLD03 = enBlinkOp;
    else if((LCDC_Blik4COMS0C2 == enBlinkDot)||(LCDC_Blik8COMS0C2 == enBlinkDot))     //blink 4COM SEG0-SOM2 or 8COM SEG0-COM2
        FM_LCDC->LCDC_BLINK_f.BLD02 = enBlinkOp;
    else if((LCDC_Blik4COMS0C1 == enBlinkDot)||(LCDC_Blik8COMS0C1 == enBlinkDot))     //blink 4COM SEG0-SOM1 or 8COM SEG0-COM1
        FM_LCDC->LCDC_BLINK_f.BLD01 = enBlinkOp;
    else if((LCDC_Blik4COMS0C0 == enBlinkDot)||(LCDC_Blik8COMS0C0 == enBlinkDot))     //blink 4COM SEG0-SOM0 or 8COM SEG0-COM0
        FM_LCDC->LCDC_BLINK_f.BLD00 = enBlinkOp;
    else
        return ErrorInvalidParameter;

    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_LCD == PDL_ON)

/**
 ******************************************************************************
 ** \brief Enable lcd interrupt
 ******************************************************************************/
void Lcd_EnableIrq(void)
{
    FM_LCDC->LCDCC2_f.LCDIEN = 1u;

    return;
}

/**
 ******************************************************************************
 ** \brief Disable lcd interrupt
 ******************************************************************************/
void Lcd_DisableIrq(void)
{
    FM_LCDC->LCDCC2_f.LCDIEN = 0u;

    return;
}
#endif

/**
 ******************************************************************************
 ** \brief Clear lcd interrupt flag
 ******************************************************************************/
void Lcd_ClrIrqFlag(void)
{
    FM_LCDC->LCDCC2_f.LCDIF = 0u;
}

/**
 ******************************************************************************
 ** \brief Get lcd interrupt flag
 **
 ** \retval TRUE LCD interrupt flag is set
 ** \retval FALSE LCD interrupt flag is clear
 **
 ******************************************************************************/
boolean_t Lcd_GetIrqFlag(void)
{
    return FM_LCDC->LCDCC2_f.LCDIF;
}

#endif /* #if (defined(PDL_PERIPHERAL_LCD_ACTIVE)) */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
