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
/** \file sd.c
 **
 ** 
 **
 ** History:
 **   - 2013-04-16  1.0  QXu  First version.
 **   - 2013-07-24  1.1  RQian  Modification.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "sd.h"

#if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/**
 ******************************************************************************
 ** \addtogroup SdifGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
#define SD_PCG 0x4003C120
#define PCG_ENABLE (1UL<<8)
#define DELAYUNIT       5000
#define WAITTIME        50000
#define RETRYTIMES      100

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
volatile stc_sd_config_t Sd_stcCfg;
stc_sd_comdconfig_t  Sd_stcCmdCfg;
volatile stc_sd_t *Sd_stcCtrl = (volatile stc_sd_t *)FM4_SDIF_BASE;

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

/**
 ******************************************************************************
 ** \brief Delay according to specified number
 **
 ** \param [in] u32Delay     Delay number
 **
 ******************************************************************************/
static void dly_1unit (uint32_t u32Delay)
{
    volatile uint32_t u32i;

    for(;u32Delay;--u32Delay)
    {
        for(u32i = DELAYUNIT;u32i;--u32i);
    }
}

/**
 ******************************************************************************
 ** \brief Init the IO configuration for SDIF.
 **
 ** SCLK   :P0E
 ** CMD    :P0D
 ** DATA1  :P0C
 ** DATA0  :P0B
 ** DATA3  :P0A
 ** DATA2  :P09 (AN19)
 ** CD     :P63
 ** WP     :P62
 **
 ******************************************************************************/
static void SdInitPins(void)
{
    FM4_GPIO->ADE &= ~(uint32_t)(1<<19);    ///< AN19 = 0 select none analog function 
    FM4_GPIO->PFR0 |= 0x7E00;               ///< P09~P0E =1 
    FM4_GPIO->PFR6 |= 0x000C;               ///< P62~P63 =1 
  
#if (BUSWIDTH == BIT4)
    FM4_GPIO->PCR0 |= 0x1E00;               ///< PCR of P09~P0C =1  ,pull up the 4  data lines 
    FM4_GPIO->PCR0 &= ~0x0400;              // DATA3 pull down
#else
    FM4_GPIO->PCR0 |= 0x0800;               ///< PCR of P0B =1  ,pull up the DAT0  lines 
#endif    

    FM4_GPIO->EPFR18 |= 0x01554000;         ///< enable peripheral function 
}

/**
 ******************************************************************************
 ** \brief Enable SD I/F peripheral clock
 **
 ******************************************************************************/
