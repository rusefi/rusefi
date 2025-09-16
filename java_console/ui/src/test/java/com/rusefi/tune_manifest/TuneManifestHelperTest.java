package com.rusefi.tune_manifest;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class TuneManifestHelperTest {
    @Test
    public void test() {
        assertEquals("docsimp123_-aonscomtunes", TuneManifestHelper.trimUrlToLocalName("https://docs.imp123_-aons.com/tunes/"));
    }
}
