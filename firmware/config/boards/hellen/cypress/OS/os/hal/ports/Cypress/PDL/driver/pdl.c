/*******************************************************************************
* \file             pdl.c
* 
* \version          1.0
*                       
* \brief            Misc helper functions for Peripheral Driver Library.
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

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "pdl.h"

/**
 ******************************************************************************
 ** addtogroup PDL Common Functions
 ******************************************************************************/
//@{

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 *****************************************************************************
 ** \brief Memory clear function for PDL_ZERO_STRUCT()
 *****************************************************************************/
void pdl_memclr(uint8_t* pu8Address, uint32_t u32Count)
{
    while(u32Count--)
    {
        *pu8Address++ = 0;
    }
}

/**
 *****************************************************************************
 ** \brief Hook function, which is called in polling loops
 *****************************************************************************/
void PDL_WAIT_LOOP_HOOK(void)
{
    // Place code for triggering Watchdog counters here, if needed
}

//@} // PDL Functions

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
