********************************************************************************
* Example Code
*
* Copyright(C) 2019 NXP Semiconductors
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
* BY LAW. NXP SEMICONDUCTOR???S TOTAL LIABILITY FOR ALL COSTS, DAMAGES, 
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
********************************************************************************

#include "dram2.def";

* ### Channel 2 - uCore0 controls dc-dc ###


* ### Initialization phase ###
init0:      stgn gain5.8 ossc;                          * Set the gain of the opamp of the current measure block 4
            load Isense4_low dac_ossc _ofs;             * Load Isense4_high current threshold in DAC 4L
            load Isense4_high dac4h4n _ofs;             * Load Isense4_high current threshold in DAC 4H
            stdm null;                                  * Set the boost voltage DAC access mode
            cwer dcdc_idle _f0 row1;                    * Wait table entry for Vboost under Vboost_low threshold condition
            cwer dcdc_on _vb row2;                      * Wait table entry for Vboost under Vboost_low threshold condition
            cwer dcdc_off vb row3;                      * Wait table entry for Vboost over Vboost_high threshold condition

* ### Asynchronous phase ###  
dcdc_on:    load Vboost_high dac4h4n _ofs;              * Load the upper Vboost threshold in vboost_dac register
            stdcctl async;                              * Enable asynchronous mode
            wait row13;                                 * Wait for one of the previously defined conditions

* ### Synchronous phase ### 
dcdc_off:   load Vboost_low dac4h4n _ofs;               * Load the upper Vboost threshold in vboost_dac register
            stdcctl sync;                               * Enable synchronous mode
            wait row12;                                 * Wait for one of the previously defined conditions
            
* ### Idle phase ### 
dcdc_idle:  stdcctl sync;                               * Enable synchronous mode
            jocr dcdc_idle _f0;                         * jump to previous line while flag 0 is low
            jmpr dcdc_on;                               * force the DC-DC converter on when flag 0 goes high

* ### End of Channel 2 - uCore0 code ###

*********************************************************************************

* ### Channel 2 - uCore1 drives fuel pump ###


* ### Initialization phase ###
init1:      stgn gain19.4 ossc;                     * Set the gain of the opamp of the current measure block 1
            ldjr1 eoact1;                           * Load the eoinj line label Code RAM address into the register jr1
            ldjr2 idle1;                            * Load the idle line label Code RAM address into the register jr2
            cwef jr1 _start row1;                   * If the start signal goes low, go to eoinj phase           

* ### Idle phase- the uPC loops here until start signal is present ###
idle1:      joslr act5_start start5;                * Perform an actuation on act5 if start 5 (only) is active
            joslr act6_start start6;                * Perform an actuation on act6 if start 6 (only) is active
            jmpf jr1;                               * If more than 1 start active at the same time(or none), no actuation

* ### Shortcuts definition per the injector to be actuated ###
act5_start: dfsct hs5 ls5 undef;                    * Set the 2 shortcuts: VBAT, LS
            jmpr peak1;                             * Jump to launch phase

act6_start: dfsct hs5 ls6 undef;                    * Set the 2 shortcuts: VBAT, LS
            jmpr peak1;                             * Jump to launch phase

* ### Launch peak phase on bat ###
peak1:      load Ipeak dac_ossc _ofs;               * Load the boost phase current threshold in the current DAC
            cwer hold1 cur3 row2;                   * Jump to peak phase when current is over threshold
            stos on on keep;                        * Turn VBAT off, BOOST on, LS on
            wait row12;                             * Wait for one of the previously defined conditions
            
* ### Hold phase on Vbat ###    
hold1:      ldcd rst _ofs keep keep Thold_tot c1;   * Load the length of the total hold phase in counter 2
            load Ihold dac_ossc _ofs;               * Load the hold current threshold in the DAC
            cwer eoact1 tc1 row2;                   * Jump to eoinj phase when tc1 reaches end of count
            cwer hold_on1 tc2 row3;                 * Jump to hold_on when tc2 reaches end of count
            cwer hold_off1 cur3 row4;               * Jump to hold_off when current is over threshold

hold_on1:   stos on on keep;                        * Turn VBAT on, LS on
            wait row124;                            * Wait for one of the previously defined conditions

hold_off1:  ldcd rst _ofs off on Thold_off c2;      * Load the length of the hold_off phase in counter 1 and turn VBAT off, LS on
            wait row123;                            * Wait for one of the previously defined conditions

* ### End of injection phase ###
eoact1:     stos off off keep;                      * Turn VBAT off, LS off
            jmpf jr2;                               * Jump back to idle phase

* ### End of Channel 2 - uCore1 code ###
