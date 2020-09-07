/*******************************************************************************
* \file             extif.h
* 
* \version          1.50
*                       
* \brief            Headerfile for External Bus Interface functions
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

#ifndef __EXTIF_H__
#define __EXTIF_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl_user.h"
#include "mcu.h"

#if (defined(PDL_PERIPHERAL_EXTIF_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupEXTIF External Bus Interface (EXTIF)
* \{
* \defgroup GroupEXTIF_Macros Macros
* \defgroup GroupEXTIF_Functions Functions
* \defgroup GroupEXTIF_DataStructures Data Structures
* \defgroup GroupEXTIF_Types Enumerated Types 
* \}
*/
/**
* \addtogroup GroupEXTIF
* \{
* The External Bus Interface (EXTIF) enables connections with SRAM, Flash memory, and 
* SDRAM outside of the device.<br>
* The features of the external bus interface across the products are as follows:<br>
* - Supports 8-bit/16-bit wide SRAM, SDRAM as well as NOR and NAND Flash memories<br>
* - Up to 8 chip select signals are available - one chip select signal is provided 
* to SDRAM exclusively<br>
* - Address and access timing parameters can be separately set for each chip select signal<br>
* - Up to 25 bits address can be output<br>
* - Supports NOR Flash memory page read<br>
* - Byte lane is fixed to little endian<br>
* - Automatically convers bus size when the access width from CPU and the external 
* bus width are different<br>
* - Separate mode and multiplex mode are supported for bus access
* ( NAND flash memory, SDRAM and the page read of NOR flash memory do not support multiplex mode)<br>
* - The access timing parameter of ALE signal is added to support the multiplex mode. In addition, 
* more detailed parameter settings, such as CS assert timing, are possible.<br>
* - Clock output feature allows synchronous accesses with target devices<br>
* - Supports external RDY feature<br>
* - Supports SDRAM power down mode<br>
* - Operates with a division clock output of the base clock (HCLK)<br>
* - When MCLKOUT and MSDCLK are to be output from this LSI, it is necessary to 
* configure a division ratio that satisfies the output standard described on the data sheet.<br>
* \section SectionEXTIF_ConfigurationConsideration Configuration Consideration
* To set up the EXTIF, relocate the GPIO pin function to the external bus pins. 
* Use the SetPinFunc macros provided by the headerfile file common/gpio_<device>.h 
* for the correct pin relocation. <br>
* <b>Example</b><br>
* SetPinFunc_MADATA00_0();<br>
* SetPinFunc_MADATA01_0();<br>
* SetPinFunc_MADATA02_0();<br>
* ...<br>
* SetPinFunc_MNCLE_0();<br>
* SetPinFunc_MNALE_0();<br>
* SetPinFunc_MNWEX_0();<br>
* To setup a memory area use the function Extif_InitArea() and pass the chip 
* select number and configuration structure. The configuration structure depends on the 
* memory device and is described below.<br>
* For error handling the functions  Extif_ReadErrorStatus(), Extif_ReadErrorAddress() 
* and Extif_ClearErrorStatus() are provided.<br>
*                  | Clock output | External RDY | Page mode   | NAND Flash  | Clock division 
* -----------------|--------------|--------------|-------------|-------------|---------------
* SRAM             | Available    | Available    | Not Allowed | Not Allowed | Available
* NOR Flash memory | Available    | Not Allowed  | Available   | Not Allowed | Available
* NAND Flash memory| Not Allowed  | Not Allowed  | Not Allowed | Available   | Available
* SDRAM            | Available    | Not allowed  | Not allowed | Not allowed | Available
*
* <b>SRAM and NOR Flash:</b><br>
* Use stc_extif_area_config_t  to configure XTIF for SRAM and NOR Flash. 
* The structure includes all settings for the configuration of the interface, 
* for example the data width, the address area, bus speed, refresh timings and refresh cycles.<br>
* For accessing the SRAM and NOR Flash memory, the same signals are used.<br>
* <b> NAND Flash </b><br>
* Use stc_extif_area_config_t  to configure XTIF for NAND Flash. Use these macros to access NAND Flash:<br>
* * Extif_Nand_ReadData – Used to read data from the NAND Flash memory<br>
* * Extif_Nand_WriteData - Used to write data to the NAND Flash memory<br>
* * Extif_Nand_SetAddress – Used to set the address <br>
* * Extif_Nand_SetCommand - issue a command for the NAND Flash memory (MNCLE is asserted)<br>
* * Extif_Nand_ClearAle – de-assert the MNALE signal between multiple write accesses.<br>
* <b>SDRAM</b><br>
* Use both stc_extif_area_config_t and stc_extif_sdram_config_t to configure XTIF for SDRAM. 
* These structures include all settings for the configuration of the interface, for example: 
* the data width, the address area, bus speed, refresh timings and refresh cycles.<br>
* Use Extif_SetSdramCommand() to write SDRAM commands. That function calls Extif_CheckSdcmdReady() 
* to determine whether the SDRAM command register is available, and returns an error if writing is not possible.<br>
* SDRAM is supported only for chip select 8.<br>
* <b>Notes:<b><br>
* * There are already some drivers for specific devices available in the 
* utilities folder of the PDL. These drivers explain setting up a new memory device.<br>
* * When an access is made to an external bus area of 256MB SRAM/Flash memory address area, 
* an area which has not been mapped with the area register, or SDRAM address area with the 
* setting of SDRAM mode register of SDON=0, the external bus interface outputs an error 
* response (by setting HRESP[1:0] to "01"). When this error occurs during a burst transfer, 
* the operation of the external bus interface is not guaranteed.<br>
* * When the setup value of registers such as the timing register is rewritten from 
* the CPU while accessing the external bus from the DMAC, written values will not be 
* reflected until the access is completed (after the idle cycle).<br>
* * When using the external bus interface, set EXBRST bit of peripheral reset control 
* register 0 (MRST0) become 1. If using external bus interface without executing 
* external bus interface reset, there is a possibility that the device become runaway, 
* because the access request to external bus interface cannot be accepted.
* 
* \section SectionEXTIF_MoreInfo More Information
* For more information on the EXTIF peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual Core Subsystem</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupEXTIF_Macros
* \{
*/  
  
