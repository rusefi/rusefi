/*******************************************************************************
* Example Code
*
* Copyright(C) 2022 NXP Semiconductors
* NXP Semiconductors Confidential and Proprietary
*
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* NXP products.  This software is supplied "AS IS" without any warranties
* of any kind, and NXP Semiconductors and its licensor disclaim any and
* all warranties, express or implied, including all implied warranties of
* merchantability, fitness for a particular purpose and non-infringement of
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights
* under any patent, copyright, mask work right, or any other intellectual
* property rights in or to any products. NXP Semiconductors reserves the
* right to make changes in the software without notification. NXP
* Semiconductors also makes no representation or warranty that such
* application will be suitable for the specified use without further testing
* or modification.
*
* IN NO EVENT WILL NXP SEMICONDUCTORS BE LIABLE, WHETHER IN CONTRACT, 
* TORT, OR OTHERWISE, FOR ANY INCIDENTAL, SPECIAL, INDIRECT, CONSEQUENTIAL 
* OR PUNITIVE DAMAGES, INCLUDING, BUT NOT LIMITED TO, DAMAGES FOR ANY 
* LOSS OF USE, LOSS OF TIME, INCONVENIENCE, COMMERCIAL LOSS, OR LOST 
* PROFITS, SAVINGS, OR REVENUES, TO THE FULL EXTENT SUCH MAY BE DISCLAIMED  
* BY LAW. NXP SEMICONDUCTOR’S TOTAL LIABILITY FOR ALL COSTS, DAMAGES, 
* CLAIMS, OR LOSSES WHATSOEVER ARISING OUT OF OR IN CONNECTION WITH THE 
* SOFTWARE IS LIMITED TO THE AGGREGATE AMOUNT PAID BY YOU TO NXP SEMICONDUCTORS
* IN CONNECTION WITH THE SOFTWARE TO WHICH LOSSES OR DAMAGES ARE CLAIMED.
*
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors' and its
* licensor's relevant copyrights in the software, without fee, provided
* that it is used in conjunction with NXP Semiconductors devices.  This
* copyright, permission, and disclaimer notice must appear in all copies
* of this code.
*******************************************************************************/

/*
 * PT2001_dram.h
 *
 *  DRAM Header File
 *
 */

#ifndef PT2001_DRAM_H_
#define PT2001_DRAM_H_

// DRAM 1 Parameter Addresses
#define PT2001_D1_Iboost  0x00
#define PT2001_D1_Ipeak  0x01
#define PT2001_D1_Ihold  0x02
#define PT2001_D1_Tpeak_off  0x03
#define PT2001_D1_Tpeak_tot  0x04
#define PT2001_D1_Tbypass  0x05
#define PT2001_D1_Thold_off  0x06
#define PT2001_D1_Thold_tot  0x07
#define PT2001_D1_Tboost_max  0x08
#define PT2001_D1_Tboost_min  0x09

// DRAM 2 Parameter Addresses
#define PT2001_D2_Vboost_high  0x40
#define PT2001_D2_Vboost_low  0x41
#define PT2001_D2_Isense4_high  0x42
#define PT2001_D2_Isense4_low  0x43
#define PT2001_D2_PCV_Ipeak  0x45
#define PT2001_D2_PCV_Ihold  0x46
#define PT2001_D2_PCV_Thold_off  0x47
#define PT2001_D2_PCV_Thold_tot  0x48

#endif /* PT2001_DRAM_H_ */
