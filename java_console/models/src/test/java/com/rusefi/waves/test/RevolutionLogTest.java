package com.rusefi.waves.test;

import com.rusefi.waves.RevolutionLog;
import junit.framework.Assert;
import org.junit.Test;

/**
 * Andrey Belomutskiy (c) 2012-2014
 * 3/19/14
 */
public class RevolutionLogTest {
    @Test
    public void backTime() {
        RevolutionLog r = RevolutionLog.parseRevolutions("2000!148958!2000!154958!2000!160958!2000!166958!");

        Assert.assertEquals(594.84, r.getCrankAngleByTime(147915));

        // too back into the past
        Assert.assertEquals(Double.NaN, r.getCrankAngleByTime(140915));
    }
}
