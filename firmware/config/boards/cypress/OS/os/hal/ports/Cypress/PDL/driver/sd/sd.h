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
/** \file sd.h
 **
 ** Headerfile for SD functions
 **  
 ** History:
 **   - 2013-04-16  1.0  QXu  First version.
 **   - 2013-07-24  1.1  RQian  Modification.
 **
 ******************************************************************************/

#ifndef __SD_H__
#define __SD_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl.h"
#include "sd_cfg.h"

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
 * SDIF registers definitions
 ******************************************************************************/

/******************************************************************************
 * Global definitions
 ******************************************************************************/
#define stc_sd_t   FM4_SDIF_TypeDef

/* The Physical Layer Specification Version 3.0x defines that SDXC card may indicate 500ms busy.
 Then host driver may need to change timeout value for SDXC.It is set more than 500ms
timeout regardless of card capacities.*/
#define SD_TIMEOUT_DATLINE  1000 //ms

/* software reset options*/
#define RESET_DAT (1<<2)
#define RESET_CMD (1<<1)
#define RESET_ALL 1
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
typedef void    (*func_ptr_sd_arg32_t)( volatile stc_sd_t *);

/**
 ******************************************************************************
 ** \brief SD clock SDCLK frequency
 ******************************************************************************/

typedef enum en_sd_clk
{
    CLK_25M = 25,   ///<25 Mhz
    CLK_50M = 50    ///<50 Mhz
}en_sd_clk_t;
/**
 ******************************************************************************
 ** \brief SD transaction cases (with or without data line)
 ******************************************************************************/

typedef enum en_sd_transaction
{
    NO_RESPONSE = 0,    ///<command without response
    NO_DATA ,           ///<no data in response or command
    SINGLE_BLOCK,       ///<read/write single block
    MULTI_BLOCK         ///<read/write multiple blocks
}en_sd_transaction_t;

/**
 ******************************************************************************
 ** \brief SD transfer direction
 ******************************************************************************/

typedef enum en_sd_dir
{
    SD_WRITE = 0,   ///< host to card 
    SD_READ = 1     ///< card to host 
}en_sd_dir_t;

/**
 ******************************************************************************
 ** \brief SD special command
 ******************************************************************************/

typedef enum en_sd_cmd_type
{
    NORMAL = 0,     ///< Non special command
    SUSPEND = 1,    ///< CMD52 for writing "Bus Suspend" in CCCR
    RESUME = 2,     ///< CMD52 for wirting "Function Select" in CCCR
    ABORT = 3       ///< CMD12,CMD52 for writing "I/O abort" in CCCR
}en_sd_scmd_type_t;
/**
 ******************************************************************************
 ** \brief SD card exist status
 ******************************************************************************/

typedef enum en_sd_existing
{
    DEBOUNCING = 0,     ///<debouncing  
    INSERTED = 1,       ///<sd card is inserted
    REMOVAL = 2         ///< sd card is removed
}en_sd_existing_t;

/**
 ******************************************************************************
 ** \brief SD bus width
 ******************************************************************************/
typedef enum en_sd_buswidth
{
    BIT_1 = 0,      ///< 1 bit 
    BIT_4 = 1       ///< 4 bits
}en_sd_buswidth_t;

/**
 ******************************************************************************
 ** \brief SD bus voltage level
 ******************************************************************************/
typedef enum en_sd_busvoltage
{
    V1_8 = 0x05,
    V3_0 = 0x06,    
    V3_3 = 0x07
}en_sd_busvoltage_t;


/**
 ******************************************************************************
 ** \brief SD response
 ******************************************************************************/
typedef enum en_sd_response
{
    NO_RSP = 0,
    R1NORMAL_R5_R6_R7 ,   //normal response     
    R1AUTO ,    
    R1B_NORMAL , 
    R1B_AUTO ,   
    R2,
    R3_R4,
    R5B       
}en_sd_response_t;

/**
 ******************************************************************************
 ** \brief SD auto command functions
 ******************************************************************************/
