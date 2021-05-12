/******************************************************************************
* \file             can_pre.c
*
* \version          1.20
*
* \brief            CAN prescaler function 
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
#include "can/can_pre.h"

#if (defined(PDL_PERIPHERAL_CANFD_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global (no 'static') and local ('static')        */
/******************************************************************************/

/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/

/**
 *****************************************************************************
 ** \brief Initialize CAN prescaler
 **
 ** Setup CAN prescaler for CANFD. If the CAN module was activated, use 
 ** Can_Init() API to initialize the CAN prescaler instead.
 **
 ** \param [in] enPrescale			Prescaler divisor.
 **
 ** \retval Ok                    	CAN prescaler has been successfully setup.
 ** \retval ErrorInvalidParameter 	Invalid divisor.
 *****************************************************************************/
en_result_t Canpre_Init( en_canfd_prescaler_t enPrescale )
{
	en_result_t	enResult;

	// Initialize result value (as 'Ok').
	enResult = Ok;

	// CAN dedicated clock prescaler.
	switch ( enPrescale )
	{
		case CanfdPreDiv11:
			FM_CANPRES->CANPRE = 0x00u;
			break;
		case CanfdPreDiv12:
			FM_CANPRES->CANPRE = 0x01u;
			break;
		case CanfdPreDiv14:
			FM_CANPRES->CANPRE = 0x02u;
			break;
		case CanfdPreDiv18:
			FM_CANPRES->CANPRE = 0x04u;
			break;
		case CanfdPreDiv23:
			FM_CANPRES->CANPRE = 0x08u;
			break;
		case CanfdPreDiv13:
			FM_CANPRES->CANPRE = 0x09u;
			break;
		case CanfdPreDiv16:
			FM_CANPRES->CANPRE = 0x0Au;
			break;
		case CanfdPreDiv112:
			FM_CANPRES->CANPRE = 0x0Bu;
			break;
		case CanfdPreDiv15:
			FM_CANPRES->CANPRE = 0x0Cu;
			break;
		case CanfdPreDiv110:
			FM_CANPRES->CANPRE = 0x0Eu;
			break;
		default:
			enResult = ErrorInvalidParameter;
			break;
	}

	// Return result.
	return enResult;
}

#endif // if (defined(PDL_PERIPHERAL_CANPRE_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
