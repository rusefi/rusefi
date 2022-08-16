/*******************************************************************************
* \file              lcd.h
* 
* \version           1.20
*
* \brief             Headerfile for LCD functions.
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

#ifndef __LCD_H__
#define __LCD_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_LCD_ACTIVE))

#if defined (__CC_ARM)
	#pragma anon_unions    
#endif

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupLCD Segment LCD Controller (LCD)
* \{
* \defgroup GroupLCD_Macros Macros
* \defgroup GroupLCD_Functions Functions
* \defgroup GroupLCD_DataStructures Data Structures
* \defgroup GroupLCD_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupLCD
* \{
* The Segment LCD Controller (LCD) displays the contents of Display Data Memory (LCDRAM) 
* directly to the Liquid Crystal Display (LCD) panel by using segment outputs and common outputs.
* You can operate with either four or eight common outputs.<br>
* * 4-COM: 44 segments (SEG00-SEG43); outputs COM0-COM3; bias 1/2, 1/3, or 1/4; LCDRAM 22 bytes (44x4 bits)
* * 8-COM: 40 segments (SEG00-SEG39); outputs COM0-COM7; bias 1/3 or 1/4; LCDRAM 40 bytes (40x8 bits)
* You can choose either 10kOhm or 100kOhm divider resistors for generating LCD drive power. LCD drive 
* power can be supplied from an external circuit. <br>
* You choose either a sub-clock or PCLK to drive the LCD controller operating clock (LCDC clock).
* The LCD supports blinking (flashing), direct drive to the LCD panel, a voltage booster, and can 
* generate an interrupt request per frame.
* 
* \note The LCD controller for FM0+ type 1 and type 2 differs. Refer to peripheral 
* manuals for details. FM4 does not provide the LCD peripheral.
*
* \section SectionLCD_ConfigurationConsideration Configuration Consideration
* To set up an LCD, you provide configuration parameters in the stc_lcd_config_t structure. Among 
* these is a clock configuration structure, stc_lcd_clk_config_t, where you specify the source clock 
* and clock divider. You also specify which segments to select in the fields stcSegSel1 (SEG00-SEG31) 
* and stcSegSel2 (SEG32-SEG39). A one in each bit turns on the corresponding segment. Similarly, the 
* stcComSel (COM0-COM7) is a structure that represents an 8-bit value to select the common outputs. 
* You also enable the LCD interrupt and provide a callback function.<br>
* Then call LCD_Init() to initialize the LCD instance according to the configuration structure.<br>
* Clearing the LCDRAM at this point is good practice. Call Lcd_ClrWholeRam().<br>
* At runtime you can use API function calls to:<br>
* * Change the input mode
* * Set the display mode
* * Enable or disable a blank display (when disabled, you display LCDRAM contents)
* * Write the contents of LCDRAM using a single bit, bits, or a byte
* * Enable and disable the LCD interrupt
* * Get or clear the interrupt flag<br>
* See the LCD function documentation for information on each API call.
* 
* \section SectionLCD_MoreInfo More Information
* For more information on the LCD peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223036/download">FM0+ Peripheral Manual - Analog Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupLCD_Types
* \{
*/

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/
 
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
 
/**
 ******************************************************************************
 ** \brief Select divider resitors to create LCD drive power 
 ******************************************************************************/
typedef enum en_lcd_div_mode
{
    LcdcExtRes = 0u,   ///<  External divider resistors are used to create LCD drive power.
    LcdcIntRes = 1u,   ///<  Internal divider resistors are used to create LCD drive power.
}en_lcd_div_mode_t;

/**
 ******************************************************************************
 ** \brief Divider resistor value selection
 ******************************************************************************/
typedef enum en_lcd_disp_mode
{
    LcdStop = 0u,              ///<  LCD controller stops display operations.
    Lcd_4Com_1Div2Duty = 1u,   ///< 4 COM mode, 1/2 duty
    Lcd_4Com_1Div3Duty = 2u,   ///< 4 COM mode, 1/3 duty
    Lcd_4Com_1Div4Duty = 3u,   ///< 4 COM mode, 1/4 duty
    Lcd_8Com_1Div8Duty = 4u,   ///< 8 COM mode, 1/8 duty
}en_lcd_disp_mode_t;

