package com.irnems.ui.test;

import com.irnems.ui.RpmModel;
import org.junit.Assert;
import org.junit.Test;

/**
 * Date: 12/27/12
 * (c) Andrey Belomutskiy
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
