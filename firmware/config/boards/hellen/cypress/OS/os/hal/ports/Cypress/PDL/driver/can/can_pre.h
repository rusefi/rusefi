/******************************************************************************
* \file             can_pre.h
*
* \version          1.20
*
* \brief            Header file for CAN prescaler functions  
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

#ifndef __CAN_PRE_H__
#define __CAN_PRE_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl_user.h"
#include "mcu.h"

#if (defined(PDL_PERIPHERAL_CANFD_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \addtogroup GroupCANFD
* \{ 
*/
/******************************************************************************
 * Global definitions
 ******************************************************************************/
/**
* \addtogroup GroupCANFD_Types
* \{
*/
/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief CAN Prescaler
 ******************************************************************************/
typedef enum en_canfd_prescaler
{
	CanfdPreDiv11  = 0,	// CAN prescaler clock: no division
	CanfdPreDiv12  = 1,   // CAN prescaler clock: 1/2
	CanfdPreDiv14  = 2,   // CAN prescaler clock: 1/4
	CanfdPreDiv18  = 3,   // CAN prescaler clock: 1/8
	CanfdPreDiv23  = 4,   // CAN prescaler clock: 2/3
	CanfdPreDiv13  = 5,   // CAN prescaler clock: 1/3
	CanfdPreDiv16  = 6,   // CAN prescaler clock: 1/6
	CanfdPreDiv112 = 7,   // CAN prescaler clock: 1/12
	CanfdPreDiv15  = 8,   // CAN prescaler clock: 1/5
	CanfdPreDiv110 = 9    // CAN prescaler clock: 1/10
} en_canfd_prescaler_t;

/** \}GroupCANFD_Types */

/******************************************************************************/
/* Global variable declarations ('extern', definition in C source)            */
/******************************************************************************/
/**
* \addtogroup GroupCANFD_Functions
* \{
*/ 
/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/

en_result_t Canpre_Init( en_canfd_prescaler_t enPrescale );

/** \}GroupCANFD_Functions */
/** \} GroupCANFD */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_CANPRE_ACTIVE))

#endif /* __CAN_PRE_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