/**
 ******************************************************************************
 ** \brief Divider resistor value selection
 ******************************************************************************/
typedef enum en_lcd_div_res_val
{
    LcdDivRes100K = 0u,   ///< 100 k�� resistors are selected.
    LcdDivRes10K  = 1u,   ///<  100 k�� resistors are selected.
}en_lcd_div_res_val_t;

/**
 ******************************************************************************
 ** \brief 8 COM mode bias selection
 ******************************************************************************/
typedef enum en_lcd_8com_bias
{
    Lcd1Div3Bias = 0u,   ///< 1/3 bias is selected in 8 COM mode.
    Lcd1Div4Bias = 1u,   ///< 1/4 bias is selected in 8 COM mode.
}en_lcd_8com_bias_t;

/**
 ******************************************************************************
 ** \brief Source clock selection
 ******************************************************************************/
typedef enum en_lcd_src_clk_sel
{
    LcdClkSelSubClk = 0u,   ///< Sub-clock is selected for LCDC source clock.
    LcdClkSelPclk   = 1u,   ///< PCLK is selected for LCDC source clock.
}en_lcd_src_clk_sel_t;
/**
 ******************************************************************************
 ** \brief controls I/O ports shared by COM and SEG.
 ******************************************************************************/
typedef enum en_lcd_input_io_mode
{
    LcdInputIoCutoff  = 0u,   ///< Input from I/O port is cut off.
    LcdInputIoConnect = 1u,   ///< Input from I/O port is not cut off.

}en_lcd_input_io_mode_t;

/**
 ******************************************************************************
 ** \brief Blink interval
 ******************************************************************************/
typedef enum en_lcd_blink_interval
{
    LcdBlinkIntHalfSecond = 0u,   ///< If sub-clock is 32.768 [kHz], the interval becomes 0.5 [s].
    LcdBlinkIntOneSecond  = 1u,   ///< If sub-clock is 32.768 [kHz], the interval becomes 1.0 [s].

}en_lcd_blink_interval_t;

/**
 ******************************************************************************
 ** \brief Blink interval
 ******************************************************************************/
typedef enum en_lcd_ram_bit_operation
{
    LcdBitClr = 0u,   ///< Clear lcd ram bit.
    LcdBitSet = 1u,   ///< Set lcd ram bit.

}en_lcd_ram_bit_operation_t;

/**
 ******************************************************************************
 ** \brief LCD blink function enumeration
 ******************************************************************************/
typedef enum en_lcd_blink_operation
{   
    LcdBlinkOff = 0u,       ///< LCD blink on
    LcdBlinkOn  = 1u,       ///< LCD blink off
}en_lcd_blink_operation_t;

/**
 ******************************************************************************
 ** \brief LCD blink SEG COM dot enumeration
 ******************************************************************************/
