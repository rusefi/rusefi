package com.rusefi.ui.widgets.tune;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * The offline/dirty toolbar label wording (#9730). Pure logic, no Swing.
 */
public class TuningToolbarStateLabelTest {

    @Test
    void offlineAndDirty() {
        assertEquals("Local changes not burned to ECU",
                TuningToolbarWidget.stateLabelText(true, true));
    }

    @Test
    void connectedAndDirty() {
        assertEquals("Pending changes not burned",
                TuningToolbarWidget.stateLabelText(false, true));
    }

    @Test
    void offlineAndClean() {
        assertEquals("Offline tune",
                TuningToolbarWidget.stateLabelText(true, false));
    }

    @Test
    void connectedAndClean() {
        // Empty label = invisible; nothing to say when connected with no pending edits.
        assertEquals("", TuningToolbarWidget.stateLabelText(false, false));
    }
}