/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/ 
#define EXTIF          (*((stc_extif_arrays_t *) FM_EXBUS_BASE))
   
/******************************************************************************
 * NAND Flash access macros (intern)
 ******************************************************************************/
#define EXITIF_NAND_ALE_OFFSET   0x00003000ul  ///< Address offset for MNALE assert until write
#define EXITIF_NAND_ADDR_OFFSET  0x00002000ul  ///< Address offset for address cycle (+ MNALE)
#define EXITIF_NAND_CMD_OFFSET   0x00001000ul  ///< Address offset for command cycle (+ MNCLE)
#define EXITIF_NAND_DATA_OFFSET  0x00000000ul  ///< Address offset for data read/write
  
/******************************************************************************
 * NAND Flash access macros (user)
 ******************************************************************************/
#define Extif_Nand_SetCommand(base, cmd)  {*(volatile unsigned char*)((base) + EXITIF_NAND_CMD_OFFSET)  = (unsigned char)(cmd);}  ///< Sets a NAND command cycle
#define Extif_Nand_SetAddress(base, addr) {*(volatile unsigned char*)((base) + EXITIF_NAND_ADDR_OFFSET) = (unsigned char)(addr);} ///< Sets a NAND address cycle
#define Extif_Nand_ReadData(base)         (*(volatile unsigned char*)((base) + EXITIF_NAND_DATA_OFFSET))                          ///< Reads NAND data
#define Extif_Nand_WriteData(base, data)  {*(volatile unsigned char*)((base) + EXITIF_NAND_DATA_OFFSET) = (unsigned char)(data);} ///< Writes NAND data
#define Extif_Nand_ClearAle(base)         {*(volatile unsigned char*)((base) + EXITIF_NAND_ALE_OFFSET)  = (unsigned char)0;}      ///< De-asserts MNALE signal
 
/** \} GroupEXTIF_Macros */

