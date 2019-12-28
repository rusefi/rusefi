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

#include "dram1.def";

* ### Channel 1 - uCore0 controls the injectors 1 and 2 ###

* ### Variables declaration  ###

* Note: The data are stored into the dataRAM of the channel 1.
* Note: The Thold_tot variable defines the current profile time out.
*       The active STARTx pin is expected to toggle in is low state before this time out.

* ### Initialization phase ###
init0:      stgn gain8.68 sssc;                     * Set the gain of the opamp of the current measure block 1
            ldjr1 eoinj0;                           * Load the eoinj line label Code RAM address into the register jr1 
            ldjr2 idle0;                            * Load the idle line label Code RAM address into the register jr2
            cwef jr1 _start row1;                   * If the start signal goes low, go to eoinj phase       

* ### Idle phase- the uPC loops here until start signal is present ###
idle0:      cwer CheckStart start row2;             * Define entry table for high start pin
            stoc on sssc;                           * Turn ON offset compensation
WaitLoop:   wait row2;                              * uPC is stuck here for almost the whole idle time
CheckStart: joslr inj1_start start1;                * Jump to inj1 if start 1 is high
            joslr inj2_start start2;                * Jump to inj2 if start 2 is high
            jmpr WaitLoop;
            
            
* ### Shortcuts definition per the injector to be actuated ###
inj1_start: dfsct hs1 hs2 ls1;                      * Set the 3 shortcuts: VBAT, VBOOST, LS
            jmpr boost0;                            * Jump to launch phase

inj2_start: dfsct hs1 hs2 ls2;                      * Set the 3 shortcuts: VBAT, VBOOST, LS
            jmpr boost0;                            * Jump to launch phase

* ### Launch phase enable boost ###
boost0:     stoc off sssc;                          * Turn OFF offset compensation
            bias all on;                            * Enable all biasing structures, kept ON even during actuation
            load Iboost dac_sssc _ofs;              * Load the boost phase current threshold in the current DAC
            cwer peak0 ocur row2;                   * Jump to peak phase when current is over threshold
            stf low b0;                             * set flag0 low to force the DC-DC converter in idle mode
            stos off on on;                         * Turn VBAT off, BOOST on, LS on
            wait row12;                             * Wait for one of the previously defined conditions

* ### Peak phase continue on Vbat ###
peak0:      ldcd rst _ofs keep keep Tpeak_tot c1;   * Load the length of the total peak phase in counter 1
            load Ipeak dac_sssc _ofs;               * Load the peak current threshold in the current DAC
            cwer bypass0 tc1 row2;                  * Jump to bypass phase when tc1 reaches end of count
            cwer peak_on0 tc2 row3;                 * Jump to peak_on when tc2 reaches end of count
            cwer peak_off0 ocur row4;               * Jump to peak_off when current is over threshold
            stf high b0;                            * set flag0 high to release the DC-DC converter idle mode

peak_on0:   stos on off on;                         * Turn VBAT on, BOOST off, LS on
            wait row124;                            * Wait for one of the previously defined conditions

peak_off0:  ldcd rst ofs keep keep Tpeak_off c2;    * Load in the counter 2 the length of the peak_off phase
            stos off off on;                        * Turn VBAT off, BOOST off, LS on
            wait row123;                            * Wait for one of the previously defined conditions
            
* ### Bypass phase ###
bypass0:    ldcd rst ofs keep keep Tbypass c3;      * Load in the counter 3 the length of the off_phase phase
            stos off off off;                       * Turn VBAT off, BOOST off, LS off
            cwer hold0 tc3 row4;                    * Jump to hold when tc3 reaches end of count
            wait row14;                             * Wait for one of the previously defined conditions
            
* ### Hold phase on Vbat ###    
hold0:      ldcd rst _ofs keep keep Thold_tot c1;   * Load the length of the total hold phase in counter 2 
            load Ihold dac_sssc _ofs;               * Load the hold current threshold in the DAC
            cwer eoinj0 tc1 row2;                   * Jump to eoinj phase when tc1 reaches end of count
            cwer hold_on0 tc2 row3;                 * Jump to hold_on when tc2 reaches end of count
            cwer hold_off0 ocur row4;               * Jump to hold_off when current is over threshold

hold_on0:   stos on off on;                         * Turn VBAT on, BOOST off, LS on
            wait row124;                            * Wait for one of the previously defined conditions

hold_off0:  ldcd rst _ofs keep keep Thold_off c2;   * Load the length of the hold_off phase in counter 1
            stos off off on;                        * Turn VBAT off, BOOST off, LS on
            wait row123;                            * Wait for one of the previously defined conditions

* ### End of injection phase ###
eoinj0:     stos off off off;                       * Turn VBAT off, BOOST off, LS off
            stf high b0;                            * set flag0 to high to release the DC-DC converter idle mode
            jmpf jr2;                               * Jump back to idle phase

