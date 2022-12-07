/*!
 ******************************************************************************
 **
 ** \file debug.h
 **
 ** \brief Debug definition
 **
 ** \author FSLA AE Team 
 **
 ** \version V0.10
 **
 ** \date 2012-01-19
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. SPANSION LLC
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** Copyright (C) 2013 Spansion LLC. All Rights Reserved.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */ 
 
#ifndef _DEBUG_H_
#define _DEBUG_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/* Uncomment the line below to expanse the "ASSERT" macro in the 
   Standard Peripheral Library drivers code */
#define  DEBUG_MODE
#define DEBUGGING
#ifdef DEBUG_MODE
    #define ASSERT(expr) \
            {do \
            { \
                if (!(expr)) \
                { \
                    printf("FILE: %s\n", __FILE__); \
                    printf("FUNC: %s\n", __FUNCTION__); \
                    printf("LINE: %u\n\n", __LINE__); \
                } \
            }while(0);}
#else
    #define ASSERT(expr)
#endif


/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DEBUG_H_ */
/*****************************************************************************/
/* END OF FILE */