/**
* \addtogroup GroupEXTIF_Types
* \{
*/ 
/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Extif data width
 ** 
 ** To select the data bus width
 ******************************************************************************/
typedef enum en_extif_width
{
    Extif8Bit   = 0u,  ///< 8 Bit mode
    Extif16Bit  = 1u,  ///< 16 Bit  mode
#if (PDL_MCU_TYPE == PDL_FM4_TYPE3)    
    Extif32Bit  = 2u,  ///< 32 Bit  mode
#endif    
} en_extif_width_t;

/**
 ******************************************************************************
 ** \brief Extif cycle defintion
 ** 
 ** To select the bus mode of the interface.
 **
 ** \note Do not change enumeration number. The numbers are taken for
 **       calculating the corresponding bitfield!
 ******************************************************************************/
typedef enum en_extif_cycle
{ 
    Extif0Cycle   = 0u,  ///<  0 cycles
    Extif1Cycle   = 1u,  ///<  1 cycle
    Extif2Cycle   = 2u,  ///<  2 cycles
    Extif3Cycle   = 3u,  ///<  3 cycles
    Extif4Cycle   = 4u,  ///<  4 cycles
    Extif5Cycle   = 5u,  ///<  5 cycles
    Extif6Cycle   = 6u,  ///<  6 cycles
    Extif7Cycle   = 7u,  ///<  7 cycles
    Extif8Cycle   = 8u,  ///<  8 cycles
    Extif9Cycle   = 9u,  ///<  9 cycles
    Extif10Cycle  = 10u, ///< 10 cycles
    Extif11Cycle  = 11u, ///< 11 cycles
    Extif12Cycle  = 12u, ///< 12 cycles
    Extif13Cycle  = 13u, ///< 13 cycles
    Extif14Cycle  = 14u, ///< 14 cycles
    Extif15Cycle  = 15u, ///< 15 cycles
    Extif16Cycle  = 16u, ///< 16 cycles
    ExtifDisabled = 17u  ///< Setting disabled
} en_extif_cycle_t;

/**
 ******************************************************************************
 ** \brief SDRAM cycle defintion
 ** 
 ** To select the bus mode of the interface.
 **
 ** \note Do not change enumeration number. The numbers are taken for
 **       calculating the corresponding bitfield!
 ******************************************************************************/
typedef enum en_sdram_cycle
{ 
    Sdram1Cycle   = 0u,  ///<  1 cycle
    Sdram2Cycle   = 1u,  ///<  2 cycles
    Sdram3Cycle   = 2u,  ///<  3 cycles
    Sdram4Cycle   = 3u,  ///<  4 cycles
    Sdram5Cycle   = 4u,  ///<  5 cycles
    Sdram6Cycle   = 5u,  ///<  6 cycles
    Sdram7Cycle   = 6u,  ///<  7 cycles
    Sdram8Cycle   = 7u,  ///<  8 cycles
    Sdram9Cycle   = 8u,  ///<  9 cycles
    Sdram10Cycle  = 9u, ///< 10 cycles
    Sdram11Cycle  = 10u, ///< 11 cycles
    Sdram12Cycle  = 11u, ///< 12 cycles
    Sdram13Cycle  = 12u, ///< 13 cycles
    Sdram14Cycle  = 13u, ///< 14 cycles
    Sdram15Cycle  = 14u, ///< 15 cycles
    Sdram16Cycle  = 15u, ///< 16 cycles
    SdramDisabled = 16u  ///< Setting disabled
} en_sdram_cycle_t;

/**
 ******************************************************************************
 ** \brief Extif mask setup (area size)
 ** 
 ** To select the Mask Setup value for the address area per chip select.
 **
 ** \note Do not change enumeration number. The numbers are taken for
 **       calculating the corresponding bitfield!
 ******************************************************************************/
