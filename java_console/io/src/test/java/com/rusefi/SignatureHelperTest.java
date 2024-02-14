package com.rusefi;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class SignatureHelperTest {
    @Test
    public void parseSignature() {
        RusEfiSignature s = SignatureHelper.parse("rusEFI master.2021.09.22.all.3378169541");

        assertEquals("master", s.getBranch());
        assertEquals("all", s.getBundleTarget());
    }
}
