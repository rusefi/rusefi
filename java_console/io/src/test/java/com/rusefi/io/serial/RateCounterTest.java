package com.rusefi.io.serial;

import com.rusefi.Timeouts;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class RateCounterTest {
    @Test
    public void testRateCalculation() {

        RateCounter rateCounter = new RateCounter();

        assertEquals(0, rateCounter.getCurrentRate());

        rateCounter.add(/*timestamp*/1);
        rateCounter.add(1);
        rateCounter.add(1);
        rateCounter.add(1);

        // cute size effect: FUTURE timestamps are also counted :)
        assertEquals(4, rateCounter.getCurrentRate(0));
        assertEquals(4, rateCounter.getSizeForUnitTest());


        assertEquals(0, rateCounter.getCurrentRate(2 * Timeouts.SECOND));
        // assert purge of oldest records
        assertEquals(0, rateCounter.getSizeForUnitTest());
    }
}

