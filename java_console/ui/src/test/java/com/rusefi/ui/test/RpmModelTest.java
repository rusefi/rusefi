package com.rusefi.ui.test;

import com.rusefi.ui.RpmModel;
import org.junit.Assert;
import org.junit.Test;

/**
 * Date: 12/27/12
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class RpmModelTest {
    @Test
    public void testRpmSmoothing() {
        RpmModel r = RpmModel.getInstance();
        r.setValue(100);

        Assert.assertEquals(100, r.getSmoothedValue());

        r.setValue(104);
        Assert.assertEquals(100, r.getSmoothedValue());

        r.setValue(96);
        Assert.assertEquals(100, r.getSmoothedValue());

        r.setValue(200);
        Assert.assertEquals(200, r.getSmoothedValue());
    }
}
