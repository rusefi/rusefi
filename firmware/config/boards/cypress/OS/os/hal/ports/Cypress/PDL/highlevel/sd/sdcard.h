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
/** \file sdcard.h
 **
 ** Headerfile for SD functions
 **  
 ** History:
 **   - 2013-05-16  1.0  QXu  First version.
 **
 ******************************************************************************/

#ifndef __SDCARD_H__
#define __SDCARD_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "sdcmd.h"

#if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
  
/**
 ******************************************************************************
 ** \defgroup SdifGroup SD Card Interface (SDIF)
 **
 ******************************************************************************/
//@{
  
/******************************************************************************
 * Global definitions
 ******************************************************************************/
#define E_SD_OK                    0
#define E_SD_NO_CARD		      -1
#define E_SD_INIT		          -2
#define E_SD_PARAMETER            -3
#define E_SD_READ                 -4
#define E_SD_PROTECT              -5
#define E_SD_WRITE                -6
#define E_SD_DEVICE_TYPE          -7
#define E_SD_ACTIVE               -8
		
#define E_SD_CMD_ERR              -11
#define E_SD_CMD_TIMEOUT          -13
#define E_SD_CMD_CRC_ERR          -14
#define E_SD_CMD_WR_ERR           -15
#define E_SD_CMD_IDLE             -16		
#define E_SD_CMD_CARD_ERR         -20

#define	BLOCK_SIZE                 512
#define	CMD8_WAIT                  (0x00001000)
//#define ARG_CMD8                 0xFF000AAF  //CRC7 --0x38 
#define ARG_CMD8                   0x000001AA
//#define ARG_ACMD41_HCS_OFF       0x00000000//lower version before 2.0
#define ARG_ACMD41_HCS_OFF         0x003C0000
#define ARG_ACMD41_HCS_ON          0x403C0000 //High speed support by higher version 2.0 or later
#define ARG_ACMD41_BUSY            0x80000000 //Card Busy status 

#define CSD_VERSION_MASK         3
#define CSD_STRUCT_OFFSET        14
#define OCR_CAPACITY_OFFSET      30 
#define OCR_CAPACITY_MASK        0x40000000
#define SD_VERSION_1_0           0   
#define SD_VERSION_2_0           1   //version 2.0 or later
#define SD_CAPACITY_STANDARD     0
#define SD_CAPACITY_HIGH         1

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
typedef struct stc_sdcard_type
{
	uint8_t		u8Capacity		: 4;
	uint8_t		u8Version		: 4;
} stc_sdcard_type_t;

// CSD Register ver1.0(for Standard Capacity Card)
typedef struct stc_csdver1
{
	uint8_t		u8Reserved1		: 6;
	uint8_t		u8Csd_structure	: 2;

	uint8_t		u8Taac			: 8;
	uint8_t		u8Nsac			: 8;
	uint8_t		u8Tran_speed		: 8;
	uint8_t		u8Ccc1			: 8;

	uint8_t		u8Read_bl_len		: 4;
	uint8_t		u8Ccc2			: 4;

	uint8_t		u8C_size1			: 2;
	uint8_t		u8Reserved2		: 2;
	uint8_t		u8Dsr_imp			: 1;
	uint8_t		u8Read_blk_misaligh	: 1;
	uint8_t		u8Write_blk_misalign	: 1;
	uint8_t		u8Read_bl_partial	: 1;

	uint8_t		u8C_size2			: 8;

	uint8_t		u8Vdd_r_curr_max	: 3;
	uint8_t		u8Vdd_r_curr_min	: 3;
	uint8_t		u8C_size3			: 2;

	uint8_t		u8C_size_multi1	: 2;
	uint8_t		u8Cdd_w_curr_max	: 3;
	uint8_t		u8Cdd_w_curr_min	: 3;

	uint8_t		u8Sector_size1	: 6;
	uint8_t		u8Erase_blk_en	: 1;
	uint8_t		u8C_size_multi2	: 1;

	uint8_t		u8Wp_grp_size		: 7;
	uint8_t		u8Sector_size2	: 1;

	uint8_t		u8Write_bl_len1	: 2;
	uint8_t		u8R2w_factor		: 3;
	uint8_t		u8Reserved3		: 2;
	uint8_t		u8Wp_grp_enable	: 1;

	uint8_t		u8Reserved4		: 5;
	uint8_t		u8Write_bl_partial	: 1;
	uint8_t		u8Write_bl_len2	: 2;

	uint8_t		u8Reserved5		: 2;
	uint8_t		u8File_format		: 2;
	uint8_t		u8Tmp_write_protect	: 1;
	uint8_t		u8Perm_write_protect	: 1;
	uint8_t		u8Copy			: 1;
	uint8_t		u8File_format_grp	: 1;

	uint8_t		u8No_use			: 1;
	uint8_t		u8Crc				: 7;
}stc_csdver1_t;

