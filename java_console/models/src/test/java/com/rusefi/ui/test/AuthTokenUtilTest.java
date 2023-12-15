package com.rusefi.ui.test;

import com.rusefi.auth.AuthTokenUtil;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class AuthTokenUtilTest {
    @Test
    public void test() {
        assertFalse(AuthTokenUtil.isToken(null));
        assertTrue(AuthTokenUtil.isToken(" 11112222-5719-4444-84d4-111122223333   "));

        assertFalse(AuthTokenUtil.isToken(" 11112222x5719-4444-84d4-111122223333   "));

        assertFalse(AuthTokenUtil.isToken(" 11112222-5719-4444-84d4-11112222333   "));
    }
}
