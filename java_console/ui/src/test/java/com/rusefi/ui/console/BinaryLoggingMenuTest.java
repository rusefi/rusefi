package com.rusefi.ui.console;

import org.junit.jupiter.api.Test;

import java.io.File;

import static org.junit.jupiter.api.Assertions.assertEquals;

class BinaryLoggingMenuTest {
    @Test
    public void binaryLogExtensionIsAddedWhenMissing() {
        assertEquals(new File("capture.mlg"), BinaryLoggingMenu.ensureMlgExtension(new File("capture")));
        assertEquals(new File("capture.MLG"), BinaryLoggingMenu.ensureMlgExtension(new File("capture.MLG")));
    }

}