typedef enum en_extif_mask
{
    Extif1MB   = 0u,  ///< Area 1 MByte 
    Extif2MB   = 1u,  ///< Area 2 MByte 
    Extif4MB   = 2u,  ///< Area 4 MByte 
    Extif8MB   = 3u,  ///< Area 8 MByte 
    Extif16MB  = 4u,  ///< Area 16 MByte 
    Extif32MB  = 5u,  ///< Area 32 MByte 
    Extif64MB  = 6u,  ///< Area 64 MByte 
    Extif128MB = 7u   ///< Area 128 MByte 
} en_extif_mask_t;

/**
 ******************************************************************************
 ** \brief Extif CAS address select
 ** 
 ** Select the address for the Column Address Select
 **
 ** \note Do not change enumeration number. The numbers are taken for
 **       calculating the corresponding bitfield!
 ******************************************************************************/
typedef enum en_extif_cas
{
    ExtifCas16Bit = 0u,  ///< MAD[9:0] = Internal address [10:1], 16-Bit width
    ExtifCas32Bit = 1u   ///< MAD[9:0] = Internal address [11:2], 32-Bit width  
} en_extif_cas_t;

/**
 ******************************************************************************
 ** \brief Extif RAS address select
 ** 
 ** Select the address for the Row Address Select
 **
 ** \note Do not change enumeration number. The numbers are taken for
 **       calculating the corresponding bitfield!
 ******************************************************************************/
typedef enum en_extif_ras
{
    ExtifRas_19_6  = 0u,   ///< MAD[13:0] = Internal address [19:6]
    ExtifRas_20_7  = 1u,   ///< MAD[13:0] = Internal address [20:7]
    ExtifRas_21_8  = 2u,   ///< MAD[13:0] = Internal address [21:8]
    ExtifRas_22_9  = 3u,   ///< MAD[13:0] = Internal address [22:9]
    ExtifRas_23_10 = 4u,   ///< MAD[13:0] = Internal address [23:10]
    ExtifRas_24_11 = 5u,   ///< MAD[13:0] = Internal address [24:11]
    ExtifRas_25_12 = 6u,   ///< MAD[13:0] = Internal address [25:12]
} en_extif_ras_t;

/**
 ******************************************************************************
 ** \brief Extif BAS address select
 ** 
 ** Select the address for the Bank Address Select
 **
 ** \note Do not change enumeration number. The numbers are taken for
 **       calculating the corresponding bitfield!
 ******************************************************************************/
typedef enum en_extif_bas
{
    ExtifBas_20_19 = 0u,   ///< MAD[13:0] = Internal address [20:19]
    ExtifBas_21_20 = 1u,   ///< MAD[13:0] = Internal address [21:20]
    ExtifBas_22_21 = 2u,   ///< MAD[13:0] = Internal address [22:21]
    ExtifBas_23_22 = 3u,   ///< MAD[13:0] = Internal address [23:22]
    ExtifBas_24_23 = 4u,   ///< MAD[13:0] = Internal address [24:23]
    ExtifBas_25_24 = 5u,   ///< MAD[13:0] = Internal address [25:24]
    ExtifBas_26_25 = 6u,   ///< MAD[13:0] = Internal address [26:25]
} en_extif_bas_t;

/** \}GroupEXTIF_Types */

/**
* \addtogroup GroupEXTIF_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Definition of MODE, TIM, AREA, ATIM register arrays
 ** 
 ** To select the data bus width
 ******************************************************************************/
typedef struct stc_extif_arrays
{
    union {
        uint32_t                au32MODE[8];
        stc_exbus_mode0_field_t astcMODE[8];
    };  
    union {
        uint32_t                au32TIM[8];
        stc_exbus_tim0_field_t  astcTIM[8];
    };   
    union {
        uint32_t                au32AREA[8];
        stc_exbus_area0_field_t astcAREA[8];
    };   
    union {
        uint32_t                au32ATIM[8];
        stc_exbus_atim0_field_t astcATIM[8];
    };
} stc_extif_arrays_t;


