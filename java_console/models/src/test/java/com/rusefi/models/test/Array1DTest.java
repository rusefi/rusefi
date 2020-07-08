package com.rusefi.models.test;

import com.rusefi.models.Array1D;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * @author Andrey Belomutskiy
 *         7/14/2015
 */
public class Array1DTest {
    @Test
    public void testArrayValidation() {
        try {
            new Array1D(new double[] {3, 1});
        } catch (Throwable e) {
            // expected behaviour
        }
        new Array1D(new double[1]);
    }

    @Test
    public void testFindIndex() {
        Array1D array = new Array1D(new double[] {1 , 2, 3, 4});

        assertEquals(-1, array.findIndex(0));
        assertEquals(0, array.findIndex(1));
        assertEquals(0, array.findIndex(1.5));
        assertEquals(4, array.findIndex(15));
    }
}
