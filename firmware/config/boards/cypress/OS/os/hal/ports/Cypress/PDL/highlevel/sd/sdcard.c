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
/** \file sdcard.c
 **
 ** History:
 **   - 2013-05-16  1.0  QXu    First version.
 **   - 2013-07-24  1.1  RQian  Modification.
 **   - 2014-01-28  1.2  MWi    Some comment and code beautifying
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "sdcard.h"
#include <string.h>

#if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/**
 ******************************************************************************
 ** \addtogroup SdifGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
#define SD_DLY_1UNIT       8000

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
static uint32_t m_u32NumSector;

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/
/**
 ******************************************************************************
 ** \brief Sd card delay function
 **
 ** \param [in] u32Delay   Delay interval
 **
 ******************************************************************************/
static void SdDly_1unit (uint32_t u32Delay)
{
    volatile uint32_t u32i;
    for(;u32Delay;--u32Delay)
    {
        for(u32i = SD_DLY_1UNIT;u32i;--u32i);
    }
}

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
/**
 ******************************************************************************
 ** \brief detect SD
 **
 ** \retval TRUE        The SD card is inserted
 ** \retval FALSE       The SD card is non-existent   
 **
 ******************************************************************************/
boolean_t Sdcard_Detect(void)
{
  if (INSERTED == Sd_CardDetect(&Sd_stcCfg))
  {
	return TRUE;
  }
  
  return FALSE;
}

/**
 ******************************************************************************
 ** \brief SD card initialization
 **
 ** \param [in] pstcSdcardReg  SD card information  
 **
 ** \retval E_SD_NO_CARD            SD card is removed
 ** \retval E_SD_INIT               SD card initialization is failed
 ** \retval E_SD_OK                 SD card is initialized and ready  SRESP1
 **                              
 ******************************************************************************/