typedef struct stc_extif_sdram_config
{
    boolean_t         bSdramEnable;              ///< TRUE: Enables SDRAM functionality (only possible for area 8)
    boolean_t         bSdramPowerDownMode;       ///< TRUE: Enables SDRAM Power Down Mode (only possible for area 8)
    boolean_t         bSdramRefreshOff;          ///< TRUE: Disables Refresh Function (only possible for area 8)
    en_extif_cas_t    enCasel;                   ///< Column Address Select, see #en_extif_cas_t for details (only possible for area 8)
    en_extif_ras_t    enRasel;                   ///< Row Address Select, see #en_extif_ras_t for details (only possible for area 8)
    en_extif_bas_t    enBasel;                   ///< Bank Address Select, see #en_extif_bas_t for details (only possible for area 8)
    uint16_t          u16RefreshCount;           ///< Refresh Count in Cycles (only possible for area 8)
    uint8_t           u8RefreshNumber;           ///< Number of Refreshs (only possible for area 8)
    boolean_t         bPreRefreshEnable;         ///< TRUE: Enables Refresh (only possible for area 8)
    uint16_t          u16PowerDownCount;         ///< Power Down Count in Cycles (only possible for area 8)
    en_sdram_cycle_t  enSdramCasLatencyCycle;    ///< Latency of CAS in Cycles (only possible for area 8)
    en_sdram_cycle_t  enSdramRasCycle;           ///< RAS Cycles (only possible for area 8)
    en_sdram_cycle_t  enSdramRasPrechargeCycle;  ///< RAS Precharge Cycles (only possible for area 8)
    en_sdram_cycle_t  enSdramRasCasDelayCycle;   ///< RAS CAS Delay Cycles (only possible for area 8)
    en_sdram_cycle_t  enSdramRasActiveCycle;     ///< RAS Active Cycles (only possible for area 8)
    en_sdram_cycle_t  enSdramRefreshCycle;       ///< Refresh Cycles (only possible for area 8)
    en_sdram_cycle_t  enSdramPrechargeCycle;     ///< Data-in to Precharge Lead Time in Cycles (only possible for area 8)
    boolean_t         bSdramErrorInterruptEnable; ///< TRUE: Enables SDRAM error interrupt (only possible for area 8)
    func_ptr_t        pfnSdramErrorCallback;     ///< Pointer to SDRAM Error Callback Function (only possible for area 8)
   
}stc_extif_sdram_config_t;

/**
 *****************************************************************************
 ** \brief Extif area configuration
 ** 
 ** The WDG configuration is done on a per area (Chip select) basis
 *****************************************************************************/
