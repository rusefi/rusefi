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
            stf high b1;
            stdcctl async;                              * Enable asynchronous mode
            wait row13;                                 * Wait for one of the previously defined conditions

* ### Synchronous phase ### 
dcdc_off:   load Vboost_low dac4h4n _ofs;               * Load the upper Vboost threshold in vboost_dac register
            stf low b1;
            stdcctl sync;                               * Enable synchronous mode
            wait row12;                                 * Wait for one of the previously defined conditions
            
* ### Idle phase ### 
dcdc_idle:  stdcctl sync;                               * Enable synchronous mode
            stf low b1;
            stf high b1;
            jocr dcdc_idle _f0;                         * jump to previous line while flag 0 is low
            jmpr dcdc_on;                               * force the DC-DC converter on when flag 0 goes high

* ### End of Channel 2 - uCore0 code ###

*********************************************************************************

* ### Channel 2 - uCore1 drives HPFP high pressure fuel pump pressure control valve ###
* todo: replace 'PCV' with HPFP

* Note: The Tpeak_tot variable defines the current profile time out. The active STARTx pin is expected to toggle in is low state before this time out.

* ### Initialization phase ###
init1:      stgn gain19.3 ossc;                     * Set the gain of the opamp of the current measure block 1 
            ldjr1 eoact1;                           * Load the eoinj line label Code RAM address into the register jr1 
            ldjr2 idle1;                            * Load the idle line label Code RAM address into the register jr2
            cwef jr1 _start row1;                   * If the start signal goes low, go to eoinj phase           

* ### Idle phase- the uPC loops here until start signal is present ###              
idle1:      joslr act6_start start6;                * Perform an actuation on act6 if start 6 (only) is active
            jmpf jr1;                               * If more than 1 start active at the same time(or none), no actuation

act6_start: dfsct hs5 ls6 undef;                    * Set the 2 shortcuts: VBAT, LS
            jmpr peak1;                             * Jump to launch phase

* ### Launch peak phase on bat ###
peak1:      load PCV_Ipeak dac_ossc _ofs;               * Load the boost phase current threshold in the current DAC
            cwer hold1 cur3 row2;                   * Jump to hold phase when current is over threshold
            stos on on keep;                        * Turn VBAT on, LS on
            wait row12;                             * Wait for one of the previously defined conditions
            
* ### Hold phase on Vbat ###    
hold1:      ldcd rst _ofs keep keep PCV_Thold_tot c1;   * Load the length of the total hold phase in counter 1
            load PCV_Ihold dac_ossc _ofs;               * Load the hold current threshold in the DAC
            cwer eoact1 tc1 row2;                   * Jump to eoinj phase when tc1 reaches end of count
            cwer hold_on1 tc2 row3;                 * Jump to hold_on when tc2 reaches end of count
            cwer hold_off1 cur3 row4;               * Jump to hold_off when current is over threshold

hold_on1:   stos on on keep;                        * Turn VBAT on, LS on
            wait row124;                            * Wait for one of the previously defined conditions

hold_off1:  ldcd rst _ofs off on PCV_Thold_off c2;  * Load the length of the hold_off phase in counter 2 and turn VBAT off, LS on
            wait row123;                            * Wait for one of the previously defined conditions

* ### End of injection phase ###
eoact1:     stos off off keep;                      * Turn VBAT off, LS off
            jmpf jr2;                               * Jump back to idle phase

* ### End of Channel 2 - uCore1 code ###