package com.rusefi.pcb.test;

import com.rusefi.pcb.PointNode;
import junit.framework.Assert;
import org.junit.Test;

/**
 * (c) Andrey Belomutskiy
 * 5/28/2014
 */
public class PointNodeTest {
    private static final double EPS = 0.0000001;

    @Test
    public void testTurn() {

        PointNode x10y50 = new PointNode(10, 50);

        PointNode angle270 = new PointNode(0, 0, 270);
        Assert.assertEquals("270 x", angle270.translate(x10y50).x, 50.0, EPS);
        Assert.assertEquals(angle270.translate(x10y50).y, -10.0, EPS);

        PointNode angle180 = new PointNode(0, 0, 180);
        Assert.assertEquals("180 x", angle180.translate(x10y50).x, -10.0, EPS);
        Assert.assertEquals("180 y", angle180.translate(x10y50).y, -50.0, EPS);

        PointNode angle90 = new PointNode(0, 0, 90);
        Assert.assertEquals(angle90.translate(x10y50).x, -50.0, EPS);
        Assert.assertEquals(angle90.translate(x10y50).y, 10.0, EPS);
    }
}