typedef enum en_sd_auto_cmd
{
    AUTO_DISABLE = 0,   //normal response
    AUTO_CMD12 = 1 ,    
    AUTO_CMD23  =2
}en_sd_auto_cmd_t;

typedef enum en_sd_boundary
{
    BOUND_4K = 0,
    BOUND_8K = 1,
    BOUND_16K = 2,
    BOUND_32K = 3,
    BOUND_64K = 4,
    BOUND_128K = 5,
    BOUND_256K = 6,
    BOUND_512K = 7    
}en_sd_boundary_t;
/**
 ******************************************************************************
 ** \brief SD command configure data
 ******************************************************************************/

typedef struct stc_sd_cmdconfig
{   
    en_sd_response_t enResponse;
    en_sd_dir_t enReadWrite;
    en_sd_scmd_type_t enCmdType;
    boolean_t bDataPresent; 
    uint8_t u8Index;
    en_sd_auto_cmd_t enAutoCmd;
    uint16_t u16BlockCount;
    uint16_t u16BlockSize;
    uint32_t u32Argument1;
#ifdef   SD_DMA  
    uint32_t u32SysAddr_Arg2;
    en_sd_boundary_t enbound;
#endif    
    uint32_t * pu32Buffer;
    func_ptr_sd_arg32_t  pfnErrorResponseCallback;
}stc_sd_comdconfig_t;
/**
 ******************************************************************************
 ** \brief SD configure data
 ******************************************************************************/

typedef struct stc_sd_config
{
    en_sd_existing_t enExist;
    en_sd_buswidth_t enBusWidth;
    boolean_t bCmdComplete;
    boolean_t bSendComplete;
    boolean_t bDMAComplete;
    func_ptr_sd_arg32_t  pfnTxCallback; 
    func_ptr_sd_arg32_t  pfnRxCallback;
    func_ptr_sd_arg32_t  pfnWakeupCallback;
    func_ptr_sd_arg32_t  pfnErrorCallback;
}stc_sd_config_t;


/******************************************************************************/
/* Global variable declarations ('extern', definition in C source)            */
/******************************************************************************/
extern volatile stc_sd_config_t Sd_stcCfg;
extern stc_sd_comdconfig_t  Sd_stcCmdCfg;
extern volatile stc_sd_t *Sd_stcCtrl;
/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/
/******************************************************************************/
/* SDIF basic control function                                                */
/******************************************************************************/
extern en_result_t  Sd_HostInit(void);
extern en_result_t  Sd_HostDeInit(void);
extern en_result_t Sd_SoftwareReset(volatile stc_sd_t* pstcSd,uint8_t u8reset);
extern en_sd_existing_t Sd_CardDetect(volatile stc_sd_config_t* pstcCfg);
extern en_result_t Sd_ClockSupply(volatile stc_sd_t* pstcSd,en_sd_clk_t enClk);
extern en_result_t Sd_ClockStop(volatile stc_sd_t *pstcSd);
extern boolean_t Sd_SendCmd(en_sd_transaction_t enTran,volatile stc_sd_t* pstcSd,stc_sd_comdconfig_t * pstcCmdCfg,uint32_t* pu32Buf);
/******************************************************************************/
/* SDIF interrupt function                                                    */
/******************************************************************************/
extern void Sd_Handler(void);
extern en_result_t Sd_EnableInt(volatile stc_sd_t *pstcSd);
extern en_result_t Sd_DisableInt(volatile stc_sd_t *pstcSd);
/******************************************************************************/
/* SDIF data transfer function                                                */
/******************************************************************************/
extern boolean_t Sd_TxData(volatile stc_sd_t* pstcSd,stc_sd_comdconfig_t * pstcCmdCfg,uint32_t* pu32Buf);
extern boolean_t Sd_RxData(en_sd_transaction_t enTran,volatile stc_sd_t* pstcSd,stc_sd_comdconfig_t * pstcCmdCfg,uint32_t* pu32Buf);

/***ToDo for second release

//power switch
//Error recovery
//Suspend/resume
//ADMA support
//MMC related

**///ToDo for second release

//@} //SdifGroup
#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_SD_ACTIVE))

#endif /* __SD_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
   