typedef struct stc_extif_area_config
{
    en_extif_width_t  enWidth;                   ///< 8, 16 bit data bus width. See description of #en_extif_width_t
    boolean_t         bReadByteMask;             ///< TRUE: Read Byte Mask enable
    boolean_t         bWriteEnableOff;           ///< TRUE: Write enable disabled
    boolean_t         bNandFlash;                ///< TRUE: NAND Flash bus enable, FLASE: NOR Flash/SRAM bus enable
    boolean_t         bPageAccess;               ///< TRUE: NOR Flash memory page access mode enabled
#if (PDL_MCU_TYPE != PDL_FM3_TYPE0)    
    boolean_t         bRdyOn;                    ///< TRUE: RDY mode enabled
    boolean_t         bStopDataOutAtFirstIdle;   ///< TRUE: Stop to write data output at first idle cycle, FALSE: Extends to write data output to the last idle cycle
    boolean_t         bMultiplexMode;            ///< TRUE: Multiplex mode
    boolean_t         bAleInvert;                ///< TRUE: Invert ALE signal (negative polarity)
    boolean_t         bAddrOnDataLinesOff;       ///< TRUE: Do not output address to data lines (Hi-Z during ALC cycle period)
    boolean_t         bMpxcsOff;                 ///< TRUE: Do not assert MCSX in ALC cycle period
    boolean_t         bMoexWidthAsFradc;         ///< TRUE: MOEX width is set with FRADC, FALSE: MOEX width is set with RACC-RADC
    boolean_t         bMclkoutEnable;            ///< TRUE: Enables MCLKOUT pin
    uint8_t           u8MclkDivision;            ///< Division ratio for MCLK (1 ... 16 div)
#endif    
    en_extif_cycle_t  enReadAccessCycle;         ///< Read Access Cycle timing
    en_extif_cycle_t  enReadAddressSetupCycle;   ///< Read Address Setup Cycle timing
    en_extif_cycle_t  enFirstReadAddressCycle;   ///< First Read Address Cycle timing
    en_extif_cycle_t  enReadIdleCycle;           ///< Read Idle Cycle timing
    en_extif_cycle_t  enWriteAccessCycle;        ///< Write Access Cycle timing
    en_extif_cycle_t  enWriteAddressSetupCycle;  ///< Write Address Setup Cycle timing
    en_extif_cycle_t  enWriteEnableCycle;        ///< Write Enable Cycle timing
    en_extif_cycle_t  enWriteIdleCycle;          ///< Write Idle Cycle timing
    uint8_t           u8AreaAddress;             ///< Address bits [27:20]
    en_extif_mask_t   enAreaMask;                ///< See description of #en_extif_mask_t
    en_extif_cycle_t  enAddressLatchCycle;       ///< Address Latch Cycles
    en_extif_cycle_t  enAddressLatchSetupCycle;  ///< Address Latch Enable Setup Cycles
    en_extif_cycle_t  enAddressLatchWidthCycle;  ///< Address Latch Enable Width Cycles
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    boolean_t         bPrecedReadContinuousWrite; ///< TRUE: Enables preceding read and continuous write request
    stc_extif_sdram_config_t* pExtifSdramConfig;  ///< Pointer to the configuration of SDRAM I/F of external bus interface 
#if (PDL_MCU_TYPE == PDL_FM4_TYPE4)    
    stc_extif_sdram_config_t* pGdcSdramConfig;    ///< Pointer to the configuration of SDRAM I/F of GDC 
#endif    
    boolean_t         bSramFlashErrorInterruptEnable; ///< TRUE: Enables SRAM/Flash Error Interrupt 
    func_ptr_t        pfnSramFlashErrorCallback; ///< Pointer to SRAM/Flash Error Callback Function
    
#endif     
} stc_extif_area_config_t;
  
/// EXTIF Error Interrupt Callback Pointers
typedef struct stc_extif_intern_data
{
  boolean_t         bSdramErrorInterruptEnable;        ///< TRUE: Enables SDRAM error interrupt
  boolean_t         bSramFlashErrorInterruptEnable;    ///< TRUE: Enables SRAM/Flash Error Interrupt
#if (PDL_MCU_TYPE == PDL_FM4_TYPE4)    
  boolean_t         bGdcSdramErrorInterruptEnable;     ///< TRUE: Enables GDC SDRAM Error Interrupt
#endif  
  func_ptr_t        pfnSdramErrorCallback;          ///< Pointer to SDRAM Error Callback Function
  func_ptr_t        pfnSramFlashErrorCallback;      ///< Pointer to SRAM/Flash Error Callback Function
#if (PDL_MCU_TYPE == PDL_FM4_TYPE4)      
  func_ptr_t        pfnGdcSdramErrorCallback;       ///< Pointer to GDC SDRAM Error Callback Function
#endif  
} stc_extif_intern_data_t;

/** \} GroupEXTIF_DataStructures */

/**
* \addtogroup GroupEXTIF_Functions
* \{
*/ 

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/

void        Extif_IrqHandler( void );

en_result_t Extif_InitArea( uint8_t                  u8Area,
                            const stc_extif_area_config_t* pstcConfig );

en_result_t Extif_ReadErrorStatus( void );

uint32_t    Extif_ReadErrorAddress( void );

en_result_t Extif_ClearErrorStatus( void );

en_result_t Extif_CheckSdcmdReady( void );

en_result_t Extif_SetSdramCommand( uint16_t  u16Address,
                                   boolean_t bMsdwex,
                                   boolean_t bMcasx,
                                   boolean_t bMrasx,
                                   boolean_t bMcsx8,
                                   boolean_t bMadcke
                                 );

/** \} GroupEXTIF_Functions */
/** \} GroupEXTIF */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_EXTIF_ACTIVE))

#endif /* __EXTIF_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
