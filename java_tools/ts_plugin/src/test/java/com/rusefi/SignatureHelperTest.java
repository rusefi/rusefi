package com.rusefi;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class SignatureHelperTest {
    @Test
    public void testUrl() {
        String url = SignatureHelper.getUrl("rusEFI master.2020.07.06.frankenso_na6.2468827536").first;
        Assertions.assertEquals("https://rusefi.com/online/ini/rusefi/master/2020/07/06/frankenso_na6/2468827536.ini", url);
    }

    @Test
    public void parseSignature() {
        RusEfiSignature s = SignatureHelper.parse("rusEFI master.2021.09.22.all.3378169541");

        assertEquals("master", s.getBranch());
        assertEquals("all", s.getBundleTarget());
    }

    @Test
    public void parseSrecName() {
        RusEfiSignature s = SignatureHelper.parseSrec("rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32_update.srec");
        assertEquals("development", s.getBranch());
        assertEquals("2026", s.getYear());
        assertEquals("05", s.getMonth());
        assertEquals("09", s.getDay());
        assertEquals("uaefi_pro", s.getBundleTarget());
        assertEquals("8849742d4267db6407b1400ae917a1ed39795d32", s.getHash());
    }

    @Test
    public void testLegacySrecName() {
        RusEfiSignature s = SignatureHelper.parseSrec("rusefi_development_2026-04-27_3659024206688255410edc1e751b6736281e0efd_update.srec");
        assertEquals("development", s.getBranch());
        assertEquals("2026", s.getYear());
        assertEquals("04", s.getMonth());
        assertEquals("27", s.getDay());
        assertEquals("all", s.getBundleTarget());
        assertEquals("3659024206688255410edc1e751b6736281e0efd", s.getHash());
    }
}