typedef enum en_lcd_blink_dot_t
{   
    LCDC_BlinkDotOff = 0u,        ///< LCD blink on
/***4COM*****************/
    LCDC_Blik4COMS3C3,            ///< lcdc blink for SEG3-COM3 
    LCDC_Blik4COMS3C2,            ///< lcdc blink for SEG3-COM2 
    LCDC_Blik4COMS3C1,            ///< lcdc blink for SEG3-COM1 
    LCDC_Blik4COMS3C0,            ///< lcdc blink for SEG3-COM0 
    LCDC_Blik4COMS2C3,            ///< lcdc blink for SEG2-COM3 
    LCDC_Blik4COMS2C2,            ///< lcdc blink for SEG2-COM2 
    LCDC_Blik4COMS2C1,            ///< lcdc blink for SEG2-COM1 
    LCDC_Blik4COMS2C0,            ///< lcdc blink for SEG2-COM0 
    LCDC_Blik4COMS1C3,            ///< lcdc blink for SEG1-COM3 
    LCDC_Blik4COMS1C2,            ///< lcdc blink for SEG1-COM2 
    LCDC_Blik4COMS1C1,            ///< lcdc blink for SEG1-COM1
    LCDC_Blik4COMS1C0,            ///< lcdc blink for SEG1-COM0
    LCDC_Blik4COMS0C3,            ///< lcdc blink for SEG0-COM3
    LCDC_Blik4COMS0C2,            ///< lcdc blink for SEG0-COM2
    LCDC_Blik4COMS0C1,            ///< lcdc blink for SEG0-COM1
    LCDC_Blik4COMS0C0,            ///< lcdc blink for SEG0-COM0
/***8COM*****************/    
    LCDC_Blik8COMS1C7,            ///< lcdc blink for SEG1-COM7 
    LCDC_Blik8COMS1C6,            ///< lcdc blink for SEG1-COM6 
    LCDC_Blik8COMS1C5,            ///< lcdc blink for SEG1-COM5 
    LCDC_Blik8COMS1C4,            ///< lcdc blink for SEG1-COM4 
    LCDC_Blik8COMS1C3,            ///< lcdc blink for SEG1-COM3 
    LCDC_Blik8COMS1C2,            ///< lcdc blink for SEG1-COM2 
    LCDC_Blik8COMS1C1,            ///< lcdc blink for SEG1-COM1 
    LCDC_Blik8COMS1C0,            ///< lcdc blink for SEG1-COM0 
    LCDC_Blik8COMS0C7,            ///< lcdc blink for SEG0-COM7 
    LCDC_Blik8COMS0C6,            ///< lcdc blink for SEG0-COM6 
    LCDC_Blik8COMS0C5,            ///< lcdc blink for SEG0-COM5
    LCDC_Blik8COMS0C4,            ///< lcdc blink for SEG0-COM4
    LCDC_Blik8COMS0C3,            ///< lcdc blink for SEG0-COM3
    LCDC_Blik8COMS0C2,            ///< lcdc blink for SEG0-COM2
    LCDC_Blik8COMS0C1,            ///< lcdc blink for SEG0-COM1
    LCDC_Blik8COMS0C0,            ///< lcdc blink for SEG0-COM0
}en_lcd_blink_dot_t;

/** \} GroupLCD_Types */

/**
* \addtogroup GroupLCD_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Select lcd source clock and configure clock division ratio 
 ******************************************************************************/
typedef struct stc_lcd_clk_config
{
    en_lcd_src_clk_sel_t enSrcClk;  ///< Set LCDC source clock.
    uint32_t u32DivVal;             ///<  These bits set LCDC clock division ratio (1 to 2097153).
}stc_lcd_clk_config_t;

/**
 ******************************************************************************
 ** \brief VVx selection bit
 **
 ** \note As VV4 pin cannot be used as GPIO when LCD controller is 
 **        selected (LCDCC1:VSEL="1"), be sure to write "1" to VE4 bit.
 ******************************************************************************/
typedef struct stc_lcd_vv_sel
{
    union /* unVvSel */
    {
        uint8_t u8Vv;
        struct /* stcVvBits */
        {            
            uint8_t VE0  : 1 ;
            uint8_t VE1  : 1 ;
            uint8_t VE2  : 1 ;
            uint8_t VE3  : 1 ;
            uint8_t VE4  : 1 ;
            uint8_t RESERVED0  : 3;
        };
    };
} stc_lcd_vv_sel_t ;

/**
 ******************************************************************************
 ** \brief These bits control I/O port status and analog switches for SEG outputs.
 ******************************************************************************/
