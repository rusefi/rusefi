package com.rusefi.ui.test;

import com.rusefi.ui.AutoTokenUtil;
import org.junit.Test;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class AuthTokenUtilTest {
    @Test
    public void test() {
        assertFalse(AutoTokenUtil.isToken(null));
        assertTrue(AutoTokenUtil.isToken(" 11112222-5719-4444-84d4-111122223333   "));

        assertFalse(AutoTokenUtil.isToken(" 11112222x5719-4444-84d4-111122223333   "));

        assertFalse(AutoTokenUtil.isToken(" 11112222-5719-4444-84d4-11112222333   "));
    }
}
