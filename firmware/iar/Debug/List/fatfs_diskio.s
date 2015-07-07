///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:49 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\vari /
//                       ous\fatfs_bindings\fatfs_diskio.c                    /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\vari /
//                       ous\fatfs_bindings\fatfs_diskio.c -lCN               /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -lA F:\stuff\rusefi_sourceforge\firmware\iar\Debug\ /
//                       List\ -o F:\stuff\rusefi_sourceforge\firmware\iar\De /
//                       bug\Obj\ --no_cse --no_unroll --no_inline            /
//                       --no_code_motion --no_tbaa --no_clustering           /
//                       --no_scheduling --debug --endian=little              /
//                       --cpu=Cortex-M4 -e --fpu=VFPv4_sp --dlib_config      /
//                       "C:\Program Files (x86)\IAR Systems\Embedded         /
//                       Workbench 6.5\arm\INC\c\DLib_Config_Normal.h"        /
//                       --misrac2004=1.2-1.4,2.3,3.4,4.2,6.5-7,8.2-8.3,9.3,  /
//                       12.3,13.6-13,15.5,17.3,19.17,20.7-20.9,20.11 -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\kernel\include\ -I F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\..\chibios\os\ports\common\ARMCMx\ -I     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\common\ARMCMx\CMSIS\include\ -I             /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\IAR\ARMCMx\ -I F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\..\chibios\os\ports\IAR\ARMCMx\STM32F4x /
//                       x\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ch /
//                       ibios\os\hal\include\ -I F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\..\chibios\os\hal\platforms\STM32\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\DMAv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\GPIOv2\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\I2Cv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\SPIv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\TIMv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\RTCv2\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\USARTv1\ -I                   /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\OTGv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32F4xx\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\various\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\chibios\os\various\devices_lib\accel\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       boards\ST_STM32F4_DISCOVERY\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ /
//                       controllers\algo\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\controllers\core\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\system\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\controllers\sensors\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\math\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\controllers\trigger\ -I                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                        -I F:\stuff\rusefi_sourceforge\firmware\iar\..\conf /
//                       ig\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\c /
//                       onfig\engines\ -I F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\..\config\stm32f4ems\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                       binary\ -I F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       ..\console_util\ -I F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\..\development\ -I                          /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\developm /
//                       ent\test\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \algo\ -I F:\stuff\rusefi_sourceforge\firmware\iar\. /
//                       .\hw_layer\lcd\ -I F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\..\hw_layer\stm32f4\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \serial_over_usb\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\hw_layer\ -I                            /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext_algo /
//                       \ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\dev /
//                       elopment\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\util\    /
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       fatfs_diskio.s                                       /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME fatfs_diskio

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN MMCD1
        EXTERN RTCD1
        EXTERN mmcSequentialRead
        EXTERN mmcSequentialWrite
        EXTERN mmcStartSequentialRead
        EXTERN mmcStartSequentialWrite
        EXTERN mmcStopSequentialRead
        EXTERN mmcStopSequentialWrite
        EXTERN mmc_lld_is_write_protected
        EXTERN rtcGetTimeFat

        PUBLIC disk_initialize
        PUBLIC disk_ioctl
        PUBLIC disk_read
        PUBLIC disk_status
        PUBLIC disk_write
        PUBLIC get_fattime
        
          CFI Names cfiNames0
          CFI StackFrame CFA R13 DATA
          CFI Resource R0:32, R1:32, R2:32, R3:32, R4:32, R5:32, R6:32, R7:32
          CFI Resource R8:32, R9:32, R10:32, R11:32, R12:32, R13:32, R14:32
          CFI Resource D0:64, D1:64, D2:64, D3:64, D4:64, D5:64, D6:64, D7:64
          CFI Resource D8:64, D9:64, D10:64, D11:64, D12:64, D13:64, D14:64
          CFI Resource D15:64
          CFI EndNames cfiNames0
        
          CFI Common cfiCommon0 Using cfiNames0
          CFI CodeAlign 2
          CFI DataAlign 4
          CFI ReturnAddress R14 CODE
          CFI CFA R13+0
          CFI R0 Undefined
          CFI R1 Undefined
          CFI R2 Undefined
          CFI R3 Undefined
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI R8 SameValue
          CFI R9 SameValue
          CFI R10 SameValue
          CFI R11 SameValue
          CFI R12 Undefined
          CFI R14 SameValue
          CFI D0 Undefined
          CFI D1 Undefined
          CFI D2 Undefined
          CFI D3 Undefined
          CFI D4 Undefined
          CFI D5 Undefined
          CFI D6 Undefined
          CFI D7 Undefined
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI D12 SameValue
          CFI D13 SameValue
          CFI D14 SameValue
          CFI D15 SameValue
          CFI EndCommon cfiCommon0
        
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\various\fatfs_bindings\fatfs_diskio.c
//    1 /*-----------------------------------------------------------------------*/
//    2 /* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
//    3 /*-----------------------------------------------------------------------*/
//    4 /* This is a stub disk I/O module that acts as front end of the existing */
//    5 /* disk I/O modules and attach it to FatFs module with common interface. */
//    6 /*-----------------------------------------------------------------------*/
//    7 
//    8 #include "main.h"
//    9 #include "ffconf.h"
//   10 #include "diskio.h"
//   11 
//   12 #if EFI_FILE_LOGGING || defined(__DOXYGEN__)
//   13 
//   14 #if HAL_USE_MMC_SPI && HAL_USE_SDC
//   15 #error "cannot specify both MMC_SPI and SDC drivers"
//   16 #endif
//   17 
//   18 #if HAL_USE_MMC_SPI
//   19 extern MMCDriver MMCD1;
//   20 #elif HAL_USE_SDC
//   21 extern SDCDriver SDCD1;
//   22 #else
//   23 #error "MMC_SPI or SDC driver must be specified"
//   24 #endif
//   25 
//   26 #if HAL_USE_RTC
//   27 #include "chrtclib.h"
//   28 extern RTCDriver RTCD1;
//   29 #endif
//   30 
//   31 /*-----------------------------------------------------------------------*/
//   32 /* Correspondence between physical drive number and physical drive.      */
//   33 
//   34 #define MMC     0
//   35 #define SDC     0
//   36 
//   37 
//   38 
//   39 /*-----------------------------------------------------------------------*/
//   40 /* Inidialize a Drive                                                    */
//   41 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function disk_initialize
        THUMB