int32_t Sdcard_Init(stc_sdcard_info_t *pstcSdcardReg)
{
    stc_csdver1_t *pstcCsdSd;
    stc_csdver2_t *pstcCsdSdHc;
    uint32_t u32ArgAcd41 = 0;
    uint32_t u32Buf[4]={0};
    boolean_t result = FALSE;
    uint8_t u8Flag = 1;//1: Support High capacity; 0: not support
    volatile uint8_t u8Retry = 0;
    uint32_t i;
    uint32_t u32Csize;
    uint32_t u32CsizeMulti;

    if(NULL == pstcSdcardReg)
    {
        return E_SD_PARAMETER;
    }
    
    pstcCsdSd = (stc_csdver1_t*)pstcSdcardReg->CSD;
    pstcCsdSdHc = (stc_csdver2_t*)pstcSdcardReg->CSD;
    
    if(Sdcard_Detect() == FALSE)  return E_SD_NO_CARD; 

    result = Sdcmd_SendCmd(SD_CMD0, 0,  u32Buf);
    if(FALSE == result)
    {
        Sd_HostDeInit();
        return E_SD_INIT;
    }
    
	result = Sdcmd_SendCmd(SD_CMD8,ARG_CMD8,u32Buf); 
	if (FALSE == result) {
		Sd_HostDeInit();
		return E_SD_INIT;
	}
	for (i = 0; i < CMD8_WAIT; i++)
	{}
    
    //<3.Get OCR
    u8Retry = 0;
    memset(u32Buf,0,sizeof(u32Buf));

    do
    {
        u8Retry++;
        SdDly_1unit(100);
        result =Sdcmd_SendCmd(SD_CMD55,0,u32Buf); 
      
        if(result == TRUE)
        {
            u32ArgAcd41 =( (u8Flag == 1)? ARG_ACMD41_HCS_ON : ARG_ACMD41_HCS_OFF );
            Sdcmd_SendCmd(SD_ACMD41, u32ArgAcd41,  u32Buf);
        }
    }while((0 == (u32Buf[0]&ARG_ACMD41_BUSY))&&(u8Retry < 10));

    if(u8Retry >= 10)
    {
        return E_SD_INIT;
    }
    pstcSdcardReg->OCR = u32Buf[0];
    
    if(0 != (u32Buf[0] & OCR_CAPACITY_MASK )) //need to verify
    {
        pstcSdcardReg->stcCardType.u8Capacity = SD_CAPACITY_HIGH ;
    }
    else
    {
        pstcSdcardReg->stcCardType.u8Capacity = SD_CAPACITY_STANDARD;
    }    
    if(FALSE == result)
    {
        return E_SD_INIT;
    }
 
    SdDly_1unit(5);
    
    //<4.Get CID
    memset(u32Buf,0,sizeof(u32Buf));
    if(Sdcmd_SendCmd(SD_CMD2,0,u32Buf) == TRUE)
    {
        for(i=0;i<15;i++)
        {
            /* Response data 127:8 is stored in Response reg: 119:0 */
            /* Make clear of LSB/MSB */
            pstcSdcardReg->CID[i] = *((uint8_t *)(u32Buf) + (14 - i));
        }
    }
    else
    {
        return E_SD_INIT;
    }
    
     //<5.Get RCA
    memset(u32Buf,0,sizeof(u32Buf));
    if(Sdcmd_SendCmd(SD_CMD3,0,u32Buf) == TRUE)
    {
        pstcSdcardReg->RCA = u32Buf[0] >> 16;               
    }
    else
    {
        return E_SD_INIT;
    }
    
    //<6.Get CSD
    memset(u32Buf,0,sizeof(u32Buf));
    if(TRUE == Sdcmd_SendCmd(SD_CMD9,pstcSdcardReg->RCA<<16,u32Buf))
    {
        for(i=0;i<15;i++)
        {
            /* Response data 127:8 is stored in Response reg: 119:0 */
            /* Make clear of LSB/MSB */
            pstcSdcardReg->CSD[i] = *((uint8_t *)(u32Buf) + (14 - i));
        }
    }
    else
    {
        return E_SD_INIT;
    }

	/* *** High Capacity *** */
    if(SD_CAPACITY_HIGH == pstcSdcardReg->stcCardType.u8Capacity)
    {
		/* number of sectors */
		u32Csize = ((unsigned int)pstcCsdSdHc->u8C_size1 << 16) + ((unsigned int)pstcCsdSdHc->u8C_size2 << 8) + pstcCsdSdHc->u8C_size3;
		m_u32NumSector = (u32Csize + 1) * 1024;
	}
	/* *** Standard Capacity ***/
	else{
		/* number of sectors */
		u32Csize = ((unsigned int)pstcCsdSd->u8C_size1 << 10) + ((unsigned int)pstcCsdSd->u8C_size2 << 2) + pstcCsdSd->u8C_size3;
		u32CsizeMulti = (pstcCsdSd->u8C_size_multi1 << 1) + pstcCsdSd->u8C_size_multi2;
		m_u32NumSector = (u32Csize + 1) << (u32CsizeMulti + 2);

		if(pstcCsdSd->u8Read_bl_len == 0x0A ){
		  m_u32NumSector *= 2;
		}
		else if(pstcCsdSd->u8Read_bl_len == 0x0B ){
		  m_u32NumSector *= 4;
		}
	}
    pstcSdcardReg->u32MaxSectorNum = m_u32NumSector;
    
    //<7.Select Card
    if(Sdcmd_SendCmd(SD_CMD7, pstcSdcardReg->RCA<<16,  u32Buf) == TRUE)
    {
        return E_SD_OK;
    }
    else
    {
        return E_SD_INIT;
    }
}

/**
 ******************************************************************************
 ** \brief read one or multiple  sectors of data
 **
 ** This function  read the data and store into buffer
 **
 **
 ** \param [in] pstcSdcardInfo  SD card information  
 ** \param [in] u32Start        Data address to read
 ** \param [in] u16Count        Sector number to read
 ** \param [in] pu8Buf          Data buffer
 **
 ** \retval E_SD_NO_CARD     SD card is removed
 ** \retval E_SD_PARAMETER   Parameters are not valid
 ** \retval E_SD_READ        Read error
 ** \retval E_SD_OK          SD card accessing is OK    
 **                                                           
 ******************************************************************************/
