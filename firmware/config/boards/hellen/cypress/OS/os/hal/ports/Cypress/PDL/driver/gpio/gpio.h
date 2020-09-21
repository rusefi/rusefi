/******************************************************************************
* \file         gpio.h
*
* \version      1.20
*
* \brief        Headerfile for Definitions of GPIO and resource pin relocation
*
******************************************************************************* 
\copyright
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


#ifndef __GPIO_H__
#define __GPIO_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "base_types.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupGPIO General-Purpose I/O ports (GPIO)
* \{ 
* \defgroup GroupGPIO_Macros Macros
* \defgroup GroupGPIO_DataStructures Data Structures
* \}
*/

/**
* \addtogroup GroupGPIO
* \{
* The PDL provides macros you can use to manage pins for General-purpose I/O ports. 
* If you use the macros you need to know which pin to use, but do not need to know the pin registers. 
* Macros enable you to:<br>
* - Set a pin as an output or input pin<br>
* - Set a pin to an initial level (high or low)<br>
* - Set a pin’s pullup<br>
* - Get or set a pin’s level<br>
* For FM0+ see also Fast General-purpose IO Ports.<br>
*
* \section SectionGPIO_ConfigurationConsideration Configuration Consideration
* There is no configuration structure for GPIO. Use the appropriate macro to accomplish your task. 
* The macros have one or two parameters.<br>
* One parameter is the name of the pin. The file gpio.h defines pin names, in the form: <br>
* GPIO1PIN_P<port><pin>. So, for Port 1, Pin A, the name is GPIO1PIN_P1A.<br>
* The second parameter (when there are two) is simply the value you want to use.<br>
* For example, you might write code like this:<br>
* Gpio1pin_InitOut(GPIO1PIN_P1A, Gpio1pin_InitVal( 1u ) );<br>
* This code initializes Port 1 Pin A to be an output pin, with initial level high.<br>
* Note: if the bus clock of a GPIO is gated, some functions of the I/O port cannot be used.<br>
* \section SectionGPIO_MoreInfo More Information
* For more information on the GPIO peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual Core Subsystem</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual Core Subsystem</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupGPIO_Macros
* \{
*/  

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#define Gpio1pin_InitIn(p,settings)    do{ stc_gpio1pin_init_t __v__;\
                                         __v__.bPullup=0u;__v__.bInitVal=0u;\
                                         (settings);\
                                         p##_INITIN(__v__); }while(0)

#define Gpio1pin_InitOut(p,settings)   do{ stc_gpio1pin_init_t __v__;\
                                         __v__.bPullup=0u;__v__.bInitVal=0u;\
                                         (settings);\
                                         p##_INITOUT(__v__); }while(0)

#define Gpio1pin_Init(p,settings)      do{ stc_gpio1pin_init_t __v__;__v__.bOutput=0u;\
                                         __v__.bPullup=0u;__v__.bInitVal=0u;\
                                         (settings);\
                                         p##_INIT( __v__ ); }while(0)

#define Gpio1pin_InitDirectionInput    (__v__.bOutput=0u)
#define Gpio1pin_InitDirectionOutput   (__v__.bOutput=1u)
#define Gpio1pin_InitPullup(v)         (__v__.bPullup=(v))
#define Gpio1pin_InitVal(v)            (__v__.bInitVal=(v))



#define Gpio1pin_Get(p)	         p##_GET
#define Gpio1pin_Put(p,v)        p##_PUT(v)

#define PINRELOC_SET_EPFR(epfr,pos,width,value)    \
          ((epfr) = ((epfr) & ~(((1u<<(width))-1u)<<(pos))) | \
          ((value) << (pos)))

/** \} GroupGPIO_Macros */
/**
* \addtogroup GroupGPIO_DataStructures
* \{
*/
/******************************************************************************/
/* Types                                                                      */
/******************************************************************************/

typedef struct stc_gpio1pin_init
{
    boolean_t bOutput;
    boolean_t bInitVal;
    boolean_t bPullup;
} stc_gpio1pin_init_t;

/** \}GroupGPIO_DataStructures */
/** \} GroupGPIO    */

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H__ */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
