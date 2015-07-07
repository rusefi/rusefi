///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:09:36 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\sdc.c                                            /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\ /
//                       src\sdc.c -lCN F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\Debug\List\ -lA F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\Debug\List\ -o F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\Obj\ --no_cse --no_unroll         /
//                       --no_inline --no_code_motion --no_tbaa               /
//                       --no_clustering --no_scheduling --debug              /
//                       --endian=little --cpu=Cortex-M4 -e --fpu=VFPv4_sp    /
//                       --dlib_config "C:\Program Files (x86)\IAR            /
//                       Systems\Embedded Workbench                           /
//                       6.5\arm\INC\c\DLib_Config_Normal.h"                  /
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
//                       sdc.s                                                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME sdc

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1


        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
// F:\stuff\rusefi_sourceforge\firmware\chibios\os\hal\src\sdc.c
//    1 /*
//    2     ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
//    3                  2011,2012,2013 Giovanni Di Sirio.
//    4 
//    5     This file is part of ChibiOS/RT.
//    6 
//    7     ChibiOS/RT is free software; you can redistribute it and/or modify
//    8     it under the terms of the GNU General Public License as published by
//    9     the Free Software Foundation; either version 3 of the License, or
//   10     (at your option) any later version.
//   11 
//   12     ChibiOS/RT is distributed in the hope that it will be useful,
//   13     but WITHOUT ANY WARRANTY; without even the implied warranty of
//   14     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   15     GNU General Public License for more details.
//   16 
//   17     You should have received a copy of the GNU General Public License
//   18     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//   19 
//   20                                       ---
//   21 
//   22     A special exception to the GPL can be applied should you wish to distribute
//   23     a combined work that includes ChibiOS/RT, without being obliged to provide
//   24     the source code for any proprietary components. See the file exception.txt
//   25     for full details of how and when the exception can be applied.
//   26 */
//   27 
//   28 /**
//   29  * @file    sdc.c
//   30  * @brief   SDC Driver code.
//   31  *
//   32  * @addtogroup SDC
//   33  * @{
//   34  */
//   35 
//   36 #include "ch.h"
//   37 #include "hal.h"
//   38 
//   39 #if HAL_USE_SDC || defined(__DOXYGEN__)
//   40 
//   41 /*===========================================================================*/
//   42 /* Driver local definitions.                                                 */
//   43 /*===========================================================================*/
//   44 
//   45 /*===========================================================================*/
//   46 /* Driver exported variables.                                                */
//   47 /*===========================================================================*/
//   48 
//   49 /*===========================================================================*/
//   50 /* Driver local variables and types.                                         */
//   51 /*===========================================================================*/
//   52 
//   53 /**
//   54  * @brief   Virtual methods table.
//   55  */
//   56 static const struct SDCDriverVMT sdc_vmt = {
//   57   (bool_t (*)(void *))sdc_lld_is_card_inserted,
//   58   (bool_t (*)(void *))sdc_lld_is_write_protected,
//   59   (bool_t (*)(void *))sdcConnect,
//   60   (bool_t (*)(void *))sdcDisconnect,
//   61   (bool_t (*)(void *, uint32_t, uint8_t *, uint32_t))sdcRead,
//   62   (bool_t (*)(void *, uint32_t, const uint8_t *, uint32_t))sdcWrite,
//   63   (bool_t (*)(void *))sdcSync,
//   64   (bool_t (*)(void *, BlockDeviceInfo *))sdcGetInfo
//   65 };
//   66 
//   67 /*===========================================================================*/
//   68 /* Driver local functions.                                                   */
//   69 /*===========================================================================*/
//   70 
//   71 /**
//   72  * @brief   Wait for the card to complete pending operations.
//   73  *
//   74  * @param[in] sdcp      pointer to the @p SDCDriver object
//   75  *
//   76  * @return              The operation status.
//   77  * @retval CH_SUCCESS   operation succeeded.
//   78  * @retval CH_FAILED    operation failed.
//   79  *
//   80  * @notapi
//   81  */
//   82 bool_t _sdc_wait_for_transfer_state(SDCDriver *sdcp) {
//   83   uint32_t resp[1];
//   84 
//   85   while (TRUE) {
//   86     if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEND_STATUS,
//   87                                    sdcp->rca, resp) ||
//   88         MMCSD_R1_ERROR(resp[0]))
//   89       return CH_FAILED;
//   90     switch (MMCSD_R1_STS(resp[0])) {
//   91     case MMCSD_STS_TRAN:
//   92       return CH_SUCCESS;
//   93     case MMCSD_STS_DATA:
//   94     case MMCSD_STS_RCV:
//   95     case MMCSD_STS_PRG:
//   96 #if SDC_NICE_WAITING
//   97       chThdSleepMilliseconds(1);
//   98 #endif
//   99       continue;
//  100     default:
//  101       /* The card should have been initialized so any other state is not
//  102          valid and is reported as an error.*/
//  103       return CH_FAILED;
//  104     }
//  105   }
//  106   /* If something going too wrong.*/
//  107   return CH_FAILED;
//  108 }
//  109 
//  110 /*===========================================================================*/
//  111 /* Driver exported functions.                                                */
//  112 /*===========================================================================*/
//  113 
//  114 /**
//  115  * @brief   SDC Driver initialization.
//  116  * @note    This function is implicitly invoked by @p halInit(), there is
//  117  *          no need to explicitly initialize the driver.
//  118  *
//  119  * @init
//  120  */
//  121 void sdcInit(void) {
//  122 
//  123   sdc_lld_init();
//  124 }
//  125 
//  126 /**
//  127  * @brief   Initializes the standard part of a @p SDCDriver structure.
//  128  *
//  129  * @param[out] sdcp     pointer to the @p SDCDriver object
//  130  *
//  131  * @init
//  132  */
//  133 void sdcObjectInit(SDCDriver *sdcp) {
//  134 
//  135   sdcp->vmt      = &sdc_vmt;
//  136   sdcp->state    = BLK_STOP;
//  137   sdcp->errors   = SDC_NO_ERROR;
//  138   sdcp->config   = NULL;
//  139   sdcp->capacity = 0;
//  140 }
//  141 
//  142 /**
//  143  * @brief   Configures and activates the SDC peripheral.
//  144  *
//  145  * @param[in] sdcp      pointer to the @p SDCDriver object
//  146  * @param[in] config    pointer to the @p SDCConfig object, can be @p NULL if
//  147  *                      the driver supports a default configuration or
//  148  *                      requires no configuration
//  149  *
//  150  * @api
//  151  */
//  152 void sdcStart(SDCDriver *sdcp, const SDCConfig *config) {
//  153 
//  154   chDbgCheck(sdcp != NULL, "sdcStart");
//  155 
//  156   chSysLock();
//  157   chDbgAssert((sdcp->state == BLK_STOP) || (sdcp->state == BLK_ACTIVE),
//  158               "sdcStart(), #1", "invalid state");
//  159   sdcp->config = config;
//  160   sdc_lld_start(sdcp);
//  161   sdcp->state = BLK_ACTIVE;
//  162   chSysUnlock();
//  163 }
//  164 
//  165 /**
//  166  * @brief   Deactivates the SDC peripheral.
//  167  *
//  168  * @param[in] sdcp      pointer to the @p SDCDriver object
//  169  *
//  170  * @api
//  171  */
//  172 void sdcStop(SDCDriver *sdcp) {
//  173 
//  174   chDbgCheck(sdcp != NULL, "sdcStop");
//  175 
//  176   chSysLock();
//  177   chDbgAssert((sdcp->state == BLK_STOP) || (sdcp->state == BLK_ACTIVE),
//  178               "sdcStop(), #1", "invalid state");
//  179   sdc_lld_stop(sdcp);
//  180   sdcp->state = BLK_STOP;
//  181   chSysUnlock();
//  182 }
//  183 
//  184 /**
//  185  * @brief   Performs the initialization procedure on the inserted card.
//  186  * @details This function should be invoked when a card is inserted and
//  187  *          brings the driver in the @p BLK_READY state where it is possible
//  188  *          to perform read and write operations.
//  189  *
//  190  * @param[in] sdcp      pointer to the @p SDCDriver object
//  191  *
//  192  * @return              The operation status.
//  193  * @retval CH_SUCCESS   operation succeeded.
//  194  * @retval CH_FAILED    operation failed.
//  195  *
//  196  * @api
//  197  */
//  198 bool_t sdcConnect(SDCDriver *sdcp) {
//  199   uint32_t resp[1];
//  200 
//  201   chDbgCheck(sdcp != NULL, "sdcConnect");
//  202   chDbgAssert((sdcp->state == BLK_ACTIVE) || (sdcp->state == BLK_READY),
//  203               "mmcConnect(), #1", "invalid state");
//  204 
//  205   /* Connection procedure in progress.*/
//  206   sdcp->state = BLK_CONNECTING;
//  207 
//  208   /* Card clock initialization.*/
//  209   sdc_lld_start_clk(sdcp);
//  210 
//  211   /* Enforces the initial card state.*/
//  212   sdc_lld_send_cmd_none(sdcp, MMCSD_CMD_GO_IDLE_STATE, 0);
//  213 
//  214   /* V2.0 cards detection.*/
//  215   if (!sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEND_IF_COND,
//  216                                   MMCSD_CMD8_PATTERN, resp)) {
//  217     sdcp->cardmode = SDC_MODE_CARDTYPE_SDV20;
//  218     /* Voltage verification.*/
//  219     if (((resp[0] >> 8) & 0xF) != 1)
//  220       goto failed;
//  221     if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, 0, resp) ||
//  222         MMCSD_R1_ERROR(resp[0]))
//  223       goto failed;
//  224   }
//  225   else {
//  226 #if SDC_MMC_SUPPORT
//  227     /* MMC or SD V1.1 detection.*/
//  228     if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, 0, resp) ||
//  229         MMCSD_R1_ERROR(resp[0]))
//  230       sdcp->cardmode = SDC_MODE_CARDTYPE_MMC;
//  231     else
//  232 #endif /* SDC_MMC_SUPPORT */
//  233     {
//  234       sdcp->cardmode = SDC_MODE_CARDTYPE_SDV11;
//  235     
//  236       /* Reset error flag illegal command.*/
//  237       sdc_lld_send_cmd_none(sdcp, MMCSD_CMD_GO_IDLE_STATE, 0);
//  238     }
//  239   }
//  240 
//  241 #if SDC_MMC_SUPPORT
//  242   if ((sdcp->cardmode &  SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_MMC) {
//  243     /* TODO: MMC initialization.*/
//  244     goto failed;
//  245   }
//  246   else
//  247 #endif /* SDC_MMC_SUPPORT */
//  248   {
//  249     unsigned i;
//  250     uint32_t ocr;
//  251 
//  252     /* SD initialization.*/
//  253     if ((sdcp->cardmode &  SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_SDV20)
//  254       ocr = 0xC0100000;
//  255     else
//  256       ocr = 0x80100000;
//  257 
//  258     /* SD-type initialization. */
//  259     i = 0;
//  260     while (TRUE) {
//  261       if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, 0, resp) ||
//  262         MMCSD_R1_ERROR(resp[0]))
//  263         goto failed;
//  264       if (sdc_lld_send_cmd_short(sdcp, MMCSD_CMD_APP_OP_COND, ocr, resp))
//  265         goto failed;
//  266       if ((resp[0] & 0x80000000) != 0) {
//  267         if (resp[0] & 0x40000000)
//  268           sdcp->cardmode |= SDC_MODE_HIGH_CAPACITY;
//  269         break;
//  270       }
//  271       if (++i >= SDC_INIT_RETRY)
//  272         goto failed;
//  273       chThdSleepMilliseconds(10);
//  274     }
//  275   }
//  276 
//  277   /* Reads CID.*/
//  278   if (sdc_lld_send_cmd_long_crc(sdcp, MMCSD_CMD_ALL_SEND_CID, 0, sdcp->cid))
//  279     goto failed;
//  280 
//  281   /* Asks for the RCA.*/
//  282   if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEND_RELATIVE_ADDR,
//  283                                  0, &sdcp->rca))
//  284     goto failed;
//  285 
//  286   /* Reads CSD.*/
//  287   if (sdc_lld_send_cmd_long_crc(sdcp, MMCSD_CMD_SEND_CSD,
//  288                                 sdcp->rca, sdcp->csd))
//  289     goto failed;
//  290 
//  291   /* Switches to high speed.*/
//  292   sdc_lld_set_data_clk(sdcp);
//  293 
//  294   /* Selects the card for operations.*/
//  295   if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEL_DESEL_CARD,
//  296                                  sdcp->rca, resp))
//  297     goto failed;
//  298 
//  299   /* Block length fixed at 512 bytes.*/
//  300   if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SET_BLOCKLEN,
//  301                                  MMCSD_BLOCK_SIZE, resp) ||
//  302       MMCSD_R1_ERROR(resp[0]))
//  303     goto failed;
//  304 
//  305   /* Switches to wide bus mode.*/
//  306   switch (sdcp->cardmode & SDC_MODE_CARDTYPE_MASK) {
//  307   case SDC_MODE_CARDTYPE_SDV11:
//  308   case SDC_MODE_CARDTYPE_SDV20:
//  309     sdc_lld_set_bus_mode(sdcp, SDC_MODE_4BIT);
//  310     if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, sdcp->rca, resp) ||
//  311         MMCSD_R1_ERROR(resp[0]))
//  312       goto failed;
//  313     if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SET_BUS_WIDTH, 2, resp) ||
//  314         MMCSD_R1_ERROR(resp[0]))
//  315       goto failed;
//  316     break;
//  317   }
//  318 
//  319   /* Determine capacity.*/
//  320   sdcp->capacity = mmcsdGetCapacity(sdcp->csd);
//  321   if (sdcp->capacity == 0)
//  322     goto failed;
//  323 
//  324   /* Initialization complete.*/
//  325   sdcp->state = BLK_READY;
//  326   return CH_SUCCESS;
//  327 
//  328   /* Connection failed, state reset to BLK_ACTIVE.*/
//  329 failed:
//  330   sdc_lld_stop_clk(sdcp);
//  331   sdcp->state = BLK_ACTIVE;
//  332   return CH_FAILED;
//  333 }
//  334 
//  335 /**
//  336  * @brief   Brings the driver in a state safe for card removal.
//  337  *
//  338  * @param[in] sdcp      pointer to the @p SDCDriver object
//  339  *
//  340  * @return              The operation status.
//  341  * @retval CH_SUCCESS   operation succeeded.
//  342  * @retval CH_FAILED    operation failed.
//  343  *
//  344  * @api
//  345  */
//  346 bool_t sdcDisconnect(SDCDriver *sdcp) {
//  347 
//  348   chDbgCheck(sdcp != NULL, "sdcDisconnect");
//  349 
//  350   chSysLock();
//  351   chDbgAssert((sdcp->state == BLK_ACTIVE) || (sdcp->state == BLK_READY),
//  352               "sdcDisconnect(), #1", "invalid state");
//  353   if (sdcp->state == BLK_ACTIVE) {
//  354     chSysUnlock();
//  355     return CH_SUCCESS;
//  356   }
//  357   sdcp->state = BLK_DISCONNECTING;
//  358   chSysUnlock();
//  359 
//  360   /* Waits for eventual pending operations completion.*/
//  361   if (_sdc_wait_for_transfer_state(sdcp)) {
//  362     sdc_lld_stop_clk(sdcp);
//  363     sdcp->state = BLK_ACTIVE;
//  364     return CH_FAILED;
//  365   }
//  366 
//  367   /* Card clock stopped.*/
//  368   sdc_lld_stop_clk(sdcp);
//  369   sdcp->state = BLK_ACTIVE;
//  370   return CH_SUCCESS;
//  371 }
//  372 
//  373 /**
//  374  * @brief   Reads one or more blocks.
//  375  * @pre     The driver must be in the @p BLK_READY state after a successful
//  376  *          sdcConnect() invocation.
//  377  *
//  378  * @param[in] sdcp      pointer to the @p SDCDriver object
//  379  * @param[in] startblk  first block to read
//  380  * @param[out] buf      pointer to the read buffer
//  381  * @param[in] n         number of blocks to read
//  382  *
//  383  * @return              The operation status.
//  384  * @retval CH_SUCCESS   operation succeeded.
//  385  * @retval CH_FAILED    operation failed.
//  386  *
//  387  * @api
//  388  */
//  389 bool_t sdcRead(SDCDriver *sdcp, uint32_t startblk,
//  390                uint8_t *buf, uint32_t n) {
//  391   bool_t status;
//  392 
//  393   chDbgCheck((sdcp != NULL) && (buf != NULL) && (n > 0), "sdcRead");
//  394   chDbgAssert(sdcp->state == BLK_READY, "sdcRead(), #1", "invalid state");
//  395 
//  396   if ((startblk + n - 1) > sdcp->capacity){
//  397     sdcp->errors |= SDC_OVERFLOW_ERROR;
//  398     return CH_FAILED;
//  399   }
//  400 
//  401   /* Read operation in progress.*/
//  402   sdcp->state = BLK_READING;
//  403 
//  404   status = sdc_lld_read(sdcp, startblk, buf, n);
//  405 
//  406   /* Read operation finished.*/
//  407   sdcp->state = BLK_READY;
//  408   return status;
//  409 }
//  410 
//  411 /**
//  412  * @brief   Writes one or more blocks.
//  413  * @pre     The driver must be in the @p BLK_READY state after a successful
//  414  *          sdcConnect() invocation.
//  415  *
//  416  * @param[in] sdcp      pointer to the @p SDCDriver object
//  417  * @param[in] startblk  first block to write
//  418  * @param[out] buf      pointer to the write buffer
//  419  * @param[in] n         number of blocks to write
//  420  *
//  421  * @return              The operation status.
//  422  * @retval CH_SUCCESS   operation succeeded.
//  423  * @retval CH_FAILED    operation failed.
//  424  *
//  425  * @api
//  426  */
//  427 bool_t sdcWrite(SDCDriver *sdcp, uint32_t startblk,
//  428                 const uint8_t *buf, uint32_t n) {
//  429   bool_t status;
//  430 
//  431   chDbgCheck((sdcp != NULL) && (buf != NULL) && (n > 0), "sdcWrite");
//  432   chDbgAssert(sdcp->state == BLK_READY, "sdcWrite(), #1", "invalid state");
//  433 
//  434   if ((startblk + n - 1) > sdcp->capacity){
//  435     sdcp->errors |= SDC_OVERFLOW_ERROR;
//  436     return CH_FAILED;
//  437   }
//  438 
//  439   /* Write operation in progress.*/
//  440   sdcp->state = BLK_WRITING;
//  441 
//  442   status = sdc_lld_write(sdcp, startblk, buf, n);
//  443 
//  444   /* Write operation finished.*/
//  445   sdcp->state = BLK_READY;
//  446   return status;
//  447 }
//  448 
//  449 /**
//  450  * @brief   Returns the errors mask associated to the previous operation.
//  451  *
//  452  * @param[in] sdcp      pointer to the @p SDCDriver object
//  453  * @return              The errors mask.
//  454  *
//  455  * @api
//  456  */
//  457 sdcflags_t sdcGetAndClearErrors(SDCDriver *sdcp) {
//  458   sdcflags_t flags;
//  459 
//  460   chDbgCheck(sdcp != NULL, "sdcGetAndClearErrors");
//  461   chDbgAssert(sdcp->state == BLK_READY,
//  462               "sdcGetAndClearErrors(), #1", "invalid state");
//  463 
//  464   chSysLock();
//  465   flags = sdcp->errors;
//  466   sdcp->errors = SDC_NO_ERROR;
//  467   chSysUnlock();
//  468   return flags;
//  469 }
//  470 
//  471 /**
//  472  * @brief   Waits for card idle condition.
//  473  *
//  474  * @param[in] sdcp      pointer to the @p SDCDriver object
//  475  *
//  476  * @return              The operation status.
//  477  * @retval CH_SUCCESS   the operation succeeded.
//  478  * @retval CH_FAILED    the operation failed.
//  479  *
//  480  * @api
//  481  */
//  482 bool_t sdcSync(SDCDriver *sdcp) {
//  483   bool_t result;
//  484 
//  485   chDbgCheck(sdcp != NULL, "sdcSync");
//  486 
//  487   if (sdcp->state != BLK_READY)
//  488     return CH_FAILED;
//  489 
//  490   /* Synchronization operation in progress.*/
//  491   sdcp->state = BLK_SYNCING;
//  492 
//  493   result = sdc_lld_sync(sdcp);
//  494 
//  495   /* Synchronization operation finished.*/
//  496   sdcp->state = BLK_READY;
//  497   return result;
//  498 }
//  499 
//  500 /**
//  501  * @brief   Returns the media info.
//  502  *
//  503  * @param[in] sdcp      pointer to the @p SDCDriver object
//  504  * @param[out] bdip     pointer to a @p BlockDeviceInfo structure
//  505  *
//  506  * @return              The operation status.
//  507  * @retval CH_SUCCESS   the operation succeeded.
//  508  * @retval CH_FAILED    the operation failed.
//  509  *
//  510  * @api
//  511  */
//  512 bool_t sdcGetInfo(SDCDriver *sdcp, BlockDeviceInfo *bdip) {
//  513 
//  514   chDbgCheck((sdcp != NULL) && (bdip != NULL), "sdcGetInfo");
//  515 
//  516   if (sdcp->state != BLK_READY)
//  517     return CH_FAILED;
//  518 
//  519   bdip->blk_num = sdcp->capacity;
//  520   bdip->blk_size = MMCSD_BLOCK_SIZE;
//  521 
//  522   return CH_SUCCESS;
//  523 }
//  524 
//  525 
//  526 /**
//  527  * @brief   Erases the supplied blocks.
//  528  *
//  529  * @param[in] sdcp      pointer to the @p SDCDriver object
//  530  * @param[in] startblk  starting block number
//  531  * @param[in] endblk    ending block number
//  532  *
//  533  * @return              The operation status.
//  534  * @retval CH_SUCCESS   the operation succeeded.
//  535  * @retval CH_FAILED    the operation failed.
//  536  *
//  537  * @api
//  538  */
//  539 bool_t sdcErase(SDCDriver *sdcp, uint32_t startblk, uint32_t endblk) {
//  540   uint32_t resp[1];
//  541 
//  542   chDbgCheck((sdcp != NULL), "sdcErase");
//  543   chDbgAssert(sdcp->state == BLK_READY, "sdcErase(), #1", "invalid state");
//  544 
//  545   /* Erase operation in progress.*/
//  546   sdcp->state = BLK_WRITING;
//  547 
//  548   /* Handling command differences between HC and normal cards.*/
//  549   if (!(sdcp->cardmode & SDC_MODE_HIGH_CAPACITY)) {
//  550     startblk *= MMCSD_BLOCK_SIZE;
//  551     endblk *= MMCSD_BLOCK_SIZE;
//  552   }
//  553 
//  554   _sdc_wait_for_transfer_state(sdcp);
//  555 
//  556   if ((sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_ERASE_RW_BLK_START,
//  557                                   startblk, resp) != CH_SUCCESS) ||
//  558       MMCSD_R1_ERROR(resp[0]))
//  559     goto failed;
//  560 
//  561   if ((sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_ERASE_RW_BLK_END,
//  562                                   endblk, resp) != CH_SUCCESS) ||
//  563       MMCSD_R1_ERROR(resp[0]))
//  564     goto failed;
//  565 
//  566   if ((sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_ERASE,
//  567                                   0, resp) != CH_SUCCESS) ||
//  568       MMCSD_R1_ERROR(resp[0]))
//  569     goto failed;
//  570 
//  571   /* Quick sleep to allow it to transition to programming or receiving state */
//  572   /* TODO: ??????????????????????????? */
//  573 
//  574   /* Wait for it to return to transfer state to indicate it has finished erasing */
//  575   _sdc_wait_for_transfer_state(sdcp);
//  576 
//  577   sdcp->state = BLK_READY;
//  578   return CH_SUCCESS;
//  579 
//  580 failed:
//  581   sdcp->state = BLK_READY;
//  582   return CH_FAILED;
//  583 }
//  584 
//  585 #endif /* HAL_USE_SDC */
//  586 
//  587 /** @} */
// 
//
// 
//
//
//Errors: none
//Warnings: none
