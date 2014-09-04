package com.rusefi.io.test;

import com.rusefi.io.LinkManager;
import org.junit.Test;
import junit.framework.Assert;

/**
 * @author Andrey Belomutskiy
 *         9/4/14
 */
public class LinkManagerTest {
    @Test
    public void testEncoding() {
        Assert.assertEquals("sec!7!rpm 400", LinkManager.encodeCommand("rpm 400"));

    }
}