static boolean_t SdEnablePCG(void)
{   
    uint32_t u32psg = 0;
    volatile uint32_t * p32Sdpcg = (uint32_t *)SD_PCG;

    *p32Sdpcg |= PCG_ENABLE;
    u32psg = *p32Sdpcg;
    if(PCG_ENABLE == (u32psg & PCG_ENABLE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 ******************************************************************************
 ** \brief Disable SD I/F peripheral clock
 **
 ******************************************************************************/
static void SdDisablePCG(void)
{
    *((uint32_t *)SD_PCG) &= ~(1<<8);
}

/**
 ******************************************************************************
 ** \brief Initial the SDIF internal data
 **
 ** \param [in] pstcSdCfg    Pointer of SDIF configuration data
 ** \param [in] pstcCmdCfg   Pointer of SDIF command configuration data
 **
 ******************************************************************************/
static void SdInitHostInternalData(volatile stc_sd_config_t *pstcSdCfg,stc_sd_comdconfig_t *pstcCmdCfg)
{   
#if (BUSWIDTH == BIT4 )
    pstcSdCfg->enBusWidth = BIT_4;
#else
    pstcSdCfg->enBusWidth = BIT_1;    
#endif   
    pstcSdCfg->enExist = REMOVAL;
    pstcSdCfg->bCmdComplete = FALSE;
    pstcSdCfg->bSendComplete = FALSE;
    pstcSdCfg->bDMAComplete = FALSE;
    pstcSdCfg->pfnErrorCallback = NULL;
    pstcSdCfg->pfnRxCallback = NULL;
    pstcSdCfg->pfnTxCallback = NULL;
    pstcSdCfg->pfnWakeupCallback = NULL;

    pstcCmdCfg->enResponse = NO_RSP;
    //pstcCmdCfg->enReadWrite = SD_READ;
    pstcCmdCfg->enCmdType = NORMAL;
    pstcCmdCfg->bDataPresent = FALSE;
    pstcCmdCfg->u8Index = 0;
    pstcCmdCfg->enAutoCmd = AUTO_DISABLE;
    pstcCmdCfg->u16BlockCount = 0;
    pstcCmdCfg->u16BlockSize = 0;
    pstcCmdCfg->u32Argument1 = 0;
#ifdef   SD_DMA  
    pstcCmdCfg->u32SysAddr_Arg2 = 0xFFFFFFFF;
    pstcCmdCfg->enbound = BOUND_4K;
#endif       
    pstcCmdCfg->pu32Buffer = NULL;
    pstcCmdCfg->pfnErrorResponseCallback = NULL;
}

/**
 ******************************************************************************
 ** \brief Check whether the command complete
 **
 ** \return TRUE    Completed
 ** \return FALSE   Imcompleted
 **
 ******************************************************************************/
static boolean_t SdGetCmdComplete(void)
{
    if(TRUE == Sd_stcCtrl->SNINTST_f.CMDCMPLT)  //command complete?
    {  
        Sd_stcCfg.bCmdComplete = TRUE;
    }
    else
    {
        Sd_stcCfg.bCmdComplete = FALSE;
    }

    return (Sd_stcCfg.bCmdComplete);  
}

/**
 ******************************************************************************
 ** \brief Clear command complete flag
 **
 ******************************************************************************/
static void SdClearCmdComplete(void)
{
   Sd_stcCfg.bCmdComplete = FALSE;
}

/**
 ******************************************************************************
 ** \brief Check whether the transfer complete
 **
 ** \return TRUE    Completed
 ** \return FALSE   Imcompleted
 **
 ******************************************************************************/
static boolean_t SdGetTrsComplete(void)
{
    if(1 == Sd_stcCtrl->SNINTST_f.TRSFCMPLT)  //Transfer complete
    {
        Sd_stcCfg.bSendComplete = TRUE;
    }
    else
    {
        Sd_stcCfg.bSendComplete = FALSE;
    }

    return (Sd_stcCfg.bSendComplete);
}

/**
 ******************************************************************************
 ** \brief Wait for card status stable and set the card status flag
 **
 ** \param [in] pstcSd    Pointer of SDIF instance
 ** \param [in] pstcCfg   Pointer of SDIF configuration data
 **
 ******************************************************************************/
static void SdGetCardStatus(volatile stc_sd_t* pstcSd,volatile stc_sd_config_t* pstcCfg)
{
    uint16_t waittime = WAITTIME;

    while((pstcSd->SPRSTAT_f.CARDSTB == 0) && (waittime > 0))
    {
        waittime--;
        dly_1unit(1);
    }
    
    if(1 == pstcSd->SPRSTAT_f.CARDINS)
    {
        pstcCfg->enExist=  INSERTED; 
    }
    else
    {
        pstcCfg->enExist=  REMOVAL; 
    }
}

/**
 ******************************************************************************
 ** \brief SD interrupt handler                 
 ** 
 ******************************************************************************/
static void SdIrqHandler(void)
{
    /* To do ... */
}

/**
 ******************************************************************************
 ** \brief get response
 **
 ** This function will  get the response data.
 **
 ** \param [in]  pstcSd          Pointer of SDIF instance
 ** \param [in]  enSdresponse    SD response type
 ** \param [out] pu32Buf         Buffer of received response data.  
 **                               
 ******************************************************************************/
static void SdGetResponse(volatile stc_sd_t* pstcSd,en_sd_response_t enSdresponse,uint32_t* pu32Buf)
{
    switch(enSdresponse)
    {
        case R1NORMAL_R5_R6_R7: 
        case R1B_NORMAL:    
        case R3_R4:
        case R5B: 
            pu32Buf[0] = pstcSd->SRESP0;
            pu32Buf[0] |=((uint32_t)pstcSd->SRESP1<<16);
            break;
        case R1AUTO:
        case R1B_AUTO:    
            pu32Buf[0] = pstcSd->SRESP6;
            pu32Buf[0] |=((uint32_t)pstcSd->SRESP7<<16);
            break;
        case R2:    //120 bits
            pu32Buf[0] = pstcSd->SRESP0;
            pu32Buf[0] |=(uint32_t)pstcSd->SRESP1<<16;
            pu32Buf[1] = pstcSd->SRESP2;
            pu32Buf[1] |=(uint32_t)pstcSd->SRESP3<<16;
            pu32Buf[2] = pstcSd->SRESP4;
            pu32Buf[2] |=(uint32_t)pstcSd->SRESP5<<16;
            pu32Buf[3] = pstcSd->SRESP6;
            pu32Buf[3] |=((uint32_t)pstcSd->SRESP7&0x00FF)<<16;
            break;
        default :
            break;
    }
}

/**
 ******************************************************************************
 ** \brief Send command without data transfer using DAT line 
 **
 ** \param [in] pstcSd      Pointer of SDIF instance
 ** \param [in] pstcCmdCfg  Pointer of SD command configuration
 **
 ** \retval TURE        Command is sent 
 ** \retval FALSE       Command sent error
 **                               
 ******************************************************************************/
static boolean_t SdSendCmdOnly(volatile stc_sd_t* pstcSd, stc_sd_comdconfig_t * pstcCmdCfg)
{
    boolean_t   result = FALSE;
    uint8_t u8Index = pstcCmdCfg->u8Index & 0x3F;
    uint16_t waittime = RETRYTIMES;

    /* Wait for data line free */
    while((TRUE == pstcSd->SPRSTAT_f.CMDDATINH) && (waittime > 0))
    {
        waittime--;
        dly_1unit(1);
    }
    
    /* Wait for command line free */
    waittime = RETRYTIMES;
    
    while((TRUE == pstcSd->SPRSTAT_f.CMDINH) && (waittime > 0))
    {
        waittime--;
        dly_1unit(1);
    }

    pstcSd->SSA1 = pstcCmdCfg->u32Argument1;
    pstcSd->SBSIZE_f.TRSFBLCKSZ = pstcCmdCfg->u16BlockSize;
    pstcSd->STRSFMD_f.DTTRSFDIR = pstcCmdCfg->enReadWrite;
    
    /* set the response type,index check enable and CRC check enable of command register*/
    switch(pstcCmdCfg->enResponse)
    {
        case R1NORMAL_R5_R6_R7:                 
        case R1AUTO:                
            pstcSd->SCMMD = 0x0002|(u8Index<<8)|0x0018|(pstcCmdCfg->bDataPresent<<5);             
            break;
        case R1B_NORMAL:
        case R1B_AUTO:
        case R5B:
            pstcSd->SCMMD = 0x0003|(u8Index<<8)|0x0018;              
            break;
        case R2:;
            pstcSd->SCMMD = 0x0001|(u8Index<<8)|0x0008;               
            break;
        case R3_R4:               
            pstcSd->SCMMD = 0x0002|(u8Index<<8);            
        break;
        default :
            pstcSd->SCMMD &= 0x0000;
            break;
    }

    waittime = RETRYTIMES;
    
    /* Wait for command line free */
    while((TRUE == pstcSd->SPRSTAT_f.CMDINH) && (waittime > 0))
    {
        waittime--;
        dly_1unit(1);
    }

    waittime = RETRYTIMES;
    
    /* Wait for data line free */
    while((TRUE == pstcSd->SPRSTAT_f.CMDDATINH) && (waittime > 0))
    {
        waittime--;
        dly_1unit(1);
    }
    
    if(TRUE == SdGetCmdComplete())
    {
        SdClearCmdComplete();
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    
    return result;
}

/**
 ******************************************************************************
 ** \brief Send command with data transfer using DAT line 
 **
 ** \param [in] pstcSd      Pointer of SDIF instance
 ** \param [in] pstcCmdCfg  Pointer of SD command configuration
 **
 ** \retval TURE        Command is sent 
 ** \retval FALSE       Command sent error
 **                               
 ******************************************************************************/
static boolean_t SdSendCmdDatLine(volatile stc_sd_t* pstcSd, stc_sd_comdconfig_t * pstcCmdCfg)
{
    uint8_t result = FALSE;
    uint16_t blockcount= pstcCmdCfg->u16BlockCount;

    pstcSd->SBSIZE_f.TRSFBLCKSZ =pstcCmdCfg->u16BlockSize & 0x0FFF;
        
    if(blockcount == 1)     //single block 
    {
        pstcSd->SBLCNT = 1;
        pstcSd->STRSFMD_f.BLCKCNTSEL = 0;
    }
    else                    //multiple block
    {
        pstcSd->STRSFMD_f.BLCKCNTSEL = 1;
        pstcSd->STRSFMD_f.BLCKCNTEN = 1;
        pstcSd->STRSFMD_f.AUTOCMDEN = pstcCmdCfg->enAutoCmd;
        pstcSd->SBLCNT = blockcount;       
    }

    result = SdSendCmdOnly(pstcSd,pstcCmdCfg) ;

    return result;
}

/**
 ******************************************************************************
 ** \brief Send data segment using DAT line 
 **
 ** \param  [in] pstcSd          Pointer of SDIF instance
 ** \param  [in] pstcCmdCfg      SD command configuration
 ** \param  [in] pu32Buf         Data to be send
 **
 ** \retval TURE            Data is sent 
 ** \retval FALSE           Error occurs during transmission 
 **                               
 ******************************************************************************/
static boolean_t SdTxData(volatile stc_sd_t* pstcSd, stc_sd_comdconfig_t * pstcCmdCfg,uint32_t* pu32Buf)
{
    uint16_t i ;
    uint16_t u16blocksize= pstcCmdCfg->u16BlockSize; //unit of Bytes
    uint32_t*  pbuf = pu32Buf;
    uint8_t result = FALSE;
    
    while(TRUE != pstcSd->SNINTST_f.BUFWRRDY);// endless loop
    
    pstcSd->SNINTST_f.BUFWRRDY = 0;
    
    for(i=u16blocksize>>2;i!=0;i--)
    {
        if((pstcSd->SEINTST_f.DTTOERR == 1)
            ||(pstcSd->SEINTST_f.DTEBERR == 1)
            ||(pstcSd->SEINTST_f.DTCRCERR == 1))
        {
            //clear the int status
            pstcSd->SEINTST_f.DTTOERR = 1;
            pstcSd->SEINTST_f.DTEBERR = 1;
            pstcSd->SEINTST_f.DTCRCERR = 1;
            return result;
        }

        while(pstcSd->SPRSTAT_f.BUFWREN == 0) ;//endless loop
        pstcSd->SBUFDP = *pbuf;        
        pbuf++;    
    }

    while(TRUE != SdGetTrsComplete());
    
    result = TRUE;

    return result;
}

/**
 ******************************************************************************
 ** \brief Get response data after command
 **
 ** \param [in]  pstcSd          Pointer of SDIF instance
 ** \param [in]  pstcCmdCfg SD   Command configuration
 ** \param [out] pu32Buf         Buffer of received response data.  
 **
 ** \retval TURE            The correct response is received
 ** \retval FALSE           Error response is received
 **                               
 ******************************************************************************/
static boolean_t SdRxRsp(volatile stc_sd_t* pstcSd,stc_sd_comdconfig_t * pstcCmdCfg,uint32_t* pu32Buf)
{
    boolean_t   result = FALSE;
    
    //clear the buffer
    pu32Buf[0] = 0;
    pu32Buf[1] = 0;
    pu32Buf[2] = 0;
    pu32Buf[3] = 0;

    //get response data    
    SdGetResponse(pstcSd,pstcCmdCfg->enResponse,pu32Buf);

    //check the error in response data
    if(NULL != pstcCmdCfg->pfnErrorResponseCallback)
    {
        pstcCmdCfg->pfnErrorResponseCallback(pstcSd);
    }
    else
    {
        result = TRUE;
    }

    return result;
}

/**
 ******************************************************************************
 ** \brief Receive data segment using DAT line 
 **
 ** \param [in] pstcSd      Pointer of SDIF instance
 ** \param [in] pstcCmdCfg  SD command configuration
 ** \param [in] pu32Buf     Pointer to reception buffer
 **
 ** \retval TURE        Command is sent 
 ** \retval FALSE       Command send error
 **                               
 ******************************************************************************/
static boolean_t SdRxData(volatile stc_sd_t* pstcSd, stc_sd_comdconfig_t * pstcCmdCfg, uint32_t* pu32Buf)
{
    uint16_t i ;
    uint16_t u16blocksize= pstcCmdCfg->u16BlockSize; //unit of Bytes
    uint32_t*  pbuf = pu32Buf;
    uint8_t result = FALSE;

    while(TRUE != pstcSd->SNINTST_f.BUFRDRDY);//endless loop

    for(i=u16blocksize>>2;i!=0;i--)
    {
        if((pstcSd->SEINTST_f.DTTOERR == 1)     //DataTimeout Error
            ||(pstcSd->SEINTST_f.DTEBERR==1)    //Data End Bit Error
            ||(pstcSd->SEINTST_f.DTCRCERR==1))  //Data CRC Error
        {
            //clear the int status
            pstcSd->SEINTST_f.DTTOERR = 1;
            pstcSd->SEINTST_f.DTEBERR=1;
            pstcSd->SEINTST_f.DTCRCERR=1;
            return result;
        }
            
        while(pstcSd->SPRSTAT_f .BUFRDEN == 0) ;//endless loop
        *pbuf = pstcSd->SBUFDP;
        pbuf++  ;
    }

    result = TRUE;

    return result;
}

/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/
/**
 ******************************************************************************
 ** \brief Enable the interrupt flag of SD
 **
 ** \param [in] pstcSd   Pointer of SDIF instance
 **
 ** \retval Ok
 **
 ******************************************************************************/
en_result_t Sd_EnableInt(volatile stc_sd_t* pstcSd )
{
    if(NULL == pstcSd)
    {
        return ErrorInvalidParameter;
    }

    //buffer read&write ready int
    pstcSd->SNINTSTE_f.BUFRDRDYS = 1;
    pstcSd->SNINTSTE_f.BUFWRRDYS = 1;

    //Transfer & command complete
    pstcSd->SNINTSTE_f.TRSFCMPLTS = 1;
    pstcSd->SNINTSTE_f.CMDCMPLTS = 1;

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable the interrupt flag of SD
 **
 ** \param [in] pstcSd   Pointer of SDIF instance
 **
 ** \retval Ok
 **
 ******************************************************************************/
en_result_t Sd_DisableInt(volatile stc_sd_t* pstcSd )
{
    if(NULL == pstcSd)
    {
        return ErrorInvalidParameter;
    }			

    //buffer read&write ready int
    pstcSd->SNINTSTE_f.BUFRDRDYS = 0;
    pstcSd->SNINTSTE_f.BUFWRRDYS = 0;

    //Transfer & command complete
    pstcSd->SNINTSTE_f.TRSFCMPLTS = 0;
    pstcSd->SNINTSTE_f.CMDCMPLTS = 0;

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Detect the SD card insertion status
 **
 ** \param [in] pstcCmdCfg  Command parameters 
 **
 ** \retval DEBOUNCING          SD card is debouncing
 ** \retval INSERTED            SD card is inserted
 ** \retval REMOVAL             SD card is removed                            
 **
 ******************************************************************************/
en_sd_existing_t Sd_CardDetect(volatile stc_sd_config_t* pstcCmdCfg)
{
    if(NULL == pstcCmdCfg)
        return DEBOUNCING;
    else
        return (pstcCmdCfg->enExist);    
}

/**
 ******************************************************************************
 ** \brief Supply clock to an SD card
 **
 ** \param [in] pstcSd  Pointer of SDIF instance
 ** \param [in] enClk   SD Clock frequency
 **
 ** \retval  Ok                                       
 **
 ******************************************************************************/
en_result_t Sd_ClockSupply(volatile stc_sd_t* pstcSd,en_sd_clk_t enClk)
{
    uint8_t u8baseclock;
    uint16_t u16divider;
    u8baseclock = pstcSd->CAPBLTY0_f.SDBASECLK;//read hardware init value of the capability reigister

    if(NULL == pstcSd)
    {
        return ErrorInvalidParameter;
    }			
    
    //set the 10 bit divider of Clock control register, N=baseclock/(2*enClk)
    if(u8baseclock)
    {
        if(u8baseclock== enClk)
        {
            u16divider = 0x0000;/// base clock
        }
        else
        {
            u16divider = (u8baseclock>>1)/enClk;
        }
                
        pstcSd->SCLKCTL_f.SDCLKSEL = (uint8_t)(u16divider &0x00FF);//lower 8bits
        pstcSd->SCLKCTL_f.UPSDCLKSEL = (u16divider &0x0300)>>8; //higher 2bits
    }
    else    //if the base clock frequenct register value is 0,using the default value 160 Mhz
    {
        pstcSd->SHCTL2_f.PREVALEN = 1;// automatic selection by preset value (4)
    }

    pstcSd->SCLKCTL_f.INTLCLCKEN = 1;
   
    while (!pstcSd->SCLKCTL_f.INTLCLCKST);
    
    pstcSd->SCLKCTL_f.SDCLCKEN = 1;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief stop clock
 **
 ** \param [in] pstcSd   Pointer of SDIF instance
 **
 ** \retval  Ok                          
 ******************************************************************************/
en_result_t  Sd_ClockStop(volatile stc_sd_t* pstcSd)
{
    if(pstcSd == NULL)
    {
        return ErrorInvalidParameter;			
    }			
    
    pstcSd->SCLKCTL_f.SDCLCKEN = 0;
    return Ok;		
}

/**
 ******************************************************************************
 ** \brief Provide power for SD card
 **
 ** \param [in] pstcSd     Pointer of SDIF instance
 ** \param [in] enVoltage  SD card voltage setting
 **
 ** \retval Ok                           
 ******************************************************************************/
en_result_t    Sd_BusPowerControl(volatile stc_sd_t* pstcSd,en_sd_busvoltage_t enVoltage)
{    
    if((NULL == pstcSd) || (enVoltage > V3_3) || (enVoltage < V1_8))
    {
        return  ErrorInvalidParameter; 			
    }			
	
    pstcSd->SPWRCTL_f.SDBUSVLSEL = enVoltage;
    pstcSd->SPWRCTL_f.SDBUSPWR = 1;  
    return Ok;		
}

/**
 ******************************************************************************
 ** \brief Transmit commands and check the corresponding response
 **
 ** \param [in] enTran      Transmission type depends on the CMD and expected response      
 ** \param [in] pstcSd      Pointer of SDIF instance
 ** \param [in] pstcCmdCfg  Command parameters 
 ** \param [in] pu32Buf     Buffer pointer storing the recieved data
 **
 ** \retval TRUE   Data is transmitted 
 ** \retval FAlSE  Error occurs during transmission
 **                               
 ******************************************************************************/
boolean_t Sd_SendCmd(en_sd_transaction_t enTran,volatile stc_sd_t* pstcSd,stc_sd_comdconfig_t * pstcCmdCfg,uint32_t* pu32Buf)
{
    boolean_t result = FALSE;
        
    switch(enTran)
    {
        case NO_RESPONSE:
            result = SdSendCmdOnly(pstcSd,pstcCmdCfg);
            break;
        case NO_DATA:
            if(TRUE == SdSendCmdOnly(pstcSd,pstcCmdCfg))
            {
                result = SdRxRsp(pstcSd,pstcCmdCfg,pu32Buf);
            }
            break;
        case SINGLE_BLOCK: 
            if(TRUE == SdSendCmdDatLine(pstcSd,pstcCmdCfg))
            {
                result = SdRxRsp(pstcSd,pstcCmdCfg,pu32Buf);                          
            }
            break; 
        case MULTI_BLOCK:
            //To Do
            break;
        default:
            break;
    }
    return result;
}

/**
 ******************************************************************************
 ** \brief Transmit data segment for write commands only.
 **        
 ** \param [in] pstcSd      Pointer of SDIF instance
 ** \param [in] pstcCmdCfg  Command parameters 
 ** \param [in] pu32Buf     Buffer pointer storing tje recieved data 
 **
 ** \retval TRUE   Data is transmitted 
 ** \retval FAlSE  Error occurs during transmission
 **                            
 ******************************************************************************/
boolean_t Sd_TxData(volatile stc_sd_t* pstcSd,stc_sd_comdconfig_t * pstcCmdCfg,uint32_t* pu32Buf)
{
    boolean_t result = FALSE; 

    result = SdTxData(pstcSd,pstcCmdCfg,pu32Buf);               

    return result;
}

/**
 ******************************************************************************
 ** \brief Receive the data of read command
 **
 ** \param [in]  enTran       Transmission type depends on the CMD and expected response      
 ** \param [in]  pstcSd       Pointer of SDIF instance
 ** \param [in]  pstcCmdCfg   Command parameters 
 ** \param [out] pu32Buf      Buffer pointer storing tje recieved data 
 **
 ** \retval TRUE   Data is received 
 ** \retval FAlSE  Error occurs during reception
 **                              
 ******************************************************************************/
boolean_t Sd_RxData(en_sd_transaction_t enTran,volatile stc_sd_t* pstcSd,stc_sd_comdconfig_t * pstcCmdCfg,uint32_t* pu32Buf)
{
    boolean_t result = FALSE;
    uint8_t waittime = RETRYTIMES;
     
    //wait for read buffer is ready
    while((0 == pstcSd->SPRSTAT_f.BUFRDEN) && (waittime > 0))
    {
        waittime--;
        dly_1unit(1);
    }        
 
    switch(enTran)
    {
        case SINGLE_BLOCK:
            result = SdRxData(pstcSd,pstcCmdCfg,pu32Buf);
            break; 
        case MULTI_BLOCK:
            //To Do
            break;
        default:
            break;
    }
    return result;
}

/**
 ******************************************************************************
 ** \brief software reset for command and data line
 **
 ** \param [in] pstcSd    Pointer of SDIF instance
 ** \param [in] u8reset   Options of reset setting,such as command ,data line and all
 **
 ** \retval Ok
 **
 ******************************************************************************/
en_result_t Sd_SoftwareReset( volatile stc_sd_t* pstcSd,uint8_t u8reset)
{
    if(NULL == pstcSd)
    {
        return ErrorInvalidParameter;
    }

    pstcSd->SSRST |= u8reset;

    while(pstcSd->SSRST & (~u8reset)); //endless loop

    pstcSd->SSRST &= ~u8reset;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief SD interrupt entry
 **
 ******************************************************************************/
void Sd_Handler(void)
{
    SdIrqHandler();
}

/**
 ******************************************************************************
 ** \brief Initialize SD host 
 **
 ** \retval Ok          Initialization successful
 ** \retval Error       Initialization failed
 ** 
 ******************************************************************************/
en_result_t Sd_HostInit( void)
{
    SdInitPins();
    
    if (TRUE == SdEnablePCG())  
    {
        SdInitHostInternalData(&Sd_stcCfg, &Sd_stcCmdCfg);      
        SdGetCardStatus(Sd_stcCtrl,&Sd_stcCfg);

        if(INSERTED == Sd_CardDetect(&Sd_stcCfg))
        {
            Sd_EnableInt(Sd_stcCtrl);
            Sd_BusPowerControl(Sd_stcCtrl,V3_3);
            Sd_ClockSupply(Sd_stcCtrl,CLK_25M);
            return Ok;    
        }
        else
        {
            return Error;
        }
     }
    else
    {
      return Error;
    }   
}

/**
 ******************************************************************************
 ** \brief Deinitializes SD
 **
 ** \retval Ok                    Internal data has been setup
 ** 
 ******************************************************************************/
en_result_t Sd_HostDeInit(void )
{  
    Sd_SoftwareReset(Sd_stcCtrl, RESET_ALL);
    Sd_DisableInt(Sd_stcCtrl);    
    SdDisablePCG();
    return Ok;
}

//@} //SdifGroup

#endif // #if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