int32_t Sdcard_ReadSector(stc_sdcard_info_t *pstcSdcardInfo, uint32_t u32Start, uint16_t u16Count, uint8_t *pu8Buf)
{  
    uint32_t u32addr;
    uint32_t u32i;
    boolean_t result = FALSE;
    uint32_t u32start_addr = u32Start;
    uint32_t u32maxsectornum = pstcSdcardInfo->u32MaxSectorNum;
    uint32_t u32buf[4] = {0};
    uint32_t *pu32 = (uint32_t*)pu8Buf;

    if(Sdcard_Detect() == FALSE)  return E_SD_NO_CARD; 
    
    if((pstcSdcardInfo ==NULL)||(u16Count == 0)||(u32Start+u16Count>u32maxsectornum)||(u32Start>u32maxsectornum))
    {
        return E_SD_PARAMETER;
    }

    if(SD_CAPACITY_STANDARD == pstcSdcardInfo->stcCardType.u8Capacity)
    {
        u32addr = u32start_addr<<9; ///<SDSC card uses byte unit address,multiply by 512
    }
    else
    {
        u32addr = u32start_addr;  ///<SDHC and SDXC card use block unit address (512 Bytes)
    }

    for (u32i = 0;u32i <u16Count;u32i++)
    {
        if(TRUE == Sdcmd_SendCmd(SD_CMD17,u32addr,u32buf))
        {
            result = Sdcmd_RxData(pu32);
        }
        else
        {
            return E_SD_READ;
        }
        
        u32start_addr++;
        pu32 += BLOCK_SIZE;
    }
    
    return (TRUE == result? E_SD_OK : E_SD_READ);
}

/**
 ******************************************************************************
 ** \brief write data into the SD card
 **
 ** This function  write one or multiple sectors of data into the SD card
 **
 **
 ** \param [in] pstcSdcardInfo  SD card information  
 ** \param [in] u32Start        Data address to write
 ** \param [in] u16Count        Sector number to write
 ** \param [in] pu8Buf          Data buffer
 **
 ** \retval E_SD_NO_CARD     SD card is removed
 ** \retval E_SD_PARAMETER   Parameters are not valid
 ** \retval E_SD_OK          SD card accessing is OK      
 **                              
 ******************************************************************************/
int32_t Sdcard_WriteSector(stc_sdcard_info_t *pstcSdcardInfo, uint32_t u32Start, uint16_t u16Count, const uint8_t *pu8Buf)
{
    uint32_t u32addr;
    uint32_t u32i;
    boolean_t result = FALSE;
    uint32_t u32start_addr = u32Start;
    uint32_t u32maxsectornum = pstcSdcardInfo->u32MaxSectorNum;
    uint32_t u32buf[4] = {0};
    uint32_t *pu32 = (uint32_t*)pu8Buf;
    
    if(Sdcard_Detect() == FALSE)  return E_SD_NO_CARD; 
    
    if((pstcSdcardInfo ==NULL)||(u16Count == 0)||(u32Start+u16Count>u32maxsectornum)||(u32Start>u32maxsectornum))
    {
        return E_SD_PARAMETER;
    }
    
    if(SD_CAPACITY_STANDARD == pstcSdcardInfo->stcCardType.u8Capacity)
    {
        u32addr = u32start_addr << 9; ///<SDSC card uses byte unit address,multiply by 512
    }
    else
    {
        u32addr = u32start_addr;  ///<SDHC and SDXC card use block unit address (512 Bytes)
    }

    for (u32i = 0;u32i <u16Count;u32i++)
    {
        if(TRUE == Sdcmd_SendCmd(SD_CMD24,u32addr,u32buf))
        {
            result = Sdcmd_TxData(pu32);
        }
        else
        {
            return E_SD_WRITE;
        }

        u32start_addr++;
        pu32 += BLOCK_SIZE;
    }
    
    return (TRUE == result? E_SD_OK : E_SD_WRITE);
}

//@} // SdifGroup

#endif // #if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