typedef struct stc_lcd_seg_sel1
{
    union /* unSegSel1 */
    {
        uint32_t u32SegSel1;
        struct /* stcSegSel1Bits */
        {
            uint32_t SEG00  : 1 ;
            uint32_t SEG01  : 1 ;
            uint32_t SEG02  : 1 ;
            uint32_t SEG03  : 1 ;
            uint32_t SEG04  : 1 ;
            uint32_t SEG05  : 1 ;
            uint32_t SEG06  : 1 ;
            uint32_t SEG07  : 1 ;
            uint32_t SEG08  : 1 ;
            uint32_t SEG09  : 1 ;
            uint32_t SEG10  : 1 ;
            uint32_t SEG11  : 1 ;
            uint32_t SEG12  : 1 ;
            uint32_t SEG13  : 1 ;
            uint32_t SEG14  : 1 ;
            uint32_t SEG15  : 1 ;
            uint32_t SEG16  : 1 ;
            uint32_t SEG17  : 1 ;
            uint32_t SEG18  : 1 ;
            uint32_t SEG19  : 1 ;
            uint32_t SEG20  : 1 ;
            uint32_t SEG21  : 1 ;
            uint32_t SEG22  : 1 ;
            uint32_t SEG23  : 1 ;
            uint32_t SEG24  : 1 ;
            uint32_t SEG25  : 1 ;
            uint32_t SEG26  : 1 ;
            uint32_t SEG27  : 1 ;
            uint32_t SEG28  : 1 ;
            uint32_t SEG29  : 1 ;
            uint32_t SEG30  : 1 ;
            uint32_t SEG31  : 1 ;
        };
    };
} stc_lcd_seg_sel1_t ;

/**
 ******************************************************************************
 ** \brief These bits control I/O port status and analog switches for SEG outputs.
 ******************************************************************************/
typedef struct stc_lcd_seg_sel2
{
    union /* unSegSel2 */
    {
        uint32_t u32SegSel2;
        struct /* stcSegSel2Bits */
        {
            uint32_t SEG32  : 1 ;
            uint32_t SEG33  : 1 ;
            uint32_t SEG34  : 1 ;
            uint32_t SEG35  : 1 ;
            uint32_t SEG36  : 1 ;
            uint32_t SEG37  : 1 ;
            uint32_t SEG38  : 1 ;
            uint32_t SEG39  : 1 ;
            uint32_t RESERVED0  : 24;
        };
    };
} stc_lcd_seg_sel2_t ;

/**
 ******************************************************************************
 ** \brief These bits control I/O port status and analog switches for COM outputs.
 ******************************************************************************/
typedef struct stc_lcd_com_sel
{
    union /* unComSel */
    {
        uint8_t u8ComSel;
        struct /* stcComSelBits */
        {
            uint32_t COM0  : 1 ;
            uint32_t COM1  : 1 ;
            uint32_t COM2  : 1 ;
            uint32_t COM3  : 1 ;
            uint32_t COM4  : 1 ;
            uint32_t COM5  : 1 ;
            uint32_t COM6  : 1 ;
            uint32_t COM7  : 1 ;
        };
    };
} stc_lcd_com_sel_t;

/**
 ******************************************************************************
 ** \brief LCD booster function
 ******************************************************************************/
typedef struct stc_lcd_booster
{
    union /* unBooster */
    {
        uint16_t u16Booster;
        struct /* stcBoosterBits */
        {
            uint16_t BSTOPT     : 1;///< booster function could be available
            uint16_t BSTPD      : 1;///< Control booster power
            uint16_t RESERVED0  : 1;
            uint16_t CENSEL     : 1;///< Control booster C1/C0 pin funciton 
            uint16_t RESERVED1  : 4;
            uint16_t BTRF       : 4;///< Booster fine setting bits for reference voltage of VV1
            uint16_t BTRC       : 2;///< Booster coarse setting bits for reference voltage of VV1
        };
    };
}stc_lcd_booster_t;

/**
 ******************************************************************************
 ** \brief Clock Supervisor configuration
 ** 
 ** The Clock Supervisor configuration settings
 ******************************************************************************/
