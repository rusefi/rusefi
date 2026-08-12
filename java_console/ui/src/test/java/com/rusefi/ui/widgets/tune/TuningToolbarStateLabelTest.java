package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;

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

    @Test
    void savesCurrentTuneWithoutEdits() {
        ConfigurationImage working = new ConfigurationImage(1);
        ConfigurationImage session = new ConfigurationImage(2);
        ConfigurationImage ecu = new ConfigurationImage(3);
        ConfigurationImage baseline = new ConfigurationImage(4);

        assertSame(working, TuningToolbarWidget.imageToSave(working, session, ecu, baseline));
        assertSame(session, TuningToolbarWidget.imageToSave(null, session, ecu, baseline));
        assertSame(ecu, TuningToolbarWidget.imageToSave(null, null, ecu, baseline));
        assertSame(baseline, TuningToolbarWidget.imageToSave(null, null, null, baseline));
    }
}