//   42 DSTATUS disk_initialize (
//   43     BYTE drv                /* Physical drive nmuber (0..) */
//   44 )
//   45 {
disk_initialize:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   46   DSTATUS stat;
//   47 
//   48   switch (drv) {
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BNE.N    ??disk_initialize_0
//   49 #if HAL_USE_MMC_SPI
//   50   case MMC:
//   51     stat = 0;
        MOVS     R0,#+0
        MOVS     R5,R0
//   52     /* It is initialized externally, just reads the status.*/
//   53     if (blkGetDriverState(&MMCD1) != BLK_READY)
        LDR.N    R0,??DataTable4
        LDRB     R0,[R0, #+4]
        CMP      R0,#+5
        BEQ.N    ??disk_initialize_1
//   54       stat |= STA_NOINIT;
        ORRS     R5,R5,#0x1
//   55     if (mmcIsWriteProtected(&MMCD1))
??disk_initialize_1:
        LDR.N    R0,??DataTable4
          CFI FunCall mmc_lld_is_write_protected
        BL       mmc_lld_is_write_protected
        CMP      R0,#+0
        BEQ.N    ??disk_initialize_2
//   56       stat |=  STA_PROTECT;
        ORRS     R5,R5,#0x4
//   57     return stat;
??disk_initialize_2:
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??disk_initialize_3
//   58 #else
//   59   case SDC:
//   60     stat = 0;
//   61     /* It is initialized externally, just reads the status.*/
//   62     if (blkGetDriverState(&SDCD1) != BLK_READY)
//   63       stat |= STA_NOINIT;
//   64     if (sdcIsWriteProtected(&SDCD1))
//   65       stat |=  STA_PROTECT;
//   66     return stat;
//   67 #endif
//   68   }
//   69   return STA_NODISK;
??disk_initialize_0:
        MOVS     R0,#+2
??disk_initialize_3:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock0
//   70 }
//   71 
//   72 
//   73 
//   74 /*-----------------------------------------------------------------------*/
//   75 /* Return Disk Status                                                    */
//   76 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function disk_status
        THUMB
//   77 DSTATUS disk_status (
//   78     BYTE drv        /* Physical drive nmuber (0..) */
//   79 )
//   80 {
disk_status:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
//   81   DSTATUS stat;
//   82 
//   83   switch (drv) {
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BNE.N    ??disk_status_0
//   84 #if HAL_USE_MMC_SPI
//   85   case MMC:
//   86     stat = 0;
        MOVS     R0,#+0
        MOVS     R5,R0
//   87     /* It is initialized externally, just reads the status.*/
//   88     if (blkGetDriverState(&MMCD1) != BLK_READY)
        LDR.N    R0,??DataTable4
        LDRB     R0,[R0, #+4]
        CMP      R0,#+5
        BEQ.N    ??disk_status_1
//   89       stat |= STA_NOINIT;
        ORRS     R5,R5,#0x1
//   90     if (mmcIsWriteProtected(&MMCD1))
??disk_status_1:
        LDR.N    R0,??DataTable4
          CFI FunCall mmc_lld_is_write_protected
        BL       mmc_lld_is_write_protected
        CMP      R0,#+0
        BEQ.N    ??disk_status_2
//   91       stat |= STA_PROTECT;
        ORRS     R5,R5,#0x4
//   92     return stat;
??disk_status_2:
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        B.N      ??disk_status_3
//   93 #else
//   94   case SDC:
//   95     stat = 0;
//   96     /* It is initialized externally, just reads the status.*/
//   97     if (blkGetDriverState(&SDCD1) != BLK_READY)
//   98       stat |= STA_NOINIT;
//   99     if (sdcIsWriteProtected(&SDCD1))
//  100       stat |= STA_PROTECT;
//  101     return stat;
//  102 #endif
//  103   }
//  104   return STA_NODISK;
??disk_status_0:
        MOVS     R0,#+2
??disk_status_3:
        POP      {R1,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//  105 }
//  106 
//  107 
//  108 
//  109 /*-----------------------------------------------------------------------*/
//  110 /* Read Sector(s)                                                        */
//  111 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock2 Using cfiCommon0
          CFI Function disk_read
        THUMB
//  112 DRESULT disk_read (
//  113     BYTE drv,        /* Physical drive nmuber (0..) */
//  114     BYTE *buff,        /* Data buffer to store read data */
//  115     DWORD sector,    /* Sector address (LBA) */
//  116     BYTE count        /* Number of sectors to read (1..255) */
//  117 )
//  118 {
disk_read:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  119   switch (drv) {
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BNE.N    ??disk_read_0
//  120 #if HAL_USE_MMC_SPI
//  121   case MMC:
//  122     if (blkGetDriverState(&MMCD1) != BLK_READY)
        LDR.N    R0,??DataTable4
        LDRB     R0,[R0, #+4]
        CMP      R0,#+5
        BEQ.N    ??disk_read_1
//  123       return RES_NOTRDY;
        MOVS     R0,#+3
        B.N      ??disk_read_2
//  124     if (mmcStartSequentialRead(&MMCD1, sector))
??disk_read_1:
        MOVS     R1,R6
        LDR.N    R0,??DataTable4
          CFI FunCall mmcStartSequentialRead
        BL       mmcStartSequentialRead
        CMP      R0,#+0
        BEQ.N    ??disk_read_3
//  125       return RES_ERROR;
        MOVS     R0,#+1
        B.N      ??disk_read_2
//  126     while (count > 0) {
??disk_read_3:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??disk_read_4
//  127       if (mmcSequentialRead(&MMCD1, buff))
        MOVS     R1,R5
        LDR.N    R0,??DataTable4
          CFI FunCall mmcSequentialRead
        BL       mmcSequentialRead
        CMP      R0,#+0
        BEQ.N    ??disk_read_5
//  128         return RES_ERROR;
        MOVS     R0,#+1
        B.N      ??disk_read_2
//  129       buff += MMCSD_BLOCK_SIZE;
??disk_read_5:
        ADDS     R5,R5,#+512
//  130       count--;
        SUBS     R7,R7,#+1
        B.N      ??disk_read_3
//  131     }
//  132     if (mmcStopSequentialRead(&MMCD1))
??disk_read_4:
        LDR.N    R0,??DataTable4
          CFI FunCall mmcStopSequentialRead
        BL       mmcStopSequentialRead
        CMP      R0,#+0
        BEQ.N    ??disk_read_6
//  133         return RES_ERROR;
        MOVS     R0,#+1
        B.N      ??disk_read_2
//  134     return RES_OK;
??disk_read_6:
        MOVS     R0,#+0
        B.N      ??disk_read_2
//  135 #else
//  136   case SDC:
//  137     if (blkGetDriverState(&SDCD1) != BLK_READY)
//  138       return RES_NOTRDY;
//  139     if (sdcRead(&SDCD1, sector, buff, count))
//  140       return RES_ERROR;
//  141     return RES_OK;
//  142 #endif
//  143   }
//  144   return RES_PARERR;
??disk_read_0:
        MOVS     R0,#+4
??disk_read_2:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock2
//  145 }
//  146 
//  147 
//  148 
//  149 /*-----------------------------------------------------------------------*/
//  150 /* Write Sector(s)                                                       */
//  151 
//  152 #if _READONLY == 0

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock3 Using cfiCommon0
          CFI Function disk_write
        THUMB
//  153 DRESULT disk_write (
//  154     BYTE drv,            /* Physical drive nmuber (0..) */
//  155     const BYTE *buff,    /* Data to be written */
//  156     DWORD sector,        /* Sector address (LBA) */
//  157     BYTE count            /* Number of sectors to write (1..255) */
//  158 )
//  159 {
disk_write:
        PUSH     {R3-R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI R7 Frame(CFA, -8)
          CFI R6 Frame(CFA, -12)
          CFI R5 Frame(CFA, -16)
          CFI R4 Frame(CFA, -20)
          CFI CFA R13+24
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,R2
        MOVS     R7,R3
//  160   switch (drv) {
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R4,#+0
        BNE.N    ??disk_write_0
//  161 #if HAL_USE_MMC_SPI
//  162   case MMC:
//  163     if (blkGetDriverState(&MMCD1) != BLK_READY)
        LDR.N    R0,??DataTable4
        LDRB     R0,[R0, #+4]
        CMP      R0,#+5
        BEQ.N    ??disk_write_1
//  164         return RES_NOTRDY;
        MOVS     R0,#+3
        B.N      ??disk_write_2
//  165     if (mmcIsWriteProtected(&MMCD1))
??disk_write_1:
        LDR.N    R0,??DataTable4
          CFI FunCall mmc_lld_is_write_protected
        BL       mmc_lld_is_write_protected
        CMP      R0,#+0
        BEQ.N    ??disk_write_3
//  166         return RES_WRPRT;
        MOVS     R0,#+2
        B.N      ??disk_write_2
//  167     if (mmcStartSequentialWrite(&MMCD1, sector))
??disk_write_3:
        MOVS     R1,R6
        LDR.N    R0,??DataTable4
          CFI FunCall mmcStartSequentialWrite
        BL       mmcStartSequentialWrite
        CMP      R0,#+0
        BEQ.N    ??disk_write_4
//  168         return RES_ERROR;
        MOVS     R0,#+1
        B.N      ??disk_write_2
//  169     while (count > 0) {
??disk_write_4:
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+0
        BEQ.N    ??disk_write_5
//  170         if (mmcSequentialWrite(&MMCD1, buff))
        MOVS     R1,R5
        LDR.N    R0,??DataTable4
          CFI FunCall mmcSequentialWrite
        BL       mmcSequentialWrite
        CMP      R0,#+0
        BEQ.N    ??disk_write_6
//  171             return RES_ERROR;
        MOVS     R0,#+1
        B.N      ??disk_write_2
//  172         buff += MMCSD_BLOCK_SIZE;
??disk_write_6:
        ADDS     R5,R5,#+512
//  173         count--;
        SUBS     R7,R7,#+1
        B.N      ??disk_write_4
//  174     }
//  175     if (mmcStopSequentialWrite(&MMCD1))
??disk_write_5:
        LDR.N    R0,??DataTable4
          CFI FunCall mmcStopSequentialWrite
        BL       mmcStopSequentialWrite
        CMP      R0,#+0
        BEQ.N    ??disk_write_7
//  176         return RES_ERROR;
        MOVS     R0,#+1
        B.N      ??disk_write_2
//  177     return RES_OK;
??disk_write_7:
        MOVS     R0,#+0
        B.N      ??disk_write_2
//  178 #else
//  179   case SDC:
//  180     if (blkGetDriverState(&SDCD1) != BLK_READY)
//  181       return RES_NOTRDY;
//  182     if (sdcWrite(&SDCD1, sector, buff, count))
//  183       return RES_ERROR;
//  184     return RES_OK;
//  185 #endif
//  186   }
//  187   return RES_PARERR;
??disk_write_0:
        MOVS     R0,#+4
??disk_write_2:
        POP      {R1,R4-R7,PC}    ;; return
          CFI EndBlock cfiBlock3
//  188 }
//  189 #endif /* _READONLY */
//  190 
//  191 
//  192 
//  193 /*-----------------------------------------------------------------------*/
//  194 /* Miscellaneous Functions                                               */
//  195 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock4 Using cfiCommon0
          CFI Function disk_ioctl
          CFI NoCalls
        THUMB
//  196 DRESULT disk_ioctl (
//  197     BYTE drv,        /* Physical drive nmuber (0..) */
//  198     BYTE ctrl,        /* Control code */
//  199     void *buff        /* Buffer to send/receive control data */
//  200 )
//  201 {
disk_ioctl:
        MOVS     R3,R0
//  202   switch (drv) {
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        CMP      R3,#+0
        BNE.N    ??disk_ioctl_0
//  203 #if HAL_USE_MMC_SPI
//  204   case MMC:
//  205     switch (ctrl) {
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,R1
        CMP      R0,#+0
        BEQ.N    ??disk_ioctl_1
        CMP      R0,#+2
        BEQ.N    ??disk_ioctl_2
        B.N      ??disk_ioctl_3
//  206     case CTRL_SYNC:
//  207         return RES_OK;
??disk_ioctl_1:
        MOVS     R0,#+0
        B.N      ??disk_ioctl_4
//  208     case GET_SECTOR_SIZE:
//  209         *((WORD *)buff) = MMCSD_BLOCK_SIZE;
??disk_ioctl_2:
        MOV      R0,#+512
        STRH     R0,[R2, #+0]
//  210         return RES_OK;
        MOVS     R0,#+0
        B.N      ??disk_ioctl_4
//  211 #if _USE_ERASE
//  212     case CTRL_ERASE_SECTOR:
//  213         mmcErase(&MMCD1, *((DWORD *)buff), *((DWORD *)buff + 1));
//  214         return RES_OK;
//  215 #endif
//  216     default:
//  217         return RES_PARERR;
??disk_ioctl_3:
        MOVS     R0,#+4
        B.N      ??disk_ioctl_4
//  218     }
//  219 #else
//  220   case SDC:
//  221     switch (ctrl) {
//  222     case CTRL_SYNC:
//  223         return RES_OK;
//  224     case GET_SECTOR_COUNT:
//  225         *((DWORD *)buff) = mmcsdGetCardCapacity(&SDCD1);
//  226         return RES_OK;
//  227     case GET_SECTOR_SIZE:
//  228         *((WORD *)buff) = MMCSD_BLOCK_SIZE;
//  229         return RES_OK;
//  230     case GET_BLOCK_SIZE:
//  231         *((DWORD *)buff) = 256; /* 512b blocks in one erase block */
//  232         return RES_OK;
//  233 #if _USE_ERASE
//  234     case CTRL_ERASE_SECTOR:
//  235         sdcErase(&SDCD1, *((DWORD *)buff), *((DWORD *)buff + 1));
//  236         return RES_OK;
//  237 #endif
//  238     default:
//  239         return RES_PARERR;
//  240     }
//  241 #endif
//  242   }
//  243   return RES_PARERR;
??disk_ioctl_0:
        MOVS     R0,#+4
??disk_ioctl_4:
        BX       LR               ;; return
          CFI EndBlock cfiBlock4
//  244 }
//  245 

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock5 Using cfiCommon0
          CFI Function get_fattime
        THUMB
//  246 DWORD get_fattime(void) {
get_fattime:
        PUSH     {R7,LR}
          CFI R14 Frame(CFA, -4)
          CFI CFA R13+8
//  247 #if HAL_USE_RTC
//  248     return rtcGetTimeFat(&RTCD1);
        LDR.N    R0,??DataTable4_1
          CFI FunCall rtcGetTimeFat
        BL       rtcGetTimeFat
        POP      {R1,PC}          ;; return
          CFI EndBlock cfiBlock5
//  249 #else
//  250     return ((uint32_t)0 | (1 << 16)) | (1 << 21); /* wrong but valid time */
//  251 #endif
//  252 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4:
        DC32     MMCD1

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable4_1:
        DC32     RTCD1

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  253 
//  254 #endif
//  255 
// 
// 368 bytes in section .text
// 
// 368 bytes of CODE memory
//
//Errors: none
//Warnings: none
