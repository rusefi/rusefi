/******************************************************************************
* \file         fgpio.h
*
* \version      1.20
*
* \brief        Headerfile for Fast GPIO driver
*
******************************************************************************
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

#ifndef __FGPIO_H__
#define __FGPIO_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/**
* \addtogroup GroupGPIO
* \{
* \defgroup GroupFGPIO Fast General-purpose I/O ports (FGPIO) 
* \{
* \defgroup GroupFGPIO_Macros Macros
* \defgroup GroupFGPIO_DataStructures Data Structures
* \defgroup GroupFGPIO_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupFGPIO
* \{
* FM0+ supports Fast General-purpose I/O ports. By default, all I/O ports operate 
* as normal GPIO. You can assign any pin to be a Fast GPIO pin. Fast GPIO can read an 
* input level and set an output level from the CPU in one cycle of HCLK.<br>
*
* \section SectionFGPIO_ConfigurationConsideration Configuration Consideration
* To set a pin to be Fast GPIO, use the FGpio_EnableOutput() macro. You provide 
* the port, and a 16-bit value as parameters. The 16-bit value specifies which 
* pins within the port to enable for Fast GPIO. A bit value of 1 in any position enables 
* that pin. For example, this code sets Port 3 pin E to be a Fast GPIO pin.<br>
* // Pin F E D C B A 9 8 7 6 5 4 3 2 1 0 <br>
* // Val 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0  or 0x4000u <br>
*    FGpio_EnableOutput(FGpioPort3, 0x4000u); <br>
* Once enabled as Fast GPIO, you use other macros to set or get pin direction, level, and pullup state.<br>
* Note: If the bus clock of a GPIO is gated, some functions of the I/O port cannot be used.<br>
* \attention
*            - Before using the Fast GPIO output, you must call the FGpio_EnableOutput() macro. 
* \section SectionFGPIO_MoreInfo More Information
* For more information on the GPIO peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual Core Subsystem</a><br>
* 
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">
* all FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets can be downloaded</a><br>
*
*/

/**
* \addtogroup GroupFGPIO_Macros
* \{
*/  

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#define FGpio_EnableOutput(port, pins)   do {uint32_t addr; \
                                            addr = (uint32_t)&FM_GPIO->FPOER0 + (uint32_t)port*4u; \
                                            *(uint16_t*)(addr) = pins;  \
                                            }while(0);
#define FGpio_DisableOutput(port)        do {uint32_t addr; \
                                            addr = (uint32_t)&FM_GPIO->FPOER0 + (uint32_t)port*4u; \
                                            *(uint16_t*)(addr) = 0x0000u;  \
                                            }while(0);

#define FGpio1pin_InitIn(p,settings)    do{ stc_fgpio1pin_init_t __v__;\
                                         __v__.bPullup=0u;__v__.bInitVal=0u;\
                                         (settings);\
                                         p##_INITIN(__v__); }while(0)

#define FGpio1pin_InitOut(p,settings)   do{ stc_fgpio1pin_init_t __v__;\
                                         __v__.bPullup=0u;__v__.bInitVal=0u;\
                                         (settings);\
                                         p##_INITOUT(__v__); }while(0)

#define FGpio1pin_Init(p,settings)      do{ stc_fgpio1pin_init_t __v__;__v__.bOutput=0u;\
                                         __v__.bPullup=0u;__v__.bInitVal=0u;\
                                         (settings);\
                                         p##_INIT( __v__ ); }while(0)

#define FGpio1pin_InitDirectionInput    (__v__.bOutput=0u)
#define FGpio1pin_InitDirectionOutput   (__v__.bOutput=1u)
#define FGpio1pin_InitPullup(v)         (__v__.bPullup=(v))
#define FGpio1pin_InitVal(v)            (__v__.bInitVal=(v))



#define FGpio1pin_Get(p)	         p##_GET
#define FGpio1pin_Put(p,v)           p##_PUT(v)


/******************************************************************************/
/* Inclusion of GPIO defines of user defined device                           */
/******************************************************************************/
/** \} GroupFGPIO_Macros */

/**
* \addtogroup GroupFGPIO_Types
* \{
*/


/**
 ******************************************************************************
 ** \brief GPIO port list
 ******************************************************************************/
typedef enum en_fgpio_port
{
    FGpioPort0 = 0u,   ///< Fast GPIO port 0
    FGpioPort1 = 1u,   ///< Fast GPIO port 1
    FGpioPort2 = 2u,   ///< Fast GPIO port 2
    FGpioPort3 = 3u,   ///< Fast GPIO port 3
    FGpioPort4 = 4u,   ///< Fast GPIO port 4
    FGpioPort5 = 5u,   ///< Fast GPIO port 5
    FGpioPort6 = 6u,   ///< Fast GPIO port 6
    FGpioPort7 = 7u,   ///< Fast GPIO port 7
    FGpioPort8 = 8u,   ///< Fast GPIO port 8
    FGpioPort9 = 9u,   ///< Fast GPIO port 9
    FGpioPortA = 10u,  ///< Fast GPIO port 10
    FGpioPortB = 11u,  ///< Fast GPIO port 11
    FGpioPortC = 12u,  ///< Fast GPIO port 12
    FGpioPortD = 13u,  ///< Fast GPIO port 13
    FGpioPortE = 14u,  ///< Fast GPIO port 14
    FGpioPortF = 15u,  ///< Fast GPIO port 15

}en_fgpio_port_t;

/** \}GroupFGPIO_Types */

/**
* \addtogroup GroupFGPIO_DataStructures
* \{
*/
/**
 ******************************************************************************
 **
 ** Definitions of Fast GPIO and resource pin relocation
 **
 ** \attention
 **            - before using the Fast GPIO output, FGpio_EnableOutput() has
 **              to be called.
 ******************************************************************************/

/******************************************************************************/
/* \brief Fast GPIO intialization structure                                   */
/******************************************************************************/

typedef struct stc_fgpio1pin_init
{
    boolean_t bOutput;
    boolean_t bInitVal;
    boolean_t bPullup;
} stc_fgpio1pin_init_t;

/** \}GroupFGPIO_DataStructures */
/** \} GroupFGPIO */
/** \} GroupGPIO */

#ifdef __cplusplus
}
#endif

#endif /* __FPIO_H__ */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
