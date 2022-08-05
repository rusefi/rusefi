#include "dram1.def";
* ### Channel 1 - uCore0 controls the injectors 1 and 2 ###


* Note: The Thold_tot variable defines the current profile time out. The active STARTx pin is expected to toggle in is low state before this time out.

* ### Initialization phase ###
init0:      stgn gain12.6 sssc;                     * Set the gain of the opamp of the current measure block 1 
            ldjr1 eoinj0;                           * Load the eoinj line label Code RAM address into the register jr1 
            ldjr2 idle0;                            * Load the idle line label Code RAM address into the register jr2
            cwef jr1 _start row1;                   * If the start signal goes low, go to eoinj phase       

* ### Idle phase- the uPC loops here until start signal is present ###              
idle0:      joslr inj1_start start1;                * Perform an actuation on inj1 if start 1 (only) is active
            joslr inj2_start start2;                * Perform an actuation on inj2 if start 2 (only) is active
            jmpf jr1;                               * If more than 1 start active at the same time(or none), no actuation

* ### Shortcuts definition per the injector to be actuated ###
inj1_start: dfsct hs1 hs2 ls1;                      * Set the 3 shortcuts: VBAT, VBOOST, LS
            jmpr boost0;                            * Jump to launch phase

inj2_start: dfsct hs1 hs2 ls2;                      * Set the 3 shortcuts: VBAT, VBOOST, LS
            jmpr boost0;                            * Jump to launch phase

* ### Launch phase enable boost ###
* Row1 - !start: injection ended
* Row2 - Overcurrent, jump to error case (threshold is reached early)
* Row3 - Overcurrent, jump to peak phase (threshold is reached on time)
* Row4 - timer1: Minimum boost time reached - switch to row3 instead of row2 to allow peak phase
* Row5 - timer2: Boost phase timeout - boost took too long, go to error phase
boost0:     load Iboost dac_sssc _ofs;              * Load the boost phase current threshold in the current DAC
            cwer boost0_err ocur row2;              * On overcurrent, go to boost error case
            cwer peak0 ocur row3;                   * Jump to peak phase when current is over threshold

            ldcd rst _ofs keep keep Tboost_min c1;  * Start boost counter to switch out of error behavior if threshold reached
            cwer boost0_mintime tc1 row4;           * On timer timeout, go allow overcurrent without error (ie, end of boost)

            ldcd rst _ofs keep keep Tboost_max c2;  * Start boost counter in case Iboost never reached
            cwer boost0_err tc2 row5;               * Jump to boost0_err in case boost phase takes too long

            stf low b0;                             * set flag0 low to force the DC-DC converter in idle mode
            stos off on on;                         * Turn VBAT off, BOOST on, LS on
            wait row1245;                           * Wait for one of the previously defined conditions

* ### Boost phase - minimum time reached ###
boost0_mintime: wait row135;                        * Minimum time for boost phase has been reached, now wait for !start, overcurrent or timeout

boost0_err: stos off off off;                       * Turn off all drivers
            stf low b10;                            * Set ch1 error flag (OA_1) to signal MCU
            stf high b0;                            * set flag0 high to release the DC-DC converter idle mode
            wait row1;                              * Wait for start signal to go low for the next injection attempt

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

* ### Initialization phase ###
init1:      stgn gain12.6 sssc;                     * Set the gain of the opamp of the current measure block 1
            ldjr1 eoinj1;                           * Load the eoinj line label Code RAM address into the register jr1
            ldjr2 idle1;                            * Load the idle line label Code RAM address into the register jr2
            cwef jr1 _start row1;                   * If the start signal goes low, go to eoinj phase
            
            
* ### Idle phase- the uPC loops here until start signal is present ###              
idle1:      joslr inj3_start start3;                * Perform an actuation on inj1 if start 1 (only) is active
            joslr inj4_start start4;                * Perform an actuation on inj2 if start 2 (only) is active
            jmpf jr1;                               * If more than 1 start active at the same time(or none), no actuation            
            
* ### Shortcuts definition per the injector to be actuated ###
inj3_start: dfsct hs3 hs4 ls3;                      * Set the 3 shortcuts: VBAT, VBOOST, LS
            jmpr boost1;                            * Jump to launch phase

inj4_start: dfsct hs3 hs4 ls4;                      * Set the 3 shortcuts: VBAT, VBOOST, LS
            jmpr boost1;                            * Jump to launch phase

* ### Launch phase enable boost ###
* Row1 - !start: injection ended
* Row2 - Overcurrent, jump to error case (threshold is reached early)
* Row3 - Overcurrent, jump to peak phase (threshold is reached on time)
* Row4 - timer1: Minimum boost time reached - switch to row3 instead of row2 to allow peak phase
* Row5 - timer2: Boost phase timeout - boost took too long, go to error phase
boost1:     load Iboost dac_sssc _ofs;              * Load the boost phase current threshold in the current DAC
            cwer boost1_err ocur row2;              * On overcurrent, go to boost error case
            cwer peak1 ocur row3;                   * Jump to peak phase when current is over threshold

            ldcd rst _ofs keep keep Tboost_min c1;  * Start boost counter to switch out of error behavior if threshold reached
            cwer boost1_mintime tc1 row4;           * On timer timeout, go allow overcurrent without error (ie, end of boost)

            ldcd rst _ofs keep keep Tboost_max c2;  * Start boost counter in case Iboost never reached
            cwer boost1_err tc2 row5;               * Jump to boost1_err in case boost phase takes too long

            stf low b0;                             * set flag0 low to force the DC-DC converter in idle mode
            stos off on on;                         * Turn VBAT off, BOOST on, LS on
            wait row1245;                           * Wait for one of the previously defined conditions

* ### Boost phase - minimum time reached ###
boost1_mintime: wait row135;                        * Minimum time for boost phase has been reached, now wait for !start, overcurrent or timeout

boost1_err: stos off off off;                       * Turn off all drivers
            stf low b11;                            * Set ch1 error flag (OA_1) to signal MCU
            stf high b0;                            * set flag0 high to release the DC-DC converter idle mode
            wait row1;                              * Wait for start signal to go low for the next injection attempt


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