// CSD Register ver2.0(for High Capacity Card)
typedef struct stc_csdver2
{
	uint8_t		u8Reserved1		: 6;
	uint8_t		u8Csd_structure	: 2;

	uint8_t		u8Taac			: 8;
	uint8_t		u8Nsac			: 8;
	uint8_t		u8Tran_speed		: 8;
	uint8_t		u8Ccc1			: 8;

	uint8_t		u8Read_bl_len		: 4;
	uint8_t		u8Ccc2			: 4;

	uint8_t		u8Reserved2_U		: 4;
	uint8_t		u8Dsr_imp			: 1;
	uint8_t		u8Read_blk_misaligh	: 1;
	uint8_t		u8Write_blk_misalign	: 1;
	uint8_t		u8Read_bl_partial	: 1;
	
	uint8_t		u8C_size1			: 6;
	uint8_t		u8Reserved2_L		: 2;

	uint8_t		u8C_size2			: 8;
	uint8_t		u8C_size3			: 8;

	uint8_t		u8Sector_size1	: 6;
	uint8_t		u8Erase_blk_en	: 1;
	uint8_t		u8Reserved3		: 1;

	uint8_t		u8Wp_grp_size		: 7;
	uint8_t		u8Sector_size2	: 1;

	uint8_t		u8Write_bl_len1	: 2;
	uint8_t		u8R2w_factor		: 3;
	uint8_t		u8Reserved4		: 2;
	uint8_t		u8Wp_grp_enable	: 1;

	uint8_t		u8Reserved5		: 5;
	uint8_t		u8Write_bl_partial	: 1;
	uint8_t		u8Write_bl_len2	: 2;

	uint8_t		u8Reserved6		: 2;
	uint8_t		u8File_format		: 2;
	uint8_t		u8Tmp_write_protect	: 1;
	uint8_t		u8Perm_write_protect	: 1;
	uint8_t		u8Copy			: 1;
	uint8_t		u8File_format_grp	: 1;

	uint8_t		u8No_use			: 1;
	uint8_t		u8Crc				: 7;
}stc_csdver2_t;
/**
 ******************************************************************************
 ** \brief SD command configure data
 ******************************************************************************/
typedef struct stc_sdcard_info  
{
	uint32_t OCR   ;   ///<Operation Condition Register
	uint8_t CID[16];   ///<Card IDentification (CID) register  CID[0-4]-- CID( 127:8)
	uint8_t CSD[16];   ///<Card-Specific Data register CSD  CSD[0-4]--CSD (127:8)
	uint16_t RCA   ;   ///<Relative Card Address
	stc_sdcard_type_t  stcCardType; ///<
	uint32_t u32MaxSectorNum; ///<maximum sector number of the SD card
}stc_sdcard_info_t; 

/******************************************************************************/
/* Global variable declarations ('extern', definition in C source)            */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/
extern int32_t Sdcard_Init(stc_sdcard_info_t *pstcSdcardInfo);
extern int32_t Sdcard_ReadSector(stc_sdcard_info_t *pstcSdcardInfo, uint32_t u32start, uint16_t u16count, uint8_t *pu8buf);
extern int32_t Sdcard_WriteSector(stc_sdcard_info_t *pstcSdcardInfo, uint32_t u32start, uint16_t u16count, const uint8_t *pu8buf);
extern boolean_t  Sdcard_Detect(void);

/***ToDo for second release

**///ToDo for second release

//@} SdifGroup
#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_SD_ACTIVE))

#endif /* __SDCARD_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