typedef struct stc_lcd_config
{
    boolean_t bTimerMod;              ///< FALSE: sops running in timer mode. TRUE:run in timer mode.
    en_lcd_div_mode_t enDivMode;      ///< LCD drive power control, see #en_lcd_div_mode_t for details
    en_lcd_disp_mode_t enDispMode;    ///< LCD controller display mode selection, see #en_lcd_disp_mode_t for details
    en_lcd_div_res_val_t enDivResVal; ///< Divider resistor value selection, see #en_lcd_div_res_val_t for details
    en_lcd_8com_bias_t en8ComBias;    ///< 8 COM mode bias selection, see #en_lcd_8com_bias_t for details
    boolean_t bEnDispRevs;            ///< Reverse display control. FALSE:Display is not reversed.  TRUE:Display is reversed.
    boolean_t bEnBlankDisp;           ///< TRUE:Blank is displayed independent of data stored in LCDRAM
    en_lcd_input_io_mode_t enInputIoMode;   ///< I/O port input control,  see #en_lcd_input_io_mode_t for details
    stc_lcd_clk_config_t stcClkConf;  ///< LCD clock control,  see #stc_lcd_clk_config_t for details
    stc_lcd_vv_sel_t stcVvSel;        ///< VVx control,  see #stc_lcd_vv_sel_t for details
    stc_lcd_seg_sel1_t stcSegSel1;    ///< Segment output pins (SEG00 to SEG31) control,  see #stc_lcd_seg_sel1_t for details
    stc_lcd_seg_sel2_t stcSegSel2;    ///< Segment output pins (SEG32 to SEG39) control,  see #stc_lcd_seg_sel2_t for details
    stc_lcd_com_sel_t stcComSel;      ///< COM output pins (COM0 to COM7) control,  see #stc_lcd_com_sel_t for details
#if (PDL_MCU_CORE == PDL_FM0P_CORE)
    boolean_t   bEnBooster;           ///< Enable booster function
    stc_lcd_booster_t stcBooster;     ///< Configure LCD booster functionl,  see #stc_lcd_booster_t for details
#endif

#if (PDL_INTERRUPT_ENABLE_LCD == PDL_ON)
    boolean_t  bIrqEn;                ///< Enable LCD interrupt
    func_ptr_t pfnIrqCb;              ///< Pointer to LCD interrupt callback function
    boolean_t bTouchNvic;             ///< TRUE: enable NVIC, FALSE: don't enable NVIC
#endif    
} stc_lcd_config_t;

/** \} GroupLCD_DataStructures */

/**
* \addtogroup GroupLCD_Functions
* \{
*/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_LCD == PDL_ON)
    void Lcd_IrqHandler(void);
#endif
    en_result_t Lcd_Init(const stc_lcd_config_t* pstcConfig);
    void Lcd_DeInit( void);
    en_result_t Lcd_ConfInputIoMode(en_lcd_input_io_mode_t enInputIoMode);
    void Lcd_SetDispMode(en_lcd_disp_mode_t enDispMode);
    void Lcd_EnableBlankDisp(void);
    void Lcd_DisableBlankDisp(void);
    en_result_t Lcd_WriteRAMBits(uint8_t u8RAMIndex, uint8_t u8Bitsmap,
                                 boolean_t bBit);
    en_result_t Lcd_WriteRAMBit(uint8_t u8RAMIndex, uint8_t u8BitIndex, 
                                boolean_t bBit);
    en_result_t Lcd_WriteRAMByte(uint8_t u8RAMIndex, uint8_t u8DataByte);  
    en_result_t Lcd_ReadRAMByte(uint8_t u8RAMIndex,uint8_t *pu8DataByte);  
    void Lcd_FillWholeRam(void);
    void Lcd_ClrWholeRam(void);
    en_result_t Lcd_SetBinkInterval(en_lcd_blink_interval_t enBlinkInterval);
    en_result_t Lcd_SetBlinkDot(en_lcd_blink_dot_t enBlinkDot, 
                                en_lcd_blink_operation_t enBlinkOp);

    /* Interrupt Setting */
#if (PDL_INTERRUPT_ENABLE_LCD == PDL_ON)
    void Lcd_EnableIrq(void);
    void Lcd_DisableIrq(void);
#endif
    void Lcd_ClrIrqFlag(void);
    boolean_t Lcd_GetIrqFlag(void);

/** \} GroupLCD_Functions */
/** \} GroupLCD */

#ifdef __cplusplus
}
#endif


#endif /* #if (defined(PDL_PERIPHERAL_LCD_ACTIVE)) */

#endif /* __LCD_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
