/*******************************************************************************
* Copyright (C) 2013 Spansion LLC. All Rights Reserved. 
*
* This software is owned and published by: 
* Spansion LLC, 915 DeGuigne Dr. Sunnyvale, CA  94088-3453 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with Spansion 
* components. This software is licensed by Spansion to be adapted only 
* for use in systems utilizing Spansion components. Spansion shall not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  Spansion is providing this software "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the software.  
*
* SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS), 
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING, 
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED 
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED 
* WARRANTY OF NONINFRINGEMENT.  
* SPANSION SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, 
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT 
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, 
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR 
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, 
* SAVINGS OR PROFITS, 
* EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED 
* FROM, THE SOFTWARE.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Disclaimer and Copyright notice must be 
* included with each copy of this software, whether used in part or whole, 
* at all times.  
*/
/******************************************************************************/
/** \file sdcmd.h
 **
 ** Headerfile for SD functions
 **  
 ** History:
 **   - 2013-05-16  1.0  QXu  First version.
 **   - 2013-07-24  1.1  RQian  Modification.
 **
 ******************************************************************************/

#ifndef __SDCMD_H__
#define __SDCMD_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "sd\sd.h"

#if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \addtogroup SdifGroup
 ******************************************************************************/
//@{
  
/******************************************************************************
 * Global definitions
 ******************************************************************************/
#define SD_ACMD_OFFSET  0x40

//supported SD card command sets
#define SD_CMD0                           (0)
#define SD_CMD2                           (2)
#define SD_CMD3                           (3)

#define SD_CMD7                           (7)
#define SD_CMD8                           (8)
#define SD_CMD9                           (9)
#define SD_CMD10                          (10)

#define SD_CMD12                          (12)
#define SD_CMD13                          (13)

#define SD_CMD16                          (16)
#define SD_CMD17                          (17)
#define SD_CMD18                          (18)

#define SD_CMD24                          (24)
#define SD_CMD25                          (25)

#define SD_CMD28                          (28)
#define SD_CMD29                          (29)
#define SD_CMD30                          (30)

#define SD_CMD55                          (55)


#define SD_ACMD6                          (SD_ACMD_OFFSET + 6)
#define SD_ACMD13                         (SD_ACMD_OFFSET + 13)
#define SD_ACMD41                         (SD_ACMD_OFFSET + 41)
#define SD_ACMD51                         (SD_ACMD_OFFSET + 51)

#define	CRC7_INIT		0
#define INIT_CCITT_CRC16	0x0000
/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/******************************************************************************/
/* Global variable declarations ('extern', definition in C source)            */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/
extern boolean_t   Sdcmd_SendCmd(uint8_t u8Cmd,uint32_t u32Arg,uint32_t *pu32Buf);
extern boolean_t   Sdcmd_RxData(uint32_t* pu32Buf);
extern boolean_t   Sdcmd_TxData(uint32_t* pu32Buf);
/***ToDo for second release

**///ToDo for second release

//@} // SdifGroup
#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_SD_ACTIVE))

#endif /* __SDCMD_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