* ### End of Channel 1 - uCore0 code ###            

*********************************************************************************

* ### Channel 1 - uCore1 controls the injectors 3 and 4 ###

* ### Variables declaration  ###

* Note: The data that defines the profiles are shared between the two microcores.

* ### Initialization phase ###
init1:      stgn gain8.68 sssc;                     * Set the gain of the opamp of the current measure block 1
            ldjr1 eoinj1;                           * Load the eoinj line label Code RAM address into the register jr1
            ldjr2 idle1;                            * Load the idle line label Code RAM address into the register jr2
            cwef jr1 _start row1;                   * If the start signal goes low, go to eoinj phase
            

* ### Idle phase- the uPC loops here until start signal is present ###
idle1:      cwer CheckStart1 start row2;            * Define entry table for high start pin
            stoc on sssc;                           * Turn ON offset compensation    
WaitLoop1:  wait row2;                              * uPC is stuck here for almost the whole idle time
CheckStart1:joslr inj3_start start3;                * Jump to inj1 if start 1 is high
            joslr inj4_start start4;                * Jump to inj2 if start 2 is high
            jmpr WaitLoop1;
            
            
* ### Shortcuts definition per the injector to be actuated ###
inj3_start: dfsct hs3 hs4 ls3;                      * Set the 3 shortcuts: VBAT, VBOOST, LS
            jmpr boost1;                            * Jump to launch phase

inj4_start: dfsct hs3 hs4 ls4;                      * Set the 3 shortcuts: VBAT, VBOOST, LS
            jmpr boost1;                            * Jump to launch phase

* ### Launch phase enable boost ###
boost1:     stoc off sssc;                          * Turn OFF offset compensation
            load Iboost dac_sssc _ofs;              * Load the boost phase current threshold in the current DAC
            cwer peak1 ocur row2;                   * Jump to peak phase when current is over threshold
            stf low b0;                             * set flag0 low to force the DC-DC converter in idle mode
            stos off on on;                         * Turn VBAT off, BOOST on, LS on
            wait row12;                             * Wait for one of the previously defined conditions

* ### Peak phase continue on Vbat ###
peak1:      ldcd rst _ofs keep keep Tpeak_tot c1;   * Load the length of the total peak phase in counter 1
            load Ipeak dac_sssc _ofs;               * Load the peak current threshold in the current DAC
            cwer bypass1 tc1 row2;                  * Jump to bypass phase when tc1 reaches end of count
            cwer peak_on1 tc2 row3;                 * Jump to peak_on when tc2 reaches end of count
            cwer peak_off1 ocur row4;               * Jump to peak_off when current is over threshold
            stf high b0;                            * set flag0 high to release the DC-DC converter idle mode
            
peak_on1:   stos on off on;                         * Turn VBAT on, BOOST off, LS on
            wait row124;                            * Wait for one of the previously defined conditions

peak_off1:  ldcd rst ofs keep keep Tpeak_off c2;    * Load in the counter 2 the length of the peak_off phase
            stos off off on;                        * Turn VBAT off, BOOST off, LS on
            wait row123;                            * Wait for one of the previously defined conditions

* ### Bypass phase ###
bypass1:    ldcd rst ofs keep keep Tbypass c3;      * Load in the counter 3 the length of the off_phase phase
            stos off off off;                       * Turn VBAT off, BOOST off, LS off
            cwer hold1 tc3 row4;                    * Jump to hold when tc3 reaches end of count
            wait row14;                             * Wait for one of the previously defined conditions

* ### Hold phase on Vbat ###
hold1:      ldcd rst _ofs keep keep Thold_tot c1;   * Load the length of the total hold phase in counter 2
            load Ihold dac_sssc _ofs;               * Load the hold current threshold in the DAC
            cwer eoinj1 tc1 row2;                   * Jump to eoinj phase when tc1 reaches end of count
            cwer hold_on1 tc2 row3;                 * Jump to hold_on when tc2 reaches end of count
            cwer hold_off1 ocur row4;               * Jump to hold_off when current is over threshold

hold_on1:   stos on off on;                         * Turn VBAT on, BOOST off, LS on
            wait row124;                            * Wait for one of the previously defined conditions

hold_off1:  ldcd rst _ofs keep keep Thold_off c2;   * Load the length of the hold_off phase in counter 1
            stos off off on;                        * Turn VBAT off, BOOST off, LS on
            wait row123;                            * Wait for one of the previously defined conditions

* ### End of injection phase ###
eoinj1:     stos off off off;                       * Turn VBAT off, BOOST off, LS off
            stf high b0;                            * set flag0 to high to release the DC-DC converter idle mode
            jmpf jr2;                               * Jump back to idle phase

* ### End of Channel 1 - uCore1 code ###
