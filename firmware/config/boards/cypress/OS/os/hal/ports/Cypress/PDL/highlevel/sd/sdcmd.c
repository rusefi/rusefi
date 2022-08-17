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
/** \file sdcmd.c
 **
 ** 
 **
 ** History:
 **   - 2013-05-16  1.0  QXu  First version.
 **   - 2013-07-24  1.1  RQian  Modification.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "sdcmd.h"

#if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/**
 ******************************************************************************
 ** \addtogroup SdifGroup
 ******************************************************************************/
//@{

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
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
/**
 ******************************************************************************
 ** \brief get data of SD card
 **
 ** This function retrieve the data of SD card
 **
 ** \param [out]  pu32Buf    Buffer to store the received data
 **
 ** \retval TRUE     Receive data success              
 ** \retval FALSE    Receive data failure
 **                              
 ******************************************************************************/
boolean_t Sdcmd_RxData(uint32_t* pu32Buf)
{
    return Sd_RxData(SINGLE_BLOCK,Sd_stcCtrl,&Sd_stcCmdCfg,pu32Buf);
}

/**
 ******************************************************************************
 ** \brief send data to SD card
 **
 ** This function send ata to SD card
 **
 ** \param [in]  pu32Buf    Data buffer to be sent 
 **
 ** \retval TRUE     Send data success              
 ** \retval FALSE    Send data failure
 **                              
 ******************************************************************************/
boolean_t   Sdcmd_TxData(uint32_t* pu32Buf)
{
    return Sd_TxData(Sd_stcCtrl,&Sd_stcCmdCfg,pu32Buf);
}
/**
 ******************************************************************************
 ** \brief send command to SD card
 **
 ** This function will send out the command based on the SD speification.
 ** 
 **
 ** \param [in]  u8Cmd      SD command index
 ** \param [in]  u32Arg     SD command argument
 ** \param [in]  pu32Buf    Buffer to store the response data
 **
 ** \retval Ok              Send success
 ** \retval Error           Send failure
 **                             
 ******************************************************************************/
boolean_t Sdcmd_SendCmd(uint8_t u8Cmd,uint32_t u32Arg,uint32_t *pu32Buf)
{ 
    en_sd_transaction_t enTran ;
    boolean_t result = FALSE;
    uint8_t u8Cmdindex = u8Cmd;
    
    switch(u8Cmdindex)
    {
        case SD_CMD0:
            enTran = NO_RESPONSE;
            Sd_stcCmdCfg.enReadWrite = SD_WRITE;
            Sd_stcCmdCfg.enResponse = NO_RSP;
            Sd_stcCmdCfg.enCmdType = NORMAL;
            Sd_stcCmdCfg.bDataPresent = FALSE;
            Sd_stcCmdCfg.u16BlockSize = 0;
            Sd_stcCmdCfg.u16BlockCount = 0;
            break;
        case SD_CMD2:
        case SD_CMD9:                          
        case SD_CMD10:     
            enTran = NO_DATA;
            Sd_stcCmdCfg.enReadWrite = SD_WRITE;
            Sd_stcCmdCfg.enResponse = R2;
            Sd_stcCmdCfg.enCmdType = NORMAL;
            Sd_stcCmdCfg.bDataPresent = FALSE;
            Sd_stcCmdCfg.u16BlockSize = 0;
            Sd_stcCmdCfg.u16BlockCount = 0;
            break;
        case SD_CMD3:                                                     
        case SD_CMD8:
        case SD_CMD13: 
        case SD_CMD16:
        case SD_CMD55:
        case SD_ACMD6:
            enTran = NO_DATA;
            Sd_stcCmdCfg.enReadWrite = SD_WRITE;
            Sd_stcCmdCfg.enResponse = R1NORMAL_R5_R6_R7;
            Sd_stcCmdCfg.enCmdType = NORMAL;
            Sd_stcCmdCfg.bDataPresent = FALSE;
            Sd_stcCmdCfg.u16BlockSize = 0;
            Sd_stcCmdCfg.u16BlockCount = 0;
            break;
        case SD_CMD7:   
            enTran = NO_DATA;
            Sd_stcCmdCfg.enReadWrite = SD_WRITE;
            Sd_stcCmdCfg.enResponse = R1B_NORMAL;
            Sd_stcCmdCfg.enCmdType = NORMAL;
            Sd_stcCmdCfg.bDataPresent = FALSE;
            Sd_stcCmdCfg.u16BlockSize = 0;
            Sd_stcCmdCfg.u16BlockCount = 0;
            break;
        case SD_CMD12:
            enTran = NO_DATA;
            Sd_stcCmdCfg.enReadWrite = SD_WRITE;
            Sd_stcCmdCfg.enResponse = R1B_AUTO;
            Sd_stcCmdCfg.enCmdType = ABORT;
            Sd_stcCmdCfg.bDataPresent = FALSE;
            Sd_stcCmdCfg.u16BlockSize = 0;
            Sd_stcCmdCfg.u16BlockCount = 0;
            break;
        case SD_CMD17:            
            enTran = SINGLE_BLOCK;
            Sd_stcCmdCfg.enReadWrite = SD_READ;
            Sd_stcCmdCfg.enResponse = R1NORMAL_R5_R6_R7;
            Sd_stcCmdCfg.enCmdType = NORMAL;
            Sd_stcCmdCfg.bDataPresent = TRUE;
            Sd_stcCmdCfg.u16BlockSize = 512;
            Sd_stcCmdCfg.u16BlockCount = 1;
            break;
        case SD_CMD24:
            enTran = SINGLE_BLOCK;
            Sd_stcCmdCfg.enReadWrite = SD_WRITE;
            Sd_stcCmdCfg.enResponse = R1NORMAL_R5_R6_R7;
            Sd_stcCmdCfg.enCmdType = NORMAL;
            Sd_stcCmdCfg.bDataPresent = TRUE;
            Sd_stcCmdCfg.u16BlockSize = 512;
            Sd_stcCmdCfg.u16BlockCount = 1;
            break; 
        case SD_ACMD13:
            enTran = SINGLE_BLOCK;
            Sd_stcCmdCfg.enReadWrite = SD_WRITE;
            Sd_stcCmdCfg.enResponse = R1NORMAL_R5_R6_R7;
            Sd_stcCmdCfg.enCmdType = NORMAL;
            Sd_stcCmdCfg.bDataPresent = TRUE;
            Sd_stcCmdCfg.u16BlockSize = 512;
            Sd_stcCmdCfg.u16BlockCount = 1;
            break;
        case SD_ACMD41:                    
            enTran = NO_DATA;
            Sd_stcCmdCfg.enReadWrite = SD_WRITE;
            Sd_stcCmdCfg.enResponse = R3_R4;
            Sd_stcCmdCfg.enCmdType = NORMAL;
            Sd_stcCmdCfg.bDataPresent = FALSE;   
            Sd_stcCmdCfg.u16BlockSize = 0;
            Sd_stcCmdCfg.u16BlockCount = 0;
            break;
        default:
            break;
    }
    
    Sd_stcCmdCfg.u8Index = u8Cmdindex;

    Sd_stcCmdCfg.u32Argument1 = u32Arg;
    
    result = Sd_SendCmd(enTran,Sd_stcCtrl,&Sd_stcCmdCfg,pu32Buf);

    return result;
}

//@} // SdifGroup

#endif // #if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